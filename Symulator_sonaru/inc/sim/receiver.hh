#ifndef RECEIVER_HH
#define RECEIVER_HH

#include <QVector3D>
#include <QVector>
#include "transmitter.hh"

class Receiver
{

public:
    Receiver();

    void set_id(uint const &id);
    void set_LocPos(const QVector3D &locPos_mm);
    void set_LocPos(double x_mm, double y_mm, double z_mm);
    void set_GlPos(QVector3D const &glPos_mm);
    void set_GlPos(double x_mm, double y_mm, double z_mm);
    void set_size(QVector3D const &size_mm);
    void set_size(double x_mm, double y_mm, double z_mm);
    void set_rotation(double const &rot_deg);


    uint get_id() const;
    QVector3D get_LocPos() const;
    QVector3D get_GlPos() const;
    double get_rotation() const;
    QVector3D get_size() const;

    void resetReceiver();

    QVector<double> calculate_TOFs(QVector3D const &transPos, QVector<QVector3D>const &objPos);
    double CompTime0_us(QVector3D const &transPos, QVector<QVector3D> const&objPos) const;
    double CompMaxTof() const;
    double CompRecSignal(Transmitter const &transmitter, double const &currentTime_us) const;
    QVector<double> getTOF_list_us() const;
private:
    uint _id;
    QVector3D _glPos_mm;
    QVector3D _locPos_mm;

    QVector3D _size_mm;
    double _rotation_deg;

    QVector<double> TOF_us_list;
};

inline void Receiver::set_id(const uint &id) {this->_id =id;}
inline void Receiver::set_LocPos(const QVector3D &locPos) {this->_locPos_mm = locPos;}
inline void Receiver::set_LocPos(double x_mm, double y_mm, double z_mm) {set_LocPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Receiver::set_GlPos(const QVector3D &glPos) {this->_glPos_mm=glPos;}
inline void Receiver::set_GlPos(double x_mm, double y_mm, double z_mm) {set_GlPos(QVector3D(x_mm, y_mm, z_mm));}
inline void Receiver::set_rotation(double const &rot_deg) {this->_rotation_deg=rot_deg;}
inline void Receiver::set_size(const QVector3D &size_mm) {this->_size_mm=size_mm;}
inline void Receiver::set_size(double x_mm, double y_mm, double z_mm) {set_size(QVector3D(x_mm, y_mm, z_mm));}

inline uint Receiver::get_id() const {return this->_id;}
inline QVector3D Receiver::get_LocPos() const {return this->_locPos_mm;}
inline QVector3D Receiver::get_GlPos() const {return this->_glPos_mm;}
inline double Receiver::get_rotation() const {return this->_rotation_deg;}
inline QVector3D Receiver::get_size() const {return this->_size_mm;}

inline QVector<double> Receiver::getTOF_list_us() const
{
    return TOF_us_list;
}




#endif // RECEIVER_HH
