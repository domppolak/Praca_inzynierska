#include "mainscene.hh"
#include <cmath>

MainScene::MainScene(QWidget *parent)
    : QOpenGLWidget(parent)
{

    alpha = 25;
    beta = -25;
    cameraFar = 300;
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    _platformModel = nullptr;
}

MainScene::~MainScene()
{
    makeCurrent();
    _platformModel->deletePlatformModel();
}

void MainScene::addPlatform()
{
    if(_platformModel != nullptr) return;
    makeCurrent();
    _platformModel = std::make_shared<PlatformModel>();
    _platformModel->setPosition(_platform.get_LocPos()/SIMULATION_SCALE);
    _platformModel->setScale(_platform.get_size()/SIMULATION_SCALE);

    for(uint sonarId = 0; sonarId < _platform.get_NumOfSonars(); ++sonarId)
        addSonar(_platform.getSonar(sonarId), _platformModel);

    _platformModel->createAllMeshes(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    _platformModel->setPlatformPosition(QVector3D(0,5,0));
}

void MainScene::deletePlatform()
{
    if(_platformModel == nullptr) return;

    makeCurrent();
    _platformModel->deletePlatformModel();
    _platformModel.reset();
    _platformModel = nullptr;
    update();
}

void MainScene::getPlatformActualPosition(Platform &inPlatform) const
{
    inPlatform.set_LocPos(_platformModel->getPostion());
    inPlatform.set_size(_platformModel->getScale()*SIMULATION_SCALE);

    for(uint sonarId=0; sonarId < inPlatform.get_NumOfSonars(); ++sonarId)
        getSonarParam(inPlatform.useSonar(sonarId), _platformModel->getSonarModel(sonarId));
}

void MainScene::getObjPosition(QVector<QVector3D> &_objPosition, Sonar const &activeSonar)
{
    _objPosition.clear();
    deleleteDetection();
    QVector3D acusticVec = (activeSonar.getTransmitter().get_GlPos() - activeSonar.get_GlPos()).normalized();
    double alpha_rad = (activeSonar.getTransmitter().getTransmittedSignal().get_alpha()/2)*M_PI/180;
    double valCos_comp = std::cos(alpha_rad);

    for(auto const &point : _pointModels){
        if((activeSonar.getTransmitter().get_GlPos() - point->getPostion()*SIMULATION_SCALE).length() <= MAX_DISTANCE){
            QVector3D uVec = point->getPostion()*SIMULATION_SCALE - activeSonar.getTransmitter().get_GlPos();
            uVec.normalize();
            double dotCompare = uVec.dotProduct(uVec, acusticVec);
            qDebug() << "point  dotCom   " << dotCompare << "   valCos    " << valCos_comp;
            if(dotCompare >= valCos_comp){
                _objPosition.push_back(point->getPostion()*SIMULATION_SCALE);
                makeCurrent();
                point->setColor(DETECT_COLOR);
                point->changeColor(_shaderProgram, _colorAtrLocation);
            }
        }
    }

    double ySonar = activeSonar.get_GlPos().y()/SIMULATION_SCALE;
    for(auto const &pole : _poleModels){
        if((activeSonar.getTransmitter().get_GlPos() - pole->getPostion()*SIMULATION_SCALE).length() <= MAX_DISTANCE){
           QVector3D uVec = pole->getPostion()*SIMULATION_SCALE - activeSonar.getTransmitter().get_GlPos();
           uVec.normalize();
           double dotCompare = uVec.dotProduct(uVec, acusticVec);
           qDebug() << "pole  dotCom   " << dotCompare << "   valCos    " << valCos_comp;
           if(dotCompare >= valCos_comp){
               if((ySonar <= pole->getPostion().y() + pole->getScale().y()/2) && (ySonar >= pole->getPostion().y() - pole->getScale().y()/2)){
                   QVector3D polePos = pole->getPostion();
                   polePos[1] = ySonar;
                   _objPosition.push_back(polePos*SIMULATION_SCALE);
                   makeCurrent();
                   pole->setColor(DETECT_COLOR);
                   pole->changeColor(_shaderProgram, _colorAtrLocation);
               }
           }
        }
    }


    getFlatSurfacePosition(_objPosition, activeSonar);
    update();
}

void MainScene::getFlatSurfacePosition(QVector<QVector3D> &_objPosition, Sonar const &activeSonar)
{
    QVector3D acusticVec = (activeSonar.getTransmitter().get_GlPos() - activeSonar.get_GlPos()).normalized();
    double alpha_rad = (activeSonar.getTransmitter().getTransmittedSignal().get_alpha()/2)*M_PI/180;
    double valCos_comp = std::cos(alpha_rad);
    double xTransmitter = activeSonar.getTransmitter().get_GlPos().x()/SIMULATION_SCALE;
    double yTransmitter = activeSonar.getTransmitter().get_GlPos().y()/SIMULATION_SCALE;
    double zTransmitter = activeSonar.getTransmitter().get_GlPos().z()/SIMULATION_SCALE;
    int flag = -1;

    for(auto const &flat : _flatSurfaceModels){
        if((activeSonar.getTransmitter().get_GlPos() - flat->getPostion()*SIMULATION_SCALE).length() <= MAX_DISTANCE && int(activeSonar.get_rotation()) % 90 == 0){
            QVector3D flatPos;
            if((yTransmitter <= flat->getPostion().y() + flat->getScale().y()/2) && (yTransmitter >= flat->getPostion().y() - flat->getScale().y()/2)){
                if((xTransmitter <= flat->getPostion().x() + flat->getScale().x()/2) && (xTransmitter >= flat->getPostion().x() - flat->getScale().x()/2)){
                    flatPos = flat->getPostion();
                    flatPos[0] = xTransmitter;
                    flatPos[1] = yTransmitter;
                    if(activeSonar.get_rotation() == 0)
                        flatPos[2] = flatPos[2] - flat->getScale().z()/2;
                    else
                        flatPos[2] = flatPos[2] + flat->getScale().z()/2;
                    flag=1;
                }
                if((zTransmitter <= flat->getPostion().z() + flat->getScale().z()/2) && (zTransmitter >= flat->getPostion().z() - flat->getScale().z()/2)){
                    flatPos = flat->getPostion();
                    if(activeSonar.get_rotation() == 90)
                        flatPos[0] = flatPos[0] - flat->getScale().x()/2;
                    else
                        flatPos[0] = flatPos[0] + flat->getScale().x()/2;
                    flatPos[1] = yTransmitter;
                    flatPos[2] = zTransmitter;
                    flag=1;
                }
                QVector3D uVec = (flatPos*SIMULATION_SCALE - activeSonar.getTransmitter().get_GlPos()).normalized();
                double dotCompare = uVec.dotProduct(uVec, acusticVec);
                qDebug() << "flat  dotCom   " << dotCompare << "   valCos    " << valCos_comp;
                if(dotCompare > valCos_comp && flag > 0){
                    _objPosition.push_back(flatPos*SIMULATION_SCALE);
                    makeCurrent();
                    flat->setColor(DETECT_COLOR);
                    flat->changeColor(_shaderProgram, _colorAtrLocation);
                }
            }
        }
    }
}

void MainScene::addPoleModel()
{
    makeCurrent();
    _poleModels.push_back(std::make_shared<PoleModel>());
    _poleModels.back()->setColor(QVector3D(0,0,0));
    _poleModels.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    _poleModels.back()->translate(-25, 15, 0);

}

void MainScene::addPointModel()
{
    makeCurrent();
    _pointModels.push_back(std::make_shared<PointModel>());
    _pointModels.back()->setColor(QVector3D(0,0,1));
    _pointModels.back()->translate(0, 5, 0);
    _pointModels.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
}

void MainScene::addFlatSurfaceModel()
{
    makeCurrent();
    _flatSurfaceModels.push_back(std::make_shared<FlatSurfaceModel>());
    _flatSurfaceModels.back()->setColor(QVector3D(0.5, 0.5, 0));
    _flatSurfaceModels.back()->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    _flatSurfaceModels.back()->translate(40, 0,0);
}

void MainScene::addGridModel()
{
    makeCurrent();
    _gridModel = std::make_shared<GridModel>();
    _gridModel->createMesh(_shaderProgram, _vertexAtrLocation, _colorAtrLocation);
    _gridModel->setScale(30, 30, 30);
}

QVector3D MainScene::getLastObjPosition(const Objects &obj) const
{
    switch(obj)
    {
        case PLATFORM:
                return _platformModel->getPostion();
        case POINT:
                return _pointModels.back()->getPostion();
        case STAKE:
                return _poleModels.back()->getPostion();
        case FLAT_SURFACE:
                return _flatSurfaceModels.back()->getPostion();
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    return QVector3D(0,0,0);
}

QVector3D MainScene::getObjPosition(const Objects &obj, const uint &objId)
{
    switch(obj)
    {
        case PLATFORM:
                assert(_platformModel != nullptr);
                qDebug() << "TransPos  " <<_platformModel->getSonarModel(0).getTransmitterModel().getPostion()*SIMULATION_SCALE;
                return _platformModel->getPostion();
        case POINT:
                assert(objId < _pointModels.size());
                return _pointModels[objId]->getPostion();
        case STAKE:
                assert(objId < _poleModels.size());
                return _poleModels[objId]->getPostion();
        case FLAT_SURFACE:
                assert(objId < _flatSurfaceModels.size());
                return _flatSurfaceModels[objId]->getPostion();
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    return QVector3D(0,0,0);
}

uint MainScene::getNumOfObject()
{
    return _pointModels.size() + _poleModels.size() + _flatSurfaceModels.size();
}

double MainScene::getPlatformRotation() const
{
    assert(_platformModel != nullptr);
    return _platformModel->get_anglRoot();

}

uint MainScene::getNumfOfObj(const Objects &obj) const
{
    switch(obj)
    {
        case PLATFORM:
                if(_platformModel != nullptr)
                    return 1;
                else
                    return 0;
        case POINT:
                return _pointModels.size();
        case STAKE:
                return _poleModels.size();
        case FLAT_SURFACE:
                return _flatSurfaceModels.size();
        default:
            qDebug() << "nie ma takiego obiektu";
            break;
    }

    return 0;
}

void MainScene::deleteSelectedObj(const Objects &obj, const uint &objId)
{
    makeCurrent();
    switch (obj) {
    case POINT:
        assert(objId < _pointModels.size());
        _pointModels[objId]->deleteModel();
        _pointModels.erase(_pointModels.cbegin()+objId);
        break;
    case STAKE:
        assert(objId < _poleModels.size());
        _poleModels[objId]->deleteModel();
        _poleModels.erase(_poleModels.cbegin()+objId);
        break;
    case FLAT_SURFACE:
        assert(objId < _flatSurfaceModels.size());
        _flatSurfaceModels[objId]->deleteModel();
        _flatSurfaceModels.erase(_flatSurfaceModels.cbegin()+objId);
        break;
    default:
        qDebug() << "Nie ma takiego obiektu";
        break;
    }
    update();
}

void MainScene::setPositionOfSelectedObj(const Objects &obj, const uint &objId, const QVector3D &position)
{
    switch (obj) {
    case PLATFORM:
        assert(_platformModel !=  nullptr);
        _platformModel->setPlatformPosition(position/SIMULATION_SCALE);
        break;
    case POINT:
        assert(objId < _pointModels.size());
        _pointModels[objId]->setPosition(position/SIMULATION_SCALE);
        break;
    case STAKE:
        assert(objId < _poleModels.size());
        _poleModels[objId]->setPosition(position/SIMULATION_SCALE);
        break;
    case FLAT_SURFACE:
        assert(objId < _flatSurfaceModels.size());
        _flatSurfaceModels[objId]->setPosition(position/SIMULATION_SCALE);
        break;
    default:
        break;
    }

    update();
}

void MainScene::deleteAllObjects()
{
    deleteAllPointModel();
    deleteAllFlatSurfaceModel();
    deleteAllPoleModel();
}

void MainScene::deleteAllPointModel()
{
    for(auto &pointModel : _pointModels)
        pointModel->deleteModel();

    if(!_pointModels.empty())
        _pointModels.clear();

}

void MainScene::deleteAllPoleModel()
{
    for(auto &poleModel : _poleModels)
        poleModel->deleteModel();

    if(!_poleModels.empty())
        _poleModels.clear();
}

void MainScene::deleteAllFlatSurfaceModel()
{
    for(auto &flatSurfaceModel : _flatSurfaceModels)
        flatSurfaceModel->deleteModel();

    if(!_flatSurfaceModels.empty())
        _flatSurfaceModels.clear();
}

void MainScene::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

    if(!initShaders())
        qDebug() << "Nie udalo sie zainicjalozowac shaderow";

    getShaderParam();
    addGridModel();
    _textShaderProgram.bind();
    _textShaderProgram.setUniformValue("inTexture", 0);
}

void MainScene::paintGL()
{
    makeCurrent();
    _shaderProgram.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix, mMatrix, sMatrix;
    sMatrix.scale(0.35);
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation.map(QVector3D(0, 0, cameraFar));
    QVector3D cameraUpDirection = cameraTransformation.map(QVector3D(0, 1, 0));

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    _shaderProgram.setUniformValue(_pMatrixLocation, _pMatrix);
    _shaderProgram.setUniformValue(_vMatrixLocation, vMatrix);

    if(_gridModel != nullptr && _gridModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, _gridModel->get_mMatrix());
        _gridModel->drawGrid();
    }

    if(_platformModel != nullptr){
        drawPlatform(_platformModel);

        for(uint sonarId = 0; sonarId < _platformModel->getNumOfSonarModels(); ++sonarId){
            drawSonar(_platformModel->getSonarModelPointer(sonarId));
            drawTransmitter(_platformModel->getSonarModel(sonarId).getTransmitterModelPointer());
            for(uint recId = 0; recId < _platformModel->getSonarModel(sonarId).getNumOfReceiverModels(); ++recId){
                drawReceiver(_platformModel->getSonarModel(sonarId).getReceiverModelPointer(recId));
            }
        }
    }

    if(!_poleModels.empty()){
        for(auto &poleModel: _poleModels){
            if(poleModel->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, poleModel->get_mMatrix());
                poleModel->draw_line();
            }
        }
    }

    if(!_pointModels.empty()){
        for(auto &pointModel : _pointModels){
            if(pointModel->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, pointModel->get_mMatrix());
                pointModel->draw_point();
            }
        }
    }

    if(!_flatSurfaceModels.empty()){
        for(auto &flatSurface : _flatSurfaceModels){
            if(flatSurface->isCreated()){
                _shaderProgram.setUniformValue(_mMatrixLocation, flatSurface->get_mMatrix());
                flatSurface->draw();
            }
        }
    }
    _shaderProgram.release();

    _textShaderProgram.bind();
    _textShaderProgram.setUniformValue(_textpMatrixLocation, _pMatrix);
    _textShaderProgram.setUniformValue(_textvMatrixLocation, vMatrix);
    if(!_detectedPoints.empty()){
        for(auto &detectPoint : _detectedPoints){
            if(detectPoint->isCreated()){
                _textShaderProgram.setUniformValue(_textmMatrixLocation, detectPoint->get_mMatrix());
                detectPoint->draw();
            }
        }
    }
    _textShaderProgram.release();
}

