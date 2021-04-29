#ifndef _IFS_H_
#define _IFS_H_

#include "vectors.h"
#include "matrix.h"
#include "image.h"
class IFS
{
private:
    int trans_num;
    Matrix* mat_array;
    float* mat_pos_array;
    float* mat_moc_array;
public:
    IFS() {}
    ~IFS();
    void input_IFS(char* str);
    void render_IFS(Image& image_ins, int num_points, int num_iteration);
};

#endif