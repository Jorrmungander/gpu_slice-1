#include "horizontalcontourlinesrenderer.h"

#include "horizontalslicetoimagemapper.h"
#include "horizontalslicetocontourlinemapper.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLTexture>

// Zeigt die Konturlinien in unserer Szene an
HorizontalContourLinesRenderer::HorizontalContourLinesRenderer(HorizontalSliceToContourLineMapper *mapperInput) : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    mapper = mapperInput;
    _z = 0;
    initOpenGlShaders();
    initContourLines();
}

HorizontalContourLinesRenderer::~HorizontalContourLinesRenderer()
{
    vertexBuffer.destroy();
    vertexArrayObject.destroy();
}

void HorizontalContourLinesRenderer::setMapper(HorizontalSliceToContourLineMapper *mapperInput)
{
    mapper = mapperInput;
}

void HorizontalContourLinesRenderer::drawContourLines(QMatrix4x4 mvpMatrix)
{
    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the image vertices.
    vertexArrayObject.bind();


    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Issue OpenGL draw commands.
       QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();

       // depth test modification
       functions->glDepthFunc(GL_LEQUAL);

       functions->glLineWidth(2);
       functions->glDrawArrays(GL_LINES, 0, numberOfVertices);

       // Release objects until next render cycle.
       vertexArrayObject.release();
       shaderProgram.release();
}

void HorizontalContourLinesRenderer::moveSlice(float steps)
{
    _z += steps;
    float stepsScaled = _z / 15;
    QVector<QVector3D> segments = getContourLines();

    QVector<QVector3D> scaledSegments;

    for(int i = 0; i < segments.size(); i++)
    {
        QVector3D segmentCache = segments.at(i);
        segmentCache.setZ(stepsScaled);
        scaledSegments.append(segmentCache);
    }

    QVector3D *contourVertices = scaledSegments.data();

    vertexBuffer.bind();
    vertexBuffer.allocate(contourVertices, numberOfVertices * 3 * sizeof(float));
    vertexBuffer.release();
}

QVector<QVector3D> HorizontalContourLinesRenderer::getContourLines()
{
    //Get Line Segments from HorizonzalSliceToContourLineMapper for Slice z
    QVector<QVector3D> contourSegments = mapper->mapSliceToContourLineSegments(_z);

    //Number of segments/vertices for rendering
    numberOfVertices = contourSegments.size();
    std::cout << "numVertices: " << numberOfVertices << "\n";


    return contourSegments;
}

void HorizontalContourLinesRenderer::initContourLines()
{
    QVector3D* contourVertices = getContourLines().data();

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(contourVertices, numberOfVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);

    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("contourVertexPosition", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("contourVertexPosition");
        vertexBuffer.release();
    }
}

void HorizontalContourLinesRenderer::initOpenGlShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "contourlines_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "contourlines_fshader.glsl"))
    {
        std::cout << "Fragment shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}


