//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Viewport.h"
#include "Layer.h"
#include "RenderWindow.h"

Viewport::Viewport(RenderWindow *renderWindow) :
		m_renderWindow(renderWindow),
		m_isDirty(true) {
	SetFullWindow(true);
	auto layer = AddLayer();
}

Viewport::~Viewport() {

}

Layer *Viewport::AddLayer() {
	auto layer = new Layer(this);
	m_layers.push_back(layer);
	return layer;
}

void Viewport::RemoveLayer(Layer *layer) {
	auto itorFind = std::find(m_layers.begin(), m_layers.end(), layer);
	if (itorFind == m_layers.end()) { return; }
	delete layer;
	m_layers.erase(itorFind);
}

void Viewport::SetFullWindow(bool value) {
	m_fullWindow = value;
	if (value) {
		auto size = m_renderWindow->GetSize();
		m_info.winX = 0;
		m_info.winY = 0;
		m_info.winWidth = size.width;
		m_info.winHeight = size.height;
	}
}

bool Viewport::IsDirty() const {
	if (m_isDirty) { return true; }
	for (auto layer : m_layers) {
		if (layer->IsDirty()) { return true; }
	}
	return false;
}

void Viewport::Set(const Info &info) {
	if (m_fullWindow) { return; }
	m_info = info;
}

void Viewport::SetSize(unsigned int winWidth, unsigned int winHeight) {
	m_info.winWidth = winWidth;
	m_info.winHeight = winHeight;
	m_isDirty = true;
	for (auto layer : m_layers) { layer->onResize(); }
}
