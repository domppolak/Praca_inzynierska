#ifndef STAKEMODEL_H
#define STAKEMODEL_H

#include "model.hh"

class PoleModel : public Model
{

public:
    PoleModel();
    ~PoleModel();
    void setColor(QVector3D const &inColor);
    QVector3D getColor() const;

private:
    void initVertices();
    void initColorVerticesWithNewColor();

    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;
    QVector<uint> _indices;

    QVector3D color;
};

inline QVector3D PoleModel::getColor() const
{
    return this->color;
}
#endif // STAKEMODEL_H
