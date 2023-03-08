#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


class Model : public QOpenGLFunctions
{
public:
    Model();
    ~Model();
    void setModelId(uint id);
    void setPositionVertices(QVector<QVector3D> posVertices);
    void setColorVertices(QVector<QVector3D> colorVertices);
    void setIndices(QVector<uint> const &indices);
    void setVertices(QVector<QVector3D> posVertices, QVector<QVector3D> colorVertices);
    void setPosition(QVector3D newPosition);
    void setPosition(double xCord, double yCord, double zCord);
    void setScale(QVector3D const &newScale);
    void setScale(double xScale, double yScale, double zScale);
    void scale(QVector3D scale);
    void scale(double scale);
    void scale(double xScale, double yScale, double zScale);
    void translate(QVector3D translate);
    void translate(double xCord, double yCord, double zCord);
    void setRotate(double angle);
    void rotate(double angle);
    void rotate(double angle, QVector3D xyz);
    void setLineWidth(double const &width);
    void setPointSize(double const &size);

    QVector3D getScale() const;
    QMatrix4x4 getScaleMatrix() const;
    QMatrix4x4 getRotateMatrix() const;
    QMatrix4x4 getTranslateMatrix() const;
    QMatrix4x4 get_mMatrix() const;
    QVector3D getPostion() const;
    uint getModelId() const;
    QVector<QVector3D> getPositionVertices() const;
    QVector<QVector3D> getColorVertices() const;
    QVector<uint> getIndices() const;
    double get_anglRoot() const;
    QVector3D getColor() const;
    double getLineWidth() const;
    double getPointSize() const;

    void deleteModel();
    bool isCreated() const;
    void bind();
    void release();

    void createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation);
    void createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation, QOpenGLBuffer::UsagePattern &pattern);

    void changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation, QVector<QVector3D> colorVertices);
    void changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation);

    virtual void draw();
    void draw_line();
    void draw_point();
    void draw_polygon();

private:
    QOpenGLVertexArrayObject _VAO;
    QOpenGLVertexArrayObject _VAO_side;
    QOpenGLBuffer _VBO_pos;
    QOpenGLBuffer _IBO;
    QOpenGLBuffer _VBO_color;
    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;
    QVector<uint> _indices;
    QMatrix4x4 tMatrix;
    QMatrix4x4 rMatrix;
    QMatrix4x4 sMatrix;

    double _anglRot;
    QVector3D _translate;
    QVector3D _scale;
    uint _modelId;

    double _lineWidth = 1;
    double _pointSize = 1;
    QVector3D _position;
    QVector3D _color;

};

inline void Model::setModelId(uint id) {this->_modelId=id;}
inline void Model::setPositionVertices(QVector<QVector3D> posVertices){this->_posVertices = posVertices;}
inline void Model::setColorVertices(QVector<QVector3D> colorVertices){this->_colorVertices = colorVertices;}
inline void Model::setIndices(const QVector<uint> &indices) {this->_indices=indices;}
inline void Model::setPosition(double xCord, double yCord, double zCord){setPosition(QVector3D(xCord, yCord, zCord));}
inline void Model::setScale(double xScale, double yScale, double zScale) {setScale(QVector3D(xScale, yScale, zScale));}
inline void Model::scale(double xScale, double yScale, double zScale){scale(QVector3D(xScale, yScale, zScale));}
inline void Model::translate(double xCord, double yCord, double zCord){translate(QVector3D(xCord, yCord, zCord));}
inline void Model::setLineWidth(const double &width) {this->_lineWidth = width;}
inline void Model::setPointSize(const double &size){this->_pointSize = size;}

inline QVector3D Model::getScale() const {return this->_scale;}
inline QMatrix4x4 Model::getScaleMatrix() const {return sMatrix;}
inline QMatrix4x4 Model::getRotateMatrix() const {return this->rMatrix;}
inline QMatrix4x4 Model::getTranslateMatrix() const {return this->tMatrix;}
inline QMatrix4x4 Model::get_mMatrix() const {return tMatrix*rMatrix*sMatrix;}
inline QVector3D Model::getPostion() const {return this->_position;}
inline uint Model::getModelId() const {return this->_modelId;}
inline QVector<QVector3D> Model::getPositionVertices() const{return this->_posVertices;}
inline QVector<QVector3D> Model::getColorVertices() const{return this->_colorVertices;}
inline QVector<uint> Model::getIndices() const {return this->_indices;}
inline double Model::get_anglRoot() const {return this->_anglRot;}
inline QVector3D Model::getColor() const {return this->_color;}
inline double Model::getLineWidth() const {return _lineWidth;}
inline double Model::getPointSize() const {return _pointSize;}

inline bool Model::isCreated() const {return this->_VAO.isCreated();}
inline void Model::bind() {this->_VAO.bind();}
inline void Model::release() {this->_VAO.release();}

#endif // MODEL_H
