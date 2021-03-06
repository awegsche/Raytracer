#ifdef WCUDA

#include <cuda_runtime.h>
#include "ray.cuh"
#include "ray.h"
#include <curand_kernel.h>
#include "CUDAhelpers.h"
#include "mcgrid.h"
#include "shaderec.h"

static __device__ bool inside_bb(const CUDAreal3 &p0, const CUDAreal3 &p1, const CUDAreal3 &point) {
	return
		point.x > p0.x && point.x < p1.x &&
		point.y > p0.y && point.y < p1.x &&
		point.z > p0.z && point.z < p1.z;
}

static __device__ CUDAreal clamp(CUDAreal value, CUDAreal a, CUDAreal b) {
	if (value < a) return a;
	if (value > b) return b;
	return value;
}

__device__ bool MCGridCUDA::shadow_hit(const rayCU& ray, CUDAreal& tmin) const
{
	return false;
}

__device__ bool MCGridCUDA::hit(const rayCU& ray, CUDAreal& tmin, ShadeRecCUDA& sr) const
{
	//Material* mat_ptr = sr.material_ptr;

	// if it's a kernel:
	/*int column = threadIdx.x + blockIdx.x * blockDim.x;
	int row = threadIdx.y + blockIdx.y * blockDim.y;


	rayCU ray = rays[column + stride * row];*/

	//else:



	//MCGridCUDA grid = *gr;

	//int nx = grid.nx;
	//int ny = grid.ny;
	//int nz = grid.nz;


	CUDAreal ox = ray.o.x;
	CUDAreal oy = ray.o.y;
	CUDAreal oz = ray.o.z;
	CUDAreal dx = ray.d.x;
	CUDAreal dy = ray.d.y;
	CUDAreal dz = ray.d.z;
	CUDAreal x0 = p0.x;
	CUDAreal y0 = p0.y;
	CUDAreal z0 = p0.z;
	CUDAreal x1 = p1.x;
	CUDAreal y1 = p1.y;
	CUDAreal z1 = p1.z;
	CUDAreal tx_min, ty_min, tz_min;
	CUDAreal tx_max, ty_max, tz_max;
	// the following code includes modifications from Shirley and Morley (2003)

	CUDAreal a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	CUDAreal b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	CUDAreal c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	CUDAreal t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return (false);


	// initial cell coordinates

	int ix, iy, iz;

	if (inside_bb(p0, p1, ray.o)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		CUDAreal3 p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	CUDAreal dtx = (tx_max - tx_min) / nx;
	CUDAreal dty = (ty_max - ty_min) / ny;
	CUDAreal dtz = (tz_max - tz_min) / nz;

	CUDAreal 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = kHugeValueCUDA;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = kHugeValueCUDA;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = kHugeValueCUDA;
		iz_step = -1;
		iz_stop = -1;
	}

	//    if (tx_next < 0) tx_next = kHugeValueCUDA;
	//    if (ty_next < 0) ty_next = kHugeValueCUDA;
	//    if (tz_next < 0) tz_next = kHugeValueCUDA;



	// Test if there is a block face glued to the bounding box:

	int block_id = cells[ix + nx * iy + nx * ny * iz];
	CUDAreal3 block_p0 = __make_CUDAreal3(x0 + nx * BLOCKLENGTH_CUDA, y0 + ny * BLOCKLENGTH_CUDA, z0 + nz * BLOCKLENGTH_CUDA);
	if (block_id != 0) {
		real t_before = kHugeValueCUDA;

		real tx_min_pp = tx_next - dtx;
		real ty_min_pp = ty_next - dty;
		real tz_min_pp = tz_next - dtz;

		if (ix != 0 && ix != (nx - 1)) tx_min_pp = -kHugeValueCUDA;
		if (iy != 0 && iy != (ny - 1)) ty_min_pp = -kHugeValueCUDA;
		if (iz != 0 && iz != (nz - 1)) tz_min_pp = -kHugeValueCUDA;


		if (tx_min_pp > ty_min_pp && tx_min_pp > tz_min_pp) {
			(sr).normal = __make_CUDAreal3(-(CUDAreal)ix_step, 0, 0);
			//sr.hdir = ix_step > 0 ? ShadeRec::South : ShadeRec::North;
			t_before = tx_min_pp;
		}
		else if (ty_min_pp > tz_min_pp) {
			(sr).normal = __make_CUDAreal3(0, -(CUDAreal)iy_step, 0);
			//sr.hdir = iy_step > 0 ? ShadeRec::Bottom : ShadeRec::Top;
			t_before = ty_min_pp;

		}
		else {
			(sr).normal = __make_CUDAreal3(0, 0, -(CUDAreal)iz_step);
			//sr.hdir = iz_step > 0 ? ShadeRec::West : ShadeRec::East;
			t_before = tz_min_pp;

		}
		if (true /*block_ptr->block_hit(ray, block_p0, t_before, sr)*/) {
			tmin = t_before;


			return (true);
		}
	}



	// traverse the grid
	tmin = kHugeValueCUDA;
	CUDAreal t_before = kHugeValueCUDA;

	while (true) {
		//        MCBlock* block_ptr = cells[ix + nx * iy + nx * ny * iz];

		if (tx_next < ty_next && tx_next < tz_next) {
			//real tmin = tx_next - kEpsilonCUDACUDA;
			//Material* mptr = sr.material_ptr;
			(sr).normal = __make_CUDAreal3(-(CUDAreal)ix_step, 0, 0);
			//sr.hdir = ix_step > 0 ? ShadeRec::South : ShadeRec::North;
			//sr.t_Before = t_before;
			t_before = tx_next;
			tx_next += dtx;
			ix += ix_step;
			if (ix == ix_stop) {
				(sr).material = 0;
				return (false);
			}


			int block_ptr = cells[ix + nx * iy + nx * ny * iz];
			CUDAreal3 block_p0 = __make_CUDAreal3(x0 + nx * BLOCKLENGTH_CUDA, y0 + ny * BLOCKLENGTH_CUDA, z0 + nz * BLOCKLENGTH_CUDA);

			if (block_ptr /* && block_ptr->block_hit(ray, block_p0, t_before, sr)*/) {
				tmin = t_before;


				return (true);
			}
			//sr.material_ptr = mptr;

		}
		else {
			if (ty_next < tz_next) {
				//Material* mptr = sr.material_ptr;
				(sr).normal = __make_CUDAreal3(0.0, -(CUDAreal)iy_step, 0);
				//sr.hdir = iy_step > 0 ? ShadeRec::Bottom : ShadeRec::Top;
				//sr.t_Before = t_before;
				t_before = ty_next;
				ty_next += dty;
				iy += iy_step;
				if (iy == iy_stop) {
					//(*sr).material = mat_ptr;
					return (false);
				}

				int block_ptr = cells[ix + nx * iy + nx * ny * iz];
				CUDAreal3 block_p0 = __make_CUDAreal3(x0 + nx * BLOCKLENGTH_CUDA, y0 + ny * BLOCKLENGTH_CUDA, z0 + nz * BLOCKLENGTH_CUDA);


				if (block_ptr /*&& block_ptr->block_hit(ray, block_p0, t_before, sr)*/) {
					//material_ptr = object_ptr->get_material();
					tmin = t_before;
					//t = ty_next;
					return (true);
				}
				//sr.material_ptr = mptr;
				//mat_ptr

			}
			else {
				//Material* mptr = sr.material_ptr;
				(sr).normal = __make_CUDAreal3(0.0, 0.0, -(CUDAreal)iz_step);
				//sr.hdir = iz_step > 0 ? ShadeRec::West : ShadeRec::East;
				//sr.t_Before = t_before;
				t_before = tz_next;
				tz_next += dtz;
				iz += iz_step;
				if (iz == iz_stop) {
					//sr.material_ptr = mat_ptr;
					return (false);
				}

				int block_ptr = cells[ix + nx * iy + nx * ny * iz];
				CUDAreal3 block_p0 = __make_CUDAreal3(x0 + nx * BLOCKLENGTH_CUDA, y0 + ny * BLOCKLENGTH_CUDA, z0 + nz * BLOCKLENGTH_CUDA);


				//material_ptr = sr.material_ptr;
				if (block_ptr/* && block_ptr->block_hit(ray, block_p0, t_before, sr)*/) {
					//material_ptr = object_ptr->get_material();
					//sr.material_ptr = material_ptr;
					tmin = t_before;
					// t = tz_next;
					return (true);
				}
				//sr.material_ptr = mptr;

			}
		}
	}
}


#endif // WCUDA
