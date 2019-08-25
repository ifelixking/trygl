//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Layer.h"

Layer::Layer()
		: m_scene(nullptr),
		  m_camera(nullptr) {

}

Layer::~Layer() {

}

void Layer::SetScene(Scene *scene) {
	m_scene = scene;
}

void Layer::SetCamera(Camera *camera) {
	m_camera = camera;
}