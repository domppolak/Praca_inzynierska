#ifndef SONARCONFWINDOW_HH
#define SONARCONFWINDOW_HH

#include <QDialog>
#include <QtWidgets>
#include "sonar.hh"
#include "signalconfwindown.hh"
#include "filemanager.hh"
#include "changesizewindow.hh"
#include <memory>

namespace Ui {
class SonarConfWindow;
}

class SonarConfWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SonarConfWindow(QWidget *parent = nullptr);
    ~SonarConfWindow();

private:
    void cordLabelUpdate(double x, double y, double z);
    void setConnections();
    void setToolBar();
    void updateScene();
    bool deleteReceiverMsg();
    void updateRecId();

signals:
    void numRecChanged();
    void recCordChanged(double x_mm, double y_mm, double z_mm);
    void addSonarToPlatformConfWindow(Sonar const &sonar);

private slots:
    void on_saveAction();
    void on_openAction();
    void on_addAction();
    void on_transAction();
    void on_addPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_numRecChanged();
    void updateRecCord(double x_mm, double y_mm, double z_mm);
    void on_idRecSpinBox_valueChanged(int arg1);
    void on_xCordSpinBox_valueChanged(double arg1);
    void on_yCordSpinBox_valueChanged(double arg1);
    void on_zCordSpinBox_valueChanged(double arg1);
    void on_receiverPositionChanged(QVector3D position);
    void on_sonarColor_comboBox_currentTextChanged(const QString &arg1);
    void onSonarSizeChanged();
    void on_sizePushButton_clicked();
    void onSignalParamChanged();

private:
    Ui::SonarConfWindow *ui;
    SignalConfWindown *_signalConfWindow;
    ChangeSizeWindow *sonarSizeWindow;

    Sonar _sonar;

    FileManager _fileManager;

    int _idUpdatedRec = -1;
};

#endif // SONARCONFWINDOW_HH
