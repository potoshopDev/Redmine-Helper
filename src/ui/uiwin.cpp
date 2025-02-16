#include "uiwin.h"

namespace helper
{

void draw(WindowsApp& app)
{
    for (const auto& window : app.windowsStack)
        draw(window);
}
void DefaultSettings(WindowsApp& app)
{
    for (const auto& window : app.windowsStack)
        DefaultSettings(window);
}

void DefaultSettings(const WindowsDraw& wd)
{
    wd.self_->defaultSettings_();
}
void draw(const WindowsDraw& wd)
{
    wd.self_->draw_();
}
}  // namespace helper

