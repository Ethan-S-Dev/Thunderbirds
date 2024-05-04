#pragma once
#include <windows.h>
#include <conio.h>
#include <memory>
#include <string>
#include <chrono>
enum VKCode {
	LeftMouseButton = 0x01,
	RightMouseButton = 0x02,
	ControlBreakProcessing = 0x03,
	MiddleMouseButton = 0x04,
	X1MouseButton = 0x05,
	X2MouseButton = 0x06,
	BackspaceKey = 0x08,
	TabKey = 0x09,
	ClearKey = 0x0C,
	EnterKey = 0x0D,
	ShiftKey = 0x10,
	ControlKey = 0x11,
	AltKey = 0x12,
	PauseKey = 0x13,
	CapsLockKey = 0x14,
	KanaHangulKey = 0x15,
	IMEOn = 0x16,
	IMEJunjaMode = 0x17,
	IMEFinalMode = 0x18,
	IMEHanjaKanjiMode = 0x19,
	IMEOff = 0x1A,
	EscKey = 0x1B,
	IMEConvert = 0x1C,
	IMENonconvert = 0x1D,
	IMEAccept = 0x1E,
	IMEModeChangeRequest = 0x1F,
	Spacebar = 0x20,
	PageUpKey = 0x21,
	PageDownKey = 0x22,
	EndKey = 0x23,
	HomeKey = 0x24,
	LeftArrowKey = 0x25,
	UpArrowKey = 0x26,
	RightArrowKey = 0x27,
	DownArrowKey = 0x28,
	SelectKey = 0x29,
	PrintKey = 0x2A,
	ExecuteKey = 0x2B,
	PrintScreenKey = 0x2C,
	InsertKey = 0x2D,
	DeleteKey = 0x2E,
	HelpKey = 0x2F,
	ZeroKey = 0x30,
	OneKey = 0x31,
	TwoKey = 0x32,
	ThreeKey = 0x33,
	FourKey = 0x34,
	FiveKey = 0x35,
	SixKey = 0x36,
	SevenKey = 0x37,
	EightKey = 0x38,
	NineKey = 0x39,
	AKey = 0x41,
	BKey = 0x42,
	CKey = 0x43,
	DKey = 0x44,
	EKey = 0x45,
	FKey = 0x46,
	GKey = 0x47,
	HKey = 0x48,
	IKey = 0x49,
	JKey = 0x4A,
	KKey = 0x4B,
	LKey = 0x4C,
	MKey = 0x4D,
	NKey = 0x4E,
	OKey = 0x4F,
	PKey = 0x50,
	QKey = 0x51,
	RKey = 0x52,
	SKey = 0x53,
	TKey = 0x54,
	UKey = 0x55,
	VKey = 0x56,
	WKey = 0x57,
	XKey = 0x58,
	YKey = 0x59,
	ZKey = 0x5A,
	LeftWindowsKey = 0x5B,
	RightWindowsKey = 0x5C,
	ApplicationsKey = 0x5D,
	ComputerSleepKey = 0x5F,
	Numpad0 = 0x60,
	Numpad1 = 0x61,
	Numpad2 = 0x62,
	Numpad3 = 0x63,
	Numpad4 = 0x64,
	Numpad5 = 0x65,
	Numpad6 = 0x66,
	Numpad7 = 0x67,
	Numpad8 = 0x68,
	Numpad9 = 0x69,
	MultiplyKey = 0x6A,
	AddKey = 0x6B,
	SeparatorKey = 0x6C,
	SubtractKey = 0x6D,
	DecimalKey = 0x6E,
	DivideKey = 0x6F,
	F1Key = 0x70,
	F2Key = 0x71,
	F3Key = 0x72,
	F4Key = 0x73,
	F5Key = 0x74,
	F6Key = 0x75,
	F7Key = 0x76,
	F8Key = 0x77,
	F9Key = 0x78,
	F10Key = 0x79,
	F11Key = 0x7A,
	F12Key = 0x7B,
	F13Key = 0x7C,
	F14Key = 0x7D,
	F15Key = 0x7E,
	F16Key = 0x7F,
	F17Key = 0x80,
	F18Key = 0x81,
	F19Key = 0x82,
	F20Key = 0x83,
	F21Key = 0x84,
	F22Key = 0x85,
	F23Key = 0x86,
	F24Key = 0x87,
	NumLockKey = 0x90,
	ScrollLockKey = 0x91,
	LeftShiftKey = 0xA0,
	RightShiftKey = 0xA1,
	LeftControlKey = 0xA2,
	RightControlKey = 0xA3,
	LeftAltKey = 0xA4,
	RightAltKey = 0xA5,
	BrowserBackKey = 0xA6,
	BrowserForwardKey = 0xA7,
	BrowserRefreshKey = 0xA8,
	BrowserStopKey = 0xA9,
	BrowserSearchKey = 0xAA,
	BrowserFavoritesKey = 0xAB,
	BrowserStartAndHomeKey = 0xAC,
	VolumeMuteKey = 0xAD,
	VolumeDownKey = 0xAE,
	VolumeUpKey = 0xAF,
	MediaNextTrackKey = 0xB0,
	MediaPrevTrackKey = 0xB1,
	MediaStopKey = 0xB2,
	MediaPlayPauseKey = 0xB3,
	StartMailKey = 0xB4,
	SelectMediaKey = 0xB5,
	StartApplication1Key = 0xB6,
	StartApplication2Key = 0xB7,
	SemicolonColonKey = 0xBA,
	PlusKey = 0xBB,
	CommaKey = 0xBC,
	MinusKey = 0xBD,
	PeriodKey = 0xBE,
	SlashQuestionKey = 0xBF,
	TildeKey = 0xC0,
	OpenBracketCurlyBraceKey = 0xDB,
	BackslashPipeKey = 0xDC,
	CloseBracketCurlyBraceKey = 0xDD,
	QuoteDoubleQuoteKey = 0xDE,
	OEM8 = 0xDF,
	OEM102 = 0xE2,
	IMEProcessKey = 0xE5,
	PacketKey = 0xE7,
	AttnKey = 0xF6,
	CrSelKey = 0xF7,
	ExSelKey = 0xF8,
	EraseEOFKey = 0xF9,
	PlayKey = 0xFA,
	ZoomKey = 0xFB,
	PA1Key = 0xFD,
	OEMClearKey = 0xFE,
};
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

