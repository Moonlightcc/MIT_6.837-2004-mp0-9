#include "camera.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "vectors.h"
#include "gl/GLUT.h"
#include "matrix.h"

OrthographicCamera::OrthographicCamera(Vec3f cp,Vec3f dir, Vec3f u, float size)
{
    center_point = cp;
    direction = dir;
    direction.Normalize();
    Vec3f temp = u;
    up = u-temp*u.Dot3(direction);
    up.Normalize();
    Vec3f::Cross3(horizontal,direction,up);
    image_size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    return Ray(center_point+(point[0]-0.5)*horizontal*image_size+(point[1]-0.5)*up*image_size,direction);
}

float OrthographicCamera::getTMin() const
{
    return MY_FLT_MIN;
}

PerspectiveCamera::PerspectiveCamera(Vec3f &c, Vec3f &dir, Vec3f &u, float a)
{
    center_point = c;
    direction = dir;
    direction.Normalize();
    Vec3f temp = u;
    up = u-temp*u.Dot3(direction);
    up.Normalize();
    Vec3f::Cross3(horizontal,direction,up);
    angle = a;
    virtual_location = 10;
    image_size = 2*virtual_location*tan(angle/2);
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f dir = (direction*virtual_location+(point[0]-0.5)*horizontal*image_size+(point[1]-0.5)*up*image_size);
    dir.Normalize();
    return Ray(center_point,dir);
}

float PerspectiveCamera::getTMin() const
{
    return 0;
}

void OrthographicCamera::glInit(int w, int h) 
{ 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    if (w > h) 
        glOrtho(-image_size / 2.0, image_size / 2.0, -image_size * (float)h / (float)w / 2.0, image_size * (float)h / (float)w / 2.0, 0.5, 40.0);
    else 
        glOrtho(-image_size * (float)w / (float)h / 2.0, image_size * (float)w / (float)h / 2.0, -image_size / 2.0, image_size / 2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void) 
{ 
    gluLookAt(center_point.x(), center_point.y(), center_point.z(), 
              center_point.x() + direction.x(), center_point.y() + direction.y(),
              center_point.z() + direction.z(), up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
    center_point += direction * dist;
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize(); 
    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);
    center_point += horizontal * dx + screenUp * dy;
}

void OrthographicCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
    rotMat.Transform(center_point);
    rotMat.TransformDirection(direction);
}

void PerspectiveCamera::glInit(int w, int h) 
{ 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(angle * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void) { 
    gluLookAt(center_point.x(), center_point.y(), center_point.z(),
        center_point.x() + direction.x(), center_point.y() + direction.y(), center_point.z() + direction.z(),
        up.x(), up.y(), up.z()); 
}

void PerspectiveCamera::dollyCamera(float dist) {
    center_point += direction * dist;
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);
    center_point += horizontal * dx + screenUp * dy;
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
    rotMat.Transform(center_point);
    rotMat.TransformDirection(direction);
    direction.Normalize();
}
