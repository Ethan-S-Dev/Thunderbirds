#pragma once
#include <windows.h>
#include <vector>
#include <string>

template <typename T>
bool contains(const std::vector<T>& vector, const T& value) {
	return std::find(vector.begin(), vector.end(), value) != vector.end();
}

template <typename T, class _Pr>
bool any_in(const std::vector<T>& vector, _Pr predicate) {
	return std::find_if(vector.begin(), vector.end(), predicate) != vector.end();
}

template <class... _Types>
void LogDebug(const std::format_string<_Types...> _Fmt, _Types&&... _Args) {
	std::string toPrint = std::format<_Types...>(_Fmt, _Args...);
	OutputDebugStringA(toPrint.c_str());
}