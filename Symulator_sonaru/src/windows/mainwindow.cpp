#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "air-parameters.hh"
#include "filemanager.hh"
#include <cmath>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _sonarConfWindow = new SonarConfWindow(this);
    _platformConfWindow = new PlatformConfWindow(this);
    _addObjToScene = new ObjToScene(this);
    _simulation = std::make_shared<Simulation>(this);
    _simThread = new SimulationThread(_simulation);
    _transmitterDialog = new plotDialog(this);
    _simWindow = new SimulationWindow(this);
    _transmitterDialog->makePlot("Transmitted signal");

    setMenuBar();
    ui->temperatureSpinBox->setDecimals(1);
    ui->temperatureSpinBox->setValue(air::Params.GetTemperature_Air_C());
    ui->temperatureSpinBox->setMinimum(-100);

    setConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _sonarConfWindow;
    delete _platformConfWindow;
    delete _addObjToScene;
    delete _transmitterDialog;
    delete _simThread;
    delete _simWindow;

    for(auto &dialog : _receiverDialogList)
        delete dialog;
}

void MainWindow::setMenuBar()
{
    ui->menubar->addAction("Save", this, SLOT(on_saveAction()));
    ui->menubar->addAction("Open", this, SLOT(on_openAction()));
    ui->menubar->addAction("Sonar", this, SLOT(on_sonarAction()));
    ui->menubar->addAction("Platform", this, SLOT(on_platformAction()));

    figuresMenu = std::make_shared<QMenu>("Figures");
    ui->menubar->addMenu(figuresMenu.get());
    figuresMenu->addAction("Transmiter figure", this, SLOT(on_transmiterFigureAction()));

    receiverMenu = std::make_shared<QMenu>("Receiver figures");
    figuresMenu->addMenu(receiverMenu.get());
    receiverMenu->addAction("All figures", this, SLOT(on_receiverFiguresAction()));

    isRecMenuCreated = true;
}

void MainWindow::on_saveAction()
{
    FileManager _fileManager;
    SimulationScene _scene;
    ui->SceneWidget->getScene(_scene);
    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)") + ".xml";
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Cannot save file!"));
        return;
    }

    _fileManager.saveScene(file, _scene);
    file.close();
}

void MainWindow::on_openAction()
{
    FileManager _fileManager;
    SimulationScene _scene;

    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open scene configuration file"));
        return;
    }else{
        SimulationScene _scene;
        if(_fileManager.readSceneConf(file, _scene)){
            ui->SceneWidget->loadScene(_scene);
        }
        else
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a scene configuration file!"));
    }

    file.close();
}
void MainWindow::on_sonarAction()
{
    _sonarConfWindow->show();
}

void MainWindow::on_platformAction()
{
    _platformConfWindow->show();
}

void MainWindow::on_transmiterFigureAction()
{
    _transmitterDialog->show();
}

void MainWindow::on_receiverFiguresAction()
{
    for(auto &dialog : _receiverDialogList)
        dialog->show();
}

void MainWindow::on_startSimPushButton_clicked()
{

    if(!ui->SceneWidget->isPlatformModelCreated()){
        QMessageBox::information(this, "Simulation", "Add platform first");
        return;
    }

    if(_platform.get_NumOfReceiver() <= 0){
        QMessageBox::information(this, "Simulation", "No receivers on scene");
        return;
    }

    if(ui->SceneWidget->getNumOfObject() == 0){
        QMessageBox::information(this, "Simulation", "Add object first");
        return;
    }

    ui->SceneWidget->getPlatformActualPosition(_platform);
    _simWindow->setMaxActiveSonar(_platform.get_NumOfSonars());
    _simWindow->show();
}

