#ifndef MCREGIONGRID_H
#define MCREGIONGRID_H

#include "compound.h"
#include "vector"
#include "mesh.h"
#include "mcblock.h"
#include <QString>
#include "world.h"
#include "textureholder.h"
#include <QMap>

#ifdef WCUDA
class MCRegionGridCUDA : public GeometricObjectCUDA {
public:
	GeometricObjectCUDA** cells;
	int nx, ny, nz;
	size_t num_cells;
	CUDAreal3 position, p1;
	CUDAreal m_unit;

	// Inherited via GeometricObjectCUDA
	virtual __device__ bool hit(const rayCU & ray, CUDAreal & tmin, ShadeRecCUDA & sr) const override;
	virtual __device__ bool shadow_hit(const rayCU & ray, CUDAreal & tmin) const override;
};
#endif // WCUDA


// MCGrid only contains Blocks. To create a hierarchical Grid, use
// MCRegionsGrid
class MCRegionGrid : public Compound
{
private:
    std::vector<GeometricObject*> cells;
    int nx, ny, nz;
    real multiplier;
    Point position;
    Point p1;

    real m_unit;

//public:
//    std::vector<MCBlock*> blocklist;


public:
    MCRegionGrid();

    void setup(int nx_, int ny_, int nz_, real unit, Point pos);


    void read_nbt(QString filename, World *w);
    void addblock(int x, int y, int z, GeometricObject* block);

    // GeometricObject interface
public:
    bool hit(const Ray &ray, real &t, ShadeRec &sr) const;
    bool shadow_hit(const Ray &ray, real &tmin) const;
    BBox get_bounding_box();

#ifdef WCUDA

	// Inherited via Compound
	virtual MCRegionGridCUDA * get_device_ptr() const override;
#endif // WCUDA
};

#endif // MCGRID_H
