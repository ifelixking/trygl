//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_LAYER_H
#define TRYGL_LAYER_H


class Layer {

private:
	friend class Viewport;

	Layer();

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
	Camera *m_camera;
	Scene *m_scene;
};


#endif //TRYGL_LAYER_H
