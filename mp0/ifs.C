#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "ifs.h"
#include "matrix.h"
#include "vectors.h"
#include "image.h"
IFS::~IFS()
{
    delete mat_array;
    delete mat_pos_array;
    delete mat_moc_array;
}

void IFS::input_IFS(char* str)
{
    FILE *input = fopen(str,"r");
    assert(input != NULL);

    int num_transforms; 
    fscanf(input,"%d",&num_transforms);
    trans_num = num_transforms;
    mat_array = new Matrix[num_transforms];
    mat_pos_array = new float[num_transforms];
    mat_moc_array = new float[num_transforms];   
    for (int i = 0; i < num_transforms; i++) {
        float probability; 
        fscanf (input,"%f",&probability);
        Matrix m; 
        m.Read3x3(input);
        mat_array[i] = m;
        mat_pos_array[i] = probability;
        if (i==0)
            mat_moc_array[0] = probability;
        else
            mat_moc_array[i] = probability + mat_moc_array[i-1];
    }
    fclose(input);
}

void IFS::render_IFS(Image& image_ins, int num_points, int num_iteration)
{
    int i,j,k;
    int width = image_ins.Width();
    int height = image_ins.Height();
    float x,y;
    Vec2f point;
    Vec3f Black = Vec3f(0,0,0);
    Matrix trans;
    for (i=0;i<num_points;i++)
    {
        x = (float)rand()/RAND_MAX;
        y = (float)rand()/RAND_MAX;
        
        point = Vec2f(x,y);
        for (j=0;j<num_iteration;j++)
        {
            double val = (double)rand()/RAND_MAX;
            int index=0;
            for (k=0;k<trans_num;k++)
                if(val<=mat_moc_array[k])
                {
                    index = k;
                    break;
                }
            trans = mat_array[index];
            trans.Transform(point);
        }
        image_ins.SetPixel(max((float)0,floor(min(point.x(),(float)0.9999)*width)),max((float)0,floor(min(point.y(),(float)0.9999)*height)),Black);
    }
}
