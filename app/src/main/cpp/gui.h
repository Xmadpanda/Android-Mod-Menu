#ifndef DPZMOD_GUI_H
#define DPZMOD_GUI_H

#include "string"
#include "EGL/egl.h"
#include "GLES3/gl3.h"
#include "imgui.h"
#include "imgui_impl_android"
#include "imgui_impl_opengl3"

using namespace ImGui;

static int gWidth, gHeight;
static std::string gIniFilename = "";
bool gInitialized = false;

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

// Setup
vodi SetupImGui() 
{
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();
	io.IniFilename = gIniFilename.c_str();
	io.DisplaySize = ImVec2((float)gWidth, (float)gHeight)
	ImGui_ImplOpenGL3_Init("#version 300 es")
	StyleColorsDark();
	ImFontConfig font_cfg;
	font_cfg.SizePixels = 22.0f;
	io.Fonts->AddFontDefault(&font_cfg);
	GetStyle().ScaleAllSizes(5.0f);
}

EGLBoolean(*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	eglQuerySurface(dpy, surface, EGL_WIDTH, &gWidth);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, &gHeight);
	if (gInitialized)
	{
		SetupImGui();
		gInitialized = true;
	}
	ImGuiIO & Io = GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	NewFrame();
	ShowDemoWindow();
	EndFrame();
	Render()
	glViewport(0, 0 (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
	return old_eglSwapBuffers(dpy, surface);
}
#endif
