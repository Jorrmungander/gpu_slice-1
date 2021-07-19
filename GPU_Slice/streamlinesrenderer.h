#ifndef STREAMLINESRENDERER_H
#define STREAMLINESRENDERER_H


#include "streamlinesmapper.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class StreamLinesRenderer
{
public:
    StreamLinesRenderer(StreamLinesMapper *mapperInput);
    virtual ~StreamLinesRenderer();

    void drawStreamLines(QMatrix4x4 mvpMatrix, QMatrix4x4 mvMatrix);

    void timeStep(int steps);

    int time;

private:
    void initOpenGlShaders();
    void initStreamLines();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;

    StreamLinesMapper *mapper;

    unsigned int numberOfVertices;

    QVector<QVector3D> vertices;
};

#endif // STREAMLINESRENDERER_H
