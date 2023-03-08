#include "simulation.hh"

Simulation::Simulation(QObject *parent)
    : QObject{parent}
{

}

Simulation::~Simulation()
{
    delete transmitterDialog;
}

void Simulation::StartSimulation()
{
    simulationStarted = true;
    emit startSimulationThread();
}

void Simulation::execMeasurement()
{
//    QVector<double> time, tran_signalData;
//    time.reserve(300); tran_signalData.reserve(300);
//    for(int i=0; i<300; i++){
//        time.push_back(0);
//        tran_signalData.push_back(0);
//    }
//    int timeSendData = 0;
//    qDebug() << isContinueLooping();
//    int time_test=0;
//    int i=0;
//    _platform.setID_ActiveSonar(0);
//    simulationTime_us = 0;
//    while(isContinueLooping()){
//        if(time_test++ == 5000) return;
//    qDebug() << i;
//    tran_signalData[i] = (_platform.getActveSonar().getTransmitter().getTransmittedSignal().generateSignal_close2Real(simulationTime_us, 0.0));

//    if(timeSendData == 299){
//        emit addTransmitterData(time, tran_signalData,0);
//        i=0;
//        timeSendData=0;
//    }else{
//        i++;
//        timeSendData++;
//    }

//    simulationTime_us++;
//    }
}

void Simulation::on_stopSimulation()
{

    simulationStarted = false;
}
void Simulation::execMeasurement(const QVector<QVector3D> &objPositions, uint const &id_ActiveSonar)
{
    _platform.setID_ActiveSonar(id_ActiveSonar);
    _platform.calculate_TOF_us(objPositions);

    QString path = "/home/dominik/praca_inzynierska/Symulator_Sonaru_3D/Symulator_sonaru/ZeroCrossingData/";
    QString fileName = "data_to_calculate_rec_signal.dat";
    path += fileName;
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        //QMessageBox::critical(this,"Simulation", "Failed save data to file");
        return;
    }
    QTextStream out(&file);
    out << "Trans Pos   " << _platform.getActveSonar().getTransmitter().get_GlPos()[0] << " " << _platform.getActveSonar().getTransmitter().get_GlPos()[1]
        << " " << _platform.getActveSonar().getTransmitter().get_GlPos()[2] << "\n";
    for(auto const &obj : objPositions)
        out << "Obj Pos   " << obj[0] << " " << obj[1] << " " <<obj[2] << "\n";

    out << "ToF dla każdego odbiornika oraz kazdego obiketu\n\n";
    for(uint sonId=0; sonId < _platform.get_NumOfSonars(); sonId++){
        for(uint recId=0; recId < _platform.getSonar(sonId).get_numOfRec(); recId++){
            for(auto const &t : _platform.getSonar(sonId).getReceiver(recId).getTOF_list_us()){
                out << t << "\n";
            }
        }
    }
    file.close();

    execTransmittedSignalMeasurement();
    execReceiverMeasurement(objPositions);

}

void Simulation::execReceiverMeasurement(const QVector<QVector3D> &objPositions)
{
    double time0 = _platform.CompTime0_us(objPositions);
    double maxTime = _platform.CompMaxTof();
    double stopTime = maxTime + _platform.getActveSonar().getTransmitter().getTransmittedSignal().get_time();
    QVector<double> recTimes;
    QVector<double> current_recData;
    QVector<QVector<double>> recsData;
    double currentTime = time0;
    while(currentTime <= stopTime){
        recTimes.push_back(currentTime);
        ++currentTime;
    }

    for(uint sonId=0; sonId < _platform.get_NumOfSonars(); sonId++){
        for(uint recId=0; recId < _platform.getSonar(sonId).get_numOfRec(); recId++){
            for(uint i=0; i < recTimes.size(); i++)
                current_recData.push_back(_platform.getSonar(sonId).getReceiver(recId).CompRecSignal(_platform.getActveSonar().getTransmitter(), recTimes[i]));

            recsData.push_back(current_recData);
            current_recData.clear();
        }
    }  

    emit recData(recTimes, recsData, time0);
}

void Simulation::execTransmittedSignalMeasurement()
{
    double currentTime = 0;
    double stopTime = _platform.getActveSonar().getTransmitter().getTransmittedSignal().get_time();
    QVector<double> signalTime;
    QVector<double> signalData;
    while(currentTime <= stopTime){
        signalTime.push_back(currentTime);
        signalData.push_back(_platform.getActveSonar().getTransmitter().SignalComputation(currentTime, 0));
        ++currentTime;
    }

    emit addTransmitterData(signalTime, signalData, 0);
}

