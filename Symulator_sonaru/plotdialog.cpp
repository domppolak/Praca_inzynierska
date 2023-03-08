#include "plotdialog.hh"
#include "ui_plotdialog.h"
#include "qcustomplot/qcustomplot.h"

plotDialog::plotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plotDialog)
{
    ui->setupUi(this);

}

plotDialog::~plotDialog()
{
    delete ui;
    delete title;
}

void plotDialog::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void plotDialog::setFigureTitle(const QString &figureTitle)
{

}

void plotDialog::makePlot(const QString &figureTitle)
{
    ui->plotWidget->setInteraction(QCP::iRangeDrag);
    ui->plotWidget->setInteraction(QCP::iRangeZoom);

    title = new QCPTextElement(ui->plotWidget, figureTitle, QFont("sans", 12, QFont::Bold));
    ui->plotWidget->plotLayout()->insertRow(0);
    ui->plotWidget->plotLayout()->addElement(title);
    ui->plotWidget->xAxis->setLabel("Time [us]");
    ui->plotWidget->yAxis->setLabel("U [mV]");

    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setPen(QPen(QColor("purple")));
    isPlot = true;
}

void plotDialog::addData(QVector<double> const &time, QVector<double> const &signal, double const &time0)
{
    ui->plotWidget->graph(0)->addData(time, signal);
    ui->plotWidget->xAxis->setRange(time0, time.back());
    ui->plotWidget->yAxis->setRange(-1000, 1000);
    ui->plotWidget->replot();
    ui->plotWidget->update();
}

void plotDialog::clearData()
{
    ui->plotWidget->graph(0)->data()->clear();
}

