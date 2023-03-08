#ifndef SONARCONFSCENE_HH
#define SONARCONFSCENE_HH

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
#include "sonarmodel.hh"
#include "sonar.hh"



class SonarConfScene :  public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SonarConfScene(QWidget *parent = nullptr);
    ~SonarConfScene();

    void addReceiver();
    void addTransmitter();
    void createSonar();
    void addReadConfSonar(Sonar const &inSonar);
    void deleteReceiver();
    void deleteReceiver(uint recId);
    void setReceiverPosition(uint recId);
    void getSonarParam(Sonar &inSonar);
    void clearScene();
    void selectReceiver(int const &recId);

    void setReceiverPosition(uint const &recId, QVector3D const &newPosition);
    void setReceiverId(uint const &recId, uint const &newId);
    void setSonarScale(QVector3D const &newScale);
    void changeSonarColor(QVector3D const &color);

    QVector3D getReceiverPosition(uint const &recId) const;
    QVector3D getSonarScale() const;

    int isSceneInitialized() const;
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *key) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void recPositionChanged(QVector3D position);

private:    
    bool initShaders();
    void getShaderParam();

    void drawSonar();
    void drawReceiver(std::shared_ptr<Model> const &inReceiver);
    void drawTransmitter(std::shared_ptr<Model> const &inTransmitter);

    void getReceiverParam(Receiver &inReceiver, Model const &receiverModel);
    void getTransmitterParam(Transmitter &inTransmitter, Model const &transmitterModel);

    void addTransmitterReadFromSonarConf(Transmitter const &inTransmitter);
    void addReceiverReadFromSonarConf(Receiver const &inReceiver, Model &receiverModel);
    void changeSelectedRecColor();
    std::shared_ptr<SonarModel> _sonarModel;

    QMatrix4x4 _pMatrix;
    QMatrix4x4 _vMatrix;
    QMatrix4x4 _mMatrix;

    QOpenGLShaderProgram _shaderProgram;
    GLuint _colorAtrLocation;
    GLuint _vertexAtrLocation;
    GLuint _pMatrixLocation;
    GLuint _vMatrixLocation;
    GLuint _mMatrixLocation;

    int selectedRecId = -1;
    int sceneInitialized = -1;
    uint lastSelectRecId = 0;

    float alpha;
    float beta;
    float cameraFar;
    QPoint lastMousePosition;
};

inline void SonarConfScene::selectReceiver(const int &recId)
{
    this->selectedRecId = recId;
    changeSelectedRecColor();
}

inline int SonarConfScene::isSceneInitialized() const
{
    return sceneInitialized;
}


#endif // SONARCONFSCENE_H
