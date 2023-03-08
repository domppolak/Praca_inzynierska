#include "objtoscene.hh"
#include "ui_objtoscene.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

ObjToScene::ObjToScene(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjToScene)
{
    ui->setupUi(this);
    _sizeWindow = new ChangeSizeWindow();
    ui->x_doubleSpinBox->setMaximum(10000);
    ui->x_doubleSpinBox->setMinimum(-10000);
    ui->y_doubleSpinBox->setMaximum(10000);
    ui->y_doubleSpinBox->setMinimum(-10000);
    ui->z_doubleSpinBox->setMaximum(10000);
    ui->z_doubleSpinBox->setMinimum(-10000);
    ui->rotSpinBox->setMinimum(0);
    ui->rotSpinBox->setMaximum(360);
    ui->loadPlatformPushButton->hide();
    ui->label_6->hide();
    ui->rotSpinBox->hide();
    ui->sizePushButton->hide();
    connect(_sizeWindow, &ChangeSizeWindow::sizeChanged, this, &ObjToScene::on_sizeChanged);
}

ObjToScene::~ObjToScene()
{
    delete ui;
    delete _sizeWindow;
}

void ObjToScene::updateCordLabel(double x, double y, double z)
{
    ui->x_doubleSpinBox->setValue(x);
    ui->y_doubleSpinBox->setValue(y);
    ui->z_doubleSpinBox->setValue(z);
}

void ObjToScene::updateCordLabel(const QVector3D &cord)
{
    updateCordLabel(cord[0], cord[1], cord[2]);
}

