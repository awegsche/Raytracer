#ifndef PREVIEWWORLD_H
#define PREVIEWWORLD_H

#include "world.h"
#include "pinhole.h"
#include "light.h"

class PreviewWorld : public World
{
private:
    Pinhole* preview_camera;
    int m_downsampling;
    int num_samples;

    Light* render_ambient;
    Light* preview_ambient;

public:
    bool preview;

public:
    PreviewWorld(int dowmsampling = 2, int supersampling = 16);

    void build();
    void render_preview();

    void Keypressed(int key);


    // QThread interface
    void set_sampler(const int nsamples = -1);

protected:
    void run();
};

#endif // PREVIEWWORLD_H
