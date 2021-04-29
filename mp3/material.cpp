#include "material.h"
#include "ray.h"
#include "vectors.h"
#include "hit.h"

#include "glCanvas.h"  
#include "gl/GLUT.h"
#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

void PhongMaterial::glSetMaterial(void) const {

    GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat specular[4] = {
    getSpecularColor().r(),
    getSpecularColor().g(),
    getSpecularColor().b(),
      1.0 };
    GLfloat diffuse[4] = {
      getDiffuseColor().r(),
      getDiffuseColor().g(),
      getDiffuseColor().b(),
      1.0 };

    // NOTE: GL uses the Blinn Torrance version of Phong...      
    float glexponent = exp;
    if (glexponent < 0) glexponent = 0;
    if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight 
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0) {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

    }
    else if (SPECULAR_FIX_WHICH_PASS == 1) {
        // Second pass, compute normal dot light 
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }
    else {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}

Vec3f PhongMaterial::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
{
    Vec3f normal = hit.getNormal();
	Vec3f v = ray.getDirection().Negative();
	Vec3f l = dirToLight;
	Vec3f h = v + l;
	h.Normalize();
	Vec3f difcol = getDiffuseColor();
    Vec3f specol = getSpecularColor();
    Vec3f dif_color = Vec3f(difcol[0] * lightColor[0], difcol[1] * lightColor[1], difcol[2] * lightColor[2])*max(normal.Dot3(l), 0.0f);
    float Lo = pow(max(normal.Dot3(h),0.0f), exp);
	Vec3f spe_color = Lo * Vec3f(specol[0] * lightColor[0], specol[1] * lightColor[1], specol[2] * lightColor[2]);
	return spe_color + dif_color;
}