#include "simulationwindow.hh"
#include "ui_simulationwindow.h"

SimulationWindow::SimulationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationWindow)
{
    ui->setupUi(this);
    ui->activeSonarSpinBox->setMinimum(1);
    ui->activeSonarSpinBox->setMaximum(1);
}

SimulationWindow::~SimulationWindow()
{
    delete ui;
}

void SimulationWindow::setMaxActiveSonar(const uint &numOfSonars)
{
    ui->activeSonarSpinBox->setMaximum(numOfSonars);
}

void SimulationWindow::on_activeSonarSpinBox_valueChanged(int arg1)
{
    activeSonarId = arg1-1;
    emit changeSonarColor(activeSonarId);
}


void SimulationWindow::on_Simulation_clicked()
{
    hide();
    emit startSimulation(activeSonarId);
}


void SimulationWindow::on_Cancel_clicked()
{
    hide();
}

