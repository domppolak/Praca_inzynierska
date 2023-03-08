#ifndef SIMULATIONTHREAD_HH
#define SIMULATIONTHREAD_HH

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include "simulation.hh"

class SimulationThread : public QThread
{
    Q_OBJECT

public:
    SimulationThread(std::shared_ptr<Simulation> const &simulation);

public slots:
    void on_simulationStarted();
    void on_simulationStopped();

private:
    void run() override;

    std::shared_ptr<Simulation> _simulation;
    QWaitCondition m_cond;
    bool test = true;
};

#endif // SIMULATIONTHREAD_HH
