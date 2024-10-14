#ifndef OPAL_RENDERER_EGL_HPP
#define OPAL_RENDERER_EGL_HPP

#include <memory>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

class Renderer {
public:
    Renderer();
    virtual ~Renderer();

    void init(wl_display* display);
    void shutdown();
    void* display();
    void* surface();
    void setSurface(void* surface);
    void* config();
    void* context();

    void* getPlatformDisplay();
    void* createPlatformWindowSurface(wl_egl_window* window);

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLConfig m_config;
    EGLContext m_context;

private:
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt;
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt;
};

inline std::unique_ptr<Renderer> RENDERER;

#endif // @END of OPAL_RENDERER_EGL_HPP
