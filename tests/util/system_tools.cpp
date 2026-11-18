#include "uxs_testsuite/util/system_tools.h"

#if WIN32
#    include <uxs/string_utf.h>

#    include <windows.h>

void util::remove_file(const char* fname) { ::DeleteFileW(uxs::utf_string_adapter<wchar_t>{}(fname).c_str()); }

std::vector<std::string> util::enum_files_in_directory(const std::string& path, std::string_view ext) {
    std::vector<std::string> file_names;
    HANDLE h_find;
    WIN32_FIND_DATAW find_file_data;
    auto search_path = uxs::utf_string_adapter<wchar_t>{}(path + "*" + std::string(ext));
    if ((h_find = ::FindFirstFileW(search_path.c_str(), &find_file_data)) != INVALID_HANDLE_VALUE) {
        do {
            file_names.emplace_back(path + uxs::utf_string_adapter<char>{}(find_file_data.cFileName));
        } while (::FindNextFileW(h_find, &find_file_data));
        ::FindClose(h_find);
    }
    return file_names;
}
#else
#    include <dirent.h>
#    include <unistd.h>

void util::remove_file(const char* fname) { ::unlink(fname); }

std::vector<std::string> util::enum_files_in_directory(const std::string& path, std::string_view ext) {
    std::vector<std::string> file_names;
    dirent* en = nullptr;
    DIR* dr = ::opendir((path + ".").c_str());  // open all directory
    if (dr) {
        while ((en = ::readdir(dr)) != nullptr) {
            std::string_view name = en->d_name;
            if (name.size() >= ext.size() && name.substr(name.size() - ext.size()) == ext) {
                file_names.emplace_back(path + std::string(name));
            }
        }
        ::closedir(dr);  // close all directory
    }
    return file_names;
}
#endif
