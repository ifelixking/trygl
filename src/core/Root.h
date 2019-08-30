//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_ROOT_H
#define TRYGL_ROOT_H


class Root {

private:
	Root();

	~Root();

public:
	static void Init(int argc, char **argv);

	static void Destroy();

	static int RunMainLoop();

	static Root *GetInstance() { return s_instance; }

	class RenderWindow *CreateRenderWindow();

	void DestroyRenderWindow(RenderWindow *renderWindow);

	// nanoSpan: time span from last render
	// nanoframeTimeLimit: suggest render time limit
	void RenderOneFrame(long int nanoSpan, long int nanoframeTimeLimit = INT64_MAX);

	bool IsInvalidate() const;

	class Scene *CreateScene();

private:
	static void onWindowInvalidate(WINDOW_HANDLE hWin);

	static void onWindowResize(WINDOW_HANDLE hWin, unsigned int width, unsigned int height);

private:
	static Root *s_instance;
	std::map<WINDOW_HANDLE, RenderWindow *> m_renderWindows;
};


#endif //TRYGL_ROOT_H
