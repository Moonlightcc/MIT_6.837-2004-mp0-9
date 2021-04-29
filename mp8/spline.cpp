#include "spline.h"
#include "gl/GLUT.h"

#define M_PI 3.14159265358979323846   // pi

const float Bezier_coff[16] = { -1.0, 3.0, -3.0, 1.0, 3.0,-6.0, 3.0, 0.0,
							    -3.0, 3.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0 };
const Matrix Spline::Bezier_Matrix = Matrix(Bezier_coff);
const float BSpline_coff[16] = { -1.0/6,3.0/6,-3.0/6,1.0/6,3.0/6,-6.0/6,0.0/6,4.0/6,
								 -3.0/6,3.0/6, 3.0/6,1.0/6,1.0/6, 0.0/6,0.0/6,0.0/6 };
const Matrix Spline::BSpline_Matrix = Matrix(BSpline_coff);

void Curve::DrawPoints()
{
	glPointSize(3);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < getNumVertices(); i++)
	{
		Vec3f point = getVertex(i);
		glVertex3f(point[0], point[1], point[2]);
	}
	glEnd();
}

void Curve::DrawLines()
{
	glLineWidth(1);
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	for (int i = 0; i < getNumVertices() - 1; i++)
	{
		Vec3f point = getVertex(i);
		glVertex3f(point[0], point[1], point[2]);
		point = getVertex(i + 1);
		glVertex3f(point[0], point[1], point[2]);
	}
	glEnd();
}

void Curve::Paint(ArgParser* args)
{
	DrawPoints();
	DrawLines();
	DrawCurves(args);
}

void BezierCurve::OutputBezier(FILE* file)
{
	fprintf(file, "bezier\n");
	fprintf(file, "num_vertices %d\n", getNumVertices());
	for (int i = 0; i < getNumVertices(); i++)
	{
		Vec3f point = getVertex(i);
		fprintf(file, "%f %f %f\n", point[0], point[1], point[2]);
	}
}

void BezierCurve::OutputBSpline(FILE* file)
{
	fprintf(file, "bspline\n");
	fprintf(file, "num_vertices %d\n", int((getNumVertices()-1)/3+3));
	Matrix ma_BS = Matrix(Spline::BSpline_Matrix);
	ma_BS.Inverse();
	for (int i = 0; i <= getNumVertices()-3; i += 3)
	{
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		Matrix res = Geogetry * (Spline::Bezier_Matrix * ma_BS);
		res.ShowMatrix();
		if (i == 0)
		{
			fprintf(file, "%f %f %f\n", res.Get(0, 0), res.Get(0, 1), res.Get(0, 2));
			fprintf(file, "%f %f %f\n", res.Get(1, 0), res.Get(1, 1), res.Get(1, 2));
			fprintf(file, "%f %f %f\n", res.Get(2, 0), res.Get(2, 1), res.Get(2, 2));
		}
		fprintf(file, "%f %f %f\n", res.Get(3, 0), res.Get(3, 1), res.Get(3, 2));
	}
}

void BSplineCurve::DrawCurves(ArgParser* args)
{
	glLineWidth(2);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int tess = args->curve_tessellation;
	float step_size = 1.0 / tess;
	for (int i = 0; i < getNumVertices()-3; i++)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= tess; j++)
		{
			float t = PowerBasis[2] + step_size;
			Spline::BSpline_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			glVertex3f(PowerBasis[0], PowerBasis[1], PowerBasis[2]);
			PowerBasis.Set(t * t * t, t * t, t, 1);
		}
	}
	glEnd();
}

void BSplineCurve::OutputBezier(FILE* file)
{
	fprintf(file, "bezier\n");
	fprintf(file, "num_vertices %d\n", (getNumVertices()-3)*3+1);
	Matrix ma_BS = Matrix(Spline::Bezier_Matrix);
	ma_BS.Inverse();
	for (int i = 0; i < getNumVertices()-3; i++)
	{
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		Matrix res = Geogetry * Spline::BSpline_Matrix * ma_BS;
		if (i == 0)
			fprintf(file, "%f %f %f\n", res.Get(0, 0), res.Get(0, 1), res.Get(0, 2));
		fprintf(file, "%f %f %f\n", res.Get(1, 0), res.Get(1, 1), res.Get(1, 2));
		fprintf(file, "%f %f %f\n", res.Get(2, 0), res.Get(2, 1), res.Get(2, 2));
		fprintf(file, "%f %f %f\n", res.Get(3, 0), res.Get(3, 1), res.Get(3, 2));
	}
}

