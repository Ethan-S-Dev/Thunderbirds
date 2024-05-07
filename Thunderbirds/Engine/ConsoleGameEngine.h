#pragma once
#include <windows.h>
#include <string>
#include "KeyState.h"

constexpr auto PHYSICS_TICKS_PER_SEC = 60;

class ConsoleGameEngine {
private:
	static bool _active;
protected:
	std::string _appName;
	COORD _screenSize;
	KeyState _processKeys[256];
	short _processKeyOldState[256];
	short _processKeyNewState[256];
	KeyState _physicsKeys[256];
	short _physicsKeyOldState[256];
	short _physicsKeyNewState[256];
private:
	std::unique_ptr<CHAR_INFO[]> _screenBuffer;
	HANDLE _screanHandle;
	SMALL_RECT _windowRect;
	float _physicsDelta;
	HWND _currentWindow;
public:
	ConsoleGameEngine();
public:
	void Start();
protected:
	virtual void OnCreate() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnPhysicsUpdate(float deltaTime) = 0;
	virtual void OnDestroy();
	virtual void OnScreenResize();

	void Clear();
	void Draw(int x, int y, char c = 0xDB, short col = 0x000F);
	void Fill(int x1, int y1, int x2, int y2, char c = 0xDB, short col = 0x000F);
	void DrawString(int x, int y, const std::string& s, short col = 0x000F);
	void Stop();
private:
	void Clip(int& x, int& y) const;
	void HandleScreenResizing();
	void HandleProcessInput();
	void HandlePhysicsInput();
	void SetCursorVisibility(bool isVisible);
	bool IsActiveWindow();
};