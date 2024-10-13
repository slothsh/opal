#ifndef OPAL_RENDERER_HPP
#define OPAL_RENDERER_HPP

#include <memory>

#include <EGL/egl.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    virtual void init(wl_display* display);
    virtual void shutdown();

    virtual void* display();
    virtual void* surface();
    virtual void setSurface(void* surface);
    virtual void* config();
    virtual void* context();

    virtual void* getPlatformDisplay();
    virtual void* createPlatformWindowSurface(wl_egl_window* window);

private:
    EGLSurface _surface;
};

inline std::unique_ptr<Renderer> RENDERER;

#endif // @END of OPAL_RENDERER_HPP
