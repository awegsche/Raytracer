#include "viewplane.h"
#include "PureRandom.h"

ViewPlane::ViewPlane()
 : gamma(1.0f), inv_gamma(1.0f),
   s(1.0f), num_samples(1),
   hres(128), vres(128),
   sampler_ptr(new PureRandom(num_samples)){

}

ViewPlane::ViewPlane(const ViewPlane &vp)
    :   gamma(vp.gamma), inv_gamma(vp.inv_gamma),
        s(vp.s), num_samples(vp.num_samples),
        hres(vp.hres), vres(vp.vres),
        sampler_ptr(vp.sampler_ptr){

}

void ViewPlane::set_gamma(float gamma_)
{
    gamma=gamma_;
    inv_gamma = 1.0/gamma_;
}
