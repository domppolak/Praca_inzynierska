#ifndef AIRPARAMETERS_HH
#define AIRPARAMETERS_HH

#define DEFAULT_SIG_PERIOD_us   25

namespace air {

class Parameters {

    double _Temperature_Air_C = 20;
    double  _AcousticSpeed_mmUS = 0;
    double  _Ultrasonic_Wavelength_mm = 0;
    const double  _Ultrasonic_WavePeriod_us =  DEFAULT_SIG_PERIOD_us;
private:
    void CompAcousticSpeed();
    void CompWavelength();

public:
    Parameters();
    double GetUltrasonic_WavePeriod_us() const { return _Ultrasonic_WavePeriod_us; }
    double GetAcousticSpeed_mmUS() const { return _AcousticSpeed_mmUS; }
    double GetAcousticSpeed_mS() const { return _AcousticSpeed_mmUS*1000; }
    void SetAcousticSpeed_mS(double Speed_mS);
    double GetUltrasonic_Wavelength_mm() const { return _Ultrasonic_Wavelength_mm; }
    void SetTemperature_Air_C( double  Temp_C );
    double GetTemperature_Air_C() { return _Temperature_Air_C; }
};

extern Parameters Params;
}
#endif // AIRPARAMETERS_HH
