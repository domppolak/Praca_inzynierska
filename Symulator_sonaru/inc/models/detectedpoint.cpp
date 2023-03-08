#include "detectedpoint.hh"
#include <QPixmap>

DetectedPoint::DetectedPoint()
{
    initializeOpenGLFunctions();
    tMatrix.setToIdentity();
    rMatrix.setToIdentity();
    _position = QVector3D(0,0,0);
    sMatrix.scale(0.7, 0.7, 0.7);

    _posVertices   << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
                   << QVector3D( 0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5)
                   << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) // Back
                   << QVector3D(-0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) // Left
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) // Right
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5)
                   << QVector3D(-0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5, -0.5) // Top
                   << QVector3D( 0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5,  0.5)
                   << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5,  0.5) // Bottom
                   << QVector3D( 0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5, -0.5);

    _textureCoordinates << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Front
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                       << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Back
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                       << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Left
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                       << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Right
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                       << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Top
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
                       << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) // Bottom
                       << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0);

    texture1 = new QOpenGLTexture((QImage(":/img/img/texture4.png").mirrored()));
}

DetectedPoint::~DetectedPoint()
{

    if(_VAO.isCreated())
       _VAO.destroy();

    if(_VBO_pos.isCreated()){
        _VBO_pos.destroy();
        _posVertices.clear();
    }
    if(_VBO_texture.isCreated()){
        _VBO_texture.destroy();
        _textureCoordinates.clear();
    }
    delete texture1;
}

void DetectedPoint::setPosition(QVector3D newPosition)
{
    QVector3D translate = newPosition - _position;
    _position = newPosition;
    tMatrix.translate(translate);
}

QMatrix4x4 DetectedPoint::get_mMatrix() const
{
    return tMatrix*rMatrix*sMatrix;
}

void DetectedPoint::createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_textureLocation)
{
    program.bind();
    _VAO.create();
    _VAO.bind();

    _VBO_pos.create();
    _VBO_pos.bind();
    _VBO_pos.allocate(_posVertices.constData(), _posVertices.size()*sizeof(_posVertices[0]));
    program.enableAttributeArray(_vertexLocation);
    program.setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3);

    _VBO_texture.create();
    _VBO_texture.bind();
    _VBO_texture.allocate(_textureCoordinates.constData(), _textureCoordinates.size()*sizeof(_textureCoordinates[0]));

    program.enableAttributeArray(_textureLocation);
    program.setAttributeBuffer(_textureLocation, GL_FLOAT, 0, 2);

    _VAO.release();
    program.release();
}

void DetectedPoint::draw()
{
    assert(_VAO.isCreated());

    _VAO.bind();
    texture1->bind();
    glDrawArrays(GL_TRIANGLES, 0, _posVertices.size());
    _VAO.release();
}

bool DetectedPoint::isCreated() const
{
    return _VAO.isCreated();
}

















