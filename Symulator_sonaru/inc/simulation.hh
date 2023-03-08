#ifndef SIMULATION_HH
#define SIMULATION_HH

#include "platform.hh"
#include <QObject>
#include "plotdialog.hh"
#include <QMutex>
#include <QWidget>

class Simulation : public QObject
{
    Q_OBJECT

public:
    explicit Simulation(QObject *parent = nullptr);
    ~Simulation();
    void setPlatform(Platform const &platform);
    bool isContinueLooping();
    void StartSimulation();
    void StopSimulation();
    bool isSimulationStarted() const;
    void setActiveSonar(uint const &_id_activeSonar);
    void execMeasurement();
    void execMeasurement(QVector<QVector3D> const &objPositions, uint const &id_ActiveSonar);
    double getTime_us() const;
    void setSimulationTime(double const &simTime);
signals:
    void startSimulationThread();
    void addTransmitterData(QVector<double> const &time, QVector<double> const &signal, double const &time0);
    void recData(QVector<double> time, QVector<QVector<double>> const &recSignals, double const &time0);

public slots:
    void on_stopSimulation();

private:
    void execReceiverMeasurement(QVector<QVector3D> const &objPositions);
    void execTransmittedSignalMeasurement();
    Platform _platform;
    bool simulationStarted = false;
    double simulationTime_us = 0.0;
    plotDialog *transmitterDialog = nullptr;
    QMutex mutex;

    double simulationTime = 0;
};

inline void Simulation::setPlatform(const Platform &platform) {
    _platform = platform;
    qDebug() << _platform.get_NumOfSonars();
}

inline bool Simulation::isContinueLooping()
{
    return simulationStarted;
}

inline void Simulation::StopSimulation() {simulationStarted = false;}

inline bool Simulation::isSimulationStarted() const {return simulationStarted;}

inline void Simulation::setActiveSonar(const uint &_id_activeSonar)
{
    assert(_id_activeSonar < _platform.get_NumOfSonars());
    _platform.setID_ActiveSonar(_id_activeSonar);
}

inline double Simulation::getTime_us() const
{
    return simulationTime_us;
}

inline void Simulation::setSimulationTime(const double &simTime)
{
    simulationTime = simTime;
}

#endif // SIMULATION_HH
