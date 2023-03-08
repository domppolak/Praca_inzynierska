#ifndef TRANSMITTEDSIGNAL_HH
#define TRANSMITTEDSIGNAL_HH

#include <cmath>

class TransmittedSignal
{

public:
    TransmittedSignal();

    void set_w(double const &_w);
    void set_p1(double const &_p1);
    void set_p2(double const &_p2);
    void set_tau1(double const &_tau1);
    void set_tau2(double const &_tau2);
    void set_M1(double const &_M1);
    void set_M2(double const &_M2);
    void set_Amp(double const &_Amp);
    void set_alpha(double const &_alpha);
    void set_time(double const &_time);
    void set_tMax(double const &t_max);

    double get_w() const;
    double get_p1() const;
    double get_p2() const;
    double get_tau1() const;
    double get_tau2() const;
    double get_M1() const;
    double get_M2() const;
    double get_Amp() const;
    double get_alpha() const;
    double get_time() const;
    double get_tMax() const;

    void setDefaultValues();
    double generateSignal_close2Real(double time_current, double time_0) const;
    double generatesignal_close2Real_notmax(double time_current, double time_0) const;

private:
    double w; //czestotliwosc katowa ultradzwiekow
    double p1, p2; //stałe przetwornika
    double tau1, tau2; //stałe przetwornika
    double M1, M2; // okreslaja wielkosc sygnalu
    double Amp; //amplituda

    double pow_p1(double x) const;
    double pow_p2(double x) const;


    double alpha; // określa kąt stożka
    double time; // określa czas przeprowadzenia symulacji charakterystycznego dla danego sygnału
    double t_max; // określa maksymalny czas generowania sygnału
};

inline void TransmittedSignal::set_w(const double &_w)
{
    this->w = _w;
}

inline void TransmittedSignal::set_p1(const double &_p1)
{
    this->p1 = _p1;
}

inline void TransmittedSignal::set_p2(const double &_p2)
{
    this->p2 =_p2;
}

inline void TransmittedSignal::set_tau1(const double &_tau1)
{
    this->tau1 = _tau1;
}

inline void TransmittedSignal::set_tau2(const double &_tau2)
{
    this->tau2 = _tau2;
}

inline void TransmittedSignal::set_M1(const double &_M1)
{
    this->M1 = _M1;
}

inline void TransmittedSignal::set_M2(const double &_M2)
{
    this->M2 = _M2;
}

inline void TransmittedSignal::set_Amp(const double &_Amp)
{
    this->Amp = _Amp;
}

inline void TransmittedSignal::set_alpha(const double &_alpha)
{
    this->alpha = _alpha;
}

inline void TransmittedSignal::set_time(const double &_time)
{
    this->time = _time;
}

inline void TransmittedSignal::set_tMax(const double &t_max)
{
    this->t_max = t_max;
}

inline double TransmittedSignal::get_w() const
{
    return this->w;
}

inline double TransmittedSignal::get_p1() const
{
    return this->p1;
}

inline double TransmittedSignal::get_p2() const
{
    return this->p2;
}

inline double TransmittedSignal::get_tau1() const
{
    return this->tau1;
}

inline double TransmittedSignal::get_tau2() const
{
    return this->tau2;
}

inline double TransmittedSignal::get_M1() const
{
    return this->M1;
}

inline double TransmittedSignal::get_M2() const
{
    return this->M2;
}

inline double TransmittedSignal::get_Amp() const
{
    return this->Amp;
}

inline double TransmittedSignal::get_alpha() const
{
    return this->alpha;
}

inline double TransmittedSignal::get_time() const
{
    return this->time;
}

inline double TransmittedSignal::get_tMax() const
{
    return this->t_max;
}

#endif // TRANSMITTEDSIGNAL_HH
