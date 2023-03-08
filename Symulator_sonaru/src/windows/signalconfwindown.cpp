#include "signalconfwindown.hh"
#include "ui_signalconfwindown.h"
#include "qcustomplot/qcustomplot.h"
#include <QMessageBox>

SignalConfWindown::SignalConfWindown(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignalConfWindown)
{
    ui->setupUi(this);
    setWindowTitle("Transmitted signal configuration");
    setDefault();
    connect(this, &SignalConfWindown::rejected, this, &SignalConfWindown::isParamSaved);

    makePlot();
    generateSignalDiagram();
    isSaved = true;

    _pixMap.load(":/img/img/singal_formula6.png");
    int w = ui->formula->width();
    int h = ui->formula->height();
    ui->formula->setPixmap(_pixMap.scaled(w,h, Qt::KeepAspectRatio));
}

SignalConfWindown::~SignalConfWindown()
{
    delete ui;
}

void SignalConfWindown::getSignalParam(TransmittedSignal &_signal)
{
    _signal.set_Amp(signal.get_Amp());
    _signal.set_w(signal.get_w());
    _signal.set_tau1(signal.get_tau1());
    _signal.set_tau2(signal.get_tau2());
    _signal.set_M1(signal.get_M1());
    _signal.set_M2(signal.get_M2());
    _signal.set_p1(signal.get_p1());
    _signal.set_p2(signal.get_p2());
    _signal.set_alpha(signal.get_alpha());
    _signal.set_time(signal.get_time());
    _signal.set_tMax(signal.get_tMax());
}

void SignalConfWindown::setSignalParam(TransmittedSignal const &_signal)
{
    signal.set_Amp(_signal.get_Amp());
    signal.set_w(_signal.get_w());
    signal.set_tau1(_signal.get_tau1());
    signal.set_tau2(_signal.get_tau2());
    signal.set_M1(_signal.get_M1());
    signal.set_M2(_signal.get_M2());
    signal.set_p1(_signal.get_p1());
    signal.set_p2(_signal.get_p2());
    signal.set_alpha(_signal.get_alpha());
    signal.set_time(_signal.get_time());
    signal.set_tMax(_signal.get_tMax());
    setParam();
    generateSignalDiagram();
}

void SignalConfWindown::resizeEvent(QResizeEvent *event)
{
    int w = ui->formula->width();
    int h = ui->formula->height();
    ui->formula->setPixmap(_pixMap.scaled(w,h));
    QDialog::resizeEvent(event);
}


void SignalConfWindown::on_saveButton_clicked()
{
    save();
}

void SignalConfWindown::on_defaultButton_clicked()
{
    setDefault();
}


void SignalConfWindown::save()
{
    if(readSignalParam()){
        isSaved = true;
        emit signalParamChanged();
    }else{
        if(isHidden()) show();
        QMessageBox::critical(this, QStringLiteral("Wrong value"), QStringLiteral("Enter the correct value!"));
        setParam();
    }
}

void SignalConfWindown::setDefault()
{
    signal.setDefaultValues();
    ui->AmpLine->setText(QString::number(signal.get_Amp()));
    ui->wLine->setText(QString::number(signal.get_w()));
    ui->tau1Line->setText(QString::number(signal.get_tau1()));
    ui->tau2Line->setText(QString::number(signal.get_tau2()));
    ui->M1Line->setText(QString::number(signal.get_M1()));
    ui->M2Line->setText(QString::number(signal.get_M2()));
    ui->p1Line->setText(QString::number(signal.get_p1()));
    ui->p2Line->setText(QString::number(signal.get_p2()));
    ui->p2Line_2->setText(QString::number(signal.get_alpha()));
    ui->p2Line_3->setText(QString::number(signal.get_time()));
    ui->t_max_4->setText(QString::number(signal.get_tMax()));

    isSaved = false;
}

void SignalConfWindown::setParam()
{
    ui->AmpLine->setText(QString::number(signal.get_Amp()));
    ui->wLine->setText(QString::number(signal.get_w()));
    ui->tau1Line->setText(QString::number(signal.get_tau1()));
    ui->tau2Line->setText(QString::number(signal.get_tau2()));
    ui->M1Line->setText(QString::number(signal.get_M1()));
    ui->M2Line->setText(QString::number(signal.get_M2()));
    ui->p1Line->setText(QString::number(signal.get_p1()));
    ui->p2Line->setText(QString::number(signal.get_p2()));
    ui->p2Line_2->setText(QString::number(signal.get_alpha()));
    ui->p2Line_3->setText(QString::number(signal.get_time()));
    ui->t_max_4->setText(QString::number(signal.get_tMax()));
}

void SignalConfWindown::makePlot()
{
    ui->plotWindow->setInteraction(QCP::iRangeDrag);
    ui->plotWindow->setInteraction(QCP::iRangeZoom);


    ui->plotWindow->plotLayout()->insertRow(0);
    ui->plotWindow->plotLayout()->addElement(0,0,( new QCPTextElement(ui->plotWindow, tr("Transmitted signal"), QFont("sans", 12, QFont::Bold))));
    ui->plotWindow->xAxis->setLabel("Time [us]");
    ui->plotWindow->yAxis->setLabel("U [mV]");

    ui->plotWindow->addGraph();
    ui->plotWindow->graph(0)->setPen(QPen(QColor("purple")));
}