void MainScene::resizeGL(int width, int height)
{
    if(height == 0) height = 1;

    _pMatrix.setToIdentity();
    _pMatrix.perspective(60.0, (float)width/(float)height, 0.001f, 1000);

    glViewport(0, 0, width, height);
}

void MainScene::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void MainScene::mouseMoveEvent(QMouseEvent *event)
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
void MainScene::translateSeletctedObj(QVector3D const &translate)
{
    switch (selectedObj) {
    case PLATFORM:
        if(_platformModel!= nullptr && _platformModel->isCreated())
            _platformModel->translatePlatform(translate);
        break;
    case POINT:
        if(!_pointModels.empty() && selectedIdObject >= 0 && selectedIdObject < int(_pointModels.size()))
            _pointModels[selectedIdObject]->translate(translate);
        break;
    case STAKE:
        if(!_poleModels.empty() && selectedIdObject >= 0 && selectedIdObject < int(_poleModels.size()))
            _poleModels[selectedIdObject]->translate(translate);
        break;
    case FLAT_SURFACE:
        if(!_flatSurfaceModels.empty() && selectedIdObject >= 0 && selectedIdObject < int(_flatSurfaceModels.size()))
            _flatSurfaceModels[selectedIdObject]->translate(translate);
        break;
    default:
        break;
    }
}

