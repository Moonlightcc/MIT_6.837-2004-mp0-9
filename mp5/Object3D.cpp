#include "Object3D.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "vectors.h"
#include "transform.h"
#include "matrix.h"
#include "gl/GLUT.h"
#include "grid.h"

#define M_PI 3.14159265358979323846

extern bool test_bool;
extern int tess_theta;
extern int tess_phi;
extern bool gour;

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f R_origin = r.getOrigin() - center;
    Vec3f R_dir = r.getDirection();
    /* at^2 +bt+c = 0*/
    float a = R_dir.Dot3(R_dir);
    float b = 2 * R_dir.Dot3(R_origin);
    float c = R_origin.Dot3(R_origin) - radius * radius;

    float d = b * b - 4 * a * c;
    float t = 10000;
    if (d >= 0) {
        float d_c = sqrt(d);
        float t1 = (-b - d_c) / (2 * a);
        float t2 = (-b + d_c) / (2 * a);
        if (t1 >= tmin) {
            t = t1;
        }
        else if (t2 >= tmin) {
            t = t2;
        }
        if (t < h.getT()) {
            Vec3f normal = R_origin + t * R_dir;
            normal.Normalize();
            h.set(t, get_material(), normal, r,t1<0);
            return true;
        }
    }
    return false;
}

/* return True if at least one object intersects */
bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag = false;
    for (int i = 0; i < size; i++)
        flag = (ptr_object[i])->intersect(r, h, tmin) || flag;
    return flag;
}

void Group::addObject(int index, Object3D *obj)
{
    ptr_object[index] = obj;
    if(obj->getBoundingBox())
        getBoundingBox()->Extend(obj->getBoundingBox());
    size++;
}


Plane::Plane(Vec3f &n, float d, Material *m):Object3D(m)
{
    normal = n;
    normal.Normalize();
    point = d*normal;
    BBox = (BoundingBox*)NULL;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    float D_N = normal.Dot3(r.getDirection());
    if(!D_N)
        return false;
    float new_t = (normal.Dot3(point)-normal.Dot3((r.getOrigin())))/normal.Dot3(r.getDirection());
    if (new_t<h.getT() && new_t > tmin)
    {
        h.set(new_t,get_material(),normal,r,false);
        return true;
    }
    return false;
}

/* only calculate the points on the plane */
Vec3f Triangle::get_bary_coor(const Ray& r)
{   
    Vec3f rc = r.getOrigin();
    Vec3f rd = r.getDirection();
    Vec3f a = points[0];
    Vec3f b = points[1];
    Vec3f c = points[2];
    float A = det3x3(a[0]-b[0],a[0]-c[0],rd[0],a[1]-b[1],a[1]-c[1],rd[1],a[2]-b[2],a[2]-c[2],rd[2]);
    Vec3f res = Vec3f(0,0,0);
    res.Set(det3x3(a[0]-rc[0],a[0]-c[0],rd[0],a[1]-rc[1],a[1]-c[1],rd[1],a[2]-rc[2],a[2]-c[2],rd[2])/A,
            det3x3(a[0]-b[0],a[0]-rc[0],rd[0],a[1]-b[1],a[1]-rc[1],rd[1],a[2]-b[2],a[2]-rc[2],rd[2])/A,
            det3x3(a[0]-b[0],a[0]-c[0],a[0]-rc[0],a[1]-b[1],a[1]-c[1],a[1]-rc[1],a[2]-b[2],a[2]-c[2],a[2]-rc[2])/A);
    return res;
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    bool D_N = (abs(normal.Dot3(r.getDirection()))<0.01);
    if(D_N)
        return false;
    Vec3f coor = get_bary_coor(r);
    if(coor[2] < h.getT() && coor[2] > tmin && coor[0] >= 0 && coor[1] >= 0 && coor[0] + coor[1] <= 1)
    {
        h.set(coor[2],get_material(),normal,r,false);
        return true;
    }
    return false;
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f ori_cen = r.getOrigin();
    Matrix inv = ma;
    inv.Inverse();
    inv.Transform(ori_cen);
    Vec3f ori_dir = r.getDirection();
    inv.TransformDirection(ori_dir);
    Ray new_ray = Ray(ori_cen,ori_dir);
    if(ptr_obj->intersect(new_ray,h,tmin))
    {
        inv.Transpose();
        Vec3f n = h.getNormal();
        inv.TransformDirection(n);
        n.Normalize();
        h.set(h.getT(),h.getMaterial(),n,r,h.isInside());
        return true;
    }
    return false;
}

