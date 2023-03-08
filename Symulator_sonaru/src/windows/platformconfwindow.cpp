#include "platformconfwindow.hh"
#include "ui_platformconfwindow.h"
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include "constant.hh"

PlatformConfWindow::PlatformConfWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatformConfWindow)
{
    ui->setupUi(this);
    sizeWindow = new ChangeSizeWindow;

    setToolBar();
    setConnections();

    ui->idSonarSpinBox->setMinimum(0);
    ui->xCordSpinBox->setMinimum(-10000);
    ui->yCordSpinBox->setMinimum(-10000);
    ui->zCordSpinBox->setMinimum(-10000);
    ui->xCordSpinBox->setMaximum(10000);
    ui->yCordSpinBox->setMaximum(10000);
    ui->zCordSpinBox->setMaximum(10000);
    ui->rotSpinBox->setMinimum(0);
    ui->rotSpinBox->setMaximum(360);

    SonarNumChanged();
    codLabelUpdate(0,0,0,0);
}

PlatformConfWindow::~PlatformConfWindow()
{
    delete ui;
    delete sizeWindow;
}

void PlatformConfWindow::codLabelUpdate(double x, double y, double z, double rot)
{

    ui->xCordSpinBox->setValue(x*SIMULATION_SCALE);
    ui->yCordSpinBox->setValue(y*SIMULATION_SCALE);
    ui->zCordSpinBox->setValue(z*SIMULATION_SCALE);
    ui->rotSpinBox->setValue(rot);
}

void PlatformConfWindow::setConnections()
{
    connect(sizeWindow, &ChangeSizeWindow::sonarSizeChanged, this, &PlatformConfWindow::onPlatformSizeChanged);
    connect(ui->SceneWidget, SIGNAL(sonarPositionChanged(QVector3D,double)), this, SLOT(on_sonarPositionChanged(QVector3D,double)));
}

void PlatformConfWindow::setToolBar()
{
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction("Save", this, SLOT(on_saveAction()));
    toolbar->addAction("Open file", this, SLOT(on_openAction()));
    toolbar->addAction("Add to simulation", this, SLOT(on_addAction()));
    layout()->setMenuBar(toolbar);
}

void PlatformConfWindow::updateScene()
{
    ui->SceneWidget->clearScene();
    ui->SceneWidget->addReadConfPlatform(_platform);
    ui->SceneWidget->update();
    SonarNumChanged();
}

void PlatformConfWindow::onPlatformSizeChanged()
{
    if(!sizeWindow->isSizeAccepted())
        return;

    sizeWindow->hide();
    ui->SceneWidget->setPlatformScale(sizeWindow->getSize()/SIMULATION_SCALE);
    ui->SceneWidget->update();
    sizeWindow->setSizeAccepted(false);
}

void PlatformConfWindow::setSonarPosition(double x_mm, double y_mm, double z_mm, double newRotation)
{
    if(_idUpdatedSonar < 0) return;

    ui->SceneWidget->setSonarPosition(_idUpdatedSonar, QVector3D(x_mm, y_mm, z_mm)/SIMULATION_SCALE);
    ui->SceneWidget->update();
    ui->SceneWidget->setSonarRotation(_idUpdatedSonar, newRotation);

}

bool PlatformConfWindow::deleteSonarMsg()
{
    if(_idUpdatedSonar < 0)
        return false;

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Delete sonar"));
    msgBox.setText(QStringLiteral("Delete sonar %1 ?").arg(_idUpdatedSonar+1));
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *yes = msgBox.addButton(QStringLiteral("Yes"), QMessageBox::ActionRole);
    QPushButton *no = msgBox.addButton(QStringLiteral("No"), QMessageBox::ActionRole);
    msgBox.exec();

    bool isDelete = false;
    if(msgBox.clickedButton() == yes)
        isDelete =  true;
    else if(msgBox.clickedButton() == no)
        isDelete =  false;

    delete yes;
    delete no;
    return isDelete;
}

void PlatformConfWindow::updateSonarId()
{
    for(uint i=0; i < _platform.get_NumOfSonars(); i++){
        _platform.useSonar(i).set_id(i);
        ui->SceneWidget->setSonarId(i, i+1);
    }
}

void PlatformConfWindow::addSonar()
{
    if(_sonarIsLoaded < 0)
        return;

    _platform.addSonar(_loadedSonar);
    ui->SceneWidget->addSonar(_loadedSonar);
    ui->SceneWidget->update();
    updateSonarId();
    SonarNumChanged();
}

void PlatformConfWindow::on_loadSonarPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to load sonar configuration"));
    }else{
        Sonar readSonar;
        if(_fileManager.readSonarConf(file, _loadedSonar)){
            _loadedSonar = readSonar;
            ui->loadSonarPushButton->setText("Sonar is loaded");
            _sonarIsLoaded = 1;
        }else
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a sonar configuration file!"));
    }

    file.close();
}