void ObjToScene::updateLabel(uint numOfObjects, const Objects &obj)
{
    QString textLabel;
    switch(obj)
    {
        case PLATFORM:
            textLabel = "Platform: " + QString::number(numOfObjects);            
            break;
        case POINT:
            textLabel = "Point: " + QString::number(numOfObjects);
            break;
        case STAKE:
            textLabel = "Stake: " + QString::number(numOfObjects);
            break;
        case FLAT_SURFACE:
            textLabel = "Flat_Surface: " + QString::number(numOfObjects);
            break;
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    ui->selectObjIdSpinBox->setMaximum(numOfObjects);
    ui->numOfObjLabel->setText(textLabel);
    /*if(numOfObjects > 0 && obj != PLATFORM){
        emit updateCordLabelOfSelectedObj(obj, 0);
        ui->selectObjIdSpinBox->setValue(1);
    }*/

}

void ObjToScene::updateSizeWindow(const QVector3D &size)
{
    _sizeWindow->updateLabel(size);
}

void ObjToScene::updatePlatformCordLabels(double x, double y, double z, double rot)
{
    if(_selectedObj == PLATFORM){
        ui->x_doubleSpinBox->setValue(x);
        ui->y_doubleSpinBox->setValue(y);
        ui->z_doubleSpinBox->setValue(z);
        updateRotLabel(rot);
    }
}

void ObjToScene::updateRotLabel(double rot_deg)
{
    double rot = rot_deg;
    if(rot < 0)
        rot += 360;

    ui->rotSpinBox->setValue(rot);
}

void ObjToScene::updateCordLabel(const Objects &obj, const int &id, const QVector3D &pos)
{
    if(obj == _selectedObj && id == _selectedObjId)
        updateCordLabel(pos);
}

void ObjToScene::on_platfromPushButton_clicked()
{
    _selectedObj = PLATFORM;
    ui->sizePushButton->hide();
    ui->label_5->hide();
    ui->selectObjIdSpinBox->hide();
    ui->loadPlatformPushButton->show();
    ui->label_6->show();
    ui->rotSpinBox->show();

    emit updateNumLabelOfSelectedObj(PLATFORM);
    emit updateCordLabelOfSelectedObj(PLATFORM, 0);
    emit selectObj(_selectedObj, -1);
}

void ObjToScene::on_pointPushButton_clicked()
{
    _selectedObj = POINT;
    ui->sizePushButton->show();
    ui->label_5->show();
    ui->selectObjIdSpinBox->show();
    ui->loadPlatformPushButton->hide();
    ui->label_6->hide();
    ui->rotSpinBox->hide();
    updateCordLabelwithZeros();

    emit updateNumLabelOfSelectedObj(POINT);
}


void ObjToScene::on_flatSurfacePushButton_clicked()
{
    _selectedObj = FLAT_SURFACE;
    if(ui->sizePushButton->isHidden())
        ui->sizePushButton->show();
    ui->label_5->show();
    ui->selectObjIdSpinBox->show();
    ui->loadPlatformPushButton->hide();
    ui->label_6->hide();
    ui->rotSpinBox->hide();
    updateCordLabelwithZeros();

    emit updateNumLabelOfSelectedObj(FLAT_SURFACE);
}


void ObjToScene::on_stakePushButton_clicked()
{
    _selectedObj = STAKE;
    if(ui->sizePushButton->isHidden())
        ui->sizePushButton->show();
    ui->label_5->show();
    ui->selectObjIdSpinBox->show();
    ui->loadPlatformPushButton->hide();
    ui->label_6->hide();
    ui->rotSpinBox->hide();
    updateCordLabelwithZeros();

    emit updateNumLabelOfSelectedObj(STAKE);
}


void ObjToScene::on_sizePushButton_clicked()
{
    if(_selectedObjId < 0){
        QMessageBox::information(this, QStringLiteral("Size window"), QStringLiteral("Select object first"));
        return;
    }
    _sizeWindow->setTitleDependOnObj(_selectedObj, _selectedObjId);
    emit getObjSize(_selectedObj, _selectedObjId);
}


void ObjToScene::on_addPushButton_clicked()
{
    if(_selectedObj == PLATFORM && platformLoaded == 1)
        emit addPlatformToScene(_platform);
    else if(_selectedObj == PLATFORM && platformLoaded == -1)
        QMessageBox::information(this, QStringLiteral("Platform"), QStringLiteral("You need to load platform configuration file first"));

    if(_selectedObj != PLATFORM)
        emit objct2Add(_selectedObj);
}


void ObjToScene::on_deletePushButton_clicked()
{
    if(_selectedObj == PLATFORM){
        if(platformLoaded == -1) return;
        if(!objDeleteMessage()) return;
        emit deletePlatform();
        return;
    }

    if(_selectedObjId < 0){
        QMessageBox::critical(this, QStringLiteral("Delete object"), QStringLiteral("Select object to delete!"));
        return;
    }

    if(!objDeleteMessage())
        return;

    emit deleteSelectedObj(_selectedObj, _selectedObjId);
}


void ObjToScene::on_selectObjIdSpinBox_valueChanged(int arg1)
{
    if(arg1 == 0){
        _selectedObjId = -1;
        updateCordLabel(0.0,0.0,0.0);
        ui->selectObjIdSpinBox->setValue(0);
        emit selectObj(_selectedObj, -1);
    }else{
        _selectedObjId = arg1-1;
        emit updateCordLabelOfSelectedObj(_selectedObj, _selectedObjId);
        emit selectObj(_selectedObj, _selectedObjId);
    }

}


void ObjToScene::on_x_doubleSpinBox_valueChanged(double arg1)
{
    if(_selectedObj == PLATFORM){
        emit setPostionOfSelectedObj(_selectedObj, 1,
            QVector3D(arg1, ui->y_doubleSpinBox->value(), ui->z_doubleSpinBox->value()));
        return;
    }

    if(_selectedObjId < 0) return;
    emit setPostionOfSelectedObj(_selectedObj, _selectedObjId,
        QVector3D(arg1, ui->y_doubleSpinBox->value(), ui->z_doubleSpinBox->value()));
}


void ObjToScene::on_y_doubleSpinBox_valueChanged(double arg1)
{
    if(_selectedObj == PLATFORM){
        emit setPostionOfSelectedObj(_selectedObj, 1,
            QVector3D(ui->x_doubleSpinBox->value(), arg1, ui->z_doubleSpinBox->value()));
        return;
    }

    if(_selectedObjId < 0) return;

    emit setPostionOfSelectedObj(_selectedObj, _selectedObjId,
        QVector3D(ui->x_doubleSpinBox->value(), arg1, ui->z_doubleSpinBox->value()));
}


void ObjToScene::on_z_doubleSpinBox_valueChanged(double arg1)
{
    if(_selectedObj == PLATFORM){
        emit setPostionOfSelectedObj(_selectedObj, 1,
            QVector3D(ui->x_doubleSpinBox->value(), ui->y_doubleSpinBox->value(), arg1));
        return;
    }

    if(_selectedObjId < 0) return;

    emit setPostionOfSelectedObj(_selectedObj, _selectedObjId,
                                 QVector3D(ui->x_doubleSpinBox->value(), ui->y_doubleSpinBox->value(), arg1));
}

bool ObjToScene::objDeleteMessage()
{
    if(_selectedObjId < 0 && _selectedObj != PLATFORM) return false;
    QString msgText;
    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("Delete object"));
    switch (_selectedObj) {
    case PLATFORM:
        msgText = "Delete platform ?";
        break;
    case POINT:
        msgText = "Delete point object " + QString::number(_selectedObjId+1) + " ?";
        break;
    case STAKE:
        msgText = "Delete stake object " + QString::number(_selectedObjId+1) + " ?";
        break;
    case FLAT_SURFACE:
        msgText = "Delete flat surface object " + QString::number(_selectedObjId+1) + " ?";
        break;
    default:
        return false;
    }

    msgBox.setText(msgText);
    msgBox.setIcon(QMessageBox::Critical);
    QPushButton *deleteButton = msgBox.addButton(QStringLiteral("Delete"), QMessageBox::ActionRole);
    QPushButton *cancel = msgBox.addButton(QStringLiteral("Cancel"), QMessageBox::ActionRole);

    bool isDelete = false;
    msgBox.exec();
    if(msgBox.clickedButton() == deleteButton)
        isDelete = true;
    else if(msgBox.clickedButton() == cancel)
        isDelete = false;

    delete deleteButton;
    delete cancel;
    return isDelete;
}

