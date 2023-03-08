#include "receiver.hh"
#include <QDebug>
#include "air-parameters.hh"
#include <QList>

Receiver::Receiver()
{
    resetReceiver();
}

void Receiver::resetReceiver()
{
    _id =0;
    _glPos_mm = QVector3D(0,0,0);
    _locPos_mm = QVector3D(0,0,0);
    _size_mm = QVector3D(10,10,10);
    _rotation_deg = 0;
}

QVector<double> Receiver::calculate_TOFs(const QVector3D &transPos, const QVector<QVector3D> &objectsPos)
{
    TOF_us_list.clear();
    double distance_mm=0;
    for(auto const &objPos : objectsPos){
        distance_mm = (transPos-objPos).length() + (objPos-_glPos_mm).length();
        TOF_us_list.push_back(distance_mm/air::Params.GetAcousticSpeed_mmUS());
    }

    return TOF_us_list;
}

double Receiver::CompTime0_us(const QVector3D &transPos, const QVector<QVector3D> &objectsPos) const
{
    QList<double> _list_time0_us;
    double distance_mm=0;
    for(auto const &objPos : objectsPos){
        distance_mm = (transPos-objPos).length() + (objPos-_glPos_mm).length();
        _list_time0_us.push_back(distance_mm/air::Params.GetAcousticSpeed_mmUS());
    }

    std::sort(_list_time0_us.begin(), _list_time0_us.end());
    return _list_time0_us.front();
}

double Receiver::CompMaxTof() const
{
    double maxTof = 0;
    for(auto const &tof : TOF_us_list)
        if(maxTof < tof)
            maxTof = tof;

    return maxTof;
}

double Receiver::CompRecSignal(const Transmitter &transmitter, const double &currentTime_us) const
{
    assert(!TOF_us_list.empty());
    double signalVal = 0;
    for(auto &TOF_time : TOF_us_list)
        signalVal += transmitter.SignalComputation(currentTime_us, TOF_time);

    return signalVal;
}

