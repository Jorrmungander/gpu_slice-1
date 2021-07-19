#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include "QVector3D"
#include "qvector.h"
#include "cmath"

class FlowDataSource
{
public:
    FlowDataSource();
    void createData(int time);
    float getDataValue(int ix, int iy, int iz, int ic);
    void printValuesOfHorizontalSlice(int iz);
    int getGridSize();
    int getNumberOfAttributes();

private:
    float* cartesianDataGrid;
    static const int gridSize = 16;
    static const int numberOfAttributes = 3; // we need 3 vectors
};

#endif // FLOWDATASOURCE_H
