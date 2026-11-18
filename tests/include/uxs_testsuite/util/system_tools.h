#pragma once

#include <uxs/string_view.h>

#include <string>
#include <vector>

namespace util {
void remove_file(const char* fname);
std::vector<std::string> enum_files_in_directory(const std::string& path, std::string_view ext);
}  // namespace util
