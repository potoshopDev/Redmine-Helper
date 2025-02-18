#include "ui/ui.h"
#include <print>
#include <utility>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::println("Error: {}", SDL_GetError());
        return std::to_underlying(ImGui::RETURN_CODE::NO_INITLIZED_SDL);
    }

    return ImGui::RunUI();
}

