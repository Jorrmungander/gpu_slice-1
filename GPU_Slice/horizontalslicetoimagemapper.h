#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H

#include "flowdatasource.h"
#include <QImage>

class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();
    virtual ~HorizontalSliceToImageMapper();

    void setDataSource(FlowDataSource *inputSource);
    QImage mapSliceToImage(int iz);

private:
    FlowDataSource *source;
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
