#include "platform.hh"
#include <assert.h>
#include <QDebug>

Platform::Platform()
{
    _loc_pos_mm = QVector3D(0,0,0);
    _gl_pos_mm = QVector3D(0,0,0);

    _size_mm = QVector3D(10,10,10);
    _color = QVector3D(0,0,0);
    _rotation_deg = 0;
}

uint Platform::get_NumOfReceiver() const
{
    uint recNum=0;
    for(auto const &son : _sonarList)
        recNum += son.get_numOfRec();

    return recNum;
}

void Platform::addSonar(Sonar &sonar)
{
    _sonarList.push_back(sonar);
}

void Platform::addSonar()
{
    _sonarList.push_back(Sonar());
    _sonarList.back().set_id(_sonarList.size());
}

bool Platform::deleteSonar()
{
    if(_sonarList.empty())
        return false;

    _sonarList.pop_back();
    return true;
}

bool Platform::deleteSonar(uint id)
{
    assert(id < _sonarList.size());

    if(_sonarList.empty())
        return false;

    _sonarList.erase(_sonarList.begin() + id);
    return true;
}

void Platform::deleteAllSonar()
{
    _sonarList.clear();
}

void Platform::resetPlatform()
{
    deleteAllSonar();
    _loc_pos_mm = QVector3D(0,0,0);
    _gl_pos_mm = QVector3D(0,0,0);

    _size_mm = QVector3D(10,10,10);
    _color = QVector3D(0,0,0);
    _rotation_deg = 0;
}

double Platform::CompTime0_us(const QVector<QVector3D> &objectPos)
{
    assert(!_sonarList.empty());
    assert(!objectPos.empty());
    double Time0_us = std::numeric_limits<double>::max();
    double Time0_i_us=0;
    const QVector3D activeTransPos = getActveSonar().getTransmitter().get_GlPos();

    for(auto &son : _sonarList){
        Time0_i_us = son.CompTime0_us(activeTransPos, objectPos);
        if(Time0_i_us < Time0_us)
            Time0_us = Time0_i_us;
    }

    return Time0_us;
}

double Platform::CompMaxTof() const
{
    assert(!_sonarList.empty());
    double maxTof = 0;
    for(auto &son : _sonarList)
        if(maxTof < son.CompMaxTof())
            maxTof = son.CompMaxTof();

    return maxTof;
}

void Platform::calculate_TOF_us(const QVector<QVector3D> &objectsPos)
{
    assert(!_sonarList.empty());
    for(auto &son : _sonarList)
        son.calculate_TOF_forAllRec(getActveSonar().getTransmitter().get_GlPos(), objectsPos);
}

QVector<QVector<double> > Platform::CompRecSignals(const double &currentTime_us) const
{
    assert(!_sonarList.empty());
    QVector<QVector<double>> recSignals;

    for(auto const &son : _sonarList)
        recSignals.push_back(son.CompRecSignals(getActveSonar().getTransmitter(), currentTime_us));

    return recSignals;
}

