#ifndef _MARCHINGINFO_H_
#define _MARCHINGINFO_H_

#include "vectors.h"

class MarchingInfo
{
private:
	float tcur;
	int curindex[3];
	float tnext[3];
	float dt[3];
	int sign[3];
	Vec3f normal;
public:
	MarchingInfo() { tcur = tnext[0] = tnext[1] = tnext[2] = INFINITY; }
	float gettcur() { return tcur; }
	void settcur(float t) { tcur = t; }
	float gettnext(int index) { assert(index >= 0 && index <= 2); return tnext[index]; }
	void settnext(int index, float val) { assert(index >= 0 && index <= 2); tnext[index] = val; }
	float getdt(int index) { assert(index >= 0 && index <= 2); return dt[index]; }
	void setdt(int index, float val) { assert(index >= 0 && index <= 2); dt[index] = val; }
	int getsign(int index) { assert(index >= 0 && index <= 2); return sign[index]; }
	void setsign(int index, int val) { assert(index >= 0 && index <= 2); sign[index] = val; }
	Vec3f getnormal() { return normal; }
	void setnormal(Vec3f t) { normal = t; }
	int getindex(int index) { assert(index >= 0 && index <= 2); return curindex[index]; }
	void setindex(int index, int val) { assert(index >= 0 && index <= 2); curindex[index] = val; }

	void nextCell();
	void showInfo()
	{
		std::cout << curindex[0] << ',' << curindex[1] << ',' << curindex[2] << std::endl;
	}
};


#endif 