void MainScene::rotateSelectectedObj(double const &rotate)
{
    if(selectedObj == PLATFORM)
        _platformModel->rotatePlatformModel(rotate);
}

void MainScene::keyPressEvent(QKeyEvent *key)
{
//    if(_platformModel != nullptr && _platformModel->isCreated()){
//        if(key->key() == Qt::Key_Q)
//            _platformModel->rotatePlatformModel(-5);
//        if(key->key() == Qt::Key_E)
//            _platformModel->rotatePlatformModel(5);
//        if(key->key() == Qt::Key_W)
//            _platformModel->translatePlatform(QVector3D(0, 2, 0));
//        if(key->key() == Qt::Key_S)
//            _platformModel->translatePlatform(QVector3D(0, -2, 0));
//        if(key->key() == Qt::Key_A)
//            _platformModel->translatePlatform(QVector3D(-2, 0, 0));
//        if(key->key() == Qt::Key_D)
//            _platformModel->translatePlatform(QVector3D(2, 0, 0));
//        if(key->key() == Qt::Key_Z)
//            _platformModel->translatePlatform(QVector3D(0, 0, 2));
//        if(key->key() == Qt::Key_X)
//            _platformModel->translatePlatform(QVector3D(0, 0, -2));

//        emit platformPositionChanged();
//    }
            if(key->key() == Qt::Key_Q)
                rotateSelectectedObj(-5);
            if(key->key() == Qt::Key_E)
                rotateSelectectedObj(5);
            if(key->key() == Qt::Key_W)
                translateSeletctedObj(QVector3D(0, 2, 0));
            if(key->key() == Qt::Key_S)
                translateSeletctedObj(QVector3D(0, -2, 0));
            if(key->key() == Qt::Key_A)
                translateSeletctedObj(QVector3D(-2, 0, 0));
            if(key->key() == Qt::Key_D)
                translateSeletctedObj(QVector3D(2, 0, 0));
            if(key->key() == Qt::Key_Z)
                translateSeletctedObj(QVector3D(0, 0, 2));
            if(key->key() == Qt::Key_X)
                translateSeletctedObj(QVector3D(0, 0, -2));

            emit objPositionChanged(selectedObj, selectedIdObject);
            update();
}