void MainWindow::on_objc2Add(const Objects &obj)
{
    switch(obj)
    {
        case PLATFORM:
            ui->SceneWidget->addPlatform();
            ui->SceneWidget->update();
            break;
        case POINT:
            ui->SceneWidget->addPointModel();
            ui->SceneWidget->update();
            break;
        case STAKE:
            ui->SceneWidget->addPoleModel();
            ui->SceneWidget->update();
            break;
        case FLAT_SURFACE:
            ui->SceneWidget->addFlatSurfaceModel();
            ui->SceneWidget->update();
            break;
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

   _addObjToScene->updateLabel(ui->SceneWidget->getNumfOfObj(obj), obj);
}

void MainWindow::on_updateNumLabelOfSelectedObj(const Objects &obj)
{
   _addObjToScene->updateLabel(ui->SceneWidget->getNumfOfObj(obj), obj);
}

void MainWindow::on_clearScenePushButton_clicked()
{
    QMessageBox msgBox;
    QString msgText = "Clear all objects and platform or only objects?";
    msgBox.setWindowTitle(QStringLiteral("Simulation"));
    msgBox.setText(msgText);
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *clearScene = msgBox.addButton(QStringLiteral("Clear scene"), QMessageBox::ActionRole);
    QPushButton *deleteObjects = msgBox.addButton(QStringLiteral("Delete objects"), QMessageBox::ActionRole);
    msgBox.addButton(QStringLiteral("Cancel"), QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == clearScene){
        ui->SceneWidget->deleteAllObjects();
        ui->SceneWidget->deletePlatform();
    }else if(msgBox.clickedButton() == deleteObjects)
        ui->SceneWidget->deleteAllObjects();

    ui->SceneWidget->update();
    delete clearScene;
    delete deleteObjects;
}

void MainWindow::on_objectsPushButton_clicked()
{
    _addObjToScene->show();
}

void MainWindow::on_setPositionOfSelectedObj(const Objects &obj, const uint &objId, const QVector3D &position)
{
    if(obj == PLATFORM && !ui->SceneWidget->isPlatformModelCreated()) return;

    ui->SceneWidget->setPositionOfSelectedObj(obj, objId, position);
}

void MainWindow::on_updateCordLabelOfSelectedObj(const Objects &obj, const uint &objId)
{
    if(obj == PLATFORM && !ui->SceneWidget->isPlatformModelCreated()) return;

    _addObjToScene->updateCordLabel(ui->SceneWidget->getObjPosition(obj, objId)*SIMULATION_SCALE);
    if(obj == PLATFORM)
        _addObjToScene->updateRotLabel(ui->SceneWidget->getPlatformRotation());
}

void MainWindow::on_deletePlatform()
{
    _simWindow->hide();
    ui->SceneWidget->deletePlatform();
    _addObjToScene->updateLabel(ui->SceneWidget->getNumfOfObj(PLATFORM), PLATFORM);
}

void MainWindow::on_deleteSelectedObj(const Objects &obj, const uint &objId)
{
    ui->SceneWidget->deleteSelectedObj(obj, objId);
    _addObjToScene->updateLabel(ui->SceneWidget->getNumfOfObj(obj), obj);
}

void MainWindow::on_addPlatformToSceneFromAddObj(const Platform &platform)
{
    if(ui->SceneWidget->isPlatformModelCreated())
        return;
    _platform = platform;
    ui->SceneWidget->setPlatform(platform);
    ui->SceneWidget->addPlatform();
    ui->SceneWidget->update();
}

void MainWindow::on_getObjSize(const Objects &obj, const uint &objId)
{
    _addObjToScene->updateSizeWindow(ui->SceneWidget->getObjSize(obj, objId));
}

void MainWindow::on_setObjSize(const Objects &obj, const uint &objId, QVector3D const &newSize)
{
    ui->SceneWidget->setObjSize(obj, objId, newSize);
}

void MainWindow::on_addPlatformToMainScene(const Platform &platform)
{
    int flag = -1;
    if(ui->SceneWidget->isPlatformModelCreated()){
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("Simulation"));
        msgBox.setText(QStringLiteral("Replace the current platform model?"));
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *replace = msgBox.addButton("Replace", QMessageBox::ActionRole);
        msgBox.addButton("Cancel", QMessageBox::ActionRole);
        msgBox.exec();
        if(msgBox.clickedButton() == replace){
            ui->SceneWidget->deletePlatform();
            delete replace;
            flag=1;
        }else{
            delete replace; return;
        }
    }

    _platform = platform;
    _addObjToScene->set_platformLoaded(1);
    ui->SceneWidget->setPlatform(_platform);
    ui->SceneWidget->addPlatform();
    ui->SceneWidget->update();
    _platformConfWindow->hide();
    if(flag >0)
        QMessageBox::information(this, QStringLiteral("Simulation"), QStringLiteral("Platform has been replaced"));
    else
        QMessageBox::information(this, QStringLiteral("Simulation"), QStringLiteral("Platform has been added to simulation"));
}


