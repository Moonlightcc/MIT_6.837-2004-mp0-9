#include "camera.h"
#include "image.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"
#include "Object3D.h"
#include "ray.h"
#include "vectors.h"
#include "scene_parser.h"
#include "group.h"
#include "light.h"
#include "triangle.h"
#include "glCanvas.h"
#include "RayTracer.h"

#include <gl/GLUT.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
Vec3f abs_vec3f(const Vec3f& v);

char* input_file = NULL;
SceneParser* SP;
int tess_theta = 0;
int tess_phi = 0;
bool gour = false;
bool shadows = false;
int bounce_num = 0;
float weight = 0;

bool test_bool = false;

void render() {}

void tracer(float x, float y)
{
    Ray ray = SP->getCamera()->generateRay(Vec2f(x, y));
    RayTracer rayTracer = RayTracer(SP, bounce_num, weight,shadows);
    float tmin = 0.001f;
    Hit hit = Hit(10000, NULL, Vec3f(0, 0, 0));
    Vec3f color = rayTracer.traceRay(ray, tmin, 0, 1.0, hit);
}

int main(int argc, char* argv[])
{
    int width = 100;
    int height = 100;
    char* output_file = NULL;
    char* normal_file = NULL;
    int depth_min = 0;
    int depth_max = 1;
    char* depth_file = NULL;
    bool shade_back = false;
    bool normal_on = false;
    bool depth_on = false;
    bool use_opengl = false;
    
    
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++; assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size")) {
            i++; assert(i < argc);
            width = atoi(argv[i]);
            i++; assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output")) {
            i++; assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals")) {
            normal_on = true;
            i++; assert(i < argc);
            normal_file = argv[i];
        }
        else if (!strcmp(argv[i], "-depth")) {
            i++; assert(i < argc);
            depth_min = atof(argv[i]);
            i++; assert(i < argc);
            depth_max = atof(argv[i]);
            i++; assert(i < argc);
            depth_file = argv[i];
            depth_on = true;
        }
        else if (!strcmp(argv[i], "-shade_back")) {
            shade_back = true;
        }
        else if (!strcmp(argv[i], "-gui")) {
            use_opengl = true;
        }
        else if (!strcmp(argv[i], "-tessellation")) {
            i++; assert(i < argc);
            tess_theta = atoi(argv[i]);
            i++; assert(i < argc);
            tess_phi = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-gouraud")) {
            gour = true;
        }
        else if (!strcmp(argv[i], "-bounces")) {
            i++; assert(i < argc);
            bounce_num = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight")) {
            i++; assert(i < argc);
            weight = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        }
        else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    SP = new SceneParser(input_file);
    Image image_ins = Image(width, height);
    image_ins.SetAllPixels(SP->getBackgroundColor());
    Image normal_ins = Image(width, height);
    normal_ins.SetAllPixels(Vec3f(0, 0, 0));
    Image depth_ins = Image(width, height);
    depth_ins.SetAllPixels(Vec3f(0, 0, 0));
    float tmin = SP->getCamera()->getTMin();
    Vec3f ambient_light = SP->getAmbientLight();
    int num_light = SP->getNumLights();
    RayTracer RT = RayTracer(SP, bounce_num, weight, shadows);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            float y = (float)1 / width / 2 + (float)j / width;
            float x = (float)1 / height / 2 + (float)i / height;
            Ray ray = SP->getCamera()->generateRay(Vec2f(x, y));
            Hit hit = Hit(10000, NULL, Vec3f(0, 0, 0));
            test_bool = (i == 0 ) && (j == height - 1);
            Vec3f tot_light = RT.traceRay(ray, tmin, 0, 1.0, hit);
            image_ins.SetPixel(i, j, tot_light);
            if (normal_on)
                normal_ins.SetPixel(i, j, abs_vec3f(hit.getNormal()));
            if (depth_on)
            {
                float t = hit.getT();
                if (t > depth_max)
                    depth_ins.SetPixel(i, j, Vec3f(0, 0, 0));
                else if (t < depth_min)
                    depth_ins.SetPixel(i, j, Vec3f(1, 1, 1));
                else
                    depth_ins.SetPixel(i, j, (t - depth_max) / (depth_min - depth_max) * Vec3f(1, 1, 1));
            }
        }
    }

    if (normal_on)
        normal_ins.SaveTGA(normal_file);
    if (depth_on)
        depth_ins.SaveTGA(depth_file);
    image_ins.SaveTGA(output_file);

    if (use_opengl)
    {
        glutInit(&argc, argv);
        GLCanvas GC = GLCanvas();
        GC.initialize(SP, render,tracer);
        delete SP;
        return 0;
    }
    delete SP;
    return 0;
}

Vec3f abs_vec3f(const Vec3f& v)
{
    return Vec3f(abs(v[0]),abs(v[1]),abs(v[2]));
}