void Transform::SetBoundingBox()
{
    BoundingBox* origin_box = ptr_obj->getBoundingBox();
    Vec3f _min = origin_box->getMin();
    Vec3f _max = origin_box->getMax();

    float min_x = _min[0];
    float min_y = _min[1];
    float min_z = _min[2];

    float max_x = _max[0];
    float max_y = _max[1];
    float max_z = _max[2];

    Vec3f points[8] = {
        Vec3f(min_x,min_y,min_z),
        Vec3f(min_x,min_y,max_z),
        Vec3f(min_x,max_y,min_z),
        Vec3f(min_x,max_y,max_z),
        Vec3f(max_x,min_y,min_z),
        Vec3f(max_x,min_y,max_z),
        Vec3f(max_x,max_y,min_z),
        Vec3f(max_x,max_y,max_z)
    };

    BBox = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY),Vec3f(-INFINITY, -INFINITY, -INFINITY));
    for (int i = 0; i < 8; i++)
    {
        ma.Transform(points[i]);
        BBox->Extend(points[i]);
    }
}
void Group::paint()
{
    for (int i = 0; i < size; i++)
        ptr_object[i]->paint();
}

void Triangle::paint()
{
    get_material()->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(normal[0], normal[1], normal[2]);
    glVertex3f(points[0][0], points[0][1], points[0][2]);
    glVertex3f(points[1][0], points[1][1], points[1][2]);
    glVertex3f(points[2][0], points[2][1], points[2][2]);
    glEnd();
}

void Plane::paint()
{
    float big = 50000;
    Vec3f v, b1, b2;
    if ((normal - Vec3f(1, 0, 0)).Length() > 0.01)
        v = Vec3f(1, 0, 0);
    else
        v = Vec3f(0, 1, 0);
    Vec3f::Cross3(b1, v, normal);
    Vec3f::Cross3(b2, normal, b1);
    get_material()->glSetMaterial();
    glBegin(GL_QUADS);
    Vec3f vertex[4] = { point - b1 * big - b2 * big,point + b1 * big - b2 * big,
                        point + b1 * big + b2 * big,point - b1 * big + b2 * big };
    glNormal3f(normal[0], normal[1], normal[2]);
    glVertex3f(vertex[0][0], vertex[0][1], vertex[0][2]);
    glVertex3f(vertex[1][0], vertex[1][1], vertex[1][2]);
    glVertex3f(vertex[2][0], vertex[2][1], vertex[2][2]);
    glVertex3f(vertex[3][0], vertex[3][1], vertex[3][2]);
    glEnd();
}

void Sphere::paint()
{
    get_material()->glSetMaterial();
    glBegin(GL_QUADS);
    Vec3f* cur_points;
    Vec3f* next_points;
    float dphi = M_PI / tess_phi;
    float dtheta = 2 * M_PI / tess_theta;
    cur_points = new Vec3f[tess_theta];
    for (int i = 0; i < tess_theta; i++)
        cur_points[i] = Vec3f(center[0], center[1]+radius, center[2]);
    for (int phi = 0; phi < tess_phi; phi++)
    {
        next_points = new Vec3f[tess_theta];
        for (int i = 0; i < tess_theta; i++)
            next_points[i] = Vec3f(center[0] + radius * sin(dphi * (phi + 1)) * cos(dtheta * i),
                                   center[1] + radius * cos(dphi * (phi + 1)),
                                   center[2] + radius * sin(dphi * (phi + 1)) * sin(dtheta * i));
        for (int theta = 0; theta < tess_theta; theta++)
        {
            if (!gour)
            {
                Vec3f nor;
                Vec3f::Cross3(nor, cur_points[theta] - next_points[theta], next_points[(theta + 1) % tess_theta] - next_points[theta]);
                nor.Normalize();
                glNormal3f(-nor[0], -nor[1], -nor[2]);
                glVertex3f(cur_points[theta][0], cur_points[theta][1], cur_points[theta][2]);
                glVertex3f(next_points[theta][0], next_points[theta][1], next_points[theta][2]);
                glVertex3f(next_points[(theta + 1) % tess_theta][0], next_points[(theta + 1) % tess_theta][1], next_points[(theta + 1) % tess_theta][2]);
                glVertex3f(cur_points[(theta + 1) % tess_theta][0], cur_points[(theta + 1) % tess_theta][1], cur_points[(theta + 1) % tess_theta][2]);
            }
            else
            {
                glNormal3f(-(cur_points[theta][0] - center[0]) / radius, -(cur_points[theta][1] - center[1]) / radius, -(cur_points[theta][2] - center[2]) / radius);
                glVertex3f(cur_points[theta][0], cur_points[theta][1], cur_points[theta][2]);
                glNormal3f(-(next_points[theta][0] - center[0]) / radius, -(next_points[theta][1] - center[1]) / radius, -(next_points[theta][2] - center[2]) / radius);
                glVertex3f(next_points[theta][0], next_points[theta][1], next_points[theta][2]);
                glNormal3f(-(next_points[(theta + 1) % tess_theta][0] - center[0]) / radius, -(next_points[(theta + 1) % tess_theta][1] - center[1]) / radius, -(next_points[(theta + 1) % tess_theta][2] - center[2]) / radius);
                glVertex3f(next_points[(theta + 1) % tess_theta][0], next_points[(theta + 1) % tess_theta][1], next_points[(theta + 1) % tess_theta][2]);
                glNormal3f(-(cur_points[(theta + 1) % tess_theta][0] - center[0]) / radius, -(cur_points[(theta + 1) % tess_theta][1] - center[1]) / radius, -(cur_points[(theta + 1) % tess_theta][2] - center[2]) / radius);
                glVertex3f(cur_points[(theta + 1) % tess_theta][0], cur_points[(theta + 1) % tess_theta][1], cur_points[(theta + 1) % tess_theta][2]);
            }
        }
        delete[] cur_points;
        cur_points = next_points;
        if (phi == tess_phi - 1)
            delete[] next_points;
    }
    glEnd();
}

