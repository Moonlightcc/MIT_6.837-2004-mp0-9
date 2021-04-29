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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 20;
    char *depth_file = NULL;

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i],"-input")) {
        i++; assert (i < argc); 
        input_file = argv[i];
    } else if (!strcmp(argv[i],"-size")) {
        i++; assert (i < argc); 
        width = atoi(argv[i]);
        i++; assert (i < argc); 
        height = atoi(argv[i]);
    } else if (!strcmp(argv[i],"-output")) {
        i++; assert (i < argc); 
        output_file = argv[i];
    } else if (!strcmp(argv[i],"-depth")) {
        i++; assert (i < argc); 
        depth_min = atof(argv[i]);
        i++; assert (i < argc); 
        depth_max = atof(argv[i]);
        i++; assert (i < argc); 
        depth_file = argv[i];
    } else {
        printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
        assert(0);
    }
    }

    SceneParser SP = SceneParser(input_file);
    int size = ((OrthographicCamera*)SP.getCamera())->get_image_size();
    Image image_ins = Image(width,height);
    image_ins.SetAllPixels(SP.getBackgroundColor());
    Image depth_ins = Image(width,height);
    depth_ins.SetAllPixels(Vec3f(0,0,0));
    float tmin = SP.getCamera()->getTMin();
    float max_depth_value = 0;
    float min_depth_value = 1000;
    for (int i=0;i<width;i++)
    {
        for (int j=0;j<height;j++)
        {
            float y =  (float)1/width/2+(float)j/width;
            float x =  (float)1/height/2+(float)i/height;
            Vec2f test = Vec2f(0.36,0.36);
            Ray ray = SP.getCamera()->generateRay(Vec2f(x,y));
            Hit hit = Hit(10000,NULL);
            if(SP.getGroup()->intersect(ray,hit,tmin))
            {
                image_ins.SetPixel(i,j,hit.getMaterial()->getDiffuseColor());
                float t = hit.getT();
                depth_ins.SetPixel(i,j,t*Vec3f(1,1,1));
                max_depth_value = (max_depth_value > t) ? max_depth_value : t;
                min_depth_value = (min_depth_value < t) ? min_depth_value : t;
            }
        }
    }

    for (int i=0; i<width;i++)
        for(int j=0;j<height;j++)
        {
            Vec3f current_t = depth_ins.GetPixel(i,j);
            if(current_t!=Vec3f(0,0,0))
            {
                depth_ins.SetPixel(i,j,(-0.6/(max_depth_value-min_depth_value)*(current_t[0]-min_depth_value)+0.8)*Vec3f(1,1,1));
            }
        }
    image_ins.SaveTGA(output_file);
    depth_ins.SaveTGA(depth_file);
    return 0;
}