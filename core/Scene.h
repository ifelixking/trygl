//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_SCENE_H
#define TRYGL_SCENE_H


class Scene {
private:
	friend class Root;
	Scene();
	~Scene();

public:
	void Render(class Camera * camera);
};


#endif //TRYGL_SCENE_H
