#ifndef OPAL_COMPOSITOR_HPP
#define OPAL_COMPOSITOR_HPP

#include <memory>

#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

#include "wlr-layer-shell-unstable-v1.h"

class Compositor {
public:
    Compositor();
    ~Compositor();

    bool init();
    void start();
    void shutdown();
    bool poll();

    wl_display* m_display = nullptr;
    wl_registry* m_registry = nullptr;
    wl_surface* m_surface = nullptr;
    wl_compositor* m_compositor = nullptr;
    wl_output* m_output = nullptr;
    wl_egl_window* m_window = nullptr;

    zwlr_layer_shell_v1* m_layer_shell = nullptr;
    zwlr_layer_surface_v1* m_layer_surface = nullptr;

private:
    bool _initialised = false;
};

inline std::unique_ptr<Compositor> COMPOSITOR;

#endif // @END of OPAL_COMPOSITOR_HPP
