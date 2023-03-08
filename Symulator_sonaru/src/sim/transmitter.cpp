#include "transmitter.hh"
#include <QDebug>

Transmitter::Transmitter()
{

    _locPos_mm = QVector3D(0,0,0);
    _roation_deg = 0;
    _size_mm = QVector3D(1,1,1);
    QVector3D direction(0, 0, 10);
    _acusticVector = direction - _locPos_mm;
}



