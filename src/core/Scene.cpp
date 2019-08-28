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

	glColor3f(1.0f, 0.0f, 0.0f);
//	// glRectf(-1.0f, -1.0f, 1.5f, 1.5f);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

//	glBegin(GL_LINES);
//	glVertex2f(0.0f, 0.0f);
//	glVertex2f(0.3f, 0.5f);
//	glEnd();

}
