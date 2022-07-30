#include "uxs/db/xml.h"

#include "db_value_tests.h"

#include "uxs/db/json.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

extern std::string g_testdata_path;

namespace {

int test_string_xml_1() {
    std::string str{
        "<?xml version='1.1' encoding='UTF-8' ?>\n"
        "<!--- --- -- - blah-blah-blah - -- --- --->\n"
        "<p xml:lang=\"en\">The quick brown fox jumps over the lazy dog.</p>\n"
        "<p xml:lang=\"en-GB\">What colour is it?</p>\n"
        "<p xml:lang=\"en-US\">What color is it?</p>\n"
        "<sp who=\"Faust\" x=\"&amp;&apos;&quot;&lt;&gt;\" />\n"};

    uxs::ostringbuf output;
    output.write(str);

    uxs::db::value root = {{"empty_array", uxs::db::make_array()},
                           {"array_of_one_element", 1},
                           {"array_of_strings", {"\'Faust\'", "\"Philosophie\"", "<Medizin>"}},
                           {"null", nullptr},
                           {"empty_object", uxs::db::make_record()},
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

    uxs::db::xml::writer(output).write(root, "root");

    std::string xml_str = output.str();
    VERIFY(xml_str ==
           "<?xml version='1.1' encoding='UTF-8' ?>\n"
           "<!--- --- -- - blah-blah-blah - -- --- --->\n"
           "<p xml:lang=\"en\">The quick brown fox jumps over the lazy dog.</p>\n"
           "<p xml:lang=\"en-GB\">What colour is it?</p>\n"
           "<p xml:lang=\"en-US\">What color is it?</p>\n"
           "<sp who=\"Faust\" x=\"&amp;&apos;&quot;&lt;&gt;\" />\n"
           "<root>\n"
           "    <array_of_one_element>1</array_of_one_element>\n"
           "    <array_of_strings>&apos;Faust&apos;</array_of_strings>\n"
           "    <array_of_strings>&quot;Philosophie&quot;</array_of_strings>\n"
           "    <array_of_strings>&lt;Medizin&gt;</array_of_strings>\n"
           "    <null>null</null>\n"
           "    <empty_object>\n"
           "    </empty_object>\n"
           "    <object_of_one_element>\n"
           "        <one_element>1</one_element>\n"
           "    </object_of_one_element>\n"
           "    <object>\n"
           "        <array_of_i32>1</array_of_i32>\n"
           "        <array_of_i32>2</array_of_i32>\n"
           "        <array_of_i32>3</array_of_i32>\n"
           "        <bool_val>true</bool_val>\n"
           "        <d_val>12.5646</d_val>\n"
           "        <i32_val>123456</i32_val>\n"
           "        <mixed_array>1</mixed_array>\n"
           "        <mixed_array>2</mixed_array>\n"
           "        <mixed_array>3</mixed_array>\n"
           "        <mixed_array>4</mixed_array>\n"
           "        <mixed_array>5</mixed_array>\n"
           "        <mixed_array>Juristerei</mixed_array>\n"
           "        <mixed_array>\n"
           "            <F>false</F>\n"
           "            <T>true</T>\n"
           "        </mixed_array>\n"
           "        <mixed_array>&amp;Theologie</mixed_array>\n"
           "        <str_val>Habe nun, ach!</str_val>\n"
           "    </object>\n"
           "</root>");

    uxs::istringbuf input(xml_str);
    uxs::db::xml::reader rd(input);
    uxs::db::xml::reader::iterator it{rd}, it_end{};

    VERIFY(it->first == uxs::db::xml::token_t::kPreamble && it->second == "xml");
    VERIFY(it.attributes()["version"] == "1.1");
    VERIFY(it.attributes()["encoding"] == "UTF-8");

    auto read_plane_text = [&it]() {
        std::string txt;
        auto result = *it;
        while (result.first == uxs::db::xml::token_t::kPlainText) { txt += result.second, result = *++it; }
        return txt;
    };

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement && it->second == "p");
    VERIFY(it.attributes()["xml:lang"] == "en");

    ++it;
    VERIFY(read_plane_text() == "The quick brown fox jumps over the lazy dog.");
    VERIFY(it->first == uxs::db::xml::token_t::kEndElement && it->second == "p");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement && it->second == "p");
    VERIFY(it.attributes()["xml:lang"] == "en-GB");

    ++it;
    VERIFY(read_plane_text() == "What colour is it?");
    VERIFY(it->first == uxs::db::xml::token_t::kEndElement && it->second == "p");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement && it->second == "p");
    VERIFY(it.attributes()["xml:lang"] == "en-US");

    ++it;
    VERIFY(read_plane_text() == "What color is it?");
    VERIFY(it->first == uxs::db::xml::token_t::kEndElement && it->second == "p");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement && it->second == "sp");
    VERIFY(it.attributes()["who"] == "Faust");
    VERIFY(it.attributes()["x"] == "&\'\"<>");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kEndElement && it->second == "sp");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement);
    std::string element(it->second);

    VERIFY(rd.read(element) == uxs::db::value{{"array_of_one_element", 1},
                                              {"array_of_strings", {"\'Faust\'", "\"Philosophie\"", "<Medizin>"}},
                                              {"null", nullptr},
                                              {"empty_object", uxs::db::make_record()},
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
    VERIFY(it->first == uxs::db::xml::token_t::kEof);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "xml reader and writer", test_string_xml_1);