void PlatformConfWindow::on_addSonarPushButton_clicked()
{
    addSonar();
}


void PlatformConfWindow::on_deletePushButton_clicked()
{

    if(_idUpdatedSonar < 0 && _platform.get_NumOfSonars() > 0){
        QMessageBox::critical(this, QStringLiteral("Delete sonar"), QStringLiteral("Select sonar to delete"));
        return;
    }

    if(!deleteSonarMsg() || _idUpdatedSonar < 0){
        return;
    }

    if(_platform.deleteSonar(_idUpdatedSonar)){
        ui->SceneWidget->deleteSonar(_idUpdatedSonar);
        updateSonarId();
        SonarNumChanged();
    }
}

void PlatformConfWindow::on_xCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(arg1, ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void PlatformConfWindow::on_yCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(ui->xCordSpinBox->value(), arg1, ui->zCordSpinBox->value(), ui->rotSpinBox->value());
}

void PlatformConfWindow::on_zCordSpinBox_valueChanged(double arg1)
{
    setSonarPosition(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), arg1, ui->rotSpinBox->value());
}

void PlatformConfWindow::on_rotSpinBox_valueChanged(double arg1)
{
    if(arg1 == 360){
        arg1 = 0;
        ui->rotSpinBox->setValue(0);
    }

    setSonarPosition(ui->xCordSpinBox->value(), ui->yCordSpinBox->value(), ui->zCordSpinBox->value(), arg1);
}

void PlatformConfWindow::on_sizePushButton_clicked()
{
    sizeWindow->show();
    sizeWindow->updateLabel(ui->SceneWidget->getPlatformScale()*SIMULATION_SCALE);
}

void PlatformConfWindow::on_saveAction()
{
    ui->SceneWidget->getPlatformParam(_platform);

    QString fileName = QFileDialog::getSaveFileName(this, "Save Xml", ".", "Xml files (*.xml)") + ".xml";
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Cannot save file!"));
        return;
    }

    _fileManager.savePlatformConf(file, _platform);
    file.close();
}

void PlatformConfWindow::on_openAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open platform configuration file"));
        return;
    }else{
        Platform readPlatform;
        if(_fileManager.readPlatformConf(file, readPlatform)){
            _platform = readPlatform;
            updateScene();
        }
        else
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a platform configuration file!"));
    }

    file.close();
}

void PlatformConfWindow::on_addAction()
{
    ui->SceneWidget->getPlatformParam(_platform);
    emit addPlatformToMainScene(_platform);
}

void PlatformConfWindow::on_sonarPositionChanged(QVector3D position, double rotation)
{
    double rot = rotation;
    if(rot < 0)
        rot +=  360;

    codLabelUpdate(position[0], position[1], position[2], rot);
}

void PlatformConfWindow::SonarNumChanged()
{
    ui->num_label->setText(QStringLiteral("Number of sonar: %1").arg(_platform.get_NumOfSonars()));
    ui->idSonarSpinBox->setMaximum(_platform.get_NumOfSonars());
}

void PlatformConfWindow::on_AddSonarToPlatformConfWindow(const Sonar &sonar)
{
    _loadedSonar = sonar;
    ui->loadSonarPushButton->setText("Sonar is loaded");
    _sonarIsLoaded = 1;
    show();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Platform configuration");
    msgBox.setText("Sonar is loaded. Add sonar to the configuration?");
    msgBox.setIcon(QMessageBox::Information);
    QPushButton *add = msgBox.addButton("Add", QMessageBox::ActionRole);
    msgBox.addButton("Cancel", QMessageBox::ActionRole);
    msgBox.exec();

    if(msgBox.clickedButton() == add){
        emit hideSonarWindow();
        addSonar();
    }

    delete add;
}

void PlatformConfWindow::on_idSonarSpinBox_valueChanged(int arg1)
{
    if(_platform.get_NumOfSonars() == 0 || arg1 == 0){
        _idUpdatedSonar =  -1;
        codLabelUpdate(0.0, 0.0, 0.0, 0.0);
        ui->idSonarSpinBox->setValue(0);        
    }else{
        _idUpdatedSonar = arg1-1;
        ui->SceneWidget->selectSonar(_idUpdatedSonar);
        QVector3D sonarPosition = ui->SceneWidget->getSonarPosition(_idUpdatedSonar);
        double sonarRotation = ui->SceneWidget->getSonarRotation(_idUpdatedSonar);
        codLabelUpdate(sonarPosition[0],sonarPosition[1],sonarPosition[2],sonarRotation);
    }
}

