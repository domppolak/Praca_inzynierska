#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QAction>

#include "sonarconfwindow.hh"
#include "platformconfwindow.hh"
#include "objtoscene.hh"
#include "plotdialog.hh"
#include "simulation.hh"
#include "simulationthread.hh"
#include <QThread>
#include "simulationwindow.hh"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setMenuBar();
private slots:
    void on_saveAction();
    void on_sonarAction();
    void on_platformAction();
    void on_transmiterFigureAction();
    void on_receiverFiguresAction();
    void on_startSimPushButton_clicked();
    void on_objc2Add(Objects const &obj);
    void on_updateNumLabelOfSelectedObj(Objects const &obj);
    void on_clearScenePushButton_clicked();
    void on_objectsPushButton_clicked();
    void on_setPositionOfSelectedObj(Objects const &obj, uint const &objId, QVector3D const &position);
    void on_updateCordLabelOfSelectedObj(Objects const &obj, uint const &objId);
    void on_deletePlatform();
    void on_deleteSelectedObj(Objects const &obj, uint const &objId);
    void on_addPlatformToSceneFromAddObj(Platform const &platform);
    void on_getObjSize(Objects const &obj, uint const &objId);
    void on_setObjSize(Objects const &obj, uint const &objId, QVector3D const &newSize);
    void on_addPlatformToMainScene(Platform const &platform);
    void on_temperatureSpinBox_valueChanged(double arg1);
    void on_platformPositionChanged();
    void on_addDataTofigureTransmitter(QVector<double> const &time, QVector<double> const &signal, double const &time0);
    void on_recData(QVector<double> const &time, QVector<QVector<double>> const &recSignals, double const &time0);
    void on_setRotationOfPlatform(const double &rot_deg);
    void on_simulation(uint const &idActiveSonar);
    void on_recFigureAction();
    void on_changeSonarColor(uint const &activeSonar);
    void on_openAction();
    void on_clearDetectionPushButton_clicked();
    void on_objPositionChanged(Objects const &obj, int const &id);
    void on_selectObj(Objects const &obj, int const &id);
    void on_DetectionPushButton_2_clicked();

signals:
    void stopSimulation();

private:
    void setConnections();
    void setAddObjectConnections();
    void addRecDialogs();
    void addRecMenu();
    void saveDetectZeroCrossingToFile(QVector<QVector<int>> timeCrossing);
    void detectZeroCrossing();
    void normalizeTimeCrossingData(QVector<QVector<int>> &timeCrossing);
    void readLineToValues(QString const &readline);
    void calculatePos();
    void addDetectedPoints();
    void clearDetectedPoints();
    void savePosRecToFile();
    void save();

    Ui::MainWindow *ui;
    SonarConfWindow *_sonarConfWindow;
    PlatformConfWindow *_platformConfWindow;
    ObjToScene *_addObjToScene;
    SimulationWindow *_simWindow;
    std::shared_ptr<Simulation> _simulation;
    SimulationThread *_simThread;

    std::shared_ptr<QMenu> figuresMenu;
    std::shared_ptr<QMenu> receiverMenu;
    QVector<std::shared_ptr<QMenu>> recMenu;

    Platform _platform;
    plotDialog *_transmitterDialog;
    QVector<plotDialog *> _receiverDialogList;
    QThread *_plotDialogThread;

    QVector<QVector3D> _objPositions;
    bool isRecMenuCreated = false;
    QVector<QAction *> actions;

    QVector<double> recTime;
    QVector<double> signalTime;
    QVector<double> signalData;
    QVector<QVector<double>> recSignalData;
    QVector<double> distances;
    QVector<double> angleElevations;
    QVector<double> angleAzimuts;
    QVector<QVector3D> detectedPos;

};
#endif // MAINWINDOW_HH
