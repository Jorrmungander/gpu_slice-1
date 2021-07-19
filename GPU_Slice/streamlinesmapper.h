#ifndef STREAMLINESMAPPER_H
#define STREAMLINESMAPPER_H

#include <QImage>
#include <QColor>
#include "flowdatasource.h"

class StreamLinesMapper
{
public:
    StreamLinesMapper(FlowDataSource *inputSource);
    virtual ~StreamLinesMapper();

    void setDataSource(FlowDataSource *inputSource);

    QVector<QVector3D> computeStreamLines(QVector<QVector3D> seedPoints);
    QVector3D interpolateVector(QVector3D position);

    QVector3D EulerIntegration(QVector3D position, float stepSize);
    QVector3D RKIntegration(QVector3D position, float stepSize);

private:
    FlowDataSource *source;
    float distance;
};

#endif // STREAMLINESMAPPER_H
