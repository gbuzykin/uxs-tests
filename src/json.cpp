#include "uxs/db/json.h"

#include "db_value_tests.h"

#include "uxs/io/filebuf.h"
#include "uxs/io/iflatbuf.h"
#include "uxs/io/oflatbuf.h"
#include "uxs/string_alg.h"

#include <vector>

#if WIN32
#    include <windows.h>
#else
#    include <dirent.h>
#    include <sys/types.h>
#endif

extern std::string g_testdata_path;

namespace {

int test_string_json_1() {
    uxs::filebuf ifile((g_testdata_path + "json/pass4.json").c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(est::as_span(&txt[0], sz)));

    uxs::iflatbuf input(txt);
    uxs::db::value root;
    VERIFY(root = uxs::db::json::read(input));
    VERIFY(root["array_of_strings"][0].as_string() == "1");
    VERIFY(root["array_of_strings"][1].as_string() == "2");
    VERIFY(root["array_of_strings"][2].as_string() == "3");
    VERIFY(root["object"]["array_of_i32"][0].as_int() == 1);
    VERIFY(root["object"]["array_of_i32"][1].as_int() == 2);
    VERIFY(root["object"]["array_of_i32"][2].as_int() == 3);
    VERIFY(root["object"]["d_val"].as_double() == 12.5646);
    VERIFY(root["object"]["i32_val"].as_int() == 123456);
    VERIFY(root["object"]["mixed_array"][0].as_int() == 1);
    VERIFY(root["object"]["mixed_array"][1]["T"].as_bool() == true);
    VERIFY(root["object"]["mixed_array"][1]["F"].as_bool() == false);
    VERIFY(root["object"]["mixed_array"][2].as_string() == "3");
    VERIFY(root["object"]["str_val"].as_string() == "hello");
    VERIFY(root["object"]["bool_val"].as_bool() == true);
    VERIFY(root["null"].is_null());

    uxs::oflatbuf output;
    uxs::db::json::write(output, root, 2, '\n');
    VERIFY(std::string_view(output.data(), output.size()) == txt);
    return 0;
}

int test_string_json_2() {
    auto enum_files_in_directory = [](const std::string& path) {
        std::vector<std::string> file_names;
#if WIN32
        HANDLE h_find;
        WIN32_FIND_DATAW find_file_data;
        auto search_path = uxs::from_utf8_to_wide(path + "*.json");
        if ((h_find = ::FindFirstFileW(search_path.c_str(), &find_file_data)) != INVALID_HANDLE_VALUE) {
            do {
                file_names.emplace_back(path + uxs::from_wide_to_utf8(find_file_data.cFileName));
            } while (::FindNextFileW(h_find, &find_file_data));
            ::FindClose(h_find);
        }
#else
        dirent* en = nullptr;
        DIR* dr = ::opendir((path + '.').c_str());  // open all directory
        if (dr) {
            while ((en = ::readdir(dr)) != nullptr) {
                std::string_view name = en->d_name;
                if (name.size() >= 5 && name.substr(name.size() - 5) == ".json") {
                    file_names.emplace_back(path + std::string(name));
                }
            }
            ::closedir(dr);  // close all directory
        }
#endif
        return file_names;
    };

    std::string test_path = g_testdata_path + "json/";
    auto file_names = enum_files_in_directory(test_path);

    for (const auto& file_name : file_names) {
        uxs::db::value root;

        bool is_valid = file_name.find("invalid") == std::string::npos && file_name.find("fail") == std::string::npos;

        try {
            {  // read
                uxs::filebuf ifile(file_name.c_str(), "r");
                VERIFY(ifile);
                root = uxs::db::json::read(ifile);
            }

            VERIFY(is_valid);

            std::string data;
            {  // write
                uxs::oflatbuf out;
                uxs::db::json::write(out, root);
                data = std::string(out.data(), out.size());
            }

            std::string output_file_name = file_name + ".out";

            {
                uxs::filebuf ofile(output_file_name.c_str(), "w");
                VERIFY(ofile);
                ofile.write(data);
            }

            bool skip_round_trip = false;

            {  // check expected
                std::string expected_file_name = file_name;
                auto pos = expected_file_name.rfind('.');
                if (pos != std::string::npos) { expected_file_name.resize(pos); }
                expected_file_name += ".expected";

                uxs::filebuf ifile(expected_file_name.c_str(), "r");
                do {
                    bool nl = true;
                    char ch = ifile.get();
                    while (ifile && (ch != '.' || !nl)) {
                        nl = ch == '\n';
                        ch = ifile.get();
                    }
                    if (!ifile) { break; }
                    std::string val;
                    uxs::db::value* v = &root;
                    bool eol = false;
                    do {
                        switch (ch) {
                            case '[': {
                                size_t i = 0;
                                ch = ifile.get();
                                while (ifile && uxs::is_digit(ch)) {
                                    i = 10u * i + ch - '0';
                                    ch = ifile.get();
                                }
                                VERIFY(ch == ']');
                                v = &(*v)[i];
                                ch = ifile.get();
                            } break;
                            case '.': {
                                std::string name;
                                ch = ifile.get();
                                while (ifile && ch != '.' && ch != '[' && ch != '=' && ch != '\n') {
                                    name += ch;
                                    ch = ifile.get();
                                }
                                if (!name.empty()) {
                                    if (name == "$") { name = ""; }  // empty name marker
                                    v = &(*v)[name];
                                }
                            } break;
                            case '=': {
                                ch = ifile.get();
                                while (ifile && ch != '\n') {
                                    val += ch;
                                    ch = ifile.get();
                                }
                                eol = true;
                            } break;
                            default: VERIFY(false); break;
                        }
                    } while (ifile && !eol);

                    VERIFY(!val.empty());

                    if (val == "null") {
                        VERIFY(v->is_null());
                    } else if (val == "false") {
                        VERIFY(v->is_bool() && v->as_bool() == false);
                    } else if (val == "true") {
                        VERIFY(v->is_bool() && v->as_bool() == true);
                    } else if (val == "[]") {
                        VERIFY(v->is_array());
                    } else if (val == "{}") {
                        VERIFY(v->is_record());
                    } else if (val[0] == '\"' && val.size() >= 2) {
                        VERIFY(v->is_string() && v->as_string_view() == val.substr(1, val.size() - 2));
                    } else if (val.find_first_of(".eEin") != std::string::npos) {  // verify as double
                        VERIFY(v->is_double() && v->as_double() == uxs::from_string<double>(val));
                        // json format does not support `inf` and `nan`
                        skip_round_trip = val.find_first_of("in") != std::string::npos;
                    } else {
                        uint32_t u32;
                        if (uxs::stoval(val, u32) != 0) {  // verify as 32-bit
                            if (val[0] == '-') {
                                VERIFY(v->is_int() && v->as_int() == static_cast<int32_t>(u32));
                            } else {
                                VERIFY(v->is_uint() && v->as_uint() == u32);
                            }
                        } else {  // verify as 64-bit
                            uint64_t u64;
                            if (uxs::stoval(val, u64) != 0) {
                                if (val[0] == '-') {
                                    VERIFY(v->is_int64() && v->as_int64() == static_cast<int64_t>(u64));
                                } else {
                                    VERIFY(v->is_uint64() && v->as_uint64() == u64);
                                }
                            } else {  // verify as double
                                VERIFY(v->is_double() && v->as_double() == uxs::from_string<double>(val));
                            }
                        }
                    }
                } while (ifile);
            }

            if (!skip_round_trip) {  // round-trip
                uxs::iflatbuf in(data);
                VERIFY(root == uxs::db::json::read(in));
            }

            uxs::sysfile::remove(output_file_name.c_str());

        } catch (const uxs::db::database_error& ex) {
            if (is_valid) { throw std::runtime_error(uxs::format("{}:{}", file_name, ex.what())); }
        }
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "json reader and writer", test_string_json_1);
ADD_TEST_CASE("", "json reader and writer", test_string_json_2);
