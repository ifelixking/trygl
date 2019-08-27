//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Viewport.h"
#include "Layer.h"

Viewport::Viewport() : m_isInvalidate(true) {

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
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_isInvalidate = false;
}

bool Viewport::IsInvalidate() const {
	return m_isInvalidate;
}