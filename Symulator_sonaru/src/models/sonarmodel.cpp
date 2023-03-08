#include "sonarmodel.hh"
#include <cmath>

SonarModel::SonarModel()
{
    trans_acusticVec = QVector3D(0,0,50);
    Model();
    initAllVertices();
    transmitterModel = std::make_shared<Model>();
}

SonarModel::~SonarModel()
{
    deleteSonarModel();
}

void SonarModel::deleteAllReceiverModels()
{
    for(auto &rec : receiverModels)
        rec->deleteModel();

    receiverModels.clear();
}

void SonarModel::addReceiverModel()
{
    receiverModels.push_back(std::make_shared<Model>());
}

void SonarModel::addReceiverModel(std::shared_ptr<Model> &receiverModel)
{
    receiverModels.push_back(receiverModel);
}

void SonarModel::deleteSonarModel()
{
    deleteModel();
    transmitterModel->deleteModel();
    for(auto &rec : receiverModels)
        rec->deleteModel();
}

void SonarModel::deleteReceiverModel()
{
    receiverModels.back()->deleteModel();
    receiverModels.pop_back();
}

void SonarModel::deleteReceiverModel(const uint &recId)
{
    assert(recId < receiverModels.size());
    receiverModels[recId]->deleteModel();
    receiverModels.erase(receiverModels.cbegin()+recId);
}

void SonarModel::setSonarColor(const QVector3D &sonarColor)
{
    _sonarColorVertices.clear();
    this->sonarColor = sonarColor;
    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);
    for(uint i=0; i<30; i++) _sonarColorVertices << sonarColor;
    setColorVertices(_sonarColorVertices);
}

void SonarModel::createMeshes(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation)
{
    if(!isCreated()){
        setVertices(_sonarVertices, _sonarColorVertices);
        createMesh(program, _vertexLocation, _colorLocation);
    }
    if(!transmitterModel->isCreated()){
        transmitterModel->setVertices(_transmitterVertices, _transmitterColorVertices);
        transmitterModel->createMesh(program, _vertexLocation, _colorLocation);
    }

    for(auto &rec : receiverModels){
        if(rec->isCreated()) continue;
        rec->setVertices(_receiverVertices, _receiverColorVertices);
        rec->createMesh(program, _vertexLocation, _colorLocation);
    }
}

void SonarModel::setSonarPostion(const QVector3D &newPosition)
{
    QVector3D _translate = newPosition - getPostion();
    setPosition(newPosition);
    transmitterModel->translate(_translate);
    trans_acusticVec += _translate;

    for(auto &rec : receiverModels)
        rec->translate(_translate);
}

void SonarModel::translateSonar(const QVector3D &_translate)
{
    translate(_translate);
    transmitterModel->translate(_translate);
    trans_acusticVec += _translate;

    for(auto &rec : receiverModels)
        rec->translate(_translate);
}

void SonarModel::scaleSonarModel(double _scale)
{
    scale(_scale);
    transmitterModel->scale(_scale);
    for(auto &rec : receiverModels)
        rec->scale(_scale);
}

void SonarModel::rotateSonarModel(const double &rot_deg)
{

    rotate(rot_deg);
    QMatrix4x4 rotMatrix;
    rotMatrix.rotate(rot_deg, 0,1,0);
    QVector3D transmitterTranslation = transmitterModel->getPostion()- getPostion();
    transmitterModel->translate(-transmitterTranslation);
    transmitterModel->rotate(rot_deg);
    transmitterModel->translate(rotMatrix.map(transmitterTranslation));

    trans_acusticVec -= transmitterTranslation;
    trans_acusticVec = rotMatrix.map(trans_acusticVec);
    trans_acusticVec += transmitterTranslation;

    for(auto &rec : receiverModels){
        QVector3D recTranslation = rec->getPostion() - getPostion();
        rec->translate(-recTranslation);
        rec->rotate(rot_deg);
        rec->translate(rotMatrix.map(recTranslation));
    }

}

void SonarModel::setSonarScale(const QVector3D &scale)
{
    setScale(scale);
    QVector3D lastPos = transmitterModel->getPostion();
    transmitterModel->setPosition(0, 0, scale[2]/2);
    trans_acusticVec = trans_acusticVec + (transmitterModel->getPostion() - lastPos);
}

void SonarModel::initAllVertices()
{
    initSonar_pos_color_Vertices();
    initReceiver_pos_color_Vertices();
    initTransmitter_pos_color_Vertices();
}

