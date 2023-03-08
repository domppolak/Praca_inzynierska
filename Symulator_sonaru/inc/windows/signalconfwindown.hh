#ifndef SIGNALCONFWINDOWN_HH
#define SIGNALCONFWINDOWN_HH

#include <QDialog>
#include <memory>
#include "transmittedsignal.hh"

namespace Ui {
class SignalConfWindown;
}

class SignalConfWindown : public QDialog
{
    Q_OBJECT

public:
    explicit SignalConfWindown(QWidget *parent = nullptr);
    ~SignalConfWindown();
    void getSignalParam(TransmittedSignal &signal);
    void setSignalParam(TransmittedSignal const &signal);

protected:
    void resizeEvent(QResizeEvent *event) override;
signals:
    void signalParamChanged();

private slots:
    void on_saveButton_clicked();
    void on_defaultButton_clicked();
    void on_AmpLine_editingFinished();
    void on_wLine_editingFinished();
    void on_tau1Line_editingFinished();
    void on_tau2Line_editingFinished();
    void on_M1Line_editingFinished();
    void on_M2Line_editingFinished();
    void on_p1Line_editingFinished();
    void on_p2Line_editingFinished();

    void isParamSaved();

    void on_pushButton_clicked();

    void on_p2Line_2_editingFinished();

    void on_p2Line_3_editingFinished();

    void on_t_max_4_editingFinished();

private:
    Ui::SignalConfWindown *ui;
    TransmittedSignal signal;
    bool isSaved = false;

    void save();
    void setDefault();
    void setParam();


    void makePlot();
    void generateSignalDiagram();
    bool readSignalParam();

    QPixmap _pixMap;
};

#endif // SIGNALCONFWINDOWN_HH
