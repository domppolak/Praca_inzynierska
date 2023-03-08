#include "simulationthread.hh"

SimulationThread::SimulationThread(std::shared_ptr<Simulation>const &simulation)
    :_simulation(simulation)
{

}

void SimulationThread::on_simulationStarted()
{
    qDebug() << _simulation->isContinueLooping();
    if(!isRunning()){
        start();
    }else{
       m_cond.wakeOne();
    }
}

void SimulationThread::on_simulationStopped()
{
   terminate();
}

void SimulationThread::run()
{
    //qDebug() << _simulation->isContinueLooping();
    if(!_simulation->isSimulationStarted()){
        qDebug() << "koniec dzialania watku";
        return;
    }

    _simulation->execMeasurement();
    qDebug() << "koniec";
}
