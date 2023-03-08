#include "stakemodel.hh"

PoleModel::PoleModel()
{
    Model();
    color = QVector3D(0,0,0);
    initVertices();
    setVertices(_posVertices, _colorVertices);
    setScale(0, 35, 0);
    translate(20, 14,0);
    setLineWidth(6);
}

PoleModel::~PoleModel()
{
    deleteModel();
}

void PoleModel::setColor(const QVector3D &inColor)
{
    this->color = inColor;
    initColorVerticesWithNewColor();
    setColorVertices(_colorVertices);
}

void PoleModel::initVertices()
{
    _posVertices << QVector3D(0, -0.5, 0) << QVector3D(0, 0.5, 0);
    _colorVertices << color << color;
    _indices << 0 << 1 << 0;
}

void PoleModel::initColorVerticesWithNewColor()
{
    _colorVertices.clear();
    _colorVertices << color << color;
}
