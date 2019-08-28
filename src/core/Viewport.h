//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_VIEWPORT_H
#define TRYGL_VIEWPORT_H


class Viewport {

private:
	friend class RenderWindow;

	Viewport(class RenderWindow *renderWindow);

	~Viewport();

public:
	class Layer *AddLayer();

	void RemoveLayer(Layer *layer);

	void Render() const;

	bool IsInvalidate() const;

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

	const Info &Get(int &winX, int &winY, unsigned int &winWidth, unsigned int &winHeight) const { return m_info; }

private:
	RenderWindow *m_renderWindow;
	std::vector<Layer *> m_layers;
	mutable bool m_isInvalidate;
	bool m_fullWindow;
	Info m_info;
};


#endif //TRYGL_VIEWPORT_H
