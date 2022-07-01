#include "uxs/db/json.h"

#include "db_value_tests.h"

#include "uxs/io/filebuf.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

extern std::string g_testdata_path;

namespace {

int test_string_json_1() {
    std::string str{
        "{\n"
        "    \"array_of_strings\": [\"1\", \"2\", \"3\"],\n"
        "    \"null\": null,\n"
        "    \"object\": {\n"
        "        \"array_of_i32\": [1, 2, 3],\n"
        "        \"bool_val\": true,\n"
        "        \"d_val\": 12.5646,\n"
        "        \"i32_val\": 123456,\n"
        "        \"mixed_array\": [1, {\n"
        "            \"F\": false,\n"
        "            \"T\": true\n"
        "        }, \"3\"],\n"
        "        \"str_val\": \"hello\"\n"
        "    }\n"
        "}"};

    uxs::istringbuf input(str);
    uxs::db::value root;
    VERIFY(root = uxs::db::json::reader(input).read());
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

    uxs::ostringbuf output;
    uxs::db::json::writer(output).write(root);
    VERIFY(output.str() == str);
    return 0;
}

int test_string_json_2() {
    static const std::string_view file_names[] = {
        "fail2.json",
        "fail3.json",
        "fail4.json",
        "fail5.json",
        "fail6.json",
        "fail9.json",
        "fail11.json",
        "fail12.json",
        "fail13.json",
        "fail14.json",
        "fail15.json",
        "fail16.json",
        "fail17.json",
        "fail19.json",
        "fail20.json",
        "fail21.json",
        "fail22.json",
        "fail23.json",
        "fail24.json",
        "fail26.json",
        "fail27.json",
        "fail28.json",
        "fail29.json",
        "fail30.json",
        "fail31.json",
        "fail32.json",
        "fail33.json",
        "pass1.json",
        "pass2.json",
        "pass3.json",
        "fail_invalid_quote.json",
        "fail_test_array_01.json",
        "fail_test_array_02.json",
        "fail_test_object_01.json",
        "legacy_test_array_01.json",
        "legacy_test_array_02.json",
        "legacy_test_array_03.json",
        "legacy_test_array_04.json",
        "legacy_test_array_05.json",
        "legacy_test_array_06.json",
        "legacy_test_array_07.json",
        "legacy_test_basic_01.json",
        "legacy_test_basic_02.json",
        "legacy_test_basic_03.json",
        "legacy_test_basic_04.json",
        "legacy_test_basic_05.json",
        "legacy_test_basic_06.json",
        "legacy_test_basic_07.json",
        "legacy_test_basic_08.json",
        "legacy_test_basic_09.json",
        "legacy_test_comment_00.json",
        "legacy_test_comment_01.json",
        "legacy_test_comment_02.json",
        "legacy_test_complex_01.json",
        "legacy_test_integer_01.json",
        "legacy_test_integer_02.json",
        "legacy_test_integer_03.json",
        "legacy_test_integer_04.json",
        "legacy_test_integer_05.json",
        "legacy_test_integer_06_64bits.json",
        "legacy_test_integer_07_64bits.json",
        "legacy_test_integer_08_64bits.json",
        "legacy_test_large_01.json",
        "legacy_test_object_01.json",
        "legacy_test_object_02.json",
        "legacy_test_object_03.json",
        "legacy_test_object_04.json",
        "legacy_test_real_01.json",
        "legacy_test_real_02.json",
        "legacy_test_real_03.json",
        "legacy_test_real_04.json",
        "legacy_test_real_05.json",
        "legacy_test_real_06.json",
        "legacy_test_real_07.json",
        "legacy_test_real_08.json",
        "legacy_test_real_09.json",
        "legacy_test_real_10.json",
        "legacy_test_real_11.json",
        "legacy_test_real_12.json",
        "legacy_test_real_13.json",
        "legacy_test_string_01.json",
        "legacy_test_string_02.json",
        "legacy_test_string_03.json",
        "legacy_test_string_04.json",
        "legacy_test_string_05.json",
        "legacy_test_string_unicode_01.json",
        "legacy_test_string_unicode_02.json",
        "legacy_test_string_unicode_03.json",
        "legacy_test_string_unicode_04.json",
        "legacy_test_string_unicode_05.json",
        "ext-invalid-0000.json",
        "ext-invalid-0001.json",
        "ext-invalid-0002.json",
        "ext-valid-0000.json",
        "ext-valid-0001.json",
        "ext-valid-0002.json",
        "ext-valid-0003.json",
        "invalid-0000.json",
        "invalid-0001.json",
        "invalid-0002.json",
        "invalid-0003.json",
        "invalid-0004.json",
        "invalid-0005.json",
        "invalid-0006.json",
        "invalid-0007.json",
        "invalid-0008.json",
        "invalid-0009.json",
        "invalid-0010.json",
        "valid-0000.json",
        "valid-0001.json",
        "valid-0002.json",
        "valid-0003.json",
        "valid-0004.json",
        "valid-0005.json",
        "valid-0006.json",
        "valid-0007.json",
        "valid-0008.json",
        "valid-0009.json",
        "valid-0010.json",
        "valid-0011.json",
        "valid-0012.json",
        "valid-0013.json",
        "valid-0014.json",
        "valid-0015.json",
    };

    for (std::string_view file_name : file_names) {
        uxs::db::value root;

        bool is_valid = file_name.find("invalid") == std::string::npos && file_name.find("fail") == std::string::npos;

        std::string full_file_name = g_testdata_path;
        full_file_name += "json/";
        full_file_name += file_name;

        try {
            {  // read
                uxs::filebuf ifile(full_file_name.c_str(), "r");
                VERIFY(ifile);
                root = uxs::db::json::reader(ifile).read();
            }

            VERIFY(is_valid);

            std::string data;
            {  // write
                uxs::ostringbuf out;
                uxs::db::json::writer(out).write(root);
                data = out.str();
            }

            bool skip_round_trip = false;

            {  // check expected
                std::string expected_full_file_name = full_file_name;
                auto pos = expected_full_file_name.rfind('.');
                if (pos != std::string::npos) { expected_full_file_name.resize(pos); }
                expected_full_file_name += ".expected";

                uxs::filebuf ifile(expected_full_file_name.c_str(), "r");
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
                uxs::istringbuf in(data);
                VERIFY(root == uxs::db::json::reader(in).read());
            }

            {
                std::string output_full_file_name = full_file_name + ".out";
                uxs::filebuf ofile(output_full_file_name.c_str(), "w");
                if (!ofile) { return -1; }
                ofile.write(data);
            }
        } catch (const uxs::db::exception& ex) {
            if (is_valid) { throw std::runtime_error(uxs::format("{}:{}", full_file_name, ex.what())); }
        }
    }
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "json reader and writer", test_string_json_1);
ADD_TEST_CASE("", "json reader and writer", test_string_json_2);
