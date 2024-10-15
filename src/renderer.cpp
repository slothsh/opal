#define _POSIX_C_SOURCE 200809L

#include <string_view>

#include <fmt/core.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

#include "./taskbar/compositor.hpp"
#include "renderer.hpp"

#define TERMINATE() do {                                                            \
	eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT); \
    if (m_display) { eglTerminate(m_display); }                                     \
	eglReleaseThread();                                                             \
} while(1); return false

bool Renderer::init() {
    std::string_view client_exts_str = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

	if (client_exts_str.empty()) {
		if (eglGetError() == EGL_BAD_DISPLAY) {
			fmt::print(stderr, "EGL_EXT_client_extensions not supported\n");
		} else {
			fmt::print(stderr, "Failed to query EGL client extensions\n");
		}
	    return false;
	}

	if (!client_exts_str.contains("EGL_EXT_platform_base")) {
		fmt::print(stderr, "EGL_EXT_platform_base not supported\n");
		return false;
	}

	if (!client_exts_str.contains("EGL_EXT_platform_wayland")) {
		fmt::print(stderr, "EGL_EXT_platform_wayland not supported\n");
		return false;
	}

    static PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(eglGetProcAddress("eglGetPlatformDisplayEXT"));
	if (getPlatformDisplayExt == nullptr) {
		fmt::print(stderr, "Failed to get eglGetPlatformDisplayEXT\n");
		return false;
	}

	static PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt = reinterpret_cast<PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC>(eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT"));
	if (createPlatformWindowSurfaceExt == NULL) {
		fmt::print(stderr, "Failed to get eglCreatePlatformWindowSurfaceEXT\n");
		return false;
	}

	m_display = getPlatformDisplayExt(EGL_PLATFORM_WAYLAND_EXT, COMPOSITOR->m_display, NULL);
	if (m_display == EGL_NO_DISPLAY) {
		fmt::print(stderr, "Failed to create EGL display\n");
		TERMINATE();
	}

	if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
		fmt::print(stderr, "Failed to initialize EGL\n");
		TERMINATE();
	}

	EGLint matched = 0;

    static const EGLint config_attribs[] = {
	    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	    EGL_RED_SIZE, 1,
	    EGL_GREEN_SIZE, 1,
	    EGL_BLUE_SIZE, 1,
	    EGL_ALPHA_SIZE, 1,
	    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	    EGL_NONE,
    };

	if (!eglChooseConfig(m_display, config_attribs, &m_config, 1, &matched)) {
		fmt::print(stderr, "eglChooseConfig failed\n");
		TERMINATE();
	}
	if (matched == 0) {
		fmt::print(stderr, "Failed to match an EGL config\n");
		TERMINATE();
	}

    static const EGLint context_attribs[] = {
	    EGL_CONTEXT_CLIENT_VERSION, 2,
	    EGL_NONE,
    };

	m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attribs);
	if (m_context == EGL_NO_CONTEXT) {
		fmt::print(stderr, "Failed to create EGL context\n");
		TERMINATE();
	}

    setSurface(createPlatformWindowSurfaceExt(RENDERER->m_display, RENDERER->m_config, COMPOSITOR->m_window, nullptr));
    if (m_surface == EGL_NO_SURFACE) {
        fmt::print(stderr, "[ERROR] could not initialize EGL window surface\n");
		TERMINATE();
    }

	return true;
}

static void surfaceFrameCallback([[maybe_unused]] void *data, wl_callback *cb, [[maybe_unused]] uint32_t time) {
	wl_callback_destroy(cb);
	RENDERER->m_frame_callback = nullptr;
	RENDERER->draw();
}

void Renderer::draw() {
	eglMakeCurrent(getDisplay(), getSurface(), getSurface(), getContext());

	glViewport(0, 0, 1620, 56);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0.5, 0.5, 1);

	RENDERER->m_frame_callback = wl_surface_frame(COMPOSITOR->m_surface);

    static constexpr wl_callback_listener frame_listener = {
	    .done = surfaceFrameCallback
    };

	wl_callback_add_listener(RENDERER->m_frame_callback, &frame_listener, nullptr);

	eglSwapBuffers(getDisplay(), getSurface());
}

void Renderer::shutdown() {
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_display, m_context);
	eglTerminate(m_display);
	eglReleaseThread();
}

EGLDisplay Renderer::getDisplay() const {
    return m_display;
}

EGLSurface Renderer::getSurface() const {
    return m_surface;
}

void Renderer::setSurface(void* surface) {
    if (surface)
        m_surface = surface;
}

EGLConfig Renderer::getConfig() const {
    return m_config;
}

EGLContext Renderer::getContext() const {
    return m_context;
}
