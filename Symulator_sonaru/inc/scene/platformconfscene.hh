#ifndef PLATFORMCONFSCENE_HH
#define PLATFORMCONFSCENE_HH

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
#include <model.hh>
#include "platform.hh"
#include "sonar.hh"
#include "platformmodel.hh"
#include <memory>

class PlatformConfScene : public QOpenGLWidget, public QOpenGLFunctions
{

    Q_OBJECT

public:
    explicit PlatformConfScene(QWidget *parent = nullptr);
    ~PlatformConfScene();

    void addSonar(Sonar const &inSonar);
    void addReadConfPlatform(Platform const &inPlatform);
    void deleteSonar();
    void deleteSonar(uint sonarId);
    void createPlatform();
    void getPlatformParam(Platform &inPlatform);
    void getSonarParam(Sonar &inSonar, SonarModel const &sonModel);
    void selectSonar(int id);


    void setSonarPosition(uint sonarId, QVector3D const &newPosition);
    void setSonarRotation(uint sonarId, double newRotation);
    void setPlatformScale(QVector3D newScale);
    void setSonarId(uint id, uint newId);


    QVector3D getSonarPosition(uint id) const;
    double getSonarRotation(uint id) const;
    QVector3D getPlatformScale() const;

    void clearScene();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void sonarPositionChanged(QVector3D position, double rotation);

private:
    bool initShaders();
    void getShaderParam();
    void initVertices();
    void sonarColorVertices(QVector3D sonarColor);
    void computeAllRecPosition(uint id, double rotation, QVector3D translate);
    void sonarTranslate(uint id, QVector3D const &translate);

    void drawPlatform(std::shared_ptr<PlatformModel> const &inPlatformModel);
    void drawSonar(std::shared_ptr<SonarModel> const &inSonarModel);
    void drawReceiver(std::shared_ptr<Model> const &inReceiverModel);
    void drawTransmitter(std::shared_ptr<Model> const &inTransmitterModel);

    void addTransmitter(Transmitter const &inTransmitter, std::shared_ptr<SonarModel> &inSonarModel);
    void addReceiver(Receiver const &inReceiver, std::shared_ptr<SonarModel> &inSonarModel);

    void getTransmitterParam(Transmitter &inTransmitter, Model const &transmitterModel);
    void getReceiverParam(Receiver &inReceiver, Model const &receiverModel);

    void addSonarReadFromPlatformConf(Sonar const &inSonar);
    void addTransmitterReadFromPlatformConf(Transmitter const &inTransmitter, std::shared_ptr<SonarModel> &inSonarModel);
    void addReceiverReadFromPlatformConf(Receiver const &inReceiver, std::shared_ptr<SonarModel> &inSonarModel);

    void changeSelectSonarColor();
    int selectedSonar = -1;
    uint lastSelectSonar = 0;
    QVector3D lastColor;

    std::shared_ptr<PlatformModel> _platformModel = nullptr;

    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    //shader program settings
    QOpenGLShaderProgram _shaderProgram;
    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    //vertices
    QVector<QVector3D> _sonarVertices;
    QVector<QVector3D> _transmitterVertices;
    QVector<QVector3D> _receiverVertices;
    QVector<QVector3D> _platformVertices;

    //color vertices
    QVector<QVector3D> _sonarColorVertices;
    QVector<QVector3D> _transmitterColorVertices;
    QVector<QVector3D> _receiverColorVertices;
    QVector<QVector3D> _platformColorVertices;

    //camera settings
    float alpha;
    float beta;
    float cameraFar;
    QPoint lastMousePosition;
    QVector<QVector3D> colorList;


};

#endif // PLATFORMCONFSCENE_HH
