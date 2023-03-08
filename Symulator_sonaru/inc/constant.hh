#ifndef CONSTANT_HH
#define CONSTANT_HH

#include <iostream>
#include <QVector3D>

enum Objects {PLATFORM, POINT, STAKE, FLAT_SURFACE};
static constexpr float TURN_SPEED = 0.3;
static constexpr uint SIMULATION_SCALE = 10;
static constexpr double dt = 0.02;
static constexpr double MAX_DISTANCE = 4000;
static constexpr QVector3D DETECT_COLOR(1,1,0);

#endif // CONSTANT_HH
