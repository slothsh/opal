#include <cstring>
#include <fmt/core.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-client-protocol.h>

#include "wlr-layer-shell-unstable-v1.h"

#include "renderer.hpp"
#include "compositor.hpp"
#include "macros.hpp"

static void layerSurfaceConfigure([[maybe_unused]] void* data,
		                          zwlr_layer_surface_v1 *surface,
		                          uint32_t serial,
		                          uint32_t w,
		                          uint32_t h)
{
	if (COMPOSITOR->m_window) {
		wl_egl_window_resize(COMPOSITOR->m_window, w, h, 0, 0);
	}

	zwlr_layer_surface_v1_ack_configure(surface, serial);
}

static void layerSurfaceClosed([[maybe_unused]] void *data,
		                       zwlr_layer_surface_v1 *surface)
{
	eglDestroySurface(RENDERER->getDisplay(), RENDERER->getSurface());
	wl_egl_window_destroy(COMPOSITOR->m_window);
	zwlr_layer_surface_v1_destroy(surface);
	wl_surface_destroy(COMPOSITOR->m_surface);
}

static void registryHandleGlobal([[maybe_unused]] void* data,
                                 wl_registry* registry,
                                 uint32_t name,
                                 const char* interface,
                                 uint32_t version)
{
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        COMPOSITOR->m_compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    }

    else if (strcmp(interface, wl_output_interface.name) == 0) {
		COMPOSITOR->m_output = static_cast<wl_output*>(wl_registry_bind(registry, name, &wl_output_interface, version));
    }

    else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        COMPOSITOR->m_layer_shell = static_cast<zwlr_layer_shell_v1*>(wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, version));
    }
}

static void registryHandleGlobalRemove([[maybe_unused]] void* data,
                                       [[maybe_unused]] wl_registry* registry,
                                       [[maybe_unused]] uint32_t name)
{
    // Nothing to do here...
}

Compositor::Compositor() {}

Compositor::~Compositor() {
    if (_initialised)
        shutdown();
}

bool Compositor::init() {
    m_display = wl_display_connect(NULL);
    if (!m_display) {
        fmt::print(stderr, "[ERROR] could not connect to Wayland display\n");
        return false;
    }

    m_registry = wl_display_get_registry(m_display);
    if (!m_registry) {
        fmt::print(stderr, "[ERROR] could not fetch display registry\n");
        return false;
    }

    static constexpr wl_registry_listener registry_listener = {
        .global = registryHandleGlobal,
        .global_remove = registryHandleGlobalRemove,
    };

    wl_registry_add_listener(m_registry, &registry_listener, nullptr);
    wl_display_roundtrip(m_display);

    m_surface = wl_compositor_create_surface(m_compositor);
    wl_surface_commit(m_surface);

    if (!m_layer_shell) {
        fmt::print(stderr, "[ERROR] could not create layer shell\n");
        return false;
    }

    m_surface = wl_compositor_create_surface(m_compositor);
    if (!m_surface) {
        fmt::print(stderr, "[ERROR] could not create surface\n");
        return false;
    }

    m_layer_surface = zwlr_layer_shell_v1_get_layer_surface(m_layer_shell,
                                                           m_surface,
                                                           m_output,
                                                           ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND,
                                                           "foobarbaz");

    if (!m_layer_surface) {
        fmt::print(stderr, "[ERROR] could not create layer surface\n");
    }

    zwlr_layer_surface_v1_set_size(m_layer_surface, 1620, 56);
    zwlr_layer_surface_v1_set_exclusive_zone(m_layer_surface, 0);
    zwlr_layer_surface_v1_set_keyboard_interactivity(m_layer_surface, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE);
    zwlr_layer_surface_v1_set_anchor(m_layer_surface, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);
    zwlr_layer_surface_v1_set_margin(m_layer_surface, 0, 0, 0, 0);

    static constexpr zwlr_layer_surface_v1_listener layer_surface_listener = {
        .configure = layerSurfaceConfigure,
        .closed = layerSurfaceClosed,
    };

    zwlr_layer_surface_v1_add_listener(m_layer_surface, &layer_surface_listener, nullptr);

    wl_surface_commit(m_surface);
    wl_display_roundtrip(m_display);

    m_window = wl_egl_window_create(m_surface, 1620, 56);
    if (!m_window) {
        fmt::print(stderr, "[ERROR] could not create a Wayland EGL window instance\n");
        return false;
    }

    wl_display_roundtrip(m_display);

    RENDERER = std::make_unique<Renderer>();
    _initialised = RENDERER->init();
    fmt::print("renderer initialised...\n");

    return _initialised;
}

void Compositor::start() {
    if (_initialised) {
        RENDERER->draw();
        while (poll()) {
            // Main Event Loop
        }
    }
}

void Compositor::shutdown() {
    _initialised = false;

    if (RENDERER)
        RENDERER->shutdown();

    if (m_display)
        wl_display_disconnect(m_display);

    m_display = nullptr;
    m_registry = nullptr;
    m_surface = nullptr;
    m_compositor = nullptr;
    m_output = nullptr;
    m_window = nullptr;
    m_layer_shell = nullptr;
    m_layer_surface = nullptr;
}

bool Compositor::poll() {
    return wl_display_dispatch(m_display) != -1;
}