constexpr auto PHYSICS_TICKS_PER_SEC = 60;

class ConsoleGameEngine {
protected:
	std::string _appName;
	COORD _screenSize = { 0 };
	struct keyState {
		bool Pressed;
		bool Released;
		bool Held;
	} _processKeys[256] = { 0 };
	short _processKeyOldState[256] = { 0 };
	short _processKeyNewState[256] = { 0 };

	struct keyState _physicsKeys[256] = { 0 };
	short _physicsKeyOldState[256] = { 0 };
	short _physicsKeyNewState[256] = { 0 };
private:
	std::unique_ptr<CHAR_INFO[]> _screenBuffer;
	static bool _active;
	HANDLE _screanHandle;
	SMALL_RECT _windowRect = { 0 };
	float _physicsDelta = 0;
public:
	ConsoleGameEngine() : _appName("Default"), _screanHandle(GetStdHandle(STD_OUTPUT_HANDLE)) {
	}
public:
	void Start() {
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
					_physicsDelta -= (1.f / PHYSICS_TICKS_PER_SEC);
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
protected:
	virtual void OnCreate() = 0;
	virtual void OnUpdate(float elapsedTime) = 0;
	virtual void OnPhysicsUpdate(float elapsedTime) = 0;
	virtual void OnDestroy() {
	}
	virtual void OnScreenResize() {
	}

	void Clear() {
		memset(_screenBuffer.get(), 0, sizeof(CHAR_INFO) * _screenSize.X * _screenSize.Y);
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
	void Stop() {
		_active = false;
	}
private:
	void Clip(int& x, int& y) const
	{
		if (x < 0) x = 0;
		if (x >= _screenSize.X) x = _screenSize.X;
		if (y < 0) y = 0;
		if (y >= _screenSize.Y) y = _screenSize.Y;
	}
	void HandleScreenResizing() {
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
	void HandleProcessInput() {
		for (int i = 0; i < 256; i++)
		{
			_processKeyNewState[i] = GetAsyncKeyState(i);

			_processKeys[i].Pressed = false;
			_processKeys[i].Released = false;

			if (_processKeyNewState[i] != _processKeyOldState[i])
			{
				if (_processKeyNewState[i] & 0x8000)
				{
					_processKeys[i].Pressed = !_processKeys[i].Held;
					_processKeys[i].Held = true;
				}
				else
				{
					_processKeys[i].Released = true;
					_processKeys[i].Held = false;
				}
			}

			_processKeyOldState[i] = _processKeyNewState[i];
		}
	}
	void HandlePhysicsInput() {
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
	void SetCursorVisibility(bool isVisible) {
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(_screanHandle, &cursorInfo);  // Get the cursor info
		cursorInfo.bVisible = isVisible;  // Set the visibility
		SetConsoleCursorInfo(_screanHandle, &cursorInfo);  // Apply the changes
	}
};

bool ConsoleGameEngine::_active = true;