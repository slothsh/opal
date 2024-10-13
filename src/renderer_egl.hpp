#ifndef OPAL_RENDERER_EGL_HPP
#define OPAL_RENDERER_EGL_HPP

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

#include "renderer.hpp"

class EglRenderer : public Renderer {
public:
    EglRenderer();
    virtual ~EglRenderer();

    void init(wl_display* display) override;
    void shutdown() override;
    void* display() override;
    void* surface() override;
    void setSurface(void* surface) override;
    void* config() override;
    void* context() override;

    void* getPlatformDisplay() override;
    void* createPlatformWindowSurface(wl_egl_window* window) override;

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLConfig m_config;
    EGLContext m_context;

private:
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt;
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt;
};

#endif // @END of OPAL_RENDERER_EGL_HPP
