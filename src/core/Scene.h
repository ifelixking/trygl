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
	struct RenderPosition{
		int lastX;
		int lastY;
	};

	// newFrame: 是否从头渲染场景
	// start[in,out]: 场景渲染起点, 返回下次渲染起点
	RENDER_FRAME_STATUS Render(bool newFrame, RenderPosition & start, class Camera * camera);

private:
};


#endif //TRYGL_SCENE_H
