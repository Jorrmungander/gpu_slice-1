#include "flowdatasource.h"
#include "gen_tornado.c"
#include <iostream>
using namespace std;

FlowDataSource::FlowDataSource()
{
}

void FlowDataSource::createData(int time){
    cartesianDataGrid = new float [gridSize * gridSize * gridSize * numberOfAttributes];

    gen_tornado(gridSize, gridSize, gridSize, time, cartesianDataGrid);
}

float FlowDataSource::getDataValue(int ix, int iy, int iz, int ic){
    if(ix < getGridSize() && iy < gridSize && iz < gridSize && ic < numberOfAttributes){
        return this->cartesianDataGrid[iz*gridSize*gridSize*numberOfAttributes+iy*gridSize*numberOfAttributes+ix*numberOfAttributes + ic];
    }
}

void FlowDataSource::printValuesOfHorizontalSlice(int iz){
    int component = 0;
    for(int iy = 0; iy < gridSize; iy++){
        cout << "\n\n row " << iy << ":";
        for (int ix = 0; ix < gridSize; ix++){
            //cout << "\n";
            //cout << "[x=" << ix << " y=" << iy << " z=" << iz << " component=" << component << "]: ";
            cout << this->getDataValue(ix, iy, iz, component) << " ";
        }
    }
    cout << "\n";
}

int FlowDataSource::getGridSize(){
    return this->gridSize;
}

int FlowDataSource::getNumberOfAttributes(){
    return this->numberOfAttributes;
}


