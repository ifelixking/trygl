//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_LAYER_H
#define TRYGL_LAYER_H


class Layer {

private:
	friend class Viewport;

	Layer(class Viewport * viewport);

	~Layer();

public:
	void SetCamera(class Camera *camera);

	Camera *GetCamera() { return m_camera; }

	const Camera *GetCamera() const { return m_camera; }

	void SetScene(class Scene *scene);

	Scene *GetScene() { return m_scene; }

	const Scene *GetScene() const { return m_scene; }

	bool IsInvalidate()const;

	void Render() const;

private:
	void initFBO();
	void onResize();

private:
	Viewport * m_viewport;
	Camera *m_camera;
	Scene *m_scene;
	unsigned int m_glFBO;
	unsigned int m_glTexture;
	unsigned int m_glRBODepth;


};


#endif //TRYGL_LAYER_H