bool ObjToScene::readPlatformConfiguration()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save Xml", ".", "Xml files (*.xml)");

    QFile file(fileName);
    if(!file.open((QFile::ReadOnly | QFile::Text))){
        file.close();
        QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("Failed to open platform configuration file"));
        return false;
    }else{
        Platform readPlatform;
        if(_fileManager.readPlatformConf(file, readPlatform)){
            _platform = readPlatform;
        }
        else{
            QMessageBox::critical(this, QStringLiteral("File"), QStringLiteral("This is not a platform configuration file!"));
            return false;
        }
    }

    file.close();
    return true;
}

void ObjToScene::updateCordLabelwithZeros()
{
    ui->selectObjIdSpinBox->setValue(0);
    ui->x_doubleSpinBox->setValue(0);
    ui->y_doubleSpinBox->setValue(0);
    ui->z_doubleSpinBox->setValue(0);
}


void ObjToScene::on_loadPlatformPushButton_clicked()
{
    if(readPlatformConfiguration()){
        QMessageBox::information(this, QStringLiteral("Platform"), QStringLiteral("Platform configuration is loaded."));
        ui->loadPlatformPushButton->setText(QStringLiteral("Platform is loaded"));
        platformLoaded = 1;
    }
}

void ObjToScene::on_sizeChanged()
{
    if(_sizeWindow->isSizeAccepted()){
        emit setObjSize(_selectedObj, _selectedObjId, _sizeWindow->getSize());

    }
}


void ObjToScene::on_rotSpinBox_valueChanged(double arg1)
{
    if(_selectedObj != PLATFORM)
        return;

    emit setRotationOfPlatform(arg1);
}

