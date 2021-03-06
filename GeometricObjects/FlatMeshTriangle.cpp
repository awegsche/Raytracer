// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "constants.h"
#include "FlatMeshTriangle.h"
#include "shaderec.h"
						

// ----------------------------------------------------------------  default constructor

FlatMeshTriangle::FlatMeshTriangle(void)
	: 	MeshTriangle()
{}


// ---------------------------------------------------------------- constructor

FlatMeshTriangle::FlatMeshTriangle (Mesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: 	MeshTriangle(_mesh_ptr, i0, i1, i2)
{}


// ---------------------------------------------------------------- clone

FlatMeshTriangle* 
FlatMeshTriangle::clone(void) const {
	return (new FlatMeshTriangle(*this));
}


// ---------------------------------------------------------------- copy constructor

FlatMeshTriangle::FlatMeshTriangle(const FlatMeshTriangle& fmt)
	:	MeshTriangle(fmt)
{}


// ---------------------------------------------------------------- assignment operator

FlatMeshTriangle& 
FlatMeshTriangle::operator= (const FlatMeshTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	MeshTriangle::operator= (rhs);
	
	return (*this);
}


// ---------------------------------------------------------------- destructor

FlatMeshTriangle::~FlatMeshTriangle(void) {}


// ---------------------------------------------------------------- hit

bool 															 
FlatMeshTriangle::hit(const Ray& ray, real& tmin, ShadeRec& sr) const {	
    Point v0(mesh_ptr->vertices[index0]);
    Point v1(mesh_ptr->vertices[index1]);
    Point v2(mesh_ptr->vertices[index2]);
	
    double a = v0.X() - v1.X(), b = v0.X() - v2.X(), c = ray.d.X(), d = v0.X() - ray.o.X();
    double e = v0.Y() - v1.Y(), f = v0.Y() - v2.Y(), g = ray.d.Y(), h = v0.Y() - ray.o.Y();
    double i = v0.Z() - v1.Z(), j = v0.Z() - v2.Z(), k = ray.d.Z(), l = v0.Z() - ray.o.Z();
		
	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;
	
	double inv_denom  = 1.0 / (a * m + b * q + c * s);
	
	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;
	
	if (beta < 0.0)
	 	return (false);
	
	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;
	
	if (gamma < 0.0)
	 	return (false);
	
	if (beta + gamma > 1.0)
		return (false);
			
	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;
	
	if (t < kEpsilon)
		return (false);
					
	tmin 				= t;
	sr.normal 			= normal;  				// for flat shading
	sr.local_hit_point 	= ray.o + t * ray.d;	
	
	return (true);	
}  


