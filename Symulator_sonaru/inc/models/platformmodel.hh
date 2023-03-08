#ifndef PLATFORMMODEL_HH
#define PLATFORMMODEL_HH

#include "sonarmodel.hh"
#include "model.hh"

class PlatformModel : public Model
{
public:
    PlatformModel();
    ~PlatformModel();

    void deleteAllSonarModels();
    void addSonarModel();
    void addSonarModel(std::shared_ptr<SonarModel> &inSonarModel);
    void deleteSonarModel();
    void deleteSonarModel(const uint &sonarId);
    void deletePlatformModel();
    void createAllMeshes(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation);
    void setPlatformPosition(QVector3D const &newPosition);
    void translatePlatform(QVector3D const &_translate);
    void scalePlatformModel(double inScale);
    void rotatePlatformModel(double const &rot);
    uint getNumOfSonarModels();

    std::shared_ptr<SonarModel> const &getSonarModelPointer(uint const &sonarId) const;
    std::shared_ptr<SonarModel> &useSonarModelPointer(uint const &sonarId);

    SonarModel const &getSonarModel(uint const &sonarId) const;
    SonarModel &useSonarModel(uint const &sonarId);


private:
    void initVertices();

    std::vector<std::shared_ptr<SonarModel>> sonarModels;

    QVector<QVector3D> _platformVertices;
    QVector<QVector3D> _platformColorVertices;
};

inline uint PlatformModel::getNumOfSonarModels()
{
    return sonarModels.size();
}

inline const std::shared_ptr<SonarModel> &PlatformModel::getSonarModelPointer(const uint &sonarId) const
{
    assert(sonarId < sonarModels.size());
    return sonarModels[sonarId];
}

inline std::shared_ptr<SonarModel> &PlatformModel::useSonarModelPointer(const uint &sonarId)
{
    assert(sonarId < sonarModels.size());
    return sonarModels[sonarId];
}

inline const SonarModel &PlatformModel::getSonarModel(const uint &sonarId) const
{
    assert(sonarId < sonarModels.size());
    return *sonarModels[sonarId];
}

inline SonarModel &PlatformModel::useSonarModel(const uint &sonarId)
{
    assert(sonarId < sonarModels.size());
    return *sonarModels[sonarId];
}

#endif // PLATFORMMODEL_HH
