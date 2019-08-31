//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Scene.h"
#include "Texture2D.h"
#include "x/XAdapter.h"

#define ADAPTER XAdapter

Texture2D *texture = nullptr;

Scene::Scene() {
}

Scene::~Scene() {

}

RENDER_FRAME_STATUS Scene::Render(bool newFrame, RenderPosition &start, class Camera *camera) {

	if (newFrame) {
		start.lastX = 0;
		start.lastY = 0;
	}

	const int size = 1000;
	const int cap = size * size / 100;
	int renderedEntryCount = 0;


	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	int x = start.lastX, y = start.lastY;
	for (; x < size; ++x) {
		for (; y < size; ++y) {
			float r = ((float) x) / ((float) size);
			float g = ((float) y) / ((float) size);
			glColor3f(r, g, 0.0f);
			glBegin(GL_POINTS);
			float a = (float) x / (float) size - 0.5f;
			float b = (float) y / (float) size - 0.5f;
			glVertex2f(a, b);
			glEnd();

			if (ADAPTER::HasEventInQueue()) {
				start.lastX = x;
				start.lastY = y + 1;
				return RFS_EVENT_BREAK;
			} else if (++renderedEntryCount == cap) {
				start.lastX = x;
				start.lastY = y + 1;
				return RFS_TIMEUP_BREAK;
			}
		}
		y = 0;
	}

	return RFS_COMPLETE;
}
