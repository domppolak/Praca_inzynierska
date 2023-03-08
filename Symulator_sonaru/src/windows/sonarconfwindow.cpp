#include "sonarconfwindow.hh"
#include "ui_sonarConfWindow.h"
#include <QDebug>
#include <QVector3D>
#include <QMessageBox>
#include "constant.hh"


QString color[] = {"Green", "Blue", "Purple", "White", "Orange", "Brown", "Cyan"};
QVector3D color_vec[] {QVector3D(0,1,0), QVector3D(0,0,1), QVector3D(0.5,0,0.5),
            QVector3D(1,1,1), QVector3D(1,0.5,0), QVector3D(0.55,0.27,0.07), QVector3D(0,1,1)};

SonarConfWindow::SonarConfWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SonarConfWindow)
{
    ui->setupUi(this);
    setToolBar();

    _signalConfWindow = new SignalConfWindown;
    sonarSizeWindow = new ChangeSizeWindow;

    setConnections();
    ui->num_label->setText(QStringLiteral("Number of receivers: %1").arg(_sonar.get_numOfRec()));
    ui->idRecSpinBox->setMaximum(_sonar.get_numOfRec());
    ui->idRecSpinBox->setMinimum(0);
    ui->xCordSpinBox->setMinimum(-1000);
    ui->yCordSpinBox->setMinimum(-1000);
    ui->zCordSpinBox->setMinimum(-1000);
    ui->xCordSpinBox->setMaximum(1000);
    ui->yCordSpinBox->setMaximum(1000);
    ui->zCordSpinBox->setMaximum(1000);
    ui->xCordSpinBox->setValue(0);
    ui->yCordSpinBox->setValue(0);
    ui->zCordSpinBox->setValue(0);

    ui->sonarColor_comboBox->addItem("Green");
    ui->sonarColor_comboBox->addItem("Blue");
    ui->sonarColor_comboBox->addItem("Purple");
    ui->sonarColor_comboBox->addItem("White");
    ui->sonarColor_comboBox->addItem("Orange");
    ui->sonarColor_comboBox->addItem("Brown");
    ui->sonarColor_comboBox->addItem("Cyan");
    ui->sonarColor_comboBox->setCurrentIndex(0);
}

SonarConfWindow::~SonarConfWindow()
{
    delete _signalConfWindow;
    delete sonarSizeWindow;
    delete ui;
}

void SonarConfWindow::cordLabelUpdate(double x, double y, double z)
{
    ui->xCordSpinBox->setValue(x*SIMULATION_SCALE);
    ui->yCordSpinBox->setValue(y*SIMULATION_SCALE);
    ui->zCordSpinBox->setValue(z*SIMULATION_SCALE);
}

void SonarConfWindow::setConnections()
{
    connect(this, &SonarConfWindow::numRecChanged, this, &SonarConfWindow::on_numRecChanged);
    connect(sonarSizeWindow, &ChangeSizeWindow::sonarSizeChanged, this, &SonarConfWindow::onSonarSizeChanged);
    connect(_signalConfWindow, &SignalConfWindown::signalParamChanged,
            this, &SonarConfWindow::onSignalParamChanged);
    connect(ui->SceneWidget, &SonarConfScene::recPositionChanged, this, &SonarConfWindow::on_receiverPositionChanged);
}

void SonarConfWindow::setToolBar()
{
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction("Save", this, SLOT(on_saveAction()));
    toolbar->addAction("Open file", this, SLOT(on_openAction()));
    toolbar->addAction("Add to platform", this, SLOT(on_addAction()));
    toolbar->addAction("Transmitter", this, SLOT(on_transAction()));
    layout()->setMenuBar(toolbar);
}

void SonarConfWindow::updateScene()
{
    ui->SceneWidget->clearScene();
    ui->SceneWidget->addReadConfSonar(_sonar);
    ui->SceneWidget->update();
    emit numRecChanged();
}

bool SonarConfWindow::deleteReceiverMsg()
{
    if(_idUpdatedRec < 0)
        return false;

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Delete receiver"));
    msgBox.setText(QStringLiteral("Delete receiver %1 ?").arg(_idUpdatedRec+1));
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *yes = msgBox.addButton(QStringLiteral("Yes"), QMessageBox::ActionRole);
    msgBox.addButton(QStringLiteral("No"), QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == yes){
        delete yes;
        return true;
    }

    delete yes;
    return false;
}

void SonarConfWindow::updateRecId()
{
    for(uint i=0; i < _sonar.get_numOfRec(); i++){
        _sonar.useReceiver(i).set_id(i);
        ui->SceneWidget->setReceiverId(i, i);
    }
}

