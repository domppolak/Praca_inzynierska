#include "transmittedsignal.hh"
#include <QDebug>

TransmittedSignal::TransmittedSignal()
{
    setDefaultValues();
}

void TransmittedSignal::setDefaultValues()
{
    set_w((2*M_PI)/25);
    set_p1(4);
    set_p2(2);
    set_tau1(80);
    set_tau2(300);
    set_M1(3.46e-9);
    set_M2(8.3e-6);
    set_Amp(723);
    set_alpha(60);
    set_time(2500);
    set_tMax(2000);
}

double TransmittedSignal::pow_p1(double x) const
{
    int i=0;
    double result = x;
    while(++i < p1)
        result *= x;

    return result;
}

double TransmittedSignal::pow_p2(double x) const
{
    int i=0;
    double result = x;
    while(++i < p2)
        result *= x;

    return result;
}

double TransmittedSignal::generateSignal_close2Real(double time_current, double time_0) const
{
    if(time_current <= time_0) return 0;


    double time_normalized = time_current - time_0;
    if(t_max <= time_normalized) return 0;

    return Amp*sin(w*time_normalized)
            *(pow_p1(time_normalized)*M1*exp(-time_normalized/tau1)
              +pow_p2(time_normalized)*M2*exp(-time_normalized/tau2));
}

double TransmittedSignal::generatesignal_close2Real_notmax(double time_current, double time_0) const
{
    if(time_current <= time_0) return 0;


    double time_normalized = time_current - time_0;

    return Amp*sin(w*time_normalized)
            *(pow_p1(time_normalized)*M1*exp(-time_normalized/tau1)
              +pow_p2(time_normalized)*M2*exp(-time_normalized/tau2));
}

