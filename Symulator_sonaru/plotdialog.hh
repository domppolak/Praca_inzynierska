#ifndef PLOTDIALOG_HH
#define PLOTDIALOG_HH

#include <QDialog>
#include "qcustomplot/qcustomplot.h"

namespace Ui {
class plotDialog;
}

class plotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit plotDialog(QWidget *parent = nullptr);
    ~plotDialog();
    void setTitle(QString const &title);
    void setFigureTitle(QString const &figureTitle);
    void makePlot(const QString &figureTitle);
    void addData(QVector<double> const &time, QVector<double> const &signal, double const &time0);
    void clearData();
    bool isMakedPlot() const;

private:
    Ui::plotDialog *ui;
    QCPTextElement *title;
    bool isPlot = false;
};

inline bool plotDialog::isMakedPlot() const
{
    return isPlot;
}

#endif // PLOTDIALOG_HH
