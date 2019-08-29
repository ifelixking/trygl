//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Layer.h"
#include "Scene.h"
#include "Viewport.h"

Layer::Layer(Viewport *viewport)
		: m_scene(nullptr),
		  m_camera(nullptr) {

	// create a gl texture
	glGenTextures(1, &m_glTexture);
	assert(m_glTexture)
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// 因为与屏幕像素一一映射, 不用使用mipmap, 也不用 LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// TODO: 当参数 width, height 不是 2 的n次方时的兼容性
	// TODO: 参数 width, height 在 opengl 标准中至少支持到1024, 如果Layer对应的Viewport（窗口或屏幕）大于1024时，如何处理?
	auto vpInfo = viewport->Get();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vpInfo.winWidth, vpInfo.winHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// create a fbo
	glGenFramebuffers(1, &m_glFBO);
	assert(m_glFBO);
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

bool Layer::IsInvalidate() const {
	return true;
}

void Layer::Render() const {


	m_scene->Render(m_camera);
}