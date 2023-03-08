#ifndef SIMULATIONWINDOW_HH
#define SIMULATIONWINDOW_HH

#include <QDialog>

namespace Ui {
class SimulationWindow;
}

class SimulationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationWindow(QWidget *parent = nullptr);
    ~SimulationWindow();
    void setMaxActiveSonar(uint const &numOfSonars);
signals:
    void startSimulation(uint const &activeSonar);
    void changeSonarColor(uint const &activeSonar);

private slots:
    void on_activeSonarSpinBox_valueChanged(int arg1);
    void on_Simulation_clicked();
    void on_Cancel_clicked();

private:
    Ui::SimulationWindow *ui;
    uint activeSonarId = 0;
};

#endif // SIMULATIONWINDOW_HH
