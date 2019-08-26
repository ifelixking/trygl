//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Viewport.h"
#include "Layer.h"

Viewport::Viewport() {

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

}

bool Viewport::IsInvalidate() const {
	return true;
}