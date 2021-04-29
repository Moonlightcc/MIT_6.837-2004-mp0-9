#include "sample.h"
#include "film.h"
Vec3f Filter::getColor(int i, int j, Film* film) 
{
	Vec3f color = Vec3f(0, 0, 0);
	float tot_weight = 0;
	int radius = getSupportRadius();
	int width = film->getWidth();
	int height = film->getHeight();
	for(int m = i-radius;m<=i+radius;m++)
		for (int n = j - radius; n <= j + radius; n++)
		{
			if (m < 0 || m >= width || n < 0 || n >= height)
				continue;
			for (int l = 0; l < film->getNumSamples(); l++)
			{
				Sample sample = film->getSample(m, n, l);
				Vec2f pos = sample.getPosition();
				float x = m + pos[0] - (i + 0.5);
				float y = n + pos[1] - (j + 0.5);
				float weight = getWeight(x, y);
				color += sample.getColor() * weight;
				tot_weight += weight;
			}
		}
	return color *(1.0 / tot_weight);
}