//
// Created by liyh on 2019/8/29.
//

#include "../stdafx.h"
#include "Texture2D.h"

Texture2D::Texture2D() {
	glGenTextures(1, &m_glTexture);
	assert(m_glTexture);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &m_glTexture);
}

void Texture2D::SetData(unsigned int width, unsigned int height, void *data) {
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	// glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);    //横坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);    //纵坐标
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}