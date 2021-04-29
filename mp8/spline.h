#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "arg_parser.h"
#include "matrix.h"
#include "triangle_mesh.h"

class Spline {
protected:
	const static Matrix Bezier_Matrix;
	const static Matrix BSpline_Matrix;
public:
	Spline() {}

	virtual int getNumVertices() = 0;
	virtual Vec3f getVertex(int i) = 0;
	virtual void set(int index, Vec3f& point) = 0;
	virtual void Paint(ArgParser* args) {}
	
	virtual void OutputBezier(FILE* file) {}
	virtual void OutputBSpline(FILE* file) {}
	virtual void moveControlPoint(int selectedPoint, float x, float y) {}
	virtual void addControlPoint(int selectedPoint, float x, float y) {}
	virtual void deleteControlPoint(int selectedPoint) {}
	virtual TriangleMesh* OutputTriangles(ArgParser* args) = 0;
};

class Curve : public Spline {
protected:
	int numVertices;
	Vec3f* vertices;
public:
	Curve(){ numVertices = 0; vertices = nullptr; }
	Curve(int n) { { numVertices = n; vertices = new Vec3f[n]; } }
	~Curve() { if (vertices) delete[] vertices; }

	virtual int getNumVertices() override { return numVertices; }
	virtual Vec3f getVertex(int i) override { return vertices[i]; }
	virtual void set(int index, Vec3f& point) override { assert(index >= 0 && index < numVertices); vertices[index] = point; }

	virtual void Paint(ArgParser* args) override;
	virtual void DrawPoints();
	virtual void DrawLines();
	virtual void DrawCurves(ArgParser* args) = 0;
};

class BezierCurve :public Curve {
public:
	BezierCurve(int i) :Curve(i) {}
	virtual void DrawCurves(ArgParser* args) override;
	virtual void OutputBezier(FILE* file) override;
	virtual void OutputBSpline(FILE* file) override;

	virtual void moveControlPoint(int selectedPoint, float x, float y) override;
	virtual void addControlPoint(int selectedPoint, float x, float y) override;
	virtual void deleteControlPoint(int selectedPoint) override;

	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;
};

class BSplineCurve : public Curve {
public:
	BSplineCurve(int i) :Curve(i) {}
	virtual void DrawCurves(ArgParser* args) override;
	virtual void OutputBezier(FILE* file) override;
	virtual void OutputBSpline(FILE* file) override;

	virtual void moveControlPoint(int selectedPoint, float x, float y) override;
	virtual void addControlPoint(int selectedPoint, float x, float y) override;
	virtual void deleteControlPoint(int selectedPoint) override;

	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;
};

class Surface : public Spline {
public:
	Surface() {};
};

class SurfaceOfRevolution : public Surface {
protected:
	Curve* curve;
public:
	SurfaceOfRevolution(Curve* c) { curve = c; }
	virtual void Paint(ArgParser* args) override { curve->Paint(args); }
	virtual TriangleMesh* OutputTriangles(ArgParser* args) override { return curve->OutputTriangles(args); }
	virtual void OutputBezier(FILE* file) override;
	virtual void OutputBSpline(FILE* file) override;
	void set(int index, Vec3f& point) { curve->set(index, point); }
	int getNumVertices() override { return curve->getNumVertices(); }
	Vec3f getVertex(int i) override { return curve->getVertex(i); }

	virtual void moveControlPoint(int selectedPoint, float x, float y) override { curve->moveControlPoint(selectedPoint, x, y); }
	virtual void addControlPoint(int selectedPoint, float x, float y) override { curve->addControlPoint(selectedPoint, x, y); }
	virtual void deleteControlPoint(int selectedPoint) override { curve->deleteControlPoint(selectedPoint); }
};

class BezierPatch : public Surface {
private:
	int numVertices;
	Vec3f* vertices;
public:
	BezierPatch() { numVertices = 16; vertices = new Vec3f[16]; }
	~BezierPatch() { delete[] vertices; }

	void set(int index, Vec3f& point) { vertices[index] = point; }
	int getNumVertices() override { return numVertices; }
	Vec3f getVertex(int i) override { return vertices[i]; }

	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;
	virtual void Paint(ArgParser* args) override;
};
#endif // !_SPLINE_H_

