#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include "horizontalslicetoimagemapper.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    virtual ~HorizontalSliceRenderer();

    void drawImage(QMatrix4x4 mvpMatrix);
    void setMapper(HorizontalSliceToImageMapper *mapper);
    void moveSlice(float steps);

    float z;

private:
    void initOpenGLShaders();
    void initImageGeometry();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    HorizontalSliceToImageMapper *_mapper;
};

#endif // HORIZONTALSLICERENDERER_H