void MainWindow::on_temperatureSpinBox_valueChanged(double arg1)
{
    air::Params.SetTemperature_Air_C(arg1);
}

void MainWindow::on_platformPositionChanged()
{
    if(!ui->SceneWidget->isPlatformModelCreated())
        return;
    QVector3D platformPosition = ui->SceneWidget->getObjPosition(PLATFORM, 1)*SIMULATION_SCALE;
    double rot_deg = ui->SceneWidget->getPlatformRotation();
    _addObjToScene->updatePlatformCordLabels(platformPosition[0], platformPosition[1], platformPosition[2], rot_deg);
}

void MainWindow::on_addDataTofigureTransmitter(QVector<double> const &time, QVector<double> const &signal, double const &time0)
{
     signalTime = time;
     signalData = signal;
     _transmitterDialog->clearData();
    _transmitterDialog->addData(time, signal, time0);
}

void MainWindow::on_recData(const QVector<double> &time, const QVector<QVector<double>> &recSignals, double const &time0)
{
    assert(!_receiverDialogList.empty());
    recTime = time;
    recSignalData = recSignals;
    for(uint i=0; i<recSignals.size(); i++)
        _receiverDialogList[i]->addData(time, recSignals[i], time0);
}

void MainWindow::on_setRotationOfPlatform(const double &rot_deg)
{
    ui->SceneWidget->setPatformRotation(rot_deg);
}

void MainWindow::on_simulation(const uint &idActiveSonar)
{
    ui->SceneWidget->getPlatformActualPosition(_platform);
    _platform.setID_ActiveSonar(idActiveSonar);
    ui->SceneWidget->getObjPosition(_objPositions, _platform.getActveSonar());
    if(_objPositions.empty()){
        QString msg=QString("All objects out of range Sonar %1").arg(idActiveSonar+1);
        QMessageBox::information(this, "Simulation", msg);
        return;
    }
    addRecDialogs();
    addRecMenu();
    _simulation->setPlatform(_platform);
    _simulation->execMeasurement(_objPositions, idActiveSonar);
    QString msg = "Simulation finished\n";
    msg += QString("Active sonar: %1\n").arg(idActiveSonar+1);
    msg += QString("Number of objects in range: %1").arg(_objPositions.size());

    QMessageBox::information(this, "Simulation", msg);
    detectZeroCrossing();
}

void MainWindow::on_recFigureAction()
{
    for(uint i=0; i<actions.size(); i++){
        if(actions[i]->isChecked()){
            _receiverDialogList[i]->show();
            actions[i]->setChecked(false);
        }else
            _receiverDialogList[i]->hide();
    }
}

void MainWindow::readLineToValues(QString const &readline)
{
    std::istringstream istrm;
    istrm.str(readline.toStdString());
    double dist=0, aElev=0, aAzim=0;
    istrm >> dist >> aAzim >> aElev;
    if(istrm.fail())
        return;

    distances.push_back(dist);
    angleAzimuts.push_back(aAzim);
    angleElevations.push_back(aElev);
}
void MainWindow::on_changeSonarColor(const uint &activeSonar)
{
    if(!ui->SceneWidget->isPlatformModelCreated())
        return;
    ui->SceneWidget->changActiveSonarColor(activeSonar);
}

void MainWindow::calculatePos()
{
    detectedPos.clear();
    QVector3D tranPos = _platform.getActveSonar().getTransmitter().get_GlPos()/SIMULATION_SCALE;
    for(uint i=0; i<distances.size(); i++){
        double dist = distances[i]/SIMULATION_SCALE/2;
        double aAzim = angleAzimuts[i]*M_PI/180;
        double aEle = angleElevations[i]*M_PI/180;
        double x = dist*std::cos(aEle)*std::sin(aAzim);
        double y = dist*std::sin(aEle);
        double z = dist*std::cos(aAzim)*std::cos(aEle);

        QVector3D objPos(x,y,z);
        QMatrix4x4 rMatrix;
        rMatrix.rotate(ui->SceneWidget->getPlatformRotation(), 0,1,0);
        objPos = rMatrix.map(objPos);
        objPos += tranPos;
        detectedPos.push_back(objPos);
    }
}

