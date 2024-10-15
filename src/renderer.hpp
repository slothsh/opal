#ifndef OPAL_RENDERER_EGL_HPP
#define OPAL_RENDERER_EGL_HPP

#include <memory>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

class Renderer {
public:
    bool init();
    void shutdown();
    void draw();

    EGLDisplay getDisplay() const;

    EGLSurface getSurface() const;
    void setSurface(void* surface);

    EGLConfig getConfig() const;
    EGLContext getContext() const;

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLConfig m_config;
    EGLContext m_context;
    wl_callback* m_frame_callback = nullptr;
};

inline std::unique_ptr<Renderer> RENDERER;

#endif // @END of OPAL_RENDERER_EGL_HPP