void SignalConfWindown::generateSignalDiagram()
{

    double u_mV_max = 0;
    double currentzeroCrossedMaxValue = 0;
    double lastZeroCrossedMaxValue = 0;
    int zeroIsCrossed = 0;
    int maxZeroCrossedValueFind = -1;
    uint currentTime = 0;
    ui->plotWindow->graph(0)->data()->clear();
    QVector<double> time, u;
    while(1){
        currentTime++;
        double u_mV = signal.generatesignal_close2Real_notmax(currentTime, 0);

        if(u_mV_max < u_mV)
            u_mV_max = u_mV;

        if(zeroIsCrossed == 1  && currentzeroCrossedMaxValue < u_mV)
            currentzeroCrossedMaxValue = u_mV;

        if(currentzeroCrossedMaxValue > signal.generatesignal_close2Real_notmax(currentTime+1, 0))
            maxZeroCrossedValueFind = 1;
        else
            maxZeroCrossedValueFind = -1;

        if(u_mV > 0 && signal.generatesignal_close2Real_notmax(currentTime+1, 0) < 0){
            zeroIsCrossed = -1;
            lastZeroCrossedMaxValue = currentzeroCrossedMaxValue;
            currentzeroCrossedMaxValue = 0;
        }
        else if(u_mV < 0 && signal.generatesignal_close2Real_notmax(currentTime+1, 0) > 0){
            zeroIsCrossed = 1;
        }

        if(lastZeroCrossedMaxValue > currentzeroCrossedMaxValue && lastZeroCrossedMaxValue < 0.05*u_mV_max
                && lastZeroCrossedMaxValue != 0 && currentzeroCrossedMaxValue != 0
                && maxZeroCrossedValueFind == 1)
            break;

        time.push_back(currentTime);
        u.push_back(u_mV);
    }
    ui->plotWindow->graph(0)->addData(time, u);
    ui->plotWindow->xAxis->setRange(0, currentTime);
    ui->plotWindow->yAxis->setRange(-u_mV_max, u_mV_max);
    ui->plotWindow->replot();
}

bool SignalConfWindown::readSignalParam()
{
    if(ui->AmpLine->text().toDouble())
        signal.set_Amp(ui->AmpLine->text().toDouble());
    else
        return false;
    if(ui->wLine->text().toDouble())
        signal.set_w(ui->wLine->text().toDouble());
    else
        return false;
    if(ui->tau1Line->text().toDouble())
        signal.set_tau1(ui->tau1Line->text().toDouble());
    else
        return false;
    if(ui->tau2Line->text().toDouble())
        signal.set_tau2(ui->tau2Line->text().toDouble());
    else
        return false;
    if(ui->M1Line->text().toDouble())
        signal.set_M1(ui->M1Line->text().toDouble());
    else
        return false;
    if(ui->M2Line->text().toDouble())
        signal.set_M2(ui->M2Line->text().toDouble());
    else
        return false;
    if(ui->p1Line->text().toDouble())
        signal.set_p1(ui->p1Line->text().toDouble());
    else
        return false;
    if(ui->p2Line->text().toDouble())
        signal.set_p2(ui->p2Line->text().toDouble());
    else
        return false;
    if(ui->p2Line_2->text().toDouble())
        signal.set_alpha(ui->p2Line_2->text().toDouble());
    else
        return false;
    if(ui->p2Line_3->text().toDouble())
        signal.set_time(ui->p2Line_3->text().toDouble());
    else
        return false;
    if(ui->t_max_4->text().toDouble())
        signal.set_tMax(ui->t_max_4->text().toDouble());
    else
        return false;

    return true;
}


void SignalConfWindown::on_AmpLine_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_wLine_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_tau1Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_tau2Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_M1Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_M2Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_p1Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::on_p2Line_editingFinished()
{
    isSaved = false;
}

void SignalConfWindown::isParamSaved()
{
    if(isSaved)
        return;

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Save"));
    msgBox.setText(QStringLiteral("Save signal parameters?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch(ret){
        case QMessageBox::Save:
            save();
            break;
        case QMessageBox::Cancel:
            show();
            break;
        case QMessageBox::Discard:
            setParam();
            break;
        default:
            break;
    }

}

void SignalConfWindown::on_pushButton_clicked()
{         
    if(readSignalParam()){
        generateSignalDiagram();;
    }else{
        QMessageBox::critical(this, QStringLiteral("Wrong value"), QStringLiteral("Enter the correct value!"));
        setParam();
    }
}


void SignalConfWindown::on_p2Line_2_editingFinished()
{
    isSaved = false;
}


void SignalConfWindown::on_p2Line_3_editingFinished()
{
    isSaved = false;
}


void SignalConfWindown::on_t_max_4_editingFinished()
{
    isSaved = false;
}

