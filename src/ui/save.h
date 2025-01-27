#pragma once
#include "windata.h"

namespace helper
{
constexpr const char* nameFileSave{"save.data"};

void SaveToFile(const helper::WindowData& data, const std::string& filename);
WindowData LoadFromFile(const std::string& filename);
}  // namespace helper
