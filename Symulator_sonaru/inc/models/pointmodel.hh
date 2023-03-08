#ifndef POINTMODEL_HH
#define POINTMODEL_HH

#include "model.hh"

class PointModel : public Model
{

public:
    PointModel();
    ~PointModel();
    void setColor(QVector3D const &color);
    QVector3D getColor() const;
private:
    void initVertices();
    void initColorVerticesWithNewColor();

    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;

    QVector3D color;
};

inline QVector3D PointModel::getColor() const
{
    return this->color;
}
#endif // POINTMODEL_HH
