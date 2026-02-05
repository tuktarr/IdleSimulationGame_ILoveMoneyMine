#pragma once

#include "Common/Common.h"
#include <Windows.h>

namespace Wanted
{
	// 콘솔에 텍스트 색상 등을 지정할 때, 사용함
	// 색상 열거형
	// 보통 RGB 순서 -> MS만 BGR
	enum class WANTED_API Color : unsigned short
	{
		Black = 0,
		Blue = FOREGROUND_BLUE,
		Green = FOREGROUND_GREEN,
		Red = FOREGROUND_RED,
		White = Blue | Green | Red,
	}; 
}