#pragma once

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