void MainScene::wheelEvent(QWheelEvent *event)
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

bool MainScene::initShaders()
{
    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader_mainScene.frag"))
        return false;

    if(!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader_mainScene.vert"))
        return false;

    if(!_shaderProgram.link())
        return false;

    if(!_textShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shaders/color_shader_mainScene_texture.frag"))
        return false;

    if(!_textShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shaders/color_shader_mainScene_texture.vert"))
        return false;

    if(!_textShaderProgram.link())
        return false;

    return true;
}

void MainScene::getShaderParam()
{
    _vertexAtrLocation = _shaderProgram.attributeLocation("vertex");
    _colorAtrLocation = _shaderProgram.attributeLocation("color");
    _pMatrixLocation = _shaderProgram.uniformLocation("pMatrix");
    _vMatrixLocation = _shaderProgram.uniformLocation("vMatrix");
    _mMatrixLocation = _shaderProgram.uniformLocation("mMatrix");

    _textVertexAtrLocation = _textShaderProgram.attributeLocation("vertex");
    _textureCoordinates = _textShaderProgram.attributeLocation("textureCoordinate");
    _textpMatrixLocation = _textShaderProgram.uniformLocation("pMatrix");
    _textvMatrixLocation = _textShaderProgram.uniformLocation("vMatrix");
    _textmMatrixLocation = _textShaderProgram.uniformLocation("mMatrix");
}

void MainScene::drawPlatform(std::shared_ptr<PlatformModel> const &inPlatformModel)
{
    if(inPlatformModel == nullptr) return;

    if(inPlatformModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inPlatformModel->get_mMatrix());
        inPlatformModel->draw();
    }
}

void MainScene::drawSonar(std::shared_ptr<SonarModel> const &inSonarModel)
{
    if(inSonarModel == nullptr) return;

    if(inSonarModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inSonarModel->get_mMatrix());
        inSonarModel->draw();
    }
}

void MainScene::drawReceiver(std::shared_ptr<Model> const &inReceiverModel)
{
    if(inReceiverModel == nullptr) return;

    if(inReceiverModel->isCreated()){
        _shaderProgram.setUniformValue(_mMatrixLocation, inReceiverModel->get_mMatrix());
        inReceiverModel->draw();
    }
}

void MainScene::drawTransmitter(std::shared_ptr<Model> const &inTransmitterModel)
{
    if(inTransmitterModel->isCreated() && inTransmitterModel != nullptr){
        _shaderProgram.setUniformValue(_mMatrixLocation, inTransmitterModel->get_mMatrix());
        inTransmitterModel->draw();
    }
}

void MainScene::addSonar(const Sonar &inSonar, std::shared_ptr<PlatformModel> &inPlatformModel)
{
    inPlatformModel->addSonarModel();
    SonarModel &sonModel = inPlatformModel->useSonarModel(inPlatformModel->getNumOfSonarModels()-1);
    sonModel.setPosition(inSonar.get_LocPos()/SIMULATION_SCALE);
    sonModel.setScale(inSonar.get_size()/SIMULATION_SCALE);
    sonModel.setSonarColor(inSonar.get_color());
    sonModel.setModelId(inSonar.get_id());
    sonModel.rotateSonarModel(inSonar.get_rotation());

    sonModel.trans_setAcusticVector(inSonar.getTransmitter().get_acusticVec()/SIMULATION_SCALE);
    addTransmitter(inSonar.getTransmitter(), sonModel);

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        addReceiver(inSonar.getReceiver(recId), sonModel);
}

void MainScene::addTransmitter(const Transmitter &inTransmitter, SonarModel &inSonarModel)
{
    Model &transmitterModel = inSonarModel.useTransmitterModel();
    transmitterModel.setPosition(inTransmitter.get_GlPos()/SIMULATION_SCALE);
    transmitterModel.setScale(inTransmitter.get_size()/SIMULATION_SCALE);
}

void MainScene::addReceiver(Receiver const &inReceiver, SonarModel &inSonarModel)
{
    inSonarModel.addReceiverModel();
    Model &receiverModel = inSonarModel.useReceiverModel(inSonarModel.getNumOfReceiverModels()-1);

    receiverModel.setPosition(inReceiver.get_GlPos()/SIMULATION_SCALE);
    receiverModel.setScale(inReceiver.get_size()/SIMULATION_SCALE);
    receiverModel.setModelId(inReceiver.get_id());
    receiverModel.setRotate(inReceiver.get_rotation());
}

void MainScene::getTransmitterParam(Transmitter &inTransmitter, const Model &transmitterModel) const
{
    inTransmitter.set_GlPos(transmitterModel.getPostion()*SIMULATION_SCALE);
    inTransmitter.set_rotation(transmitterModel.get_anglRoot());
}

void MainScene::getReceiverParam(Receiver &inReceiver, const Model &receiverModel) const
{
    inReceiver.set_GlPos(receiverModel.getPostion()*SIMULATION_SCALE);
    inReceiver.set_id(receiverModel.getModelId());
    inReceiver.set_rotation(receiverModel.get_anglRoot());
}

void MainScene::getSonarParam(Sonar &inSonar, const SonarModel &sonModel) const
{
    inSonar.set_GlPos(sonModel.getPostion()*SIMULATION_SCALE);
    inSonar.set_rotation(sonModel.get_anglRoot());
    inSonar.set_id(sonModel.getModelId());

    inSonar.useTransmitter().set_acusticVec(sonModel.trans_getAcusticVector()*SIMULATION_SCALE);
    getTransmitterParam(inSonar.useTransmitter(), sonModel.getTransmitterModel());

    for(uint recId=0; recId < inSonar.get_numOfRec(); ++recId)
        getReceiverParam(inSonar.useReceiver(recId), sonModel.getReceiverModel(recId));
}

bool MainScene::isPlatformModelCreated() const
{
    if(_platformModel != nullptr && _platformModel->isCreated())
        return true;

    return false;
}

void MainScene::setObjSize(const Objects &obj, const uint &objId, const QVector3D &newSize)
{
    switch (obj) {
    case PLATFORM:
        return;
        break;
    case POINT:
        assert(objId < _pointModels.size());
        _pointModels[objId]->setPointSize(newSize[0]);
        break;
    case STAKE:
        assert(objId < _poleModels.size());
        _poleModels[objId]->setScale(QVector3D(0, newSize[0], 0)/SIMULATION_SCALE);
        break;
    case FLAT_SURFACE:
        assert(objId < _flatSurfaceModels.size());
        _flatSurfaceModels[objId]->setScale(newSize/SIMULATION_SCALE);
        break;
    default:
        break;
    }

    update();
}

QVector3D MainScene::getObjSize(const Objects &obj, const uint &objId) const
{
    switch (obj) {
    case POINT:
        assert(objId < _pointModels.size());
        return QVector3D(_pointModels[objId]->getPointSize(), 0, 0);
        break;
    case STAKE:
        assert(objId < _poleModels.size());
        return QVector3D(_poleModels[objId]->getScale()[1], 0, 0)*SIMULATION_SCALE;
        break;
    case FLAT_SURFACE:
        assert(objId < _flatSurfaceModels.size());
        return _flatSurfaceModels[objId]->getScale()*SIMULATION_SCALE;
        break;
    default:
        break;
    }

    return QVector3D(0,0,0);
}

void MainScene::setPatformRotation(const double &rot_deg)
{
    if(_platformModel->get_anglRoot() == rot_deg)
        return;


    double rot = rot_deg - _platformModel->get_anglRoot();
    _platformModel->rotatePlatformModel(rot);
    update();
}

void MainScene::changActiveSonarColor(const uint &activeSonarId)
{

    makeCurrent();
    if(idActiveSonar < _platformModel->getNumOfSonarModels()){
        _platformModel->useSonarModel(idActiveSonar).setSonarColor(_platform.getSonar(idActiveSonar).get_color());
        _platformModel->useSonarModel(idActiveSonar).changeColor(_shaderProgram, _colorAtrLocation);
    }

    _platformModel->useSonarModel(activeSonarId).setSonarColor(QVector3D(0.96, 0.071, 0.98));
    _platformModel->useSonarModel(activeSonarId).changeColor(_shaderProgram, _colorAtrLocation);

    update();
    idActiveSonar = activeSonarId;
}

void MainScene::addDetectedPoint(QVector3D position)
{
    makeCurrent();
    _detectedPoints.push_back(std::make_shared<DetectedPoint>());
    _detectedPoints.back()->createMesh(_textShaderProgram, _textVertexAtrLocation, _textureCoordinates);
    _detectedPoints.back()->setPosition(position);
    update();
}

void MainScene::clearDetectedPoints()
{
    makeCurrent();
    _detectedPoints.clear();
}

void MainScene::setSelectedObj(const Objects &obj, const int &id)
{
    selectedIdObject = id;
    selectedObj = obj;
}

void MainScene::deleleteDetection()
{
    makeCurrent();
    for(auto &point : _pointModels){
        point->setColor(QVector3D(0,0,1));
        point->changeColor(_shaderProgram, _colorAtrLocation);
    }
    for(auto &pole : _poleModels){
        pole->setColor(QVector3D(0,0,0));
        pole->changeColor(_shaderProgram, _colorAtrLocation);
    }
    for(auto &flat : _flatSurfaceModels){
        flat->setColor(QVector3D(0.5, 0.5, 0));
        flat->changeColor(_shaderProgram, _colorAtrLocation);
    }

    update();
}

void MainScene::getScene(SimulationScene &scene)
{
    deleleteDetection();
    for(auto const &point : _pointModels){
        PointObject pointObject;
        pointObject.size = point->getPointSize();
        pointObject.position = point->getPostion()*SIMULATION_SCALE;
        pointObject.color = point->getColor();
        scene.points.push_back(pointObject);
    }
    for(auto const &pole : _poleModels){
        PoleObjects poleObject;
        poleObject.size = pole->getScale()*SIMULATION_SCALE;
        poleObject.position = pole->getPostion()*SIMULATION_SCALE;
        poleObject.color = pole->getColor();
        scene.poles.push_back(poleObject);
    }
    for(auto const &flat : _flatSurfaceModels){
        FlatObjects flatObject;
        flatObject.size = flat->getScale()*SIMULATION_SCALE;
        flatObject.position = flat->getPostion()*SIMULATION_SCALE;
        flatObject.color = flat->getColor();
        scene.flats.push_back(flatObject);
    }
}

void MainScene::loadScene(const SimulationScene &scene)
{

    deleteAllObjects();
    for(auto const & point : scene.points){
        addPointModel();
        _pointModels.back()->setPointSize(point.size);
        _pointModels.back()->setColor(point.color);
        _pointModels.back()->setPosition(point.position/SIMULATION_SCALE);
    }
    for(auto const & pole : scene.poles){
        addPoleModel();
        _poleModels.back()->setScale(pole.size/SIMULATION_SCALE);
        _poleModels.back()->setColor(pole.color);
        _poleModels.back()->setPosition(pole.position/SIMULATION_SCALE);
    }
    for(auto const & flat : scene.flats){
        addFlatSurfaceModel();
        _flatSurfaceModels.back()->setScale(flat.size/SIMULATION_SCALE);
        _flatSurfaceModels.back()->setColor(flat.color);
        _flatSurfaceModels.back()->setPosition(flat.position/SIMULATION_SCALE);
    }

    update();
}
