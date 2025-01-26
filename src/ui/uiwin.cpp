#include "uiwin.h"

namespace helper
{

void draw(WindowsApp& app)
{
    for (const auto& window : app.windowsStack)
        draw(window);
}

void draw(const WindowsDraw& wd)
{
    wd.self_->draw_();
}
}  // namespace helper

