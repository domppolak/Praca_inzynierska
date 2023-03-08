#include "sonarconfscene.hh"
#include <QDebug>
#include <QTime>
#include <iterator>
#include "constant.hh"

SonarConfScene::SonarConfScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    alpha = 25;
    beta = -25;
    cameraFar = 20;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

}

SonarConfScene::~SonarConfScene()
{
    makeCurrent();
    _sonarModel->deleteSonarModel();
}

void SonarConfScene::addReceiver()
{
    std::shared_ptr<Model> receiverModel(new Model());
    receiverModel->setScale(0.5, 0.5, 1.25);
    receiverModel->setModelId(_sonarModel->getNumOfReceiverModels()+1);
    receiverModel->translate(0, -5, 0);

    _sonarModel->addReceiverModel(receiverModel);

    makeCurrent();
    _sonarModel->createMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void SonarConfScene::createSonar()
{

    makeCurrent();
    _sonarModel = std::make_shared<SonarModel>();
    _sonarModel->setSonarScale(QVector3D(5, 3.5, 5));
    _sonarModel->createMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void SonarConfScene::addReadConfSonar(const Sonar &inSonar)
{
    _sonarModel->setScale(inSonar.get_size()/SIMULATION_SCALE);
    _sonarModel->setSonarColor(inSonar.get_color());

    addTransmitterReadFromSonarConf(inSonar.getTransmitter());

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId){
        _sonarModel->addReceiverModel();
        addReceiverReadFromSonarConf(inSonar.getReceiver(recId), _sonarModel->useReceiverModel(recId));
    }

    makeCurrent();
    _sonarModel->changeColor(_shaderProgram, _colorAtrLocation);
    _sonarModel->createMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);  
    update();
}

void SonarConfScene::deleteReceiver()
{
    makeCurrent();
    _sonarModel->deleteReceiverModel();
    update();
}

void SonarConfScene::deleteReceiver(uint recId)
{
    assert(recId < _sonarModel->getNumOfReceiverModels());
    makeCurrent();
    _sonarModel->deleteReceiverModel(recId);
    update();
}

void SonarConfScene::getSonarParam(Sonar &inSonar)
{
    inSonar.set_LocPos(_sonarModel->getPostion()*SIMULATION_SCALE);
    inSonar.set_size(_sonarModel->getScale()*SIMULATION_SCALE);
    inSonar.set_id(_sonarModel->getModelId());
    inSonar.set_color(_sonarModel->getSonarColor());

    getTransmitterParam(inSonar.useTransmitter(), _sonarModel->getTransmitterModel());

    for(uint recId=0; recId < _sonarModel->getNumOfReceiverModels(); ++recId)
        getReceiverParam(inSonar.useReceiver(recId), _sonarModel->getReceiverModel(recId));
}

void SonarConfScene::clearScene()
{
    makeCurrent();
    _sonarModel->deleteAllReceiverModels();
    update();
}

void SonarConfScene::setReceiverPosition(const uint &recId, const QVector3D &newPosition)
{
    assert(recId < _sonarModel->getNumOfReceiverModels());
    _sonarModel->useReceiverModel(recId).setPosition(newPosition);
}

void SonarConfScene::setReceiverId(const uint &recId, const uint &newId)
{
    assert(recId < _sonarModel->getNumOfReceiverModels());
    _sonarModel->useReceiverModel(recId).setModelId(newId);
}

void SonarConfScene::setSonarScale(const QVector3D &newScale)
{
    _sonarModel->setSonarScale(newScale);
    update();
}

void SonarConfScene::changeSonarColor(const QVector3D &color)
{
    makeCurrent();
    _sonarModel->setSonarColor(color);
    _sonarModel->changeColor(_shaderProgram, _colorAtrLocation);
    update();
}

QVector3D SonarConfScene::getReceiverPosition(const uint &recId) const
{
    assert(recId < _sonarModel->getNumOfReceiverModels());
    return _sonarModel->getReceiverModel(recId).getPostion();
}

QVector3D SonarConfScene::getSonarScale() const
{
    return _sonarModel->getScale();
}

bool SonarConfScene::initShaders()
{
    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader.frag"))
        return false;

    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader.vert"))
        return false;

    if(!_shaderProgram.link())
        return false;

    return true;
}

void SonarConfScene::getShaderParam()
{
    _vertexAtrLocation = _shaderProgram.attributeLocation("vertex");
    _colorAtrLocation = _shaderProgram.attributeLocation("color");
    _pMatrixLocation = _shaderProgram.uniformLocation("pMatrix");
    _vMatrixLocation = _shaderProgram.uniformLocation("vMatrix");
    _mMatrixLocation = _shaderProgram.uniformLocation("mMatrix");
}

void SonarConfScene::drawSonar()
{
    if(!_sonarModel->isCreated() && _sonarModel == nullptr)
        return;


    _shaderProgram.setUniformValue(_mMatrixLocation, _sonarModel->get_mMatrix());
    _sonarModel->draw();
}

void SonarConfScene::drawReceiver(const std::shared_ptr<Model> &inReceiver)
{
    if(!inReceiver->isCreated() && inReceiver == nullptr)
        return;


    _shaderProgram.setUniformValue(_mMatrixLocation, inReceiver->get_mMatrix());
    inReceiver->draw();
}

void SonarConfScene::drawTransmitter(const std::shared_ptr<Model> &inTransmitter)
{
    if(!inTransmitter->isCreated() && inTransmitter == nullptr)
        return;


    _shaderProgram.setUniformValue(_mMatrixLocation, inTransmitter->get_mMatrix());
    inTransmitter->draw();
}

