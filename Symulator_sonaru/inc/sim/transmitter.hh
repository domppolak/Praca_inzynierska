#ifndef TRANSMITTER_HH
#define TRANSMITTER_HH

#include <QVector3D>
#include "transmittedsignal.hh"

class Transmitter
{

public:
    Transmitter();
    void set_LocPos(QVector3D const &locPos_mm);
    void set_LocPos(double x_mm, double y_mm, double z_mm);
    void set_GlPos(QVector3D const &glPos_mm);
    void set_GlPos(double x_mm, double y_mm, double z_mm);
    void set_size(QVector3D const &size_mm);
    void set_size(double x_mm, double y_mm, double z_mm);
    void set_rotation(double const &rot_deg);
    void set_acusticVec(QVector3D const &vec);

    QVector3D get_LocPos() const;
    QVector3D get_GlPos() const;
    double get_rotation() const;
    QVector3D get_size() const;
    QVector3D get_acusticVec() const;

    double SignalComputation(double time_currnet, double time_0) const;

    TransmittedSignal const &getTransmittedSignal() const;
    TransmittedSignal &useTransmittedSignal();

private:
    QVector3D _glPos_mm;
    QVector3D _locPos_mm;

    QVector3D _size_mm;
    double _roation_deg;

    TransmittedSignal signal;
    QVector3D _acusticVector;

};

inline void Transmitter::set_LocPos(QVector3D const &locPos_mm) {this->_locPos_mm = locPos_mm;}
inline void Transmitter::set_LocPos(double x_mm, double y_mm, double z_mm) {set_LocPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Transmitter::set_GlPos(QVector3D const &glPos_mm) {this->_glPos_mm = glPos_mm;}
inline void Transmitter::set_GlPos(double x_mm, double y_mm, double z_mm) {set_GlPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Transmitter::set_size(QVector3D const &size_mm) {this->_size_mm = size_mm;}
inline void Transmitter::set_size(double x_mm, double y_mm, double z_mm) {set_size(QVector3D(x_mm, y_mm, z_mm));}
inline void Transmitter::set_rotation(double const &rot_deg) {this->_roation_deg = rot_deg;}
inline void Transmitter::set_acusticVec(const QVector3D &vec) {this->_acusticVector = vec;}

inline QVector3D Transmitter::get_LocPos() const {return this->_locPos_mm;}
inline QVector3D Transmitter::get_GlPos() const {return this->_glPos_mm;}
inline double Transmitter::get_rotation() const {return this->_roation_deg;}
inline QVector3D Transmitter::get_size() const {return this->_size_mm;}
inline QVector3D Transmitter::get_acusticVec() const {return this->_acusticVector;}

inline double Transmitter::SignalComputation(double time_currnet, double time_0) const
{
    return signal.generateSignal_close2Real(time_currnet, time_0);
}

inline const TransmittedSignal &Transmitter::getTransmittedSignal() const
{
    return signal;
}

inline TransmittedSignal &Transmitter::useTransmittedSignal()
{
    return signal;
}

#endif // TRANSMITTER_HH
