#include "streamlinesrenderer.h"

#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <flowdatasource.h>
#include <opengldisplaywidget.h>


StreamLinesRenderer::StreamLinesRenderer(StreamLinesMapper *mapperInput) : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    mapper = mapperInput;
    initOpenGlShaders();
    initStreamLines();
}

StreamLinesRenderer::~StreamLinesRenderer()
{
    vertexBuffer.destroy();
    vertexArrayObject.destroy();
}

void StreamLinesRenderer::drawStreamLines(QMatrix4x4 mvpMatrix, QMatrix4x4 mvMatrix)
{
    shaderProgram.link();

    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // TODO: wozu die ModelView-Matrix? wird irgendwie nirgends benutzt, oder?
    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    // Set the model-view matrix as a uniform value.
    shaderProgram.setUniformValue("modelviewMatrix", mvMatrix);

    // Issue OpenGL draw commands.
    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();

    // depth test modification
    functions->glDepthFunc(GL_LEQUAL);

    functions->glDrawArrays(GL_LINES, 0, vertices.size()); // Liniensegmente
    functions->glLineWidth(2);

    // Release objects until next render cycle. - release() ist das gleiche wie glUseProgram()
    vertexArrayObject.release();
    shaderProgram.release();
}

void StreamLinesRenderer::initStreamLines()
{
    vertices.clear();

    QVector<QVector3D> seedPoints;

    for (int i = 0; i <= 100; i++)
    {
        seedPoints.append(QVector3D(0.5f, 0.5f, i/100));
    }

    vertices = mapper->computeStreamLines(seedPoints);
    const int numberOfVertices = vertices.size();

    float streamLineVertices[numberOfVertices][3];

    for (int i = 0; i < numberOfVertices; i++)
    {
        QVector3D tempVec = vertices.value(i);
        streamLineVertices[i][0] = tempVec.x();
        streamLineVertices[i][1] = tempVec.y();
        streamLineVertices[i][2] = tempVec.z();
    }

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(streamLineVertices, numberOfVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("streamVertexPosition", GL_FLOAT, 0, 3, 3 * sizeof(float));
        shaderProgram.enableAttributeArray("streamVertexPosition");
        vertexBuffer.release();
    }
}

void StreamLinesRenderer::initOpenGlShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "streamlines_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "streamlines_fshader.glsl"))
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


void StreamLinesRenderer::timeStep(int steps)
{
    time = time + steps;
}

