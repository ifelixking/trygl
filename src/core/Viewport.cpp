//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Viewport.h"
#include "Layer.h"

Viewport::Viewport() :
		m_isInvalidate(true),
		m_fullWindow(true) {
}

Viewport::~Viewport() {

}

Layer *Viewport::AddLayer() {
	auto layer = new Layer;
	m_layers.push_back(layer);
	return layer;
}

void Viewport::RemoveLayer(Layer *layer) {
	auto itorFind = std::find(m_layers.begin(), m_layers.end(), layer);
	if (itorFind == m_layers.end()) { return; }
	delete layer;
	m_layers.erase(itorFind);
}

void Viewport::Render() const {

	glViewport(0, 0, 320, 240);

	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	// glRectf(-1.0f, -1.0f, 1.5f, 1.5f);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

//	glBegin(GL_LINES);
//	glVertex2f(0.0f, 0.0f);
//	glVertex2f(0.3f, 0.5f);
//	glEnd();


	m_isInvalidate = false;
}

bool Viewport::IsInvalidate() const {
	return m_isInvalidate;
}

void Viewport::Set(const Info &info) {
	if (m_fullWindow) { return; }
	m_info = info;
}