void MainWindow::addDetectedPoints()
{
    ui->SceneWidget->clearDetectedPoints();
    for(auto const & detectedPointPos : detectedPos)
        ui->SceneWidget->addDetectedPoint(detectedPointPos);
}

void MainWindow::clearDetectedPoints()
{
    ui->SceneWidget->clearDetectedPoints();
}

void MainWindow::savePosRecToFile()
{
    QDateTime date;
    date = date.currentDateTime();
    QString path = "/home/dominik/praca_inzynierska/Symulator_Sonaru_3D/Symulator_sonaru/RecPosData/";
    QString fileName = "recPos_"+date.toString("yyyy_MM_dd_hh_mm_ss")+".cfg";
    path += fileName;
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this,"Simulation", "Failed save data to file");
        return;
    }
    QTextStream out(&file);
    for(uint sonId=0; sonId < _platform.get_NumOfSonars(); sonId++){
        for(uint recId=0; recId < _platform.getSonar(sonId).get_numOfRec(); recId++){
            QVector3D recPos = _platform.getSonar(sonId).getReceiver(recId).get_LocPos();
            out << recPos.x() << " " << recPos.y() << " " << recPos.z() << "\n";
        }
    }

    file.close();
}

void MainWindow::setConnections()
{
    connect(_sonarConfWindow, &SonarConfWindow::addSonarToPlatformConfWindow,
            _platformConfWindow, &PlatformConfWindow::on_AddSonarToPlatformConfWindow);
    connect(_platformConfWindow, &PlatformConfWindow::addPlatformToMainScene, this, &MainWindow::on_addPlatformToMainScene);
    connect(ui->SceneWidget, &MainScene::platformPositionChanged, this, &MainWindow::on_platformPositionChanged);
    connect(_simulation.get(), &Simulation::startSimulationThread, _simThread, &SimulationThread::on_simulationStarted);
    connect(_simThread, &SimulationThread::finished, _simThread, &QObject::deleteLater);
    connect(this, &MainWindow::stopSimulation, _simulation.get(), &Simulation::on_stopSimulation);
    connect(_simulation.get(), &Simulation::recData, this, &MainWindow::on_recData);
    connect(_simulation.get(), &Simulation::addTransmitterData, this, &MainWindow::on_addDataTofigureTransmitter);
    connect(_platformConfWindow, &PlatformConfWindow::hideSonarWindow, _sonarConfWindow, &SonarConfWindow::hide);
    setAddObjectConnections();
    connect(_simWindow, &SimulationWindow::startSimulation, this, &MainWindow::on_simulation);
    connect(_simWindow, &SimulationWindow::changeSonarColor, this, &MainWindow::on_changeSonarColor);
    connect(ui->SceneWidget, &MainScene::objPositionChanged, this, &MainWindow::on_objPositionChanged);
}

void MainWindow::setAddObjectConnections()
{
    connect(_addObjToScene, &ObjToScene::objct2Add, this, &MainWindow::on_objc2Add);
    connect(_addObjToScene, &ObjToScene::updateNumLabelOfSelectedObj, this, &MainWindow::on_updateNumLabelOfSelectedObj);
    connect(_addObjToScene, &ObjToScene::setPostionOfSelectedObj, this, &MainWindow::on_setPositionOfSelectedObj);
    connect(_addObjToScene, &ObjToScene::updateCordLabelOfSelectedObj, this, &MainWindow::on_updateCordLabelOfSelectedObj);
    connect(_addObjToScene, &ObjToScene::deletePlatform, this, &MainWindow::on_deletePlatform);
    connect(_addObjToScene, &ObjToScene::deleteSelectedObj, this, &MainWindow::on_deleteSelectedObj);
    connect(_addObjToScene, &ObjToScene::addPlatformToScene, this, &MainWindow::on_addPlatformToSceneFromAddObj);
    connect(_addObjToScene, &ObjToScene::getObjSize, this, &MainWindow::on_getObjSize);
    connect(_addObjToScene, &ObjToScene::setObjSize, this, &MainWindow::on_setObjSize);
    connect(_addObjToScene, &ObjToScene::setRotationOfPlatform, this, &MainWindow::on_setRotationOfPlatform);
    connect(_addObjToScene, &ObjToScene::selectObj, this, &MainWindow::on_selectObj);
}

void MainWindow::addRecDialogs()
{

    if(_receiverDialogList.size() > _platform.get_NumOfReceiver()){
        while(_receiverDialogList.size() != _platform.get_NumOfReceiver()){
            _receiverDialogList.back()->deleteLater();
            _receiverDialogList.back() = nullptr;
            delete _receiverDialogList.back();
            _receiverDialogList.pop_back();
        }
    }

    if(_receiverDialogList.size() < _platform.get_NumOfReceiver()){
        uint needToAdd = _platform.get_NumOfReceiver() - _receiverDialogList.size();
        for(uint i=0; i<needToAdd; i++){
            _receiverDialogList.push_back(new plotDialog);
        }
    }

    QString windowTitle;
    uint k=0;
    for(uint i=0; i < _platform.get_NumOfSonars(); i++){
        for(uint j=0; j<_platform.getSonar(i).get_numOfRec(); j++){
                windowTitle =  QString("Sonar %1 receiver %2").arg(i+1).arg(j+1);
                _receiverDialogList[k]->setTitle(windowTitle);
            ++k;
        }
    }

    for(auto &dialog : _receiverDialogList){
        if(!dialog->isMakedPlot())
            dialog->makePlot("Received signal");
    }

    for(auto &dialog : _receiverDialogList)
        dialog->clearData();
}

void MainWindow::addRecMenu()
{
    if(!actions.empty()){
        for(auto &act : actions){
            act->deleteLater();
            act = nullptr;
            delete act;
            actions.pop_back();
        }
    }

    if(isRecMenuCreated){
        receiverMenu->deleteLater();
        receiverMenu = nullptr;
    }

    receiverMenu = std::make_shared<QMenu>("Receiver figures");
    figuresMenu->addMenu(receiverMenu.get());
    receiverMenu->addAction("All figures", this, SLOT(on_receiverFiguresAction()));
    for(uint i=0; i < _platform.get_NumOfReceiver(); i++){
        actions.push_back(receiverMenu->addAction(QString("Receiver %1").arg(i+1), this, SLOT(on_recFigureAction())));
        actions.back()->setCheckable(true);
        actions.back()->setChecked(false);
    }

    isRecMenuCreated = true;
}

void MainWindow::save()
{
    for(uint rec=0; rec<recSignalData.size(); rec++){
        QString path = "/home/dominik/praca_inzynierska/Symulator_Sonaru_3D/Symulator_sonaru/ZeroCrossingData/";
        QString fileName = "rec_" + QString::number(rec) + "_signal.dat";
        path += fileName;
        QFile file(path);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            file.close();
            QMessageBox::critical(this,"Simulation", "Failed save data to file");
            return;
        }
        QTextStream out(&file);
        for(uint i=0; i<recSignalData[rec].size()-1; i++){
            out << recTime[i] << "   " << recSignalData[rec][i] << "\n";
        }
        file.close();
    }

}

void MainWindow::saveDetectZeroCrossingToFile(QVector<QVector<int>> timeCrossing)
{
    normalizeTimeCrossingData(timeCrossing);
    QDateTime date;
    date = date.currentDateTime();
    QString path = "/home/dominik/praca_inzynierska/Symulator_Sonaru_3D/Symulator_sonaru/ZeroCrossingData/";
    QString fileName = "data_"+date.toString("yyyy_MM_dd_hh_mm_ss")+".dat";
    path += fileName;
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this,"Simulation", "Failed save data to file");
        return;
    }

    QTextStream out(&file);
    for(auto &cros : timeCrossing){
        qDebug() << cros.size();
    }
    //out << _platform.get_NumOfReceiver() << "\n";\timeCrossing[rec].size()
    out << timeCrossing[0].size() << "\n";
    for(uint rec=0; rec<timeCrossing.size(); rec++){
        for(uint i=0; i<timeCrossing[rec].size(); i++)
            out << timeCrossing[rec][i] << "\n";
    }

    file.close();
    QString information = "Data save in file: " + fileName;
    QMessageBox::information(this, "Simulation", information);
}

