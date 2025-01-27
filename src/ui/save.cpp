
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>  // для работы с кавычками
#include "save.h"
#include "ui/windata.h"
#include "windata.h"
#include <fstream>
#include <filesystem>

#include <windows.h>

namespace fs = std::filesystem;

namespace helper
{

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
        // Сохраняем ключ
        out << std::quoted(key) << " ";  // std::quoted позволяет сохранять ключи с пробелами

        if (std::holds_alternative<bool>(value))
        {
            out << "bool " << std::get<bool>(value);
        }
        else if (std::holds_alternative<float>(value))
        {
            out << "float " << std::get<float>(value);
        }
        else if (std::holds_alternative<std::string>(value))
        {
            out << "string " << std::quoted(std::get<std::string>(value));
        }

        out << "\n";
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
        std::istringstream iss(line);
        std::string key, type;

        // Читаем ключ
        iss >> std::quoted(key);

        // Читаем тип значения
        iss >> type;

        if (type == "bool")
        {
            bool bool_value;
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

// int main() {
//     UnorderedMap data = {
//         {"key with space", true},
//         {"float_key", 3.14f},
//         {"string key", "Hello, World!"}
//     };
//
//     // Сохраняем данные в файл
//     try {
//         SaveToFile(data, "save.dat");
//         std::cout << "Данные успешно сохранены в файл.\n";
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
//
//     // Загружаем данные из файла
//     try {
//         UnorderedMap loaded_data = LoadFromFile("save.dat");
//         std::cout << "Данные успешно загружены из файла:\n";
//         for (const auto& [key, value] : loaded_data) {
//             std::cout << key << ": ";
//             if (std::holds_alternative<bool>(value)) {
//                 std::cout << std::boolalpha << std::get<bool>(value);
//             } else if (std::holds_alternative<float>(value)) {
//                 std::cout << std::get<float>(value);
//             } else if (std::holds_alternative<std::string>(value)) {
//                 std::cout << std::get<std::string>(value);
//             }
//             std::cout << "\n";
//         }
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
//
//     return 0;
// }
