#ifndef CHANGESIZEWINDOW_HH
#define CHANGESIZEWINDOW_HH

#include <QDialog>
#include <QVector3D>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include "constant.hh"

namespace Ui {
class ChangeSizeWindow;
}


class ChangeSizeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeSizeWindow(QWidget *parent = nullptr);
    ~ChangeSizeWindow();
    QVector3D getSize() const {return _sonarSize;}
    void updateLabel(QVector3D const &size);
    bool isSizeAccepted() const {return sizeAccepted;}
    void setSizeAccepted(bool status) { sizeAccepted = status;}

    void setTitleDependOnObj(Objects const &obj, const uint &id);
    void setLabel(Objects const &obj);

private slots:
    void on_buttonBox_accepted();

signals:
    void sonarSizeChanged();
    void sizeChanged();
private:
    void updateSonarSize(double x, double y, double z);
    Ui::ChangeSizeWindow *ui;
    QVector3D _sonarSize;

    QDialogButtonBox *buttonBox;
    bool sizeAccepted = false;
};

#endif // CHANGESIZEWINDOW_HH
