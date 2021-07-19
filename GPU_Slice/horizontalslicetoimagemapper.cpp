#include "horizontalslicetoimagemapper.h"

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper()
{
}

HorizontalSliceToImageMapper::~HorizontalSliceToImageMapper()
{
    delete _source;
}

void HorizontalSliceToImageMapper::setDataSource(FlowDataSource *source){
    _source = source;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage(int iz){
    QImage img(16, 16, QImage::Format_RGB32);
    QRgb color;

    for(int iy = 0; iy < _source->getGridSize(); iy++){

        for(int ix = 0; ix < _source->getGridSize(); ix++){

            float input = _source->getDataValue(ix, iy, iz, 0) * 3 * 255;

            // values less 0 become blue
            if ( input <= 0 ) {
                color = qRgb(0, 0, -input);
            }
            else {
                color = qRgb(input, 0, 0);
            }

            img.setPixel(ix, iy, color);
        }
    }

    return img;
}
