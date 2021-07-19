#include "horizontalslicetocontourlinemapper.h"

#include "opengldisplaywidget.h"
#include "QBitArray"
#include <iostream>

// implementiert den marching Squares Algorithmus
HorizontalSliceToContourLineMapper::HorizontalSliceToContourLineMapper(FlowDataSource *inputSource)
{
    source = inputSource;
}

HorizontalSliceToContourLineMapper::~HorizontalSliceToContourLineMapper()
{
    delete source;
}

void HorizontalSliceToContourLineMapper::setDataSource(FlowDataSource *inputSource)
{
    source = inputSource;
}

QVector<QVector3D> HorizontalSliceToContourLineMapper::mapSliceToContourLineSegments(int iz)
{
    QVector<QVector3D> lineSegments;

    // iso value for marhing squares...
    float iso = 0.005;

    //(pj - pi) "width/hight" of cell
    distance = 1.0/ source->getGridSize();


    for (int iy = 0; iy < source->getGridSize(); iy++){
        for (int ix = 0; ix < source->getGridSize(); ix++){

            // get the data points of the square
            float v0 = source->getDataValue(ix, iy,   iz,   0);
            float v1 = source->getDataValue(ix+1, iy, iz, 0);
            float v2 = source->getDataValue(ix+1, iy+1, iz, 0);
            float v3 = source->getDataValue(ix, iy+1, iz,   0);

             //bit array for ease of use
            QBitArray segments = QBitArray(4,false);
            if(v0 < iso){
                segments[0]=true;
            }
            if(v1 < iso){
                segments[1]=true;
            }
            if(v2 < iso){
                segments[2]=true;
            }
            if(v3 < iso){
                segments[3]=true;
            }

            float x = ix;
            float y = iy;
            x *= distance;
            y *= distance;

            //different cases of marching squares algorithm
            //top left
            if((!segments[0] && !segments[1] && !segments[2] && segments[3]) || (segments[0] && segments[1] && segments[2] && !segments[3]))
            {
                lineSegments.append(QVector3D(x, y + isoCrossingBetweenTwoVertices(v0, v3, iso), iz));
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v3, v2, iso), y + distance, iz));
            }
            //top right
            else if((!segments[0] && !segments[1] && segments[2] && !segments[3]) || (segments[0] && segments[1] && !segments[2] && segments[3]))
            {
                lineSegments.append(QVector3D(x + distance, y + isoCrossingBetweenTwoVertices(v1, v2, iso), iz));
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v3, v2, iso), y + distance, iz));
            }
            //bottom left
            else if((segments[0] && !segments[1] && !segments[2] && !segments[3]) || (!segments[0] && segments[1] && segments[2] && segments[3]))
            {
                lineSegments.append(QVector3D(x, y + isoCrossingBetweenTwoVertices(v0, v3, iso), iz));
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v0, v1, iso), y, iz));
            }
            //bottom right
            else if((!segments[0] && segments[1] && !segments[2] && !segments[3]) || (segments[0] && !segments[1] && segments[2] && segments[3]))
            {
                lineSegments.append(QVector3D(x + distance, y + isoCrossingBetweenTwoVertices(v1, v2, iso), iz));
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v0, v1, iso), y, iz));
            }
            //two points same side
            //top or bottom
            else if((segments[0] && segments[1] && !segments[2] && !segments[3]) || (!segments[0] && !segments[1] && segments[2] && segments[3]))
            {
                lineSegments.append(QVector3D(x, y + isoCrossingBetweenTwoVertices(v0, v3, iso), iz));
                lineSegments.append(QVector3D(x + distance, y + isoCrossingBetweenTwoVertices(v1, v2, iso), iz));
            }
            //left or right
            else if((segments[0] && !segments[1] && !segments[2] && segments[3]) || (!segments[0] && segments[1] && segments[2] && !segments[3]))
            {
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v0, v1, iso), y, iz));
                lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v3, v2, iso), y + distance, iz));
            }
            //two points diagonal
            else if((segments[0] && !segments[1] && segments[2] && !segments[3]) || (!segments[0] && segments[1] && !segments[2] && segments[3]))
            {
                if( ((v0 + v1 + v2 + v3) / 4) < iso)
                {
                    lineSegments.append(QVector3D(x, y + isoCrossingBetweenTwoVertices(v0, v3, iso), iz));
                    lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v3, v2, iso), y + distance, iz));
                    lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v0, v1, iso), y, iz));
                    lineSegments.append(QVector3D(x + distance, y + isoCrossingBetweenTwoVertices(v1, v2, iso), iz));
                }
                else
                {
                    lineSegments.append(QVector3D(x, y + isoCrossingBetweenTwoVertices(v0, v3, iso), iz));
                    lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v0, v1, iso), y, iz));
                    lineSegments.append(QVector3D(x + isoCrossingBetweenTwoVertices(v3, v2, iso), y + distance, iz));
                    lineSegments.append(QVector3D(x + distance, y + isoCrossingBetweenTwoVertices(v1, v2, iso), iz));
                }
            }
        }
    }
    return lineSegments;
}

// gibt interpolierten Schnittpunkt einer Konturlinie zwischen zwei Eckpunkten zurück
float HorizontalSliceToContourLineMapper::isoCrossingBetweenTwoVertices(float vi, float vj, float iso)
{
    float distanceIsoVi = iso - vi;
    float fullDistance = vj - vi;
    float alpha = distanceIsoVi / fullDistance;
    return alpha * distance;
}