void Transform::paint()
{
    glPushMatrix();
    GLfloat* glMatrix = ma.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    ptr_obj->paint();
    glPopMatrix();
}

void Sphere::insertIntoGrid(Grid* g, Matrix* m)
{
    if (m)
    {
        this->Object3D::insertIntoGrid(g, m);
        return;
    }

    BoundingBox* box = g->getBoundingBox();
    Vec3f _min = box->getMin();
    Vec3f _max = box->getMax();
    int x = g->getx();
    int y = g->gety();
    int z = g->getz();
    float dx = (_max[0] - _min[0]) / float(x);
    float dy = (_max[1] - _min[1]) / float(y);
    float dz = (_max[2] - _min[2]) / float(z);
    float diag = sqrt(dx * dx + dy * dy + dz * dz) / 2;
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++)
            for (int k=0;k<z;k++)
            {
                Vec3f c = _min+ Vec3f((i + 0.5) * dx, (j + 0.5) * dy, (k + 0.5) * dz);
                if ((c - center).Length() <= radius+ diag)
                    g->set(i, j, k, this);
            }
}

void Group::insertIntoGrid(Grid* g, Matrix* m)
{
    for (int i = 0; i < size; i++)
        ptr_object[i]->insertIntoGrid(g, m);
}

void Transform::insertIntoGrid(Grid* g, Matrix* m)
{
    if (m) {
        Matrix t = (*m) * ma;
        ptr_obj->insertIntoGrid(g, &t);
    }
    else {
        ptr_obj->insertIntoGrid(g, &ma);
    }
}
void Triangle::insertIntoGrid(Grid* g, Matrix* m)
{
    assert(g);
    BoundingBox* new_box;
    if (m)
    {
        Vec3f a = points[0];
        Vec3f b = points[1];
        Vec3f c = points[2];
        m->Transform(a);
        m->Transform(b);
        m->Transform(c);
        new_box = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
        new_box->Extend(a);
        new_box->Extend(b);
        new_box->Extend(c);
    }
    else
    {
        new_box = getBoundingBox();
    }
    BoundingBox* grid_box = g->getBoundingBox();
    Vec3f grid_min = grid_box->getMin();
    Vec3f grid_max = grid_box->getMax();

    int x = g->getx();
    int y = g->gety();
    int z = g->getz();

    float dx = (grid_max[0] - grid_min[0]) / float(x);
    float dy = (grid_max[1] - grid_min[1]) / float(y);
    float dz = (grid_max[2] - grid_min[2]) / float(z);

    Vec3f obj_min = new_box->getMin();
    Vec3f obj_max = new_box->getMax();
    int start_index_i = int((obj_min[0] - grid_min[0]) / dx);
    int start_index_j = int((obj_min[1] - grid_min[1]) / dy);
    int start_index_k = int((obj_min[2] - grid_min[2]) / dz);
    int end_index_i = int((obj_max[0] - grid_min[0]) / dx);
    int end_index_j = int((obj_max[1] - grid_min[1]) / dy);
    int end_index_k = int((obj_max[2] - grid_min[2]) / dz);

    if (end_index_i < 0 || end_index_j < 0 || end_index_k < 0 || start_index_i > x || start_index_j > y || start_index_k > z)
        return;

    start_index_i = (start_index_i > 0) ? start_index_i : 0;
    start_index_i = (start_index_i == x) ? x - 1 : start_index_i;
    start_index_j = (start_index_j > 0) ? start_index_j : 0;
    start_index_j = (start_index_j == y) ? y - 1 : start_index_j;
    start_index_k = (start_index_k > 0) ? start_index_k : 0;
    start_index_k = (start_index_k == z) ? z - 1 : start_index_k;

    end_index_i = (end_index_i < x - 1) ? end_index_i : x - 1;
    end_index_j = (end_index_j < y - 1) ? end_index_j : y - 1;
    end_index_k = (end_index_k < z - 1) ? end_index_k : z - 1;

    for (int i = start_index_i; i <= end_index_i; i++)
        for (int j = start_index_j; j <= end_index_j; j++)
            for (int k = start_index_k; k <= end_index_k; k++)
                g->set(i, j, k, this);
}