void MainWindow::detectZeroCrossing()
{
    save();
    QVector<QVector<int>> zeroCrossingTime;
    QVector<int> recZerosCross;
    for(uint rec=0; rec<recSignalData.size(); rec++){
        for(uint i=0; i<recSignalData[rec].size()-1; i++){
            if(recSignalData[rec][i] < 0 &&  recSignalData[rec][i+1] > 0){
                double tx = recTime[i] + (((recTime[i+1]-recTime[i])/(recSignalData[rec][i+1]-recSignalData[rec][i])) * std::fabs(recSignalData[rec][i]));
                recZerosCross.push_back(static_cast<int>(tx/dt));
            }
//            else if(recSignalData[rec][i] > 0 &&  recSignalData[rec][i+1] < 0){
//                double tx = recTime[i+1] + (((recTime[i]-recTime[i+1])/(recSignalData[rec][i]-recSignalData[rec][i+1])) * std::fabs(recSignalData[rec][i+1]));
//                recZerosCross.push_back(static_cast<int>(tx/dt));
//            }
        }
        zeroCrossingTime.push_back(recZerosCross);
        recZerosCross.clear();
    }
    saveDetectZeroCrossingToFile(zeroCrossingTime);
    savePosRecToFile();
}

void MainWindow::normalizeTimeCrossingData(QVector<QVector<int> > &timeCrossing)
{
    uint currentSize = std::numeric_limits<uint>::max();

    for(uint rec=0; rec<timeCrossing.size(); rec++){
        if(currentSize > timeCrossing[rec].size())
            currentSize = timeCrossing[rec].size();
    }

    for(uint rec=0; rec<timeCrossing.size(); rec++){
        while(timeCrossing[rec].size() > currentSize)
            timeCrossing[rec].pop_back();
    }

}


void MainWindow::on_clearDetectionPushButton_clicked()
{

    QMessageBox msgBox;
    msgBox.setWindowTitle("Simulation");
    msgBox.setText("Clear detecion?");
    QPushButton *clear = msgBox.addButton("Clear", QMessageBox::ActionRole);
    msgBox.addButton("Cancel", QMessageBox::ActionRole);
    msgBox.exec();
    if(msgBox.clickedButton() == clear){
        ui->SceneWidget->clearDetectedPoints();
        ui->SceneWidget->deleleteDetection();
    }

    delete clear;
}

void MainWindow::on_objPositionChanged(const Objects &obj, const int &id)
{
    QVector3D pos;
    if(id < 0 && obj != PLATFORM)
        return;
    switch (obj) {
    case PLATFORM:
        on_platformPositionChanged();
        break;
    case POINT:
        pos = ui->SceneWidget->getObjPosition(obj, id)*SIMULATION_SCALE;
        _addObjToScene->updateCordLabel(obj, id, pos);
        break;
    case STAKE:
        pos = ui->SceneWidget->getObjPosition(obj, id)*SIMULATION_SCALE;
        _addObjToScene->updateCordLabel(obj, id, pos);
        break;
    case FLAT_SURFACE:
        pos = ui->SceneWidget->getObjPosition(obj, id)*SIMULATION_SCALE;
        _addObjToScene->updateCordLabel(obj, id, pos);
        break;
    default:
        break;
    }
}

void MainWindow::on_selectObj(const Objects &obj, const int &id)
{
    ui->SceneWidget->setSelectedObj(obj, id);
}


void MainWindow::on_DetectionPushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open file"));
    }else{
        distances.clear();
        angleElevations.clear();
        angleAzimuts.clear();
        QTextStream in(&file);
        while(!in.atEnd()){
            readLineToValues(in.readLine());
        }
    }

    file.close();
    calculatePos();
    addDetectedPoints();
}

