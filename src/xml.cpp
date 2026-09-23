#include "test_suite.h"

#include <uxs/db/value.h>
#include <uxs/db/xml.h>
#include <uxs/io/filebuf.h>
#include <uxs/io/iflatbuf.h>
#include <uxs/io/iostate.h>
#include <uxs/io/oflatbuf.h>
#include <uxs/span.h>

#include <string_view>

extern std::string g_testdata_path;

static_assert(std::is_same<decltype(uxs::db::xml::get<0>(std::declval<uxs::db::xml::parser<char>&>())),
                           uxs::db::xml::token_t>::value,
              "");
static_assert(
    std::is_same<decltype(uxs::db::xml::get<1>(std::declval<uxs::db::xml::parser<char>&>())), std::string_view>::value,
    "");
static_assert(
    std::is_same<decltype(uxs::db::xml::get<2>(std::declval<uxs::db::xml::parser<char>&>())), uxs::db::value&>::value,
    "");
static_assert(std::is_same<decltype(uxs::db::xml::get<2>(std::declval<const uxs::db::xml::parser<char>&>())),
                           const uxs::db::value&>::value,
              "");
static_assert(
    std::is_same<decltype(uxs::db::xml::get<2>(std::declval<uxs::db::xml::parser<char>&&>())), uxs::db::value&&>::value,
    "");
static_assert(std::is_same<decltype(uxs::db::xml::get<2>(std::declval<const uxs::db::xml::parser<char>&&>())),
                           const uxs::db::value&>::value,
              "");

namespace {

int test_xml_1() {
    uxs::filebuf ifile((g_testdata_path + "xml/test-001.xml").c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(est::as_span(&txt[0], sz)));

#if __cplusplus >= 201703L
    {
        uxs::iflatbuf input(txt);
        uxs::db::xml::parser<char> rd(input);
        for (auto&& [tt, text, attrs] : rd) {
            (void)tt;
            (void)text;
            (void)attrs;
        }
    }
#endif

    uxs::iflatbuf input(txt);
    uxs::db::xml::parser<char> rd(input);
    uxs::db::xml::parser_iterator<char> it{rd}, it_end{};

    VERIFY(it->token_type() == uxs::db::xml::token_t::preamble && it->name() == "xml");
    VERIFY(it->attributes().value<std::string_view>("version") == "1.1");
    VERIFY(it->attributes().value<std::string_view>("encoding") == "UTF-8");

    auto read_plane_text = [&it]() {
        std::string txt;
        for (; it->token_type() == uxs::db::xml::token_t::plain_text; ++it) { txt += it->text(); }
        return txt;
    };

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::start_element && it->name() == "p");
    VERIFY(it->attributes().value<std::string_view>("xml:lang") == "en");

    ++it;
    VERIFY(read_plane_text() == "The quick brown fox jumps over the lazy dog.");
    VERIFY(it->token_type() == uxs::db::xml::token_t::end_element && it->name() == "p");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::start_element && it->name() == "p");
    VERIFY(it->attributes().value<std::string_view>("xml:lang") == "en-GB");

    ++it;
    VERIFY(read_plane_text() == "What colour is it?");
    VERIFY(it->token_type() == uxs::db::xml::token_t::end_element && it->name() == "p");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::start_element && it->name() == "p");
    VERIFY(it->attributes().value<std::string_view>("xml:lang") == "en-US");

    ++it;
    VERIFY(read_plane_text() == "What color is it?");
    VERIFY(it->token_type() == uxs::db::xml::token_t::end_element && it->name() == "p");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::start_element && it->name() == "sp");
    VERIFY(it->attributes().value<std::string_view>("who") == "Faust");
    VERIFY(it->attributes().value<std::string_view>("attr") == "asdf 12234 ggg   ");
    VERIFY(it->attributes().value<std::string_view>("x") == "&\'\"<>");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::end_element && it->name() == "sp");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it == it_end);

    return 0;
}

int test_xml_2() {
    uxs::filebuf ifile((g_testdata_path + "xml/test-002.xml").c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(est::as_span(&txt[0], sz)));

    uxs::db::value root = {{"empty_array", uxs::db::make_array()},
                           {"array_of_one_element", {1}},
                           {"array_of_strings", {"\'Faust\'", "\"Philosophie\"", "<Medizin>"}},
                           {"null", nullptr},
                           {"empty_object", uxs::db::make_object()},
                           {"object_of_one_element", {{"one_element", 1}}},
                           {"object",
                            {{"array_of_i32", {1, 2, 3}},
                             {"bool_val", true},
                             {"d_val", 12.5646},
                             {"i32_val", 123456},
                             {"mixed_array",
                              {uxs::db::make_array(),
                               {1, 2, {3, 4, 5}},
                               "Juristerei",
                               {
                                   {"F", false},
                                   {"T", true},
                               },
                               "&Theologie"}},
                             {"str_val", "Habe nun, ach!"}}}};

    uxs::oflatbuf output;
    uxs::print(output, "<?xml version='1.1' encoding='UTF-8' ?>\n");
    uxs::db::xml::write(output, root, "root");
    VERIFY(std::string_view(output.data(), output.size()) == txt);

    uxs::iflatbuf input(txt);
    uxs::db::xml::parser<char> rd(input);
    uxs::db::xml::parser_iterator<char> it{rd}, it_end{};

    VERIFY(it->token_type() == uxs::db::xml::token_t::preamble && it->name() == "xml");
    VERIFY(it->attributes().value<std::string_view>("version") == "1.1");
    VERIFY(it->attributes().value<std::string_view>("encoding") == "UTF-8");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::plain_text && it->text() == "\n");

    ++it;
    VERIFY(it->token_type() == uxs::db::xml::token_t::start_element);
    std::string element(it->name());

    VERIFY(element == "root");

    VERIFY(rd.parse(element) == uxs::db::value{{"array_of_one_element", 1},
                                               {"array_of_strings", {"\'Faust\'", "\"Philosophie\"", "<Medizin>"}},
                                               {"null", nullptr},
                                               {"empty_object", uxs::db::make_object()},
                                               {"object_of_one_element", {{"one_element", 1}}},
                                               {"object",
                                                {{"array_of_i32", {1, 2, 3}},
                                                 {"bool_val", true},
                                                 {"d_val", 12.5646},
                                                 {"i32_val", 123456},
                                                 {"mixed_array",
                                                  {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   "Juristerei",
                                                   {
                                                       {"F", false},
                                                       {"T", true},
                                                   },
                                                   "&Theologie"}},
                                                 {"str_val", "Habe nun, ach!"}}}});

    ++it;
    VERIFY(it == it_end);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "xml reader and writer", test_xml_1);
ADD_TEST_CASE("", "xml reader and writer", test_xml_2);
