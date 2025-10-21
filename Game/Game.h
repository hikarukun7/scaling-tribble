#pragma once

#include <Windows.h>
#include "GMath.h"
#include "GameUtil.h"
#include "Keyboard.h"

#include <memory>
#include "Scene.h"

class Game
{
public:
	Game();
	~Game();

	void initialize(HWND hwnd, int width, int height);
	bool loop();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	const Keyboard& getKeyboard() const { return m_keyboard; }
	void drawCircle(const Circle& circle, COLORREF color);
	void drawString(const wchar_t* str, const Vector2d& pos,
		COLORREF color, int fsize);
	void drawString(const wchar_t* str, int num, const Vector2d& pos,
		COLORREF color, int fsize);
	void drawRect(Box rect, COLORREF fillColor, COLORREF penColor);

private:
	HWND m_hwnd;
	int  m_width;
	int  m_height;

	Keyboard m_keyboard;

	std::unique_ptr<Scene> m_scene;

	

	HBITMAP  m_backBuffer;
	HDC      m_backBufferDC;
	COLORREF m_backColor;

	static const float FrameRate;
	static const float MaxDeltaTime;

	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_freqTime;

	

	void input();
	void update(float deltaTime);
	void draw();

	void clear();
	void flip();

	bool tick(float& deltaTime);

	

};


