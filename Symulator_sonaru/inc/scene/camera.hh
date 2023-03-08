#ifndef CAMERA_HH
#define CAMERA_HH

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

class Camera
{

public:
    Camera() {}
    Camera(QVector3D startPosition,
                        QVector3D startUp,
                            float startYaw,
                                float startPitch,
                                    float startMoveSpeed,
                                        float startTurnSpeed);
     ~Camera();
    void mouseControl(float xChange, float yChange);
    void rotate(float xChange, float yChange);
    void wheelControl();

    QVector3D getCameraPosition();
    QVector3D getCameraDirection();
    QMatrix4x4 calculateViewMatrix();


private:
    void updateView();

    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;

    float yaw;
    float pitch;

    float moveSpeed;
    float turnSpeed;

    QVector3D m_translation;
    QMatrix4x4 m_vMatrix;
    QQuaternion m_rotation;

    static constexpr float MOUSE_SENSITIVITY = 0.06f;
};

/*void translate(const QVector3D &transtale);
void translate(float x, float y, float z);
void rotate(const QQuaternion &rotation);
void rotate(float angle, const QVector3D &axis);
void rotate(float angle, float ax, float ay, float az);

void setTranslate(const QVector3D &transtale);
void setTranslate(float x, float y, float z);
void setRotate(const QQuaternion &rotation);
void setRotate(float angle, const QVector3D &axis);
void setRotate(float angle, float ax, float ay, float az);*/
/*inline void Camera::translate(float x, float y, float z) { translate(QVector3D(x,y,z)); }
inline void Camera::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle));}
inline void Camera::rotate(float angle, float ax, float ay, float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

inline void Camera::setTranslate(float x, float y, float z) { setTranslate(QVector3D(x,y,z)); }
inline void Camera::setRotate(float angle, const QVector3D &axis) { setRotate(QQuaternion::fromAxisAndAngle(axis, angle));}
inline void Camera::setRotate(float angle, float ax, float ay, float az) { setRotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
*/
#endif // CAMERA_HH
