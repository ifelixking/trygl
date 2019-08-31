//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "Root.h"
#include "RenderWindow.h"
#include "Scene.h"
#include "Viewport.h"
#include "Layer.h"
#include "x/XAdapter.h"

#define ADAPTER XAdapter

Root *Root::s_instance = nullptr;

void Root::Init(int argc, char **argv) {
	if (s_instance) { return; }
	XAdapter::InitParams params = {argc, argv, 30, &onWindowInvalidate, &onWindowResize};
	XAdapter::Initialize(&params);

	Root *root = new Root;
	s_instance = root;
}

void Root::Destroy() {
	if (!s_instance) { return; }
	delete s_instance;
	s_instance = nullptr;
	XAdapter::Uninitialize();
}

int Root::RunMainLoop() {
	return XAdapter::RunMainLoop();
}

Root::Root() : m_lastRenderFrameComplete(true) {}

Root::~Root() {}

void Root::RenderOneFrame(long int nanoSpan, long int nanoframeTimeLimit) {
	for (auto result = doRenderOneFrame(true); result == RFS_TIMEUP_BREAK;) {
		// 没有渲染完, 只是因为时间到了, 并没有事件触发
		result = doRenderOneFrame(false);
	}
	// 有可能渲染了完整的一帧, 有可能被事件打断
}

RENDER_FRAME_STATUS Root::doRenderOneFrame(bool newFrame) {

	// TODO: 先将RenderWindow里的Viewport里的所有 "整批渲染" 的 Layer 先渲染, 再渲染 "支持分批渲染" 的 Layer
	// 		 merge 时 再按 z-index, disable 掉 depth-test, 渲染即可

	RENDER_FRAME_STATUS result = RFS_COMPLETE;
	// Loop RenderWindow
	for (auto pairRenderWindow : m_renderWindows) {
		auto renderWindow = pairRenderWindow.second;
		if (!renderWindow->IsInvalidate()) { continue; }

		// opengl make current
		ADAPTER::WindowMakeCurrent(renderWindow->m_hWindow);

		// TODO: 使用了 Layer, 似乎可以不用在 RenderWindow 上 glClear 了
//		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Loop Viewport
		bool allViewportRenderComplete = true;
		for (auto viewport : renderWindow->GetViewports()) {
			if (!viewport->IsDirty()) { continue; }

			// opengl setting viewport
			const Viewport::Info &vpInfo = viewport->Get();
			glViewport(vpInfo.winX, vpInfo.winY, vpInfo.winWidth, vpInfo.winHeight);




			// Loop Layer for render
			bool allLayerRenderComplete = true;
			for (auto layer : viewport->GetLayers()) {
				if (!layer->IsDirty()) { continue; }

				// layer Render
				auto rfStatus = layer->Render(newFrame);
				if (rfStatus != RFS_COMPLETE) {
					result = rfStatus;
					allLayerRenderComplete = false;
					allViewportRenderComplete = false;
				} else {
					layer->m_isDirty = false;
				}
			}

			// Loop Layer for merge
			const float VP_SIZE = 0.99f;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			for (auto layer : viewport->GetLayers()) {		// TODO: 考虑使用多重纹理, 不用每次都渲染一个矩形
				glBindTexture(GL_TEXTURE_2D, layer->m_glTexture);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 0);
					glVertex2f(-VP_SIZE, -VP_SIZE);
					glTexCoord2f(0, 1.0);
					glVertex2f(-VP_SIZE, VP_SIZE);
					glTexCoord2f(1.0, 1.0);
					glVertex2f(VP_SIZE, VP_SIZE);
					glTexCoord2f(1.0, 0);
					glVertex2f(VP_SIZE, -VP_SIZE);
				}
				glEnd();
			}

			viewport->m_isDirty = !allLayerRenderComplete;
		}

		// RenderWindow Swap Buffer
		ADAPTER::WindowSwapBuffer(renderWindow->m_hWindow);

		renderWindow->m_windowInvalidate = !allViewportRenderComplete;
	}

	return result;
}

RenderWindow *Root::CreateRenderWindow() {
	auto renderWindow = new RenderWindow;
	m_renderWindows.insert(std::make_pair(renderWindow->m_hWindow, renderWindow));
	return renderWindow;
}

void Root::DestroyRenderWindow(RenderWindow *renderWindow) {
	auto itorFind = m_renderWindows.find(renderWindow->m_hWindow);
	if (itorFind == m_renderWindows.end()) {
		assert(false);
		return;
	}
	m_renderWindows.erase(itorFind);
	delete renderWindow;
}

void Root::onWindowInvalidate(WINDOW_HANDLE hWin) {
	auto _this = Root::GetInstance();
	auto itorFind = _this->m_renderWindows.find(hWin);
	if (itorFind == _this->m_renderWindows.end()) {
		assert(false);
		return;
	}
	itorFind->second->SetInvalidate();
}

void Root::onWindowResize(WINDOW_HANDLE hWin, unsigned int width, unsigned int height) {
	auto _this = Root::GetInstance();
	auto itorFind = _this->m_renderWindows.find(hWin);
	if (itorFind == _this->m_renderWindows.end()) {
		assert(false);
		return;
	}
	itorFind->second->onResize(width, height);
}

bool Root::IsDirty() const {
	for (auto pair : m_renderWindows) {
		if (pair.second->IsInvalidate()) { return true; }
	}
	return false;
}

Scene *Root::CreateScene() {
	return new Scene;
}