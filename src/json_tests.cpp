#include "test_suite.h"
#include "util/db/json.h"
#include "util/io/istringbuf.h"
#include "util/io/ostringbuf.h"

namespace {

int test_string_json_0() {
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

    util::istringbuf input(str);
    util::db::value root;
    VERIFY(util::db::json::reader(input).read(root) == 0);
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

    util::ostringbuf output;
    util::db::json::writer(output).write(root);
    VERIFY(output.str() == str);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "json reader and writer", test_string_json_0);
