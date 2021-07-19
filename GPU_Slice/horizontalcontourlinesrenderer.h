#ifndef HORIZONTALCONTOURLINESRENDERER_H
#define HORIZONTALCONTOURLINESRENDERER_H


#include "horizontalslicetoimagemapper.h"
#include "horizontalslicetocontourlinemapper.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class HorizontalContourLinesRenderer
{
public:
    HorizontalContourLinesRenderer(HorizontalSliceToContourLineMapper *mapperInput);
    virtual ~HorizontalContourLinesRenderer();

    void drawContourLines(QMatrix4x4 mvpMatrix);
    void setMapper(HorizontalSliceToContourLineMapper *mapperInput);
    void moveSlice(float steps);

    float _z;

private:
    void initOpenGlShaders();
    void initContourLines();
    QVector<QVector3D> getContourLines();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    HorizontalSliceToContourLineMapper *mapper;
    float numberOfVertices;
};

#endif // HORIZONTALCONTOURLINESRENDERER_H
