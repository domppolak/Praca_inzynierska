#ifndef GRIDMODEL_HH
#define GRIDMODEL_HH

#include "model.hh"

class GridModel : public Model
{

public:
    GridModel();
    ~GridModel();
    void drawGrid();

private:
    void initVertices();

    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;
};
#endif // GRIDMODEL_HH
