#include <windows.h>
#include <conio.h>

#include <string>
#include <chrono>

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0xDB,
};

void gotoxy(HANDLE hConsoleOutput, short x, short y)
{
	COORD coordinate = {0};
	coordinate.X = x;
	coordinate.Y = y;
	SetConsoleCursorPosition(hConsoleOutput, coordinate);
}

class ConsoleGameEngine {
public:
	ConsoleGameEngine() {
		_appName = "Default";
		_screanHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Clear() {
		memset(_screenBuffer, 0, sizeof(CHAR_INFO) * _screenSize.X * _screenSize.Y);
	}

	void Draw(int x, int y, char c = 0xDB, short col = 0x000F) {
		if (x >= 0 && x < _screenSize.X && y >= 0 && y < _screenSize.Y) {
			_screenBuffer[y * _screenSize.X + x].Char.AsciiChar = c;
			_screenBuffer[y * _screenSize.X + x].Attributes = col;
		}
	}

	void Fill(int x1, int y1, int x2, int y2, char c = 0xDB, short col = 0x000F)
	{
		Clip(x1, y1);
		Clip(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, c, col);
	}

	void DrawString(int x, int y, std::string c, short col = 0x000F) {
		for (size_t i = 0; i < c.size(); i++) {
			if (x + i >= 0 && x + i < _screenSize.X && y >= 0 && y < _screenSize.Y) {
				_screenBuffer[y * _screenSize.X + x + i].Char.AsciiChar = c[i];
				_screenBuffer[y * _screenSize.X + x + i].Attributes = col;
			}
		}
	}

	void Clip(int& x, int& y)
	{
		if (x < 0) x = 0;
		if (x >= _screenSize.X) x = _screenSize.X;
		if (y < 0) y = 0;
		if (y >= _screenSize.Y) y = _screenSize.Y;
	}

public:
	void Start() {
		_active = true;
		if (!OnUserCreate()) _active = false;

		auto start_time = std::chrono::system_clock::now();
		auto end_time = std::chrono::system_clock::now();
		while (_active)
		{
			while (_active) {
				// Handle Timing
				end_time = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = end_time - start_time;
				start_time = end_time;
				auto fElapsedTime = elapsedTime.count();

				// Handle Input
				HandleKeyboardInput();

				HandleScreenResizing();

				_active = OnUserUpdate(fElapsedTime);

				char s[256];
				sprintf_s(s, 256, "%s - FPS: %3.2f", _appName.c_str(), 1.0f / fElapsedTime);
				SetConsoleTitleA(s);
				WriteConsoleOutputA(_screanHandle, _screenBuffer, _screenSize, {0}, &_windowRect);
			}

			if (OnUserDestroy()) {
				// Do nothing
			}
			else {
				_active = true;
			}
		}
	}
private:
	void HandleScreenResizing() {
		CONSOLE_SCREEN_BUFFER_INFO CSBI;
		GetConsoleScreenBufferInfo(_screanHandle, &CSBI);
		if (_screenSize.X != CSBI.dwSize.X || _screenSize.Y != CSBI.dwSize.Y) {
			_screenSize = CSBI.dwSize;
			_windowRect = CSBI.srWindow;

			delete[] _screenBuffer;
			_screenBuffer = new CHAR_INFO[_screenSize.X * _screenSize.Y];
			Clear();
		}
	}
	void HandleKeyboardInput() {
		for (int i = 0; i < 256; i++)
		{
			_keyNewState[i] = GetAsyncKeyState(i);

			_keys[i].Pressed = false;
			_keys[i].Released = false;

			if (_keyNewState[i] != _keyOldState[i])
			{
				if (_keyNewState[i] & 0x8000)
				{
					_keys[i].Pressed = !_keys[i].Held;
					_keys[i].Held = true;
				}
				else
				{
					_keys[i].Released = true;
					_keys[i].Held = false;
				}
			}

			_keyOldState[i] = _keyNewState[i];
		}
	}
protected:
	struct sKeyState
	{
		bool Pressed;
		bool Released;
		bool Held;
	} _keys[256];
	short _keyOldState[256] = { 0 };
	short _keyNewState[256] = { 0 };

public:
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;

	virtual bool OnUserDestroy() {
		return true;
	}

protected:
	int Error(const char* msg)
	{
		char buf[256];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		printf("ERROR: %s\n\t%s\n", msg, buf);
		return 0;
	}

protected:
	std::string _appName;
	CHAR_INFO *_screenBuffer;
	static bool _active;
	HANDLE _screanHandle;
	COORD _screenSize = { 0 };
	SMALL_RECT _windowRect = { 0 };
};

bool ConsoleGameEngine::_active = true;

class Thunderbirds : public ConsoleGameEngine {
public:
	Thunderbirds()
	{
		_appName = "Thunderbirds";
	}

public:
	bool OnUserCreate() {
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) {
		Clear();
		char str[256];
		sprintf_s(str, 256, "Thunderbirds - Size: %dx%d", _screenSize.X, _screenSize.Y);
		auto len = strlen(str);

		if (_keys[0x41].Pressed) {
			_xPos--;
		}

		if (_keys[0x44].Pressed) {
			_xPos++;
		}

		if (_keys[0x57].Pressed) {
			_yPos--;
		}

		if (_keys[0x53].Pressed) {
			_yPos++;
		}

		short x = (_screenSize.X / 2) - (len / 2) + (_xPos * 2);
		short y = 2 + _yPos;
		Fill(x, y, x + len, y + len/2, ' ', BG_DARK_GREEN);
		DrawString(x, y, str, 0x000F | BG_DARK_GREEN);
		
		return true;
	}

private:
	short _xPos = 0;
	short _yPos = 0;
};

int main() {
	Thunderbirds tb;
	tb.Start();
	return 0;
}