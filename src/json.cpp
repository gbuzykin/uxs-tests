#include "uxs/db/json.h"

#include "db_value_tests.h"
#include "thread_pool.h"

#include "uxs/io/filebuf.h"
#include "uxs/io/iflatbuf.h"
#include "uxs/io/oflatbuf.h"
#include "uxs/string_alg.h"

#include <random>
#include <vector>

#if WIN32
#    include <windows.h>
#else
#    include <dirent.h>
#    include <sys/types.h>
#endif

#ifdef min
#    undef min
#endif

#ifdef max
#    undef max
#endif

using namespace uxs_test_suite;

extern std::string g_testdata_path;
extern unsigned g_proc_num;

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
    VERIFY(!(root = uxs::db::json::read(input)).is_null());
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

    VERIFY(uxs::format("{:.2a}", root) == txt);
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

            std::string data = uxs::format("{}", root);

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
                        if (uxs::from_string(val, u32) != 0) {  // verify as 32-bit
                            if (val[0] == '-') {
                                VERIFY(v->is_int() && v->as_int() == static_cast<int32_t>(u32));
                            } else {
                                VERIFY(v->is_uint() && v->as_uint() == u32);
                            }
                        } else {  // verify as 64-bit
                            uint64_t u64;
                            if (uxs::from_string(val, u64) != 0) {
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

uxs::db::value gen_random_database(std::default_random_engine& generator, int level = 0) {
    auto get_string = [&generator](size_t max_len) {
        const size_t sz = std::uniform_int_distribution<size_t>{0, max_len}(generator);
        std::string s;
        s.reserve(sz);
        for (size_t n = 0; n != sz; ++n) {
            const bool is_special = std::uniform_int_distribution<int>{0, 99}(generator) == 0;
            s += is_special ? static_cast<char>(std::uniform_int_distribution<int>{1, 32}(generator)) :
                              static_cast<char>(std::uniform_int_distribution<int>{32, 126}(generator));
        }
        return s;
    };

    const int case_min = level == 0 ? 8 : 0;
    const int case_max = level < 10 ? 9 : 7;

    switch (std::uniform_int_distribution<int>{case_min, case_max}(generator)) {
        case 0: return nullptr;
        case 1: return std::uniform_int_distribution<int>{0, 1}(generator) == 1;
        case 2: {
            return std::uniform_int_distribution<int32_t>{std::numeric_limits<int32_t>::min(),
                                                          std::numeric_limits<int32_t>::max()}(generator);
        } break;
        case 3: {
            return std::uniform_int_distribution<uint32_t>{
                static_cast<uint32_t>(std::numeric_limits<int32_t>::max()) + 1,
                std::numeric_limits<uint32_t>::max()}(generator);
        } break;
        case 4: {
            if (std::uniform_int_distribution<int>{0, 1}(generator) == 1) {
                return std::uniform_int_distribution<int64_t>{
                    static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1,
                    std::numeric_limits<int64_t>::max()}(generator);
            } else {
                return std::uniform_int_distribution<int64_t>{
                    std::numeric_limits<int64_t>::min(),
                    static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1,
                }(generator);
            }
        } break;
        case 5: {
            return std::uniform_int_distribution<uint64_t>{
                static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1,
                std::numeric_limits<uint64_t>::max()}(generator);
        } break;
        case 6: return std::uniform_real_distribution<double>{-1.e+100, 1.e+100}(generator);
        case 7: return uxs::db::value{get_string(50)};
        case 8: {
            const size_t sz = std::uniform_int_distribution<size_t>{0, 20}(generator);
            uxs::db::value v;
            v.reserve(sz);
            for (size_t n = 0; n != sz; ++n) { v.emplace_back(gen_random_database(generator, level + 1)); }
            if (sz == 0) { v = uxs::db::make_array(); }
            return v;
        } break;
        case 9: {
            const size_t sz = std::uniform_int_distribution<size_t>{0, 20}(generator);
            uxs::db::value v;
            for (size_t n = 0; n != sz; ++n) { v.emplace(get_string(20), gen_random_database(generator, level + 1)); }
            if (sz == 0) { v = uxs::db::make_record(); }
            return v;
        } break;
        default: return {};
    }
}

int test_json_bruteforce() {
#if defined(NDEBUG)
    const int iter_count = 100000;
#else   // defined(NDEBUG)
    const int iter_count = 10000;
#endif  // defined(NDEBUG)

    auto test_func = []() {
        std::random_device rd;
        std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
        std::default_random_engine generator(seed);
        auto v = gen_random_database(generator);
        auto s = uxs::format("{}", v);
        uxs::iflatbuf is(s);
        return uxs::db::json::read(is) == v;
    };

    auto results = est::make_unique<bool[]>(g_proc_num);

    using work_item_t = decltype(uxs::make_work_item(test_func));
    uxs::test_thread_pool thread_pool(g_proc_num - 1);
    not_relocatable_vector<work_item_t> work_items(g_proc_num - 1);

    for (int iter = 0, perc0 = 0; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        for (unsigned proc = 0; proc < g_proc_num; ++proc, ++iter) {
            results[proc] = false;
            if (proc < g_proc_num - 1) {
                thread_pool.queue(work_items.emplace_back(test_func));
            } else {
                results[proc] = test_func();
            }
        }

        for (unsigned proc = 0; proc < g_proc_num - 1; ++proc) { results[proc] = work_items[proc].get_future().get(); }
        work_items.clear();

        for (unsigned proc = 0; proc < g_proc_num; ++proc) { VERIFY(results[proc]); }
    }

    return 0;
}

int test_json_bruteforce_file() {
    std::default_random_engine generator;

    const int iter_count = 10000;

    auto fname = g_testdata_path + "json/test-file.json";

    for (int iter = 0, perc0 = 0; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
            perc0 = perc;
        }

        auto v = gen_random_database(generator);

        {
            uxs::filebuf ofile(fname.c_str(), "w");
            VERIFY(ofile);
            uxs::print(ofile, "{:.2tA}", v);
        }

        uxs::filebuf ifile(fname.c_str(), "r");
        VERIFY(uxs::db::json::read(ifile) == v);
    }

    uxs::sysfile::remove(fname.c_str());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "json reader and writer", test_string_json_1);
ADD_TEST_CASE("", "json reader and writer", test_string_json_2);
ADD_TEST_CASE("1-bruteforce", "json reader and writer", test_json_bruteforce);
ADD_TEST_CASE("1-bruteforce", "json reader and writer", test_json_bruteforce_file);