void BSplineCurve::OutputBSpline(FILE* file)
{
	fprintf(file, "bspline\n");
	fprintf(file, "num_vertices %d\n", getNumVertices());
	for (int i = 0; i < getNumVertices(); i++)
	{
		Vec3f point = getVertex(i);
		fprintf(file, "%f %f %f\n", point[0], point[1], point[2]);
	}
}

void BezierCurve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set(x, y, 0);
}

void BezierCurve::addControlPoint(int selectedPoint, float x, float y)
{
	numVertices += 1;
	Vec3f* newVertices = new Vec3f[numVertices];
	for (int i = 0; i < selectedPoint; i++)
		newVertices[i] = vertices[i];
	newVertices[selectedPoint] = Vec3f(x, y, 0);
	for (int i = selectedPoint + 1; i < numVertices; i++)
		newVertices[i] = vertices[i - 1];
	delete[] vertices;
	vertices = newVertices;
}

void BezierCurve::deleteControlPoint(int selectedPoint)
{
	numVertices -= 1;
	Vec3f* newVertices = new Vec3f[numVertices];
	for (int i = 0; i < selectedPoint; i++)
		newVertices[i] = vertices[i];
	for (int i = selectedPoint; i < numVertices; i++)
		newVertices[i] = vertices[i + 1];
	delete[] vertices;
	vertices = newVertices;
}

void BSplineCurve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set(x, y, 0);
}

void BSplineCurve::addControlPoint(int selectedPoint, float x, float y)
{
	numVertices += 1;
	Vec3f* newVertices = new Vec3f[numVertices];
	for (int i = 0; i < selectedPoint; i++)
		newVertices[i] = vertices[i];
	newVertices[selectedPoint] = Vec3f(x, y, 0);
	for (int i = selectedPoint + 1; i < numVertices; i++)
		newVertices[i] = vertices[i - 1];
	delete[] vertices;
	vertices = newVertices;
}

void BSplineCurve::deleteControlPoint(int selectedPoint)
{
	numVertices -= 1;
	Vec3f* newVertices = new Vec3f[numVertices];
	for (int i = 0; i < selectedPoint; i++)
		newVertices[i] = vertices[i];
	for (int i = selectedPoint; i < numVertices; i++)
		newVertices[i] = vertices[i + 1];
	delete[] vertices;
	vertices = newVertices;
}

void BezierCurve::DrawCurves(ArgParser* args)
{
	glLineWidth(2);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int tess = args->curve_tessellation;
	float step_size = 1.0 / tess;
	for (int i = 0; i <= getNumVertices() - 3; i += 3)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= tess; j++)
		{
			float t = PowerBasis[2] + step_size;
			Spline::Bezier_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			glVertex3f(PowerBasis[0], PowerBasis[1], PowerBasis[2]);
			PowerBasis.Set(t * t * t, t * t, t, 1);
		}
	}
	glEnd();
}

TriangleMesh* BezierCurve::OutputTriangles(ArgParser* args)
{
	int curve_tess = args->curve_tessellation;
	float curve_step_size = 1.0 / curve_tess;
	int revo_tess = args->revolution_tessellation;
	float revo_step_size = 2.0 * M_PI / revo_tess;

	TriangleNet* TN = new TriangleNet(revo_tess, (getNumVertices()-1) / 3 * curve_tess);
	for (int i = 0; i <= getNumVertices()-3; i += 3)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= curve_tess; j++)
		{
			float t = PowerBasis[2] + curve_step_size;
			Spline::Bezier_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			float theta = 0;
			for (int k = 0; k <= revo_tess; k++)
			{
				float x = cos(theta) * PowerBasis[0] + sin(theta) * PowerBasis[2];
				float z = -sin(theta) * PowerBasis[0] + cos(theta) * PowerBasis[2];
				TN->SetVertex(k, j + (i / 3 * curve_tess), Vec3f(x, PowerBasis[1], z));
				theta += revo_step_size;
			}
			PowerBasis.Set(t * t * t, t * t, t, 1);

		}
	}
	return TN;
}

