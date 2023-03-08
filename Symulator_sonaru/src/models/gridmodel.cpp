#include "gridmodel.hh"

GridModel::GridModel()
{
    Model();
    initVertices();
    setVertices(_posVertices, _colorVertices);
    setScale(2,2,2);
    translate(0, -7, 0);
}

GridModel::~GridModel()
{
    deleteModel();
}

void GridModel::drawGrid()
{
    draw_line();
}

void GridModel::initVertices()
{
    float num = 6.0;
    for (float i = -num; i <= num; i += 0.25) {
      _posVertices << QVector3D(i, 0, num) << QVector3D(i, 0, -num)
                    << QVector3D(num, 0, i) <<  QVector3D(-num, 0, i);
    }

    for(uint i=0; i<_posVertices.size(); ++i) _colorVertices << QVector3D(1,1,1);
}
