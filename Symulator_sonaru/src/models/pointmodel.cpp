#include "pointmodel.hh"

PointModel::PointModel()
{
    Model();
    color = QVector3D(0, 0, 1);
    initVertices();
    setVertices(_posVertices, _colorVertices);
    setPointSize(10);
}

PointModel::~PointModel()
{
    deleteModel();
}

void PointModel::setColor(const QVector3D &color)
{
    this->color = color;
    initColorVerticesWithNewColor();
    setColorVertices(_colorVertices);
}

void PointModel::initVertices()
{
    _posVertices << QVector3D(0,0,0);
    _colorVertices << color;
}

void PointModel::initColorVerticesWithNewColor()
{
    _colorVertices.clear();
    _colorVertices << color;
}