void SonarConfScene::getReceiverParam(Receiver &inReceiver, const Model &receiverModel)
{
    inReceiver.set_LocPos(receiverModel.getPostion()*SIMULATION_SCALE);
    inReceiver.set_size(receiverModel.getScale()*SIMULATION_SCALE);
    inReceiver.set_id(receiverModel.getModelId());
}

void SonarConfScene::getTransmitterParam(Transmitter &inTransmitter, const Model &transmitterModel)
{
    inTransmitter.set_LocPos(transmitterModel.getPostion()*SIMULATION_SCALE);
    inTransmitter.set_size(transmitterModel.getScale()*SIMULATION_SCALE);
    inTransmitter.set_acusticVec(_sonarModel->trans_getAcusticVector()*SIMULATION_SCALE);
}

void SonarConfScene::addTransmitterReadFromSonarConf(const Transmitter &inTransmitter)
{
    Model &transmitterModel = _sonarModel->useTransmitterModel();
    transmitterModel.setPosition(inTransmitter.get_LocPos()/SIMULATION_SCALE);
    transmitterModel.setScale(inTransmitter.get_size()/SIMULATION_SCALE);
    _sonarModel->trans_setAcusticVector(inTransmitter.get_acusticVec()/SIMULATION_SCALE);
}

void SonarConfScene::addReceiverReadFromSonarConf(const Receiver &inReceiver, Model &receiverModel)
{
    receiverModel.setPosition(inReceiver.get_LocPos()/SIMULATION_SCALE);
    receiverModel.setScale(inReceiver.get_size()/SIMULATION_SCALE);
    receiverModel.setModelId(inReceiver.get_id());
}

void SonarConfScene::changeSelectedRecColor()
{
    QVector<QVector3D> _receiverColorVertices, _receiverColorVerticesSelect;
    for(uint i=0; i<24; i++) _receiverColorVerticesSelect << QVector3D(1,1,0);
    for(uint i=0; i<114; i++) _receiverColorVerticesSelect << QVector3D(0.96, 0.071, 0.98);

    for(uint i=0; i<24; i++) _receiverColorVertices << QVector3D(1,1,0);
    for(uint i=0; i<114; i++) _receiverColorVertices << QVector3D(0,0,0);

    makeCurrent();
    if(lastSelectRecId < _sonarModel->getNumOfReceiverModels())
        _sonarModel->useReceiverModel(lastSelectRecId).changeColor(_shaderProgram, _colorAtrLocation, _receiverColorVertices);
    _sonarModel->useReceiverModel(selectedRecId).changeColor(_shaderProgram, _colorAtrLocation, _receiverColorVerticesSelect);

    update();
    lastSelectRecId = selectedRecId;
}

void SonarConfScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    if(!initShaders())
        qDebug() << "Nie udalo sie zainicjalizowac shaderow";

    getShaderParam();

    createSonar();
    //_sonarModel->useTransmitterModel().setPosition(0,0, _sonarModel->getScale()[2]/2.1);
    sceneInitialized = 1;
}

void SonarConfScene::paintGL()
{
    makeCurrent();
    _shaderProgram.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix, mMatrix;
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation.map(QVector3D(0, 0, cameraFar));
    QVector3D cameraUpDirection = cameraTransformation.map(QVector3D(0, 1, 0));

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    _shaderProgram.setUniformValue(_pMatrixLocation, _pMatrix);
    _shaderProgram.setUniformValue(_vMatrixLocation, vMatrix);
    glEnable(GL_CULL_FACE);
    if(_sonarModel != nullptr){
        drawSonar();
        glDisable(GL_CULL_FACE);
        drawTransmitter(_sonarModel->getTransmitterModelPointer());
        for(uint recId = 0; recId < _sonarModel->getNumOfReceiverModels(); ++recId)
            drawReceiver(_sonarModel->getReceiverModelPointer(recId));
    }

    _shaderProgram.release();
}

void SonarConfScene::resizeGL(int width, int height)
{
    if(height == 0) height = 1;

    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)width/(float)height, 0.001f, 1000);

    glViewport(0, 0, width, height);
}

void SonarConfScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void SonarConfScene::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    double deltaX = (pos.x() - lastMousePosition.x())*0.3;
    double deltaY = (lastMousePosition.y() - pos.y())*0.3;

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -360) {
            beta = -360;
        }
        if (beta > 360) {
            beta = 360;
        }

        update();
    }

    lastMousePosition = event->pos();

    event->accept();
}

void SonarConfScene::keyPressEvent(QKeyEvent *key)
{
    if(selectedRecId < int(_sonarModel->getNumOfReceiverModels()) && selectedRecId >= 0){
        if(key->key() == Qt::Key_W)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(0, 0.2, 0));
        if(key->key() == Qt::Key_S)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(0, -0.2, 0));
        if(key->key() == Qt::Key_A)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(-0.2, 0, 0));
        if(key->key() == Qt::Key_D)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(0.2, 0, 0));
        if(key->key() == Qt::Key_Z)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(0, 0, 0.2));
        if(key->key() == Qt::Key_X)
            _sonarModel->useReceiverModel(selectedRecId).translate(QVector3D(0, 0, -0.2));

        emit recPositionChanged(_sonarModel->getReceiverModel(selectedRecId).getPostion());
    }

    update();

}

void SonarConfScene::wheelEvent(QWheelEvent *event)
{
    QPoint delta = event->angleDelta();

    if(delta.y() > 0){
        cameraFar *= 0.95;
    }else{
        cameraFar *= 1.05;
    }

    update();
    event->accept();
}


