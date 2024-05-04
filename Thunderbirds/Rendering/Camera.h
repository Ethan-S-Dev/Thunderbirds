#pragma once
#include <cmath>
#include "Rect.h"

class Camera {
private:
	Point _cameraWorldFocusePoint;
	Rect _cameraConsoleRect;
	Point _cameraConsoleFucsePoint;
public:
	Rect CameraConsoleRect() const {
		return _cameraConsoleRect;
	}
	void Focuse(const Point freeMoveWorldFocusePoint) {
		auto focusePointXDif = freeMoveWorldFocusePoint.X - _cameraWorldFocusePoint.X;
		auto focusePointYDif = freeMoveWorldFocusePoint.Y - _cameraWorldFocusePoint.Y;
		auto maxXDif = ((_cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X) / 4) * 1;
		auto maxYDif = ((_cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y) / 4) * 1;

		if (std::abs(focusePointXDif) > maxXDif + 1) {
			_cameraWorldFocusePoint = freeMoveWorldFocusePoint;
			return;
		}

		if (std::abs(focusePointYDif) > maxYDif + 1) {
			_cameraWorldFocusePoint = freeMoveWorldFocusePoint;
			return;
		}

		if (focusePointXDif + maxXDif == -1) {
			_cameraWorldFocusePoint.X--;
		}
		if (focusePointXDif - maxXDif == 1) {
			_cameraWorldFocusePoint.X++;
		}
		if (focusePointYDif - maxYDif == 1) {
			_cameraWorldFocusePoint.Y++;
		}
		if (focusePointYDif + maxYDif == -1) {
			_cameraWorldFocusePoint.Y--;
		}
	}
	Point TransformToConsole(const Point pointInWorld) const {
		auto centerXdif = _cameraWorldFocusePoint.X - _cameraConsoleFucsePoint.X;
		auto centerYdif = (_cameraConsoleRect.LeftDown.Y - _cameraWorldFocusePoint.Y) - _cameraConsoleFucsePoint.Y;

		Point pointInConsole(pointInWorld.X - centerXdif, (_cameraConsoleRect.LeftDown.Y - pointInWorld.Y) - centerYdif);

		if (
			pointInConsole.X < _cameraConsoleRect.LeftDown.X ||
			pointInConsole.X > _cameraConsoleRect.RightUp.X - 1 ||
			pointInConsole.Y < _cameraConsoleRect.RightUp.Y ||
			pointInConsole.Y > _cameraConsoleRect.LeftDown.Y) {
			pointInConsole.X = -1;
			pointInConsole.Y = -1;
		}
		return pointInConsole;
	}
	void SetCameraConsoleRectangle(Point consoleScreenSize, int topMargin) {
		_cameraConsoleRect.LeftDown.X = 0;
		_cameraConsoleRect.LeftDown.Y = consoleScreenSize.Y;
		_cameraConsoleRect.RightUp.X = consoleScreenSize.X;
		_cameraConsoleRect.RightUp.Y = topMargin;

		_cameraConsoleFucsePoint.X = (_cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X) / 2;
		_cameraConsoleFucsePoint.Y = (_cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y) / 2;
	}
	int Hight() const {
		return _cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y;
	}
	int Width() const {
		return _cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X;
	}
};

