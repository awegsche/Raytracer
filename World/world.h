#ifndef WORLD_H
#define WORLD_H


#include "rgbcolor.h"
#include "sphere.h"
#include "viewplane.h"
#include "tracer.h"
#include "qthread.h"
#include "ray.h"
#include "geometricobject.h"
#include "light.h"
#include <QThread>
#include "pixel.h"
#include "mcworld.h"
#include "grid.h"
#include "textureholder.h"
#include "mcblock.h"
#include <QMap>
#include "pixel.h"

#include "mcgrid.h"


#include <vector>

class Camera;
class MCRegionGrid;




class World : public QThread
{
    Q_OBJECT
public:
    //QMutex mutex;
    ViewPlane vp;
    RGBColor background_color;
    std::vector<GeometricObject*> objects;
    Tracer* tracer_ptr;
    Camera* camera_ptr;
    Light* ambient_ptr;
    std::vector<Light*> lights;
    bool running;
    int max_depth;
    bool haze;
    real haze_distance;
    real haze_attenuation;


public:
    World(QObject* parent = nullptr);

    virtual void build() = 0;
    void add_object(GeometricObject* o);
    void add_light(Light* l);

public:
    void render_scene_();
    void render_camera();
    ShadeRec hit_objects(const Ray& ray);
    void dosplay_p(int r, int c, const RGBColor &pixel_color);
    static Pixel display_p(Pixel& result, const Pixel& p);

signals:
    void display_pixel(int row, int column, int r, int g, int b);
    void display_line(const int row, const uint* rgb);
    void done();

    // QThread interface
protected:
    void run();
};



#endif // WORLD_H
