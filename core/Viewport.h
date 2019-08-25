//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_VIEWPORT_H
#define TRYGL_VIEWPORT_H


class Viewport {

private:
	friend class RenderWindow;
	Viewport();
	~Viewport();

public:
	class Layer * AddLayer();
	void RemoveLayer(Layer * layer);

private:
	std::vector<Layer *> m_layers;
};


#endif //TRYGL_VIEWPORT_H
