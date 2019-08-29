//
// Created by liyh on 2019/8/29.
//

#ifndef TRYGL_TEXTURE2D_H
#define TRYGL_TEXTURE2D_H


class Texture2D {

public:
	Texture2D();
	~Texture2D();

public:
	void SetData(unsigned int width, unsigned int height, void *data);

	unsigned int GetHandle() const { return m_glTexture; }

private:
	unsigned int m_glTexture;
};


#endif //TRYGL_TEXTURE2D_H
