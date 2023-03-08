#include "air-parameters.hh"
#include <cmath>

namespace air {
    Parameters Params;
}

#define SPEED_COEFF   0.3314

void air::Parameters::CompAcousticSpeed()
{
    _AcousticSpeed_mmUS = SPEED_COEFF*sqrt(1+_Temperature_Air_C/273.15);
}

void air::Parameters::CompWavelength()
{
    _Ultrasonic_Wavelength_mm = GetUltrasonic_WavePeriod_us()*GetAcousticSpeed_mmUS();
}

air::Parameters::Parameters()
{
    SetTemperature_Air_C(22);
}

inline
double Pow2(double const &arg) {return arg*arg;}

void air::Parameters::SetAcousticSpeed_mS(double Speed_mS)
{
    _AcousticSpeed_mmUS = Speed_mS/1000;
    _Temperature_Air_C = (Pow2(_AcousticSpeed_mmUS/SPEED_COEFF)-1)*273.15;
}

void air::Parameters::SetTemperature_Air_C(double Temp_C)
{
    _Temperature_Air_C = Temp_C;
    CompAcousticSpeed();
    CompWavelength();
}
