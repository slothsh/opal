#define _POSIX_C_SOURCE 200809L

#include <string_view>

#include <fmt/core.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

#include "renderer_egl.hpp"

#define TERMINATE() do {                                                            \
	eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT); \
    if (m_display) { eglTerminate(m_display); }                                     \
	eglReleaseThread();                                                             \
} while(1)

static const EGLint config_attribs[] = {
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_RED_SIZE, 1,
	EGL_GREEN_SIZE, 1,
	EGL_BLUE_SIZE, 1,
	EGL_ALPHA_SIZE, 1,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_NONE,
};

static const EGLint context_attribs[] = {
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE,
};

EglRenderer::EglRenderer() {}

EglRenderer::~EglRenderer() {}

void EglRenderer::init(wl_display* display) {
    std::string_view client_exts_str = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

	if (client_exts_str.empty()) {
		if (eglGetError() == EGL_BAD_DISPLAY) {
			fmt::print(stderr, "EGL_EXT_client_extensions not supported\n");
		} else {
			fmt::print(stderr, "Failed to query EGL client extensions\n");
		}
	    return;
	}

	if (!client_exts_str.contains("EGL_EXT_platform_base")) {
		fmt::print(stderr, "EGL_EXT_platform_base not supported\n");
		return;
	}

	if (!client_exts_str.contains("EGL_EXT_platform_wayland")) {
		fmt::print(stderr, "EGL_EXT_platform_wayland not supported\n");
		return;
	}

	getPlatformDisplayExt = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(eglGetProcAddress("eglGetPlatformDisplayEXT"));
	if (getPlatformDisplayExt == nullptr) {
		fmt::print(stderr, "Failed to get eglGetPlatformDisplayEXT\n");
		return;
	}

	createPlatformWindowSurfaceExt = reinterpret_cast<PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC>(eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT"));
	if (createPlatformWindowSurfaceExt == NULL) {
		fmt::print(stderr, "Failed to get eglCreatePlatformWindowSurfaceEXT\n");
		return;
	}

	m_display = getPlatformDisplayExt(EGL_PLATFORM_WAYLAND_EXT, display, NULL);
	if (m_display == EGL_NO_DISPLAY) {
		fmt::print(stderr, "Failed to create EGL display\n");
		TERMINATE();
	}

	if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
		fmt::print(stderr, "Failed to initialize EGL\n");
		TERMINATE();
	}

	EGLint matched = 0;
	if (!eglChooseConfig(m_display, config_attribs, &m_config, 1, &matched)) {
		fmt::print(stderr, "eglChooseConfig failed\n");
		TERMINATE();
	}
	if (matched == 0) {
		fmt::print(stderr, "Failed to match an EGL config\n");
		TERMINATE();
	}

	m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attribs);
	if (m_context == EGL_NO_CONTEXT) {
		fmt::print(stderr, "Failed to create EGL context\n");
		TERMINATE();
	}

	return;
}

void EglRenderer::shutdown() {
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_display, m_context);
	eglTerminate(m_display);
	eglReleaseThread();
}

void* EglRenderer::display() {
    return m_display;
}

void* EglRenderer::surface() {
    return m_surface;
}

void EglRenderer::setSurface(void* surface) {
    if (surface)
        this->m_surface = surface;
}

void* EglRenderer::config() {
    return m_config;
}

void* EglRenderer::getPlatformDisplay() {
    return nullptr;
}

void* EglRenderer::createPlatformWindowSurface(wl_egl_window* window) {
    return createPlatformWindowSurfaceExt(m_display, m_config, window, nullptr);
}

void* EglRenderer::context() {
    return m_context;
}
