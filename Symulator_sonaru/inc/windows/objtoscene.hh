#ifndef OBJTOSCENE_HH
#define OBJTOSCENE_HH

#include <QDialog>
#include <QModelIndexList>
#include "constant.hh"
#include "changesizewindow.hh"
#include "platform.hh"
#include "filemanager.hh"

namespace Ui {
class ObjToScene;
}

class ObjToScene : public QDialog
{
    Q_OBJECT

signals:
    void objct2Add(Objects const &obj);
    void updateNumLabelOfSelectedObj(Objects const &obj);
    void setPostionOfSelectedObj(Objects const &obj, uint const &objId, QVector3D const &position);
    void updateCordLabelOfSelectedObj(Objects const &obj, uint const &objId);
    void deletePlatform();
    void deleteSelectedObj(Objects const &obj, uint const &objId);
    void addPlatformToScene(Platform const &platform);
    void getObjSize(Objects const &obj, uint const &objId);
    void setObjSize(Objects const &obj, uint const &objId, QVector3D const &newSize);
    void setRotationOfPlatform(double const &rot_deg);
    void selectObj(Objects const &obj, int const &id);
public:
    explicit ObjToScene(QWidget *parent = nullptr);
    ~ObjToScene();
    void updateCordLabel(double x, double y, double z);
    void updateCordLabel(QVector3D const &cord);
    void updateLabel(uint numOfObjects, Objects const &obj);
    void set_platformLoaded(int const &val);
    int isPlatformLoaded() const;
    void updateSizeWindow(QVector3D const &size);
    void updatePlatformCordLabels(double x, double y, double z, double rot);
    void updateRotLabel(double rot);
    Platform getPlatform() const;
    void updateCordLabel(Objects const &obj, int const &id, QVector3D const &pos);

private slots:
    void on_platfromPushButton_clicked();
    void on_pointPushButton_clicked();
    void on_flatSurfacePushButton_clicked();
    void on_stakePushButton_clicked();
    void on_sizePushButton_clicked();
    void on_addPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_selectObjIdSpinBox_valueChanged(int arg1);
    void on_x_doubleSpinBox_valueChanged(double arg1);
    void on_y_doubleSpinBox_valueChanged(double arg1);
    void on_z_doubleSpinBox_valueChanged(double arg1);

    void on_loadPlatformPushButton_clicked();
    void on_sizeChanged();

    void on_rotSpinBox_valueChanged(double arg1);
private:
    bool objDeleteMessage();
    bool readPlatformConfiguration();
    void updateCordLabelwithZeros();
    Ui::ObjToScene *ui;
    ChangeSizeWindow *_sizeWindow;

    Objects _selectedObj;
    int _selectedObjId = -1;
    Platform _platform;
    FileManager _fileManager;

    int platformLoaded = -1;
};

inline void ObjToScene::set_platformLoaded(const int &val)
{
    platformLoaded = val;
}

inline int ObjToScene::isPlatformLoaded() const
{
    return platformLoaded;
}

inline Platform ObjToScene::getPlatform() const
{
    return _platform;
}

#endif // OBJTOSCENE_HH
