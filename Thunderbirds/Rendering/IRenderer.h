#pragma once

class IRenderer {
public:
	virtual void Clear() = 0;
	virtual void Draw(int x, int y, char c = 0xDB, short col = 0x000F) = 0;
	virtual void Fill(int x1, int y1, int x2, int y2, char c = 0xDB, short col = 0x000F) = 0;
	virtual void DrawString(int x, int y, const std::string& s, short col = 0x000F) = 0;
};