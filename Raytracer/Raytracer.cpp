// Raytracer.cpp : Defines the entry point for the application.
//

#include "Raytracer.h"

#include <spdlog/spdlog.h>
#include "ui/Application.h"

using std::cout, std::endl;

int main() {
    cout << "Hello CMake." << endl;
    spdlog::info("info");

    Application app(2048, 1440);

    spdlog::info("info");

    if (app.is_valid()) {
        while (!app.should_close()) {
            app.begin_frame();
            app.end_frame();
        }
    }

    return 0;
}
