#include "model.hh"

Model::Model()
{
    initializeOpenGLFunctions();
    tMatrix.setToIdentity();
    rMatrix.setToIdentity();
    _anglRot= 0;
    _translate = QVector3D(0,0,0);
    _scale = QVector3D(1,1,1);
    _modelId = 0;
    _position = QVector3D(0,0,0);
}

Model::~Model()
{
    deleteModel();
}

void Model::setVertices(QVector<QVector3D> posVertices, QVector<QVector3D> colorVertices)
{
    setPositionVertices(posVertices);
    setColorVertices(colorVertices);
}

void Model::createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation)
{
    program.bind();
    _VAO.create();
    _VAO.bind();

    _VBO_pos.create();
    _VBO_pos.bind();
    _VBO_pos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_pos.allocate(_posVertices.constData(), _posVertices.size()*sizeof(_posVertices[0]));

    if(!_indices.empty()){
        _IBO.create();
        _IBO.bind();
        _IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        _IBO.allocate(_indices.constData(), _indices.size()*sizeof(_indices[0]));
    }

    program.enableAttributeArray(_vertexLocation);
    program.setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3);
    _VBO_pos.release();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VAO.release();
    program.release();
}

void Model::createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation, QOpenGLBuffer::UsagePattern &pattern)
{
    program.bind();
    _VAO.create();
    _VAO.bind();

    program.release();

    _VBO_pos.create();
    _VBO_pos.bind();
    _VBO_pos.setUsagePattern(pattern);
    _VBO_pos.allocate(_posVertices.constData(), _posVertices.size()*sizeof(_posVertices[0]));

    if(!_indices.empty()){
        _IBO.create();
        _IBO.bind();
        _IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        _IBO.allocate(_indices.constData(), _indices.size()*sizeof(_indices[0]));
        _IBO.release();
    }
    program.enableAttributeArray(_vertexLocation);
    program.setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3, 0);
    _VBO_pos.release();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(pattern);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3, 0);
    _VBO_color.release();

    _VAO.release();
    program.release();
}

void Model::draw()
{
    assert(_VAO.isCreated());

    _VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, _posVertices.size());
    _VAO.release();
}

void Model::draw_line()
{
    assert(_VAO.isCreated());

    _VAO.bind();
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(_lineWidth);
    glDrawArrays(GL_LINES, 0, _posVertices.size());
    glDisable(GL_LINE_SMOOTH);
    _VAO.release();
}

void Model::draw_point()
{
    assert(_VAO.isCreated());

    _VAO.bind();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(_pointSize);
    glDrawArrays(GL_POINTS, 0, _posVertices.size());
    glDisable(GL_POINT_SMOOTH);
    _VAO.release();
}

void Model::draw_polygon()
{
    assert(_VAO.isCreated());

    _VAO.bind();
    glEnable(GL_POLYGON_SMOOTH);
    glDrawArrays(GL_POLYGON, 0, 8);
    glDrawArrays(GL_POLYGON, 8, 8);
    glDisable(GL_POLYGON_SMOOTH);
    _VAO.release();

    _VAO_side.bind();
    glEnable(GL_QUADS);
    for(int i=0; i<8; i++)
        glDrawArrays(GL_QUADS, i*4, 4);
    glDisable(GL_QUADS);
    _VAO_side.release();
}

void Model::deleteModel()
{
    if(_VAO.isCreated())
       _VAO.destroy();

    if(_VBO_color.isCreated()){
        _VBO_pos.destroy();
        _posVertices.clear();
    }
    if(_VBO_color.isCreated()){
        _VBO_color.destroy();
        _colorVertices.clear();
    }
    if(_IBO.isCreated()){
        _IBO.destroy();
        _indices.clear();
    }
}

void Model::setPosition(QVector3D newPosition)
{
    translate(newPosition - _position);
    _position = newPosition;
}

void Model::setScale(const QVector3D &newScale)
{
    _scale = newScale;
    sMatrix.setToIdentity();
    sMatrix.scale(_scale);
}

void Model::scale(QVector3D scale)
{
    sMatrix.scale(scale);
}

void Model::scale(double scale)
{
    sMatrix.scale(scale);
}

void Model::translate(QVector3D translate)
{
    tMatrix.translate(translate);
    _position += translate;
}

void Model::setRotate(double angle)
{
    rMatrix.setToIdentity();
    rMatrix.rotate(angle, 0,1,0);
    _anglRot = angle;
}

void Model::rotate(double angle)
{
    _anglRot += angle;
    if(_anglRot >= 360) _anglRot = 0;
    if(_anglRot <= -360) _anglRot = 0;
    rMatrix.rotate(angle, 0,1,0);
}

void Model::rotate(double angle, QVector3D xyz)
{
    _anglRot += angle;
    if(_anglRot >= 360) _anglRot = 0;
    if(_anglRot <= -360) _anglRot = 0;
    rMatrix.rotate(angle, xyz);
}

void Model::changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation, QVector<QVector3D> colorVertices)
{
    setColorVertices(colorVertices);

    program.bind();
    _VAO.bind();
    _VBO_color.destroy();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VBO_color.release();
    _VAO.release();
    program.release();
}

void Model::changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation)
{
    program.bind();
    _VAO.bind();
    _VBO_color.destroy();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VBO_color.release();
    _VAO.release();
    program.release();
}
