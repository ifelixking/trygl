//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Layer.h"
#include "Scene.h"
#include "Viewport.h"

Layer::Layer(Viewport *viewport)
		: m_scene(nullptr),
		  m_camera(nullptr),
		  m_viewport(viewport),
		  m_isDirty(true) {
	initFBO();
}

void Layer::initFBO() {
	const auto vpInfo = m_viewport->Get();

	// save the current frame buffer object, texture 2d, render buffer object for recover
	int old_fboDraw, old_fboRead, old_texture, old_rbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &old_fboDraw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_fboRead);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_texture);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &old_rbo);

	// create a gl texture for color buffer
	glGenTextures(1, &m_glTexture);
	assert(m_glTexture);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// 因为与屏幕像素一一映射, 不用使用mipmap, 也不用 LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// TODO: 当参数 width, height 不是 2 的n次方时的兼容性
	// TODO: 参数 width, height 在 opengl 标准中至少支持到1024, 如果Layer对应的Viewport（窗口或屏幕）大于1024时，如何处理?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vpInfo.winWidth, vpInfo.winHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// create a rbo for depth buffer
	glGenRenderbuffers(1, &m_glRBODepth);
	assert(m_glRBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_glRBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, vpInfo.winWidth,
						  vpInfo.winHeight);    // GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32

	// create a fbo
	glGenFramebuffers(1, &m_glFBO);
	assert(m_glFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFBO);

	// bind texture to the fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTexture, 0);

	// bind rbo of depth to the fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_glRBODepth);

	// check the fro is ok
	auto checkResult = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(checkResult == GL_FRAMEBUFFER_COMPLETE);

	// recover the GL_FRAMEBUFFER to default
	glBindFramebuffer(GL_READ_FRAMEBUFFER, old_fboRead);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, old_fboDraw);
	glBindTexture(GL_TEXTURE_2D, old_texture);
	glBindRenderbuffer(GL_RENDERBUFFER, old_rbo);
}

Layer::~Layer() {
	int currentTexture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	if (currentTexture == m_glTexture) { glBindTexture(GL_TEXTURE_2D, 0); }
	glDeleteTextures(1, &m_glTexture);
}

void Layer::SetScene(Scene *scene) {
	m_scene = scene;
}

void Layer::SetCamera(Camera *camera) {
	m_camera = camera;
}

bool Layer::IsDirty() const {
	// TODO: 还有 scene 和 camera 的 dirty
	return m_isDirty;
}

RENDER_FRAME_STATUS Layer::Render(bool newFrame) const {
	// render scene to the fbo(即: texture)
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFBO);
	glEnable(GL_DEPTH_TEST);
	if (newFrame) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	return m_scene->Render(newFrame, m_sceneRP, m_camera);
}

void Layer::onResize() {
	const auto vpInfo = m_viewport->Get();

	int old_fboDraw, old_fboRead, old_texture, old_rbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &old_fboDraw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_fboRead);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_texture);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &old_rbo);

	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// TODO: 当参数 width, height 不是 2 的n次方时的兼容性
	// TODO: 参数 width, height 在 opengl 标准中至少支持到1024, 如果Layer对应的Viewport（窗口或屏幕）大于1024时，如何处理?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vpInfo.winWidth, vpInfo.winHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindRenderbuffer(GL_RENDERBUFFER, m_glRBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, vpInfo.winWidth, vpInfo.winHeight);

	// check the fro is ok
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFBO);
	auto checkResult = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(checkResult == GL_FRAMEBUFFER_COMPLETE);

	// recover the GL_FRAMEBUFFER to default
	glBindFramebuffer(GL_READ_FRAMEBUFFER, old_fboRead);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, old_fboDraw);
	glBindTexture(GL_TEXTURE_2D, old_texture);
	glBindRenderbuffer(GL_RENDERBUFFER, old_rbo);

	m_isDirty = true;
}