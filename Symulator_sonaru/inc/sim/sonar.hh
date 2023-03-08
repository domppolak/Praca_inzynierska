#ifndef SONAR_HH
#define SONAR_HH

#include <QObject>
#include <QVector>
#include <QDebug>
#include <memory>
#include <cassert>
#include "receiver.hh"
#include "transmitter.hh"

class Sonar
{

public:
    Sonar();
    ~Sonar();

    void set_id(uint const &id);
    void set_LocPos(QVector3D const &locPos);
    void set_LocPos(double x_mm, double y_mm, double z_mm);
    void set_GlPos(QVector3D const &glPos);
    void set_GlPos(double x_mm, double y_mm, double z_mm);
    void set_size(QVector3D const &size_mm);
    void set_size(double x_mm, double y_mm, double z_mm);
    void set_rotation(double const &rot_deg);
    void set_color(QVector3D const &color);
    void set_color(double red, double green, double blue);

    uint get_id() const;
    QVector3D get_LocPos() const;
    QVector3D get_GlPos() const;
    QVector3D get_size() const;
    double get_rotation() const;
    QVector3D get_color() const;
    uint get_numOfRec() const;


    Transmitter const &getTransmitter() const;
    Transmitter &useTransmitter();

    Receiver const &getReceiver(uint id) const;
    Receiver &useReceiver(uint id);

    double CompTime0_us(QVector3D const &transPos, const QVector<QVector3D> &objectsPos);
    void calculate_TOF_forAllRec(const QVector3D &transPos, const QVector<QVector3D> &objectsPos);
    double CompMaxTof() const;
    QVector<double> CompRecSignals(Transmitter const &transmitter, double const &currentTime_us) const;
    QVector<QVector<double>> getTOF_list() const;

    void rec_setId(uint const &id, uint const &newId);
    void rec_setSize(uint const &id, QVector3D const &size_mm);
    void rec_setLocPos(uint const &id, QVector3D const &locPos_mm);
    void rec_setGlPos(uint const &id, QVector3D const &glPos_mm);
    void rec_setRotation(uint const &id, double const &rot_deg);

    uint rec_getId(uint const &id) const;
    QVector3D rec_getSize(uint const &id) const;
    QVector3D rec_getLocPos(uint const &id) const;
    QVector3D rec_getGlPos(uint const &id) const;
    double rec_getRotation(uint const &id) const;

    void transmitter_setLocPos(QVector3D const &locPos_mm);
    void transmitter_setGlPos(QVector3D const &glPos_mm);
    void transmitter_setRotation(double const &rot_deg);
    void transmitter_setSize(QVector3D const &newSize);

    QVector3D transmitter_getLocPos() const;
    QVector3D transmitter_getGlPos() const;
    double transmitter_getRotation() const;
    QVector3D transmitter_getSize() const;

    uint addReceiver();
    void addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm);
    void addReceiver(uint id, QVector3D size_mm, double rot_deg, QVector3D locPos_mm, QVector3D glPos_mm);
    bool deleteReceiver();
    bool deleteReceiver(uint const &id);
    void deleteAllRec();
    void resetSonar();

    TransmittedSignal const &getTransmittedSignal() const;
    TransmittedSignal &useTransmittedSignal();



private:
    uint _id;
    QVector3D _glPos_mm;
    QVector3D _locPos_mm;

    QVector3D _size_mm;
    double _rotation_deg;

    QVector3D _color;

    Transmitter _transmitter;
    std::vector<Receiver> _receiverList;

    QVector<QVector<double>> TOF_list_us;
    double Time0_us = 0;
};

