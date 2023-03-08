#include "sonar.hh"
#include <QDebug>
#include <QVector3D>
#include <algorithm>
#include <iterator>

Sonar::Sonar()
{
    _id = 0;
    _locPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(1,1,1);
    _rotation_deg = 0;
    _color = QVector3D(0,0,0);

}

Sonar::~Sonar()
{

}

double Sonar::CompTime0_us(const QVector3D &transPos, const QVector<QVector3D> &objectsPos)
{
    assert(!objectsPos.empty());
    double time0_i_us = 0;
    Time0_us = std::numeric_limits<double>::max();
    for(auto &rec : _receiverList){
        time0_i_us = rec.CompTime0_us(transPos, objectsPos);
        if(time0_i_us < Time0_us)
            Time0_us = time0_i_us;
    }

    return Time0_us;

}

void Sonar::calculate_TOF_forAllRec(const QVector3D &transPos, const QVector<QVector3D> &objectsPos)
{
    // w tym wypadku nasz sonar ogranicza sie tylko do transmitera
    if(_receiverList.empty())
        return;

    for(auto &rec : _receiverList)
        rec.calculate_TOFs(transPos, objectsPos);
}

double Sonar::CompMaxTof() const
{
    double maxTof=0;
    for(auto const &rec : _receiverList)
        if(maxTof < rec.CompMaxTof())
            maxTof = rec.CompMaxTof();

    return maxTof;
}

QVector<double> Sonar::CompRecSignals(const Transmitter &transmitter, const double &currentTime_us) const
{
    QVector<double> recSignals;
    for(auto const &rec : _receiverList)
        recSignals.push_back(rec.CompRecSignal(transmitter, currentTime_us));

    return recSignals;
}

uint Sonar::addReceiver()
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(_receiverList.size());

    return _receiverList.back().get_id();
}

void Sonar::addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm)
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(id);
    _receiverList.back().set_size(size_mm);
    _receiverList.back().set_rotation(rot_deg);
    _receiverList.back().set_LocPos(locPos_mm);
}

void Sonar::addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm, QVector3D glPos_mm)
{
    _receiverList.push_back(Receiver());
    _receiverList.back().set_id(id);
    _receiverList.back().set_size(size_mm);
    _receiverList.back().set_rotation(rot_deg);
    _receiverList.back().set_LocPos(locPos_mm);
    _receiverList.back().set_GlPos(glPos_mm);
}

bool Sonar::deleteReceiver()
{
    if(_receiverList.empty())
        return false;

    _receiverList.pop_back();
    return true;
}

bool Sonar::deleteReceiver(const uint &id)
{
    assert(id < _receiverList.size());

    if(_receiverList.empty())
        return false;

    _receiverList.erase(_receiverList.begin() +id);
    return true;
}

void Sonar::deleteAllRec()
{
    _receiverList.clear();
}

void Sonar::resetSonar()
{
    _id = 0;
    _locPos_mm = QVector3D(0,0,0);
    _glPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(10,10,10);
    _rotation_deg = 0;
    _color = QVector3D(0,0,0);

    _receiverList.clear();
}


