#ifndef MCGRID_H
#define MCGRID_H

#include "compound.h"
#include "vector"
#include "mesh.h"
#include "mcblock.h"
#include <QString>


#include "constants.h"
#include "shaderec.h"




class MCSceneRenderer;
class MCRegionGrid;
class World;

// MCGrid only contains Blocks. To create a hierarchical Grid, use
// MCRegionsGrid
class MCGrid : public Compound
{
private:
    std::vector<int> cells;
	Point position;
	Point p1;
	MCRegionGrid* parent;
	MCSceneRenderer* _w;


    real m_unit;
	int nx, ny, nz;
	real multiplier;



public:
    MCGrid();

    void setup(int nx_, int ny_, int nz_, real unit, Point pos);

    void read_nbt(QString filename, World *w);
    void addblock(int x, int y, int z, int block);
    void set_parent(MCRegionGrid *grid, MCSceneRenderer *w);

    // GeometricObject interface
public:
    bool hit(const Ray &ray, real &t, ShadeRec &sr) const;
    bool shadow_hit(const Ray &ray, real &tmin) const;
    BBox get_bounding_box();

	//bool __device__ hitCUDA(rayCU& ray, CUDAreal& t, ShadeRecCUDA &sr) const;


};

#endif // MCGRID_H
