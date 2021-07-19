#ifndef HORIZONTALSLICETOCONTOURLINEMAPPER_H
#define HORIZONTALSLICETOCONTOURLINEMAPPER_H

#include <QImage>
#include <QColor>
#include "flowdatasource.h"

class HorizontalSliceToContourLineMapper
{
public:
    HorizontalSliceToContourLineMapper(FlowDataSource *inputSource);
    virtual ~HorizontalSliceToContourLineMapper();

    void setDataSource(FlowDataSource *inputSource);
    QVector<QVector3D> mapSliceToContourLineSegments(int iz);

private:
    FlowDataSource *source;
    float distance;
    float isoCrossingBetweenTwoVertices(float vi, float vj, float iso);
};

#endif // HORIZONTALSLICETOCONTOURLINEMAPPER_H
