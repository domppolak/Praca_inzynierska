#include "camera.hh"


Camera::~Camera()
{

}

Camera::Camera(QVector3D startPosition, QVector3D startUp, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = QVector3D(0.0f, 1.0f, 0.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    rotate(yaw, pitch);
    updateView();
}

void Camera::mouseControl(float xChange, float yChange)
{
    pitch += turnSpeed*xChange;
    yaw += turnSpeed*yChange;

    if(pitch > 89.0f)
        pitch = -89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    rotate(pitch, yaw);
    updateView();

    /*if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -180) {
            beta = -180;
        }
        if (beta > 180) {
            beta = 180;
        }

        update();
    }*/
}

void Camera::rotate(float xangel, float yangel)
{
    m_vMatrix.setToIdentity();
    m_vMatrix.rotate(xangel, QVector3D(1,0,0));
    m_vMatrix.rotate(yangel, QVector3D(0,1,0));
    updateView();
}

QVector3D Camera::getCameraPosition()
{
    return position;
}

QVector3D Camera::getCameraDirection()
{
    return front.normalized();
}

QMatrix4x4 Camera::calculateViewMatrix()
{
    return m_vMatrix;
}

void Camera::updateView()
{
    //m_vMatrix.setToIdentity();
    //m_vMatrix.rotate(m_rotation.conjugated());
    position = m_vMatrix.map(QVector3D(0,0,20)); //position
    up = m_vMatrix.map(QVector3D(0,1,0)); // up direction

    m_vMatrix.lookAt(position, QVector3D(0,0,0), up);
}

/*void Camera::updateView()
{
    front.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.normalize();
    qDebug() << "Front" << front;
    right = right.crossProduct(front, worldUp).normalized();
    up = right.crossProduct(right, front).normalized();
    qDebug() << "right" << right;
    qDebug() << "Up" << up;


}*/
