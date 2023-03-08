#include "platformmodel.hh"

PlatformModel::PlatformModel()
{
    Model();
    initVertices();
}

PlatformModel::~PlatformModel()
{
    deletePlatformModel();
}

void PlatformModel::deleteAllSonarModels()
{
    for(auto &sonar : sonarModels)
        sonar->deleteSonarModel();

    sonarModels.clear();
}

void PlatformModel::addSonarModel()
{
    sonarModels.push_back(std::make_shared<SonarModel>());
}

void PlatformModel::addSonarModel(std::shared_ptr<SonarModel> &inSonarModel)
{
    sonarModels.push_back(inSonarModel);
}

void PlatformModel::deleteSonarModel()
{
    if(sonarModels.empty())
        return;
    sonarModels.back()->deleteSonarModel();
    sonarModels.pop_back();
}

void PlatformModel::deleteSonarModel(const uint &sonarId)
{
    assert(sonarId < sonarModels.size());
    sonarModels[sonarId]->deleteSonarModel();
    sonarModels.erase(sonarModels.cbegin()+sonarId);
}

void PlatformModel::deletePlatformModel()
{
    deleteModel();

    for(auto &sonar : sonarModels)
        sonar->deleteSonarModel();
}

void PlatformModel::createAllMeshes(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation)
{
    if(!isCreated()){
        setVertices(_platformVertices, _platformColorVertices);
        createMesh(program, _vertexLocation, _colorLocation);
    }

    for(auto &sonar : sonarModels)
        sonar->createMeshes(program, _vertexLocation, _colorLocation);
}

void PlatformModel::setPlatformPosition(const QVector3D &newPosition)
{
    QVector3D _translate = newPosition - getPostion();
    setPosition(newPosition);

    for(auto &sonar : sonarModels)
        sonar->translateSonar(_translate);
}

void PlatformModel::translatePlatform(const QVector3D &_translate)
{
    translate(_translate);

    for(auto &sonar : sonarModels)
        sonar->translateSonar(_translate);
}

void PlatformModel::scalePlatformModel(double inScale)
{
    scale(inScale);
    for(auto &son : sonarModels)
        son->scaleSonarModel(inScale);
}

void PlatformModel::rotatePlatformModel(const double &rot)
{
    rotate(rot);
    QMatrix4x4 rotMatrix;
    rotMatrix.rotate(rot, 0,1,0);
    for(auto &sonar : sonarModels){
        QVector3D sonarTranslation = sonar->getPostion() - getPostion();
        sonar->translateSonar(-sonarTranslation);
        sonar->rotateSonarModel(rot);
        sonar->translateSonar(rotMatrix.map(sonarTranslation));
    }
}

void PlatformModel::initVertices()
{
    _platformVertices  << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
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

    for(uint i=0; i<6; ++i) _platformColorVertices << QVector3D(0, 0.2, 0.2);
    for(uint i=0; i<30; ++i) _platformColorVertices << QVector3D(1,0.5,0.5);
}




