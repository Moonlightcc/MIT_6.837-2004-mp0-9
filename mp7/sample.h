#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "vectors.h"
#include <stdlib.h>

class Film;
// ==================================================================
// A helper class for super-sampling.  The position specifies the 2D
// offset within the pixel from (0,0) -> (1,1).

class Sample {

public:

    // CONSTRUCTOR & DESTRUCTOR
    Sample() {
        position = Vec2f(0.5, 0.5);
        color = Vec3f(0, 0, 0);
    }
    ~Sample() {}

    // ACCESSORS
    Vec2f getPosition() { return position; }
    Vec3f getColor() { return color; }

    // MODIFIERS
    void set(Vec2f p, Vec3f c) {
        assert(p.x() >= 0 && p.x() <= 1);
        assert(p.y() >= 0 && p.y() <= 1);
        position = p;
        color = c;
    }

private:

    // REPRESENTATION
    Vec2f position;
    Vec3f color;

};

class Sampler {
protected:
    int numSamples;
public:
    Sampler(int n) { numSamples = n; }
    virtual Vec2f getSamplePosition(int n) = 0;
};

class RandomSampler : public Sampler {
public:
    RandomSampler(int n) :Sampler(n) {}
    virtual Vec2f getSamplePosition(int n)
    {
        float x = rand() * 1.0 / RAND_MAX;
        float y = rand() * 1.0 / RAND_MAX;
        return Vec2f(x, y);
    }
};

class UniformSampler : public Sampler {
public:
    UniformSampler(int n) : Sampler(n) 
    {
        sampleTable = new Vec2f[n];
        int numAxis = (int)sqrt(n);
        assert(n == numAxis * numAxis);
        float stepSize = 0.9999f / numAxis;
        for (int i = 0; i < n; i++)
            sampleTable[i] =Vec2f((i % numAxis+0.5) * stepSize, (i / numAxis + 0.5) * stepSize);
    }
    ~UniformSampler() { delete[] sampleTable; }
    virtual Vec2f getSamplePosition(int n)
    {
        return sampleTable[n];
    }
private:
    Vec2f* sampleTable;
};

class JitteredSampler : public Sampler {
public:
    JitteredSampler(int n) : Sampler(n) {}
    virtual Vec2f getSamplePosition(int n)
    {
        int numAxis = (int)sqrt(numSamples);
        assert(numSamples == numAxis * numAxis);
        float stepSize = 0.9999f /numAxis;
        Vec2f pos = Vec2f((n % numAxis) * stepSize, (n / numAxis) * stepSize);
        float x = rand() * stepSize / RAND_MAX;
        float y = rand() * stepSize / RAND_MAX;
        Vec2f res = pos + Vec2f(x, y);
        return res;
    }
};

class Filter {
public:
    Filter() {}
    Vec3f getColor(int i, int j, Film* film);
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {
private:
    float radius;
public:
    BoxFilter(float r) { radius = r; }
    virtual float getWeight(float x, float y)
    {
        if (fabs(x) < radius && fabs(y) < radius)
            return 1;
        return 0;
    }
    virtual int getSupportRadius() { return radius+0.4999; }
};

class TentFilter : public Filter {
private:
    float radius;
public:
    TentFilter(float r) { radius = r; }
    virtual float getWeight(float x, float y)
    {
        float dis = sqrt(x * x + y * y);
        if (dis < radius)
            return 1 - dis / radius;
        return 0;
    }
    virtual int getSupportRadius() { return radius + 0.4999; }
};

class GaussianFilter : public Filter {
private:
    float sigma;
public:
    GaussianFilter(float s) { sigma = s; }
    virtual float getWeight(float x, float y)
    {
        float dis_2 = x * x + y * y;
        float dis = sqrt(dis_2);
        if (dis >= 2 * sigma)
            return 0;
        return exp(-dis_2 / 2.0 / sigma / sigma);
    }
    virtual int getSupportRadius(){ return 2 * sigma; }
};
#endif
