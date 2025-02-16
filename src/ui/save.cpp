
#include <format>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>  // для работы с кавычками
#include "save.h"
#include "ui/windata.h"
#include "windata.h"
#include <fstream>
#include <filesystem>
#include <format>

#include <windows.h>

namespace fs = std::filesystem;

namespace helper
{

constexpr uint8_t ENCRYPTION_KEY = 0x7F;

void xorEncryptDecrypt(char* data, size_t size, uint8_t key)
{
    for (size_t i = 0; i < size; ++i)
    {
        data[i] ^= key;
    }
}

std::filesystem::path GetExePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
}

fs::path getFullPath(const std::string& filename)
{
    const fs::path exePath = fs::current_path();
    const fs::path directory = fs::canonical(GetExePath());
    return directory / filename;
}

void SaveToFile(const helper::WindowData& data, const std::string& filename)
{
    const auto fullPath{getFullPath(filename)};

    std::ofstream out(fullPath, std::ios::binary);
    if (!out.is_open())
    {
        throw std::ios_base::failure("Не удалось открыть файл для записи");
    }

    for (const auto& [key, value] : data._self->data)
    {
        std::string line{};
        // Сохраняем ключ
        // out << std::quoted(key) << " ";  // std::quoted позволяет сохранять ключи с пробелами
        line = std::format("\"{}\" ", key);

        if (std::holds_alternative<bool>(value))
        {
            // out << "bool " << std::get<bool>(value);
            line += std::format("bool {}", std::get<bool>(value) ? 1 : 0);
        }
        else if (std::holds_alternative<float>(value))
        {
            // out << "float " << std::get<float>(value);
            line += std::format("float {}", std::get<float>(value));
        }
        else if (std::holds_alternative<std::string>(value))
        {
            // out << "string " << std::quoted(std::get<std::string>(value));
            line += std::format("string \"{}\"", std::get<std::string>(value));
        }

        // out << "\n";
        xorEncryptDecrypt(line.data(), line.size(), ENCRYPTION_KEY);
        out << line << std::endl;
    }

    out.close();
}
WindowData LoadFromFile(const std::string& filename)
{
    const auto fullPath{getFullPath(filename)};

    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open())
    {
        return {};
    }

    WindowData data;
    std::string line;

    while (std::getline(in, line))
    {
        xorEncryptDecrypt(line.data(), line.size(), ENCRYPTION_KEY);
        std::istringstream iss(line);
        std::string key, type;

        // Читаем ключ
        iss >> std::quoted(key);

        // Читаем тип значения
        iss >> type;

        if (type == "bool")
        {
            bool bool_value{};
            iss >> bool_value;
            helper::setBool(data, key, bool_value);
        }
        else if (type == "float")
        {
            float float_value;
            iss >> float_value;
            helper::setFloat(data, key, float_value);
        }
        else if (type == "string")
        {
            std::string string_value;
            iss >> std::quoted(string_value);
            helper::setString(data, key, string_value);
        }
        else
        {
            throw std::runtime_error("Неизвестный тип данных: " + type);
        }
    }

    in.close();
    return data;
}
}  // namespace helper