void SonarModel::initReceiver_pos_color_Vertices()
{
    uint n=8;
    double angle = (360/8)*(M_PI/180);
    double r = 0.5;
    double h=0.5;
    QVector<QVector3D> front, back, side;
    for(uint i=0; i<n;i++){
        front.push_back(QVector3D(0,0, h));
        front.push_back(QVector3D(r*cos(angle*i), r*sin(angle*i), h));
        front.push_back(QVector3D(r*cos(angle*(i+1)), r*sin(angle*(i+1)), h));
    }
    for(uint i=0; i<n;i++){
        back.push_back(QVector3D(0,0, -h));
        back.push_back(QVector3D(r*cos(angle*i), r*sin(angle*i), -h));
        back.push_back(QVector3D(r*cos(angle*(i+1)), r*sin(angle*(i+1)), -h));
    }

    //zmienic na for ale to kiedys + dodac do tego IBO
    // bo to troche nie ma sensu rysowanie ciagle tych samych wspolrzednych
    //1
    side.push_back(front[1]);
    side.push_back(front[2]);
    side.push_back(back[1]);

    side.push_back(back[1]);
    side.push_back(front[2]);
    side.push_back(back[2]);

    //2
    side.push_back(front[2]);
    side.push_back(back[2]);
    side.push_back(front[4]);

    side.push_back(front[4]);
    side.push_back(back[4]);
    side.push_back(back[2]);

    //3
    side.push_back(front[4]);
    side.push_back(back[4]);
    side.push_back(front[5]);

    side.push_back(front[5]);
    side.push_back(back[4]);
    side.push_back(back[5]);
    //4
    side.push_back(front[5]);
    side.push_back(back[5]);
    side.push_back(front[7]);

    side.push_back(front[7]);
    side.push_back(back[5]);
    side.push_back(back[7]);
    //5
    side.push_back(front[7]);
    side.push_back(back[7]);
    side.push_back(front[8]);

    side.push_back(front[8]);
    side.push_back(back[7]);
    side.push_back(back[8]);
    //6
    side.push_back(front[8]);
    side.push_back(back[8]);
    side.push_back(front[10]);

    side.push_back(front[10]);
    side.push_back(back[8]);
    side.push_back(back[10]);
    //7
    side.push_back(front[10]);
    side.push_back(back[10]);
    side.push_back(front[11]);

    side.push_back(front[11]);
    side.push_back(back[10]);
    side.push_back(back[11]);
    //8
    side.push_back(front[11]);
    side.push_back(back[11]);
    side.push_back(front[13]);

    side.push_back(front[13]);
    side.push_back(back[11]);
    side.push_back(back[13]);
    //9
    side.push_back(front[13]);
    side.push_back(back[13]);
    side.push_back(front[14]);

    side.push_back(front[14]);
    side.push_back(back[13]);
    side.push_back(back[14]);
    //10
    side.push_back(front[14]);
    side.push_back(back[14]);
    side.push_back(front[16]);

    side.push_back(front[16]);
    side.push_back(back[14]);
    side.push_back(back[16]);
    //11
    side.push_back(front[16]);
    side.push_back(back[16]);
    side.push_back(front[17]);

    side.push_back(front[17]);
    side.push_back(back[16]);
    side.push_back(back[17]);
    //12
    side.push_back(front[17]);
    side.push_back(back[17]);
    side.push_back(front[19]);

    side.push_back(front[19]);
    side.push_back(back[17]);
    side.push_back(back[19]);
    //13
    side.push_back(front[19]);
    side.push_back(back[19]);
    side.push_back(front[20]);

    side.push_back(front[20]);
    side.push_back(back[19]);
    side.push_back(back[20]);
    //14
    side.push_back(front[20]);
    side.push_back(back[20]);
    side.push_back(front[22]);

    side.push_back(front[22]);
    side.push_back(back[20]);
    side.push_back(back[22]);
    //15
    side.push_back(front[22]);
    side.push_back(back[22]);
    side.push_back(front[23]);

    side.push_back(front[23]);
    side.push_back(back[22]);
    side.push_back(back[23]);

    for(auto const &cord : front) _receiverVertices.push_back(cord);
    for(auto const &cord : back) _receiverVertices.push_back(cord);
    for(auto const &cord : side) _receiverVertices.push_back(cord);
    for(uint i=0; i<3*n; i++) _receiverColorVertices << QVector3D(1,1,0);
    for(uint i=0; i<_receiverVertices.size()-3*n; i++) _receiverColorVertices << QVector3D(0,0,0);
}

void SonarModel::initTransmitter_pos_color_Vertices()
{
    _transmitterVertices = _receiverVertices;
    for(uint i=0; i<_transmitterVertices.size(); i++) _transmitterColorVertices << QVector3D(0,0,0);
}

void SonarModel::initSonar_pos_color_Vertices()
{
    sonarColor = QVector3D(0,1,0);
    _sonarVertices << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
                   << QVector3D( 0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5)
                   << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) // Back
                   << QVector3D(-0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) // Left
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) // Right
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5)
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5, -0.5) // Top
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5,  0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5,  0.5) // Bottom
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5, -0.5);

    for(uint i=0; i<6; i++) _sonarColorVertices << QVector3D(1,0,0);
    for(uint i=0; i<30; i++) _sonarColorVertices<< sonarColor;
}
