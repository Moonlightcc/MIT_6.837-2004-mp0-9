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
#include "raytracing_stats.h"
#include "sample.h"
#include "film.h"

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
bool fastrender = false;
int nx = 0;
int ny = 0;
int nz = 0;
int numSamples = 1;
bool randomSample = false;
bool uniformSample = false;
bool jitteredSample = false;
bool renderSample = false;
char* sample_file = NULL;
int zoomFactor = 0;

bool boxFilter = false;
bool tentFilter = false;
bool gaussianFilter = false;
float filterRadius = 0;
bool renderFilter = false;
char* filter_file = NULL;
int zoomFactorFilter = 0;

bool visualize_grid = false;
void render() {}
bool test = false;
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
    bool stat = false;
    
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
            depth_min = atoi(argv[i]);
            i++; assert(i < argc);
            depth_max = atoi(argv[i]);
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
        else if (!strcmp(argv[i], "-grid")) {
            i++; assert(i < argc);
            nx = atoi(argv[i]);
            i++; assert(i < argc);
            ny = atoi(argv[i]);
            i++; assert(i < argc);
            nz = atoi(argv[i]);
            fastrender = true;
        }
        else if (!strcmp(argv[i], "-visualize_grid")) {
            visualize_grid = true;
        }
        else if (!strcmp(argv[i], "-stats"))
        {
            stat = true;
        }
        else if (!strcmp(argv[i], "-random_samples"))
        {
            randomSample = true;
            i++; assert(i < argc);
            numSamples = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-uniform_samples"))
        {
            uniformSample = true;
            i++; assert(i < argc);
            numSamples = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-jittered_samples"))
        {
            jitteredSample = true;
            i++; assert(i < argc);
            numSamples = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-render_samples"))
        {
            renderSample = true;
            i++; assert(i < argc);
            sample_file = argv[i];
            i++; assert(i < argc);
            zoomFactor = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-box_filter"))
        {
            boxFilter = true;
            i++; assert(i < argc);
            filterRadius = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-tent_filter"))
        {
            tentFilter = true;
            i++; assert(i < argc);
            filterRadius = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-gaussian_filter"))
        {
            gaussianFilter = true;
            i++; assert(i < argc);
            filterRadius = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-render_filter"))
        {
            renderFilter = true;
            i++; assert(i < argc);
            filter_file = argv[i];
            i++; assert(i < argc);
            zoomFactorFilter = atoi(argv[i]);
        }
        else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
    SP = new SceneParser(input_file);
    RayTracingStats::Initialize(width, height, SP->getGroup()->getBoundingBox(), nx, ny, nz);
    Image image_ins = Image(width, height);
    image_ins.SetAllPixels(SP->getBackgroundColor());
    Image normal_ins = Image(width, height);
    normal_ins.SetAllPixels(Vec3f(0, 0, 0));
    Image depth_ins = Image(width, height);
    depth_ins.SetAllPixels(Vec3f(0, 0, 0));
    float tmin = 0.001f;
    Vec3f ambient_light = SP->getAmbientLight();
    int num_light = SP->getNumLights();
    RayTracer RT = RayTracer(SP, bounce_num, weight, shadows);
    Film* film =new Film(width, height, numSamples);

    Sampler* sampler = nullptr;
    if (randomSample)
        sampler = new RandomSampler(numSamples);
    else if (uniformSample)
        sampler = new UniformSampler(numSamples);
    else if (jitteredSample)
        sampler = new JitteredSampler(numSamples);
    else
        sampler = new UniformSampler(1);

    Filter* filter = nullptr;
    if (boxFilter)
        filter = new BoxFilter(filterRadius);
    else if (tentFilter)
        filter = new TentFilter(filterRadius);
    else if (gaussianFilter)
        filter = new GaussianFilter(filterRadius);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            std::cout << "Rendering: i=" << i << " ,j=" << j << std::endl;
            for (int k = 0; k < numSamples; k++)
            {
                RayTracingStats::IncrementNumNonShadowRays();
                Vec2f offset = sampler->getSamplePosition(k);
                Vec2f pos = Vec2f((float(i)+offset[0]) / width, ((float)j+offset[1]) / height);
                Ray ray = SP->getCamera()->generateRay(pos);
                Hit hit = Hit(10000, NULL, Vec3f(0, 0, 0));
                Vec3f tot_light = RT.traceRay(ray, tmin, 0, 1.0, hit);
                film->setSample(i, j, k, offset, tot_light);
            }
        }
    }
    if (output_file)
    {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                if (filter)
                    image_ins.SetPixel(i, j, filter->getColor(i, j, film));
                else
                    image_ins.SetPixel(i, j, film->getSample(i, j, 0).getColor());
            }
        }
        image_ins.SaveTGA(output_file);
    }
    if (renderSample)
        film->renderSamples(sample_file, zoomFactor);
    if (renderFilter)
        film->renderFilter(filter_file, zoomFactorFilter,filter);
    if (stat)
        RayTracingStats::PrintStatistics();
    if (use_opengl)
    {
        glutInit(&argc, argv);
        GLCanvas GC = GLCanvas();
        Grid* grid = nullptr;
        
        if (nx != 0 && ny != 0 && nz != 0)
        {
            grid = new Grid(SP->getGroup()->getBoundingBox(), nx, ny, nz);
            SP->getGroup()->insertIntoGrid(grid, nullptr);
        }
        else
            grid = nullptr;
        GC.initialize(SP, render,tracer, grid,visualize_grid);
        delete SP;
        return 0;
    }
    delete SP;
    delete film;
    return 0;
}

Vec3f abs_vec3f(const Vec3f& v)
{
    return Vec3f(abs(v[0]),abs(v[1]),abs(v[2]));
}
