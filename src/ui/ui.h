#pragma once

#include <SDL.h>
#include <SDL_syswm.h>

namespace ImGui
{
	enum class RETURN_CODE : int
	{
		NO_INITILIZE_DIREXTX = -100,
		NO_INITLIZED_SDL = -99,
		SUCCESED = 0
	};

	int RunUI();
}
