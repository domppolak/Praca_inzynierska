#include "changesizewindow.hh"
#include "ui_changesizewindow.h"


ChangeSizeWindow::ChangeSizeWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeSizeWindow)
{
    ui->setupUi(this);
    ui->xSpinBox->setMinimum(0);
    ui->ySpinBox->setMinimum(0);
    ui->zSpinBox->setMinimum(0);
    ui->xSpinBox->setMaximum(10000);
    ui->ySpinBox->setMaximum(10000);
    ui->zSpinBox->setMaximum(10000);

}

ChangeSizeWindow::~ChangeSizeWindow()
{
    delete ui;
}

void ChangeSizeWindow::updateLabel(QVector3D const &size)
{
    ui->xSpinBox->setValue(size[0]);
    ui->ySpinBox->setValue(size[1]);
    ui->zSpinBox->setValue(size[2]);
}

void ChangeSizeWindow::setTitleDependOnObj(const Objects &obj, const uint &id)
{
    QString setTitle;
    switch(obj)
    {
        case PLATFORM:
            if(!isHidden())
                hide();
            break;
        case POINT:
            setTitle = "Point " + QString::number(id+1) + " size";
            setWindowTitle(setTitle);
            ui->label_4->setText("Point size:");
            ui->label->hide();
            ui->label_2->hide();
            ui->label_3->hide();
            ui->ySpinBox->hide();
            ui->zSpinBox->hide();
            show();
            break;
        case STAKE:
            setTitle = "Stake " + QString::number(id+1) + " length";
            setWindowTitle(setTitle);
            ui->label_4->setText("Stake length [mm]:");
            ui->label->hide();
            ui->label_2->hide();
            ui->label_3->hide();
            ui->ySpinBox->hide();
            ui->zSpinBox->hide();
            show();
            break;
        case FLAT_SURFACE:
            setTitle = "Flat surface " + QString::number(id+1) + " size";
            setWindowTitle(setTitle);
            ui->label_4->setText("Flat surface size:");
            ui->label->show();
            ui->label_2->show();
            ui->label_3->show();
            ui->ySpinBox->show();
            ui->zSpinBox->show();
            show();
            break;
        default:
            hide();
            qDebug() << "nie ma takiego obiektu";
            break;
    }
}

void ChangeSizeWindow::updateSonarSize(double x, double y, double z)
{
    _sonarSize = QVector3D(x,y,z);
}

void ChangeSizeWindow::on_buttonBox_accepted()
{
    updateSonarSize(ui->xSpinBox->value(), ui->ySpinBox->value(), ui->zSpinBox->value());
    sizeAccepted = true;
    emit sonarSizeChanged();
    emit sizeChanged();
}

