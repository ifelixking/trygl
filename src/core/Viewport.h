//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_VIEWPORT_H
#define TRYGL_VIEWPORT_H


class Viewport {

private:
	friend class RenderWindow;

	friend class Root;

	Viewport(class RenderWindow *renderWindow);

	~Viewport();

public:
	class Layer *AddLayer();

	void RemoveLayer(Layer *layer);

	// void Render() const;

	bool IsDirty() const;

	bool IsFullWindow() const { return m_fullWindow; }

	void SetFullWindow(bool value);

	struct Info {
		int winX;
		int winY;
		unsigned int winWidth;
		unsigned int winHeight;
	};

	// enable on not full window, and (0,0) is the left bottom corner
	void Set(const Info &info);

	const Info &Get() const { return m_info; }

	Layer *GetLayer(unsigned long index) { return m_layers[index]; }

	const Layer *GetLayer(unsigned long index) const { return m_layers[index]; }

	std::vector<Layer *> &GetLayers() { return m_layers; }

	const std::vector<Layer *> &GetLayers() const { return m_layers; }

	void SetSize(unsigned int winWidth, unsigned int winHeight);

private:
	RenderWindow *m_renderWindow;
	std::vector<Layer *> m_layers;
	mutable bool m_isDirty;
	bool m_fullWindow;
	Info m_info;
};


#endif //TRYGL_VIEWPORT_H