void Object3D::insertIntoGrid(Grid* g, Matrix* m)
{
    assert(g);
    BoundingBox* new_box;
    if (m)
    {
        BoundingBox* ori_box = getBoundingBox();
        Vec3f ori_min = ori_box->getMin();
        Vec3f ori_max = ori_box->getMax();
        Vec3f ori_points[8] = {
            Vec3f(ori_min[0],ori_min[1],ori_min[2]),Vec3f(ori_min[0],ori_min[1],ori_max[2]),
            Vec3f(ori_min[0],ori_max[1],ori_min[2]),Vec3f(ori_min[0],ori_max[1],ori_max[2]),
            Vec3f(ori_max[0],ori_min[1],ori_min[2]),Vec3f(ori_max[0],ori_min[1],ori_max[2]),
            Vec3f(ori_max[0],ori_max[1],ori_min[2]),Vec3f(ori_max[0],ori_max[1],ori_max[2])
        };

        for (int i = 0; i < 8; i++)
            m->Transform(ori_points[i]);

        new_box = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
        for (int i = 0; i < 8; i++)
            new_box->Extend(ori_points[i]);
    }
    else
    {
        new_box = getBoundingBox();
    }
    BoundingBox* grid_box = g->getBoundingBox();
    Vec3f grid_min = grid_box->getMin();
    Vec3f grid_max = grid_box->getMax();

    int x = g->getx();
    int y = g->gety();
    int z = g->getz();

    float dx = (grid_max[0] - grid_min[0]) / float(x);
    float dy = (grid_max[1] - grid_min[1]) / float(y);
    float dz = (grid_max[2] - grid_min[2]) / float(z);

    Vec3f obj_min = new_box->getMin();
    Vec3f obj_max = new_box->getMax();
    int start_index_i = int((obj_min[0] - grid_min[0])/dx);
    int start_index_j = int((obj_min[1] - grid_min[1])/dy);
    int start_index_k = int((obj_min[2] - grid_min[2])/dz);
    int end_index_i = int((obj_max[0] - grid_min[0]) / dx);
    int end_index_j = int((obj_max[1] - grid_min[1]) / dy);
    int end_index_k = int((obj_max[2] - grid_min[2]) / dz);

    if (end_index_i < 0 || end_index_j < 0 || end_index_k < 0 || start_index_i > x || start_index_j > y || start_index_k > z)
        return;

    start_index_i = (start_index_i > 0) ? start_index_i : 0;
    start_index_i = (start_index_i == x) ? x-1 : start_index_i;
    start_index_j = (start_index_j > 0) ? start_index_j : 0;
    start_index_j = (start_index_j == y) ? y - 1 : start_index_j;
    start_index_k = (start_index_k > 0) ? start_index_k : 0;
    start_index_k = (start_index_k == z) ? z - 1 : start_index_k;

    end_index_i = (end_index_i < x - 1) ? end_index_i : x - 1;
    end_index_j = (end_index_j < y - 1) ? end_index_j : y - 1;
    end_index_k = (end_index_k < z - 1) ? end_index_k : z - 1;

    for (int i = start_index_i; i <= end_index_i; i++)
        for (int j = start_index_j; j <= end_index_j; j++)
            for (int k = start_index_k; k <= end_index_k; k++)
                g->set(i, j, k, this);
}