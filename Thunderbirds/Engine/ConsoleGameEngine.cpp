#include <chrono>
#include <memory>
#include "../Extensions.h"

#include "ConsoleGameEngine.h"

// https://learn.microsoft.com/en-us/troubleshoot/windows-server/performance/obtain-console-window-handle
// This was the only way that worked
HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.

	// Fetch current window title.

	GetConsoleTitleA(pszOldWindowTitle, MY_BUFSIZE);

	// Format a "unique" NewWindowTitle.

	sprintf_s(pszNewWindowTitle, "%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	// Change current window title.

	SetConsoleTitleA(pszNewWindowTitle);

	// Ensure window title has been updated.

	Sleep(40);

	// Look for NewWindowTitle.

	hwndFound = FindWindowA(NULL, pszNewWindowTitle);

	// Restore original window title.

	SetConsoleTitleA(pszOldWindowTitle);

	return(hwndFound);
}

bool ConsoleGameEngine::_active = true;

ConsoleGameEngine::ConsoleGameEngine() :
	_appName("Default"),
	_screenSize{ 0 },
	_processKeys{ 0 },
	_processKeyOldState{ 0 },
	_processKeyNewState{ 0 },
	_physicsKeys{ 0 },
	_physicsKeyOldState{ 0 },
	_physicsKeyNewState{ 0 },
	_screenBuffer(),
	_screanHandle(GetStdHandle(STD_OUTPUT_HANDLE)),
	_windowRect{ 0 },
	_physicsDelta(0),
	_currentWindow(GetConsoleHwnd())
{}

void ConsoleGameEngine::Start() {
	_active = true;
	OnCreate();
	SetCursorVisibility(false);
	auto start_time = std::chrono::system_clock::now();
	auto end_time = std::chrono::system_clock::now();
	while (_active) {
		while (_active) {
			// Handle Timing
			end_time = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = end_time - start_time;
			start_time = end_time;
			auto fElapsedTime = elapsedTime.count();
			_physicsDelta += fElapsedTime;

			HandleScreenResizing();
			HandleProcessInput();

			if (_physicsDelta >= (1.f / PHYSICS_TICKS_PER_SEC)) {
				HandlePhysicsInput();
				OnPhysicsUpdate(_physicsDelta);
				_physicsDelta = 0;
			}

			OnUpdate(fElapsedTime);

			char s[256];
			sprintf_s(s, 256, "%s - FPS: %3.2f", _appName.c_str(), 1.0f / fElapsedTime);
			SetConsoleTitleA(s);
			WriteConsoleOutputA(_screanHandle, _screenBuffer.get(), _screenSize, { 0 }, &_windowRect);
		}
		OnDestroy();
	}

	SetCursorVisibility(true);
}

void ConsoleGameEngine::OnDestroy() {}

void ConsoleGameEngine::OnScreenResize() {}

void ConsoleGameEngine::Clear() {
	memset(_screenBuffer.get(), 0, sizeof(CHAR_INFO) * _screenSize.X * _screenSize.Y);
}

void ConsoleGameEngine::Draw(int x, int y, char c, short col) {
	if (x >= 0 && x < _screenSize.X && y >= 0 && y < _screenSize.Y) {
		_screenBuffer[y * _screenSize.X + x].Char.AsciiChar = c;
		_screenBuffer[y * _screenSize.X + x].Attributes = col;
	}
}

void ConsoleGameEngine::Fill(int x1, int y1, int x2, int y2, char c, short col)
{
	Clip(x1, y1);
	Clip(x2, y2);
	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			Draw(x, y, c, col);
}

void ConsoleGameEngine::DrawString(int x, int y, const std::string& s, short col) {
	for (size_t i = 0; i < s.size(); i++) {
		if (x + i >= 0 && x + i < _screenSize.X && y >= 0 && y < _screenSize.Y) {
			_screenBuffer[y * _screenSize.X + x + i].Char.AsciiChar = s[i];
			_screenBuffer[y * _screenSize.X + x + i].Attributes = col;
		}
	}
}

void ConsoleGameEngine::Stop() {
	_active = false;
}

void ConsoleGameEngine::Clip(int& x, int& y) const
{
	if (x < 0) x = 0;
	if (x >= _screenSize.X) x = _screenSize.X;
	if (y < 0) y = 0;
	if (y >= _screenSize.Y) y = _screenSize.Y;
}

void ConsoleGameEngine::HandleScreenResizing() {
	CONSOLE_SCREEN_BUFFER_INFO CSBI;
	GetConsoleScreenBufferInfo(_screanHandle, &CSBI);
	if (_screenSize.X != CSBI.dwSize.X || _screenSize.Y != CSBI.dwSize.Y) {
		_screenSize = CSBI.dwSize;
		_windowRect = CSBI.srWindow;

		_screenBuffer.reset(new CHAR_INFO[_screenSize.X * _screenSize.Y]);
		Clear();
		OnScreenResize();
	}
}

void ConsoleGameEngine::HandleProcessInput() {
	if (!IsActiveWindow()) {
		return;
	}

	for (int i = 0; i < 256; i++)
	{
		_processKeyNewState[i] = GetAsyncKeyState(i);

		_processKeys[i].Pressed = false;
		_processKeys[i].Released = false;

		if (_processKeyNewState[i] != _processKeyOldState[i])
		{
			if (_processKeyNewState[i])
			{
				_processKeys[i].Pressed = !_processKeys[i].Held;
				_processKeys[i].Held = true;
			}
			else
			{
				_processKeys[i].Released = true;
				_processKeys[i].Held = false;
			}
			std::string value = std::format("({}) - ({}) - ({})", _processKeys[i].Pressed ? "Pressed" : "", _processKeys[i].Held ? "Held" : "", _processKeys[i].Released ? "Released" : "");
			LogDebug("VK: {} is {}\n", i, value);
		}

		_processKeyOldState[i] = _processKeyNewState[i];
	}
}

void ConsoleGameEngine::HandlePhysicsInput() {
	for (int i = 0; i < 256; i++)
	{
		_physicsKeyNewState[i] = _processKeyNewState[i];

		_physicsKeys[i].Pressed = false;
		_physicsKeys[i].Released = false;

		if (_physicsKeyNewState[i] != _physicsKeyOldState[i])
		{
			if (_physicsKeyNewState[i] & 0x8000)
			{
				_physicsKeys[i].Pressed = !_physicsKeys[i].Held;
				_physicsKeys[i].Held = true;
			}
			else
			{
				_physicsKeys[i].Released = true;
				_physicsKeys[i].Held = false;
			}
		}

		_physicsKeyOldState[i] = _physicsKeyNewState[i];
	}
}

void ConsoleGameEngine::SetCursorVisibility(bool isVisible) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(_screanHandle, &cursorInfo);  // Get the cursor info
	cursorInfo.bVisible = isVisible;  // Set the visibility
	SetConsoleCursorInfo(_screanHandle, &cursorInfo);  // Apply the changes
}
#include <conio.h>
bool ConsoleGameEngine::IsActiveWindow() {
	return _currentWindow == GetForegroundWindow();
}

