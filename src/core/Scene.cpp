//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Scene.h"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::Render(Camera *camera) {
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
}
