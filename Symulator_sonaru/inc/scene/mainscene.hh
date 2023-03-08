#ifndef MAINSCENE_HH
#define MAINSCENE_HH

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QPoint>
#include <QMatrix4x4>
#include <QVector4D>
#include <QMouseEvent>
#include <memory>
#include "platformmodel.hh"
#include "platform.hh"
#include "stakemodel.hh"
#include "pointmodel.hh"
#include "flatsurfacemodel.hh"
#include "gridmodel.hh"
#include "constant.hh"
#include "detectedpoint.hh"

struct PointObject{
    double size;
    QVector3D position;
    QVector3D color;
};
struct PoleObjects{
    QVector3D size;
    QVector3D position;
    QVector3D color;
};
struct FlatObjects{
    QVector3D size;
    QVector3D position;
    QVector3D color;
};


struct SimulationScene{
    QVector<PointObject> points;
    QVector<PoleObjects> poles;
    QVector<FlatObjects> flats;
};

class MainScene :  public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();
    void addPlatform();
    void deletePlatform();
    void setPlatform(Platform const &inPlatform);
    void getPlatformActualPosition(Platform &inPlatform) const;
    void getObjPosition(QVector<QVector3D> &_objPosition, Sonar const &activeSonar);
    void getFlatSurfacePosition(QVector<QVector3D> &_objPosition, Sonar const &activeSonar);
    void addPoleModel();
    void addPointModel();
    void addFlatSurfaceModel();
    void addGridModel();
    QVector3D getLastObjPosition(Objects const &obj) const;
    QVector3D getObjPosition(Objects const &obj, uint const &objId);
    uint getNumOfObject();
    double getPlatformRotation() const;
    uint getNumfOfObj(Objects const &obj) const;
    void deleteSelectedObj(Objects const &obj, uint const &objId);
    void setPositionOfSelectedObj(Objects const &obj, uint const &objId, const QVector3D &position);
    void deleteAllObjects();
    void deleteAllPointModel();
    void deleteAllPoleModel();
    void deleteAllFlatSurfaceModel();
    bool isPlatformModelCreated() const;
    void setObjSize(Objects const &obj, uint const &objId, const QVector3D &newSize);
    QVector3D getObjSize(Objects const &obj, uint const &objId) const;
    void setPatformRotation(double const &rot_deg);
    void changActiveSonarColor(uint const &activeSonarId);
    void addDetectedPoint(QVector3D position);
    void clearDetectedPoints();
    void setSelectedObj(Objects const &obj, int const &id);
    void deleleteDetection();
    void getScene(SimulationScene &scene);
    void loadScene(SimulationScene const &scene);
signals:
    void platformPositionChanged();
    void objPositionChanged(Objects const &obj, int const &id);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool initShaders();
    void getShaderParam();
    void drawPlatform(std::shared_ptr<PlatformModel> const &inPlatformModel);
    void drawSonar(std::shared_ptr<SonarModel> const &inSonarModel);
    void drawReceiver(std::shared_ptr<Model> const &inReceiverModel);
    void drawTransmitter(std::shared_ptr<Model> const &inTransmitterModel);

    void addSonar(Sonar const &inSonar, std::shared_ptr<PlatformModel> &inPlatformModel);
    void addTransmitter(Transmitter const &inTransmitter, SonarModel &inSonarModel);
    void addReceiver(Receiver const &inReceiver, SonarModel &inSonarModel);

    void getTransmitterParam(Transmitter &inTransmitter, Model const &transmitterModel) const;
    void getReceiverParam(Receiver &inReceiver, Model const &receiverModel) const;
    void getSonarParam(Sonar &inSonar, SonarModel const &sonModel) const;

    void translateSeletctedObj(QVector3D const &translate);
    void rotateSelectectedObj(double const &rotate);

    QOpenGLShaderProgram _shaderProgram, _textShaderProgram;
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    GLuint _textureCoordinates;
    GLuint _texture;
    GLuint _textVertexAtrLocation;
    GLuint _textpMatrixLocation;
    GLuint _textvMatrixLocation;
    GLuint _textmMatrixLocation;

    std::shared_ptr<PlatformModel> _platformModel = nullptr;
    std::vector<std::shared_ptr<PoleModel>> _poleModels;
    std::vector<std::shared_ptr<PointModel>> _pointModels;
    std::vector<std::shared_ptr<FlatSurfaceModel>> _flatSurfaceModels;
    std::vector<std::shared_ptr<DetectedPoint>> _detectedPoints;
    std::shared_ptr<GridModel> _gridModel = nullptr;

    Platform _platform;

    float alpha;
    float beta;
    float cameraFar;
    QPoint lastMousePosition;
    uint idActiveSonar = 0;
    int selectedIdObject = -1;
    Objects selectedObj;

};

inline void MainScene::setPlatform(const Platform &inPlatform)
{
    _platform = inPlatform;
}

#endif // MAINSCENE_HH
