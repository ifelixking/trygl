//
// Created by liyh on 2019/8/23.
//
#include "../stdafx.h"
#include "Scene.h"
#include "Texture2D.h"

Texture2D *texture = nullptr;

Scene::Scene() {

//	glEnable(GL_TEXTURE_2D);
//	if (texture == nullptr){
//		texture = new Texture2D;
//		unsigned char data[] = {
//				255, 0, 0, 255,
//				0, 255, 0, 255,
//				0, 0, 255, 255,
//				255, 255, 0, 255,
//		};
//		texture->SetData(2, 2, data);
//	}

}

Scene::~Scene() {

}

void Scene::Render(Camera *camera) {



	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0f, 0.0f, 1.0f);


	// glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture->GetHandle());

	glBegin(GL_QUADS);
	{
		// glTexCoord2f(0, 0);
		glVertex2f(-0.5f, -0.5f);
		// glTexCoord2f(0, 1.0);
		glVertex2f(-0.5f, 0.5f);
		// glTexCoord2f(1.0, 1.0);
		glVertex2f(0.5f, 0.5f);
		// _glTexCoord2f(1.0, 0);
		glVertex2f(0.5f, -0.5f);
	}
	glEnd();

	// glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
}