inline void Sonar::set_id(const uint &id) {this->_id = id;}
inline void Sonar::set_LocPos(const QVector3D &locPos) {this->_locPos_mm = locPos;}
inline void Sonar::set_LocPos(double x_mm, double y_mm, double z_mm) {set_LocPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Sonar::set_GlPos(const QVector3D &glPos) {this->_glPos_mm = glPos;}
inline void Sonar::set_GlPos(double x_mm, double y_mm, double z_mm) {set_GlPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Sonar::set_size(const QVector3D &size_mm) {this->_size_mm = size_mm;}
inline void Sonar::set_size(double x_mm, double y_mm, double z_mm) {set_size(QVector3D(x_mm, y_mm, z_mm));}
inline void Sonar::set_rotation(const double &rot_deg) {this->_rotation_deg = rot_deg;}
inline void Sonar::set_color(const QVector3D &color) {this->_color = color;}
inline void Sonar::set_color(double red, double green, double blue) {set_color(QVector3D(red, green, blue));}

inline uint Sonar::get_id() const {return this->_id;}
inline QVector3D Sonar::get_LocPos() const {return this->_locPos_mm;}
inline QVector3D Sonar::get_GlPos() const {return this->_glPos_mm;}
inline QVector3D Sonar::get_size() const {return this->_size_mm;}
inline double Sonar::get_rotation() const {return this->_rotation_deg;}
inline QVector3D Sonar::get_color() const {return this->_color;}
inline uint Sonar::get_numOfRec() const {return _receiverList.size();}

inline void Sonar::rec_setId(const uint &id, const uint &newId)
{
    assert(id < _receiverList.size());
    _receiverList[id].set_id(newId);
}

inline void Sonar::rec_setSize(const uint &id, const QVector3D &size_mm)
{
    assert(id < _receiverList.size());
    _receiverList[id].set_size(size_mm);
}

inline void Sonar::rec_setLocPos(const uint &id, const QVector3D &locPos_mm)
{
    assert(id < _receiverList.size());
    _receiverList[id].set_LocPos(locPos_mm);
}

inline void Sonar::rec_setGlPos(const uint &id, const QVector3D &glPos_mm)
{
    assert(id < _receiverList.size());
    _receiverList[id].set_GlPos(glPos_mm);
}

inline void Sonar::rec_setRotation(const uint &id, const double &rot_deg)
{
    assert(id < _receiverList.size());
    _receiverList[id].set_rotation(rot_deg);
}

inline uint Sonar::rec_getId(const uint &id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id].get_id();
}

inline QVector3D Sonar::rec_getSize(const uint &id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id].get_size();
}

inline QVector3D Sonar::rec_getLocPos(const uint &id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id].get_LocPos();
}

inline QVector3D Sonar::rec_getGlPos(const uint &id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id].get_GlPos();
}

inline double Sonar::rec_getRotation(const uint &id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id].get_rotation();
}

inline void Sonar::transmitter_setLocPos(const QVector3D &locPos_mm)
{
    _transmitter.set_LocPos(locPos_mm);
}

inline void Sonar::transmitter_setGlPos(const QVector3D &glPos_mm)
{
    _transmitter.set_GlPos(glPos_mm);
}

inline void Sonar::transmitter_setRotation(const double &rot_deg)
{
    _transmitter.set_rotation(rot_deg);
}

inline void Sonar::transmitter_setSize(const QVector3D &newSize)
{
    _transmitter.set_size(newSize);
}

inline QVector3D Sonar::transmitter_getLocPos() const
{
    return _transmitter.get_LocPos();
}

inline QVector3D Sonar::transmitter_getGlPos() const
{
    return _transmitter.get_GlPos();
}

inline double Sonar::transmitter_getRotation() const
{
    return _transmitter.get_rotation();
}

inline QVector3D Sonar::transmitter_getSize() const
{
    return _transmitter.get_size();
}

inline const TransmittedSignal &Sonar::getTransmittedSignal() const
{
    return _transmitter.getTransmittedSignal();
}

inline TransmittedSignal &Sonar::useTransmittedSignal()
{
    return _transmitter.useTransmittedSignal();
}

inline const Transmitter &Sonar::getTransmitter() const
{
    return _transmitter;
}

inline Transmitter &Sonar::useTransmitter()
{
    return _transmitter;
}

inline const Receiver &Sonar::getReceiver(uint id) const
{
    assert(id < _receiverList.size());
    return _receiverList[id];
}

inline Receiver &Sonar::useReceiver(uint id)
{
    assert(id < _receiverList.size());
    return _receiverList[id];
}

inline QVector<QVector<double> > Sonar::getTOF_list() const
{
    return TOF_list_us;
}

#endif // SONAR_HH
