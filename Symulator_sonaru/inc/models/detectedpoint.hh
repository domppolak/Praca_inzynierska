#ifndef DETECTEDPOINT_H
#define DETECTEDPOINT_H
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class DetectedPoint : public QOpenGLFunctions
{
    QOpenGLVertexArrayObject _VAO;
    QOpenGLBuffer _VBO_pos;
    QOpenGLBuffer _VBO_texture;

    QMatrix4x4 tMatrix;
    QMatrix4x4 rMatrix;
    QMatrix4x4 sMatrix;
    QVector<QVector3D> _posVertices;
    QVector<QVector2D> _textureCoordinates;
    QVector<std::shared_ptr<QOpenGLTexture>> texture;
    QOpenGLTexture *texture1;
    QVector3D _position;

public:
    DetectedPoint();
    ~DetectedPoint();
    void setPosition(QVector3D newPosition);
    QMatrix4x4 get_mMatrix() const;
    void createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_textureLocation);
    void draw();
    bool isCreated() const;
};

#endif // DETECTEDPOINT_H
