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

    bool init();
    void draw();

    void shutdown();

    EGLDisplay getDisplay() const;

    EGLSurface getSurface() const;
    void setSurface(void* surface);

    EGLConfig getConfig() const;
    EGLContext getContext() const;

    void* getPlatformDisplay();
    void* createPlatformWindowSurface(wl_egl_window* window);

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLConfig m_config;
    EGLContext m_context;
    wl_callback* m_frame_callback = nullptr;

private:
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt;
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt;
};

inline std::unique_ptr<Renderer> RENDERER;

#endif // @END of OPAL_RENDERER_EGL_HPP