TriangleMesh* BSplineCurve::OutputTriangles(ArgParser* args)
{
	int curve_tess = args->curve_tessellation;
	float curve_step_size = 1.0 / curve_tess;
	int revo_tess = args->revolution_tessellation;
	float revo_step_size = 2.0 * M_PI / revo_tess;

	TriangleNet* TN = new TriangleNet(revo_tess, (getNumVertices() - 3) * curve_tess);

	for (int i = 0; i < getNumVertices()-3; i++)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= curve_tess; j++)
		{
			float t = PowerBasis[2] + curve_step_size;
			Spline::BSpline_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			float theta = 0;
			for (int k = 0; k <= revo_tess; k++)
			{
				float x = cos(theta) * PowerBasis[0] + sin(theta) * PowerBasis[2];
				float z = -sin(theta) * PowerBasis[0] + cos(theta) * PowerBasis[2];
				TN->SetVertex(k, j + (i * curve_tess), Vec3f(x, PowerBasis[1], z));
				theta += revo_step_size;
			}
			PowerBasis.Set(t * t * t, t * t, t, 1);
		}
	}
	return TN;
}
void SurfaceOfRevolution::OutputBezier(FILE* file) { curve->OutputBezier(file); }
void SurfaceOfRevolution::OutputBSpline(FILE* file) { curve->OutputBSpline(file); }

TriangleMesh* BezierPatch::OutputTriangles(ArgParser* args)
{
	int patch_tess = args->patch_tessellation;
	float step_size = 1.0 / patch_tess;

	TriangleNet* TN = new TriangleNet(patch_tess, patch_tess);

	Matrix Geogetries[4];
	for (int i = 0; i < 4; i++)
	{
		Vec3f p1 = getVertex(i * 4);
		Vec3f p2 = getVertex(i * 4 + 1);
		Vec3f p3 = getVertex(i * 4 + 2);
		Vec3f p4 = getVertex(i * 4 + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Geogetries[i] = Matrix(coffi);
	}
	for (int i = 0; i <= patch_tess; i++)
	{
		float t = i * step_size;
		Vec4f points[4];
		for (int j = 0; j < 4; j++)
		{
			Vec4f PowerBasis = Vec4f(t * t * t, t * t, t, 1);
			Bezier_Matrix.Transform(PowerBasis);
			Geogetries[j].Transform(PowerBasis);
			points[j] = PowerBasis;
		}
		float coffi[16] = { points[0][0],points[1][0],points[2][0],points[3][0],
							points[0][1],points[1][1],points[2][1],points[3][1],
							points[0][2],points[1][2],points[2][2],points[3][2],
			                0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= patch_tess; j++)
		{
			float m = j * step_size;
			Vec4f PowerBasis_m = Vec4f(m * m * m, m * m, m, 1);
			Bezier_Matrix.Transform(PowerBasis_m);
			Geogetry.Transform(PowerBasis_m);
			TN->SetVertex(i, j, Vec3f(PowerBasis_m[0], PowerBasis_m[1], PowerBasis_m[2]));
		}
	}
	return TN;
}

void BezierPatch::Paint(ArgParser* args)
{
	glPointSize(3);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < getNumVertices(); i++)
	{
		Vec3f point = getVertex(i);
		glVertex3f(point[0], point[1], point[2]);
	}
	glEnd();

	glLineWidth(2);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int patch_tess = args->patch_tessellation;
	float step_size = 1.0 / patch_tess;
	Matrix Geogetries[4];
	for (int i = 0; i < 4; i++)
	{
		Vec3f p1 = getVertex(i * 4);
		Vec3f p2 = getVertex(i * 4 + 1);
		Vec3f p3 = getVertex(i * 4 + 2);
		Vec3f p4 = getVertex(i * 4 + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Geogetries[i] = Matrix(coffi);
	}
	for (int i = 0; i <= patch_tess; i++)
	{
		float t = i * step_size;
		Vec4f points[4];
		for (int j = 0; j < 4; j++)
		{
			Vec4f PowerBasis = Vec4f(t * t * t, t * t, t, 1);
			Bezier_Matrix.Transform(PowerBasis);
			Geogetries[j].Transform(PowerBasis);
			points[j] = PowerBasis;
		}
		float coffi[16] = { points[0][0],points[1][0],points[2][0],points[3][0],
							points[0][1],points[1][1],points[2][1],points[3][1],
							points[0][2],points[1][2],points[2][2],points[3][2],
							0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= patch_tess; j++)
		{
			float m = j * step_size;
			Vec4f PowerBasis_m = Vec4f(m * m * m, m * m, m, 1);
			Bezier_Matrix.Transform(PowerBasis_m);
			Geogetry.Transform(PowerBasis_m);
			glVertex3f(PowerBasis_m[0], PowerBasis_m[1], PowerBasis_m[2]);
		}
	}
	glEnd();
}