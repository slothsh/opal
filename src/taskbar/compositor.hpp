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

    void init();
    void shutdown();
    void draw();
    bool online();

private:
    static void registryHandleGlobal(void* data,
                                     wl_registry* registry,
                                     uint32_t name,
                                     const char* interface,
                                     uint32_t version);

    static void registryHandleGlobalRemove(void* data,
                                           wl_registry* registry,
                                           uint32_t name);


    static constexpr wl_registry_listener s_registry_listener = {
        .global = registryHandleGlobal,
        .global_remove = registryHandleGlobalRemove,
    };

    static void layerSurfaceConfigure(void *data,
		                              zwlr_layer_surface_v1 *surface,
		                              uint32_t serial,
		                              uint32_t w,
		                              uint32_t h);
    
    static void layerSurfaceClosed(void *data,
		                           zwlr_layer_surface_v1 *surface);

    static constexpr zwlr_layer_surface_v1_listener layer_surface_listener = {
        .configure = layerSurfaceConfigure,
        .closed = layerSurfaceClosed,
    };

    static void surfaceFrameCallback(void *data, wl_callback *cb, uint32_t time);

    static constexpr wl_callback_listener frame_listener = {
	    .done = surfaceFrameCallback
    };

    bool _initialized = false;

    wl_display* _display = nullptr;
    wl_registry* _registry = nullptr;
    wl_surface* _surface = nullptr;
    wl_compositor* _compositor = nullptr;
    wl_output* _output = nullptr;
    wl_egl_window* _window = nullptr;
    wl_callback* _frame_callback = nullptr;

    zwlr_layer_shell_v1* _layer_shell = nullptr;
    zwlr_layer_surface_v1* _layer_surface = nullptr;
};

inline std::unique_ptr<Compositor> COMPOSITOR;

#endif // @END of OPAL_COMPOSITOR_HPP