void SonarConfWindow::on_saveAction()
{

    ui->SceneWidget->getSonarParam(_sonar);

    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)") + ".xml";
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to save file"));
        return;
    }

    _fileManager.saveSonarConf(file, _sonar);
    file.close();
}

void SonarConfWindow::on_openAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open sonar configuration file"));
        return;
    }else{
        Sonar readSonar;
        if(_fileManager.readSonarConf(file, readSonar)){
            _sonar = readSonar;
            updateScene();
            _signalConfWindow->setSignalParam(_sonar.getTransmittedSignal());
        }else
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a sonar configuration file!"));
    }

    file.close();
}

void SonarConfWindow::on_addAction()
{
    qDebug() << "add";
    ui->SceneWidget->getSonarParam(_sonar);
    emit addSonarToPlatformConfWindow(_sonar);
}

void SonarConfWindow::on_transAction()
{
    _signalConfWindow->setSignalParam(_sonar.getTransmittedSignal());
    _signalConfWindow->show();
}

void SonarConfWindow::on_addPushButton_clicked()
{  
    _sonar.addReceiver();
    ui->SceneWidget->addReceiver();
    ui->SceneWidget->update();
    emit numRecChanged();
}

void SonarConfWindow::on_deletePushButton_clicked()
{
    if(_idUpdatedRec < 0 && _sonar.get_numOfRec() > 0){
        QMessageBox::critical(this, QStringLiteral("Delete receiver"), QStringLiteral("Select receiver to delete"));
        return;
    }

    if(!deleteReceiverMsg() || _idUpdatedRec < 0)
        return;

    if(_sonar.deleteReceiver(_idUpdatedRec)){
        ui->SceneWidget->deleteReceiver(_idUpdatedRec);
        ui->SceneWidget->update();
        updateRecId();
        emit numRecChanged();
    }
}

void SonarConfWindow::on_numRecChanged()
{
    ui->num_label->setText(QStringLiteral("Number of receivers: %1").arg(_sonar.get_numOfRec()));
    ui->idRecSpinBox->setMaximum(_sonar.get_numOfRec());
}

void SonarConfWindow::updateRecCord(double x_mm, double y_mm, double z_mm)
{
    if(_idUpdatedRec < 0)
        return;

    ui->SceneWidget->setReceiverPosition(_idUpdatedRec, QVector3D(x_mm, y_mm, z_mm)/SIMULATION_SCALE);
    ui->SceneWidget->update();
}

void SonarConfWindow::on_idRecSpinBox_valueChanged(int arg1)
{
    if(_sonar.get_numOfRec() == 0 || arg1 == 0){
        _idUpdatedRec = -1;
        cordLabelUpdate(0.0, 0.0, 0.0);
        ui->idRecSpinBox->setValue(0);
    }else{
        _idUpdatedRec = arg1-1;
        ui->SceneWidget->selectReceiver(_idUpdatedRec);
        QVector3D recCenterPosition = ui->SceneWidget->getReceiverPosition(_idUpdatedRec);
        cordLabelUpdate(recCenterPosition[0], recCenterPosition[1], recCenterPosition[2]);
    }
}

void SonarConfWindow::on_xCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(arg1, ui->yCordSpinBox->value(), ui->zCordSpinBox->value());
}

void SonarConfWindow::on_yCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(ui->xCordSpinBox->value(), arg1, ui->zCordSpinBox->value());
}

void SonarConfWindow::on_zCordSpinBox_valueChanged(double arg1)
{
    updateRecCord(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), arg1);
}

void SonarConfWindow::on_receiverPositionChanged(QVector3D position)
{
    cordLabelUpdate(position[0], position[1], position[2]);
}

void SonarConfWindow::on_sonarColor_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->SceneWidget->isSceneInitialized() < 0)
        return;

    for(uint i=0; i<7; ++i){
        if(arg1 == color[i]){
            ui->SceneWidget->changeSonarColor(color_vec[i]);
        }
    }
}

void SonarConfWindow::onSonarSizeChanged()
{
    if(sonarSizeWindow->isSizeAccepted()){
        sonarSizeWindow->hide();
        ui->SceneWidget->setSonarScale(sonarSizeWindow->getSize()/SIMULATION_SCALE);
        sonarSizeWindow->setSizeAccepted(false);
    }
}

void SonarConfWindow::on_sizePushButton_clicked()
{
    sonarSizeWindow->show();
    sonarSizeWindow->updateLabel(ui->SceneWidget->getSonarScale()*SIMULATION_SCALE);
}

void SonarConfWindow::onSignalParamChanged()
{
    _signalConfWindow->getSignalParam(_sonar.useTransmittedSignal());
}

