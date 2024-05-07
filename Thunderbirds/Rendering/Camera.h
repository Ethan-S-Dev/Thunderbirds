#pragma once
#include "Rect.h"

class Camera {
	Point _cameraWorldFocusePoint = {-1,-1};
	Rect _cameraConsoleRect;
	Point _cameraConsoleFucsePoint;
public:
	Rect CameraConsoleRect() const;
	void Focuse(const Point freeMoveWorldFocusePoint);
	Point TransformToConsole(const Point pointInWorld) const;
	void SetCameraConsoleRectangle(Point consoleScreenSize, int topMargin);
	int Hight() const;
	int Width() const;
};

