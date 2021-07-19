#include "horizontalslicerenderer.h"

#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLTexture>

HorizontalSliceRenderer::HorizontalSliceRenderer() : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    z = 0;
    initOpenGLShaders();
    initImageGeometry();
}

HorizontalSliceRenderer::~HorizontalSliceRenderer(){
    vertexBuffer.destroy();
    vertexArrayObject.destroy();
}

void HorizontalSliceRenderer::setMapper(HorizontalSliceToImageMapper *mapper){
    _mapper = mapper;
}

void HorizontalSliceRenderer::drawImage(QMatrix4x4 mvpMatrix){
    QImage image = _mapper->mapSliceToImage(z);

    QOpenGLTexture colorTexture(QOpenGLTexture::Target2D);
    colorTexture.create();
    colorTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    colorTexture.setData(image);

    shaderProgram.link();
    // Tells OpenGL to use the classes shader programm
    shaderProgram.bind();

    // Bind the vertex array object that links to the image vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // set the texture as a uniform variable
    const int textureUnit = 0;
    colorTexture.bind(textureUnit);
    // Set the textureUnit as a uniform value.
    shaderProgram.setUniformValue("colorTexture", textureUnit);

    // Issue OpenGL draw commands.
    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();
    functions->glLineWidth(2); //TODO: make "2" to a constant-variable
    functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();
    colorTexture.destroy();
}

void HorizontalSliceRenderer::initImageGeometry(){
    const unsigned int numberOfVertices = 5;

    float textureCoordinates[numberOfVertices][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0}
    };

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(textureCoordinates, numberOfVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vertexArrayObjectBinder(&vertexArrayObject);
    if(vertexArrayObject.isCreated()){
        vertexBuffer.bind();
        // the AttributeBuffer and AttributeArray must have the same name as in the shader-file (slice_vshader.glsl)
        shaderProgram.setAttributeBuffer("textureCoordinates", GL_FLOAT, 0, 3, 3 * sizeof(float));
        shaderProgram.enableAttributeArray("textureCoordinates");
        vertexBuffer.release();
    }
}

void HorizontalSliceRenderer::initOpenGLShaders(){
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "slice_vshader.glsl")){
        std::cout << "Vertex shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "slice_fshader.glsl")){
        std::cout << "Fragment shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}

void HorizontalSliceRenderer::moveSlice(float steps){
    z += steps;

    float z15 = z/15 - 0.001;

    // manipulate coordinates to move slice accordingly
    const unsigned int numberOfVertices = 5;
    float textureCoordinates[numberOfVertices][3] = {
        {0, 0, z15}, {1, 0, z15}, {1, 1, z15}, {0, 1, z15}, {0, 0, z15}
    };

    vertexBuffer.bind();
    vertexBuffer.allocate(textureCoordinates, numberOfVertices * 3 * sizeof(float));
    vertexBuffer.release();
}

