#ifndef FLATSURFACEMODEL_HH
#define FLATSURFACEMODEL_HH

#include "model.hh"

class FlatSurfaceModel : public Model
{

public:
    FlatSurfaceModel();
    ~FlatSurfaceModel();
    void setColor(QVector3D const &inColor);
    QVector3D getColor() const;

private:
    void initVertices();
    void initColorVerticesWithNewColor();

    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;

    QVector3D color;

};

inline QVector3D FlatSurfaceModel::getColor() const
{
    return this->color;
}
#endif // FLATSURFACEMODEL_HH
