#include <GLES2/gl2.h>
#include <cstring>
#include <fmt/core.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-client-protocol.h>

#include "wlr-layer-shell-unstable-v1.h"

#include "renderer.hpp"
#include "compositor.hpp"
#include "macros.hpp"

void Compositor::layerSurfaceConfigure(void *data,
		                               zwlr_layer_surface_v1 *surface,
		                               uint32_t serial,
		                               uint32_t w,
		                               uint32_t h)
{
    Compositor* self = static_cast<Compositor*>(data);

	if (self->_window) {
		wl_egl_window_resize(self->_window, w, h, 0, 0);
	}

	zwlr_layer_surface_v1_ack_configure(surface, serial);
}

void Compositor::layerSurfaceClosed(void *data,
		                            zwlr_layer_surface_v1 *surface)
{
    Compositor* self = static_cast<Compositor*>(data);
	eglDestroySurface(RENDERER->display(), RENDERER->surface());
	wl_egl_window_destroy(self->_window);
	zwlr_layer_surface_v1_destroy(surface);
	wl_surface_destroy(self->_surface);
}

void Compositor::registryHandleGlobal(void* data,
                                      wl_registry* registry,
                                      uint32_t name,
                                      const char* interface,
                                      uint32_t version)
{
    Compositor* self = static_cast<Compositor*>(data);

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        self->_compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    }

    else if (strcmp(interface, wl_output_interface.name) == 0) {
		self->_output = static_cast<wl_output*>(wl_registry_bind(registry, name, &wl_output_interface, version));
    }

    else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        self->_layer_shell = static_cast<zwlr_layer_shell_v1*>(wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, version));
    }
}

void Compositor::registryHandleGlobalRemove([[maybe_unused]] void* data,
                                            [[maybe_unused]] wl_registry* registry,
                                            [[maybe_unused]] uint32_t name)
{
    // Nothing to do here...
}

void Compositor::surfaceFrameCallback(void *data, wl_callback *cb, [[maybe_unused]] uint32_t time) {
	Compositor* self = static_cast<Compositor*>(data);
	wl_callback_destroy(cb);
	self->_frame_callback = nullptr;
	self->draw();
}

Compositor::Compositor() {}

Compositor::~Compositor() {}

void Compositor::init() {
    _display = wl_display_connect(NULL);
    if (!_display) {
        fmt::print(stderr, "[ERROR] could not connect to Wayland display\n");
        return;
    }

    _registry = wl_display_get_registry(_display);
    if (!_registry) {
        fmt::print(stderr, "[ERROR] could not fetch display registry\n");
        return;
    }

    wl_registry_add_listener(_registry, &Compositor::s_registry_listener, this);
    wl_display_roundtrip(_display);

    _surface = wl_compositor_create_surface(_compositor);
    wl_surface_commit(_surface);

    if (!_layer_shell) {
        fmt::print(stderr, "[ERROR] could not create layer shell\n");
        return;
    }

    RENDERER->init(_display);
    fmt::print("renderer initialised...\n");

    _surface = wl_compositor_create_surface(_compositor);
    if (!_surface) {
        fmt::print(stderr, "[ERROR] could not create surface\n");
        return;
    }

    _layer_surface = zwlr_layer_shell_v1_get_layer_surface(_layer_shell,
                                                           _surface,
                                                           _output,
                                                           ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND,
                                                           "foobarbaz");

    if (!_layer_surface) {
        fmt::print(stderr, "[ERROR] could not create layer surface\n");
    }

    zwlr_layer_surface_v1_set_size(_layer_surface, 1620, 56);
    zwlr_layer_surface_v1_set_exclusive_zone(_layer_surface, 0);
    zwlr_layer_surface_v1_set_keyboard_interactivity(_layer_surface, ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE);
    zwlr_layer_surface_v1_set_anchor(_layer_surface, ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT);
    zwlr_layer_surface_v1_set_margin(_layer_surface, 0, 0, 0, 0);
    zwlr_layer_surface_v1_add_listener(_layer_surface, &layer_surface_listener, this);
    wl_surface_commit(_surface);
    wl_display_roundtrip(_display);

    _window = wl_egl_window_create(_surface, 1620, 56);
    if (!_window) {
        fmt::print(stderr, "[ERROR] could not create a Wayland EGL window instance\n");
        return;
    }

    auto const ptr = RENDERER->createPlatformWindowSurface(_window);

    RENDERER->setSurface(ptr);
    if (RENDERER->surface() == EGL_NO_SURFACE) {
        fmt::print(stderr, "[ERROR] could not initialize EGL window surface\n");
        return;
    }

    wl_display_roundtrip(_display);
}

void Compositor::shutdown() {
    if (_display)
        wl_display_disconnect(_display);
}

void Compositor::draw() {
	eglMakeCurrent(RENDERER->display(), RENDERER->surface(), RENDERER->surface(), RENDERER->context());

	glViewport(0, 0, 1620, 56);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0.5, 0.5, 1);

	_frame_callback = wl_surface_frame(_surface);
	wl_callback_add_listener(_frame_callback, &frame_listener, this);

	eglSwapBuffers(RENDERER->display(), RENDERER->surface());
}

bool Compositor::online() {
    return wl_display_dispatch(_display) != -1;
}
