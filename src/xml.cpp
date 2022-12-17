#include "uxs/db/xml.h"

#include "db_value_tests.h"

#include "uxs/db/json.h"
#include "uxs/io/filebuf.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

extern std::string g_testdata_path;

namespace {

int test_string_xml_1() {
    uxs::filebuf ifile((g_testdata_path + "xml/test-001.xml").c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::kEnd));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(uxs::as_span(&txt[0], sz)));

    uxs::istringbuf input(txt);
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
    VERIFY(it->first == uxs::db::xml::token_t::kEof);

    return 0;
}

int test_string_xml_2() {
    uxs::filebuf ifile((g_testdata_path + "xml/test-002.xml").c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::kEnd));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(uxs::as_span(&txt[0], sz)));

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

    uxs::ostringbuf output;
    output.write("<?xml version='1.1' encoding='UTF-8' ?>\n");
    uxs::db::xml::writer(output).write(root, "root");
    VERIFY(output.str() == txt);

    uxs::istringbuf input(txt);
    uxs::db::xml::reader rd(input);
    uxs::db::xml::reader::iterator it{rd}, it_end{};

    VERIFY(it->first == uxs::db::xml::token_t::kPreamble && it->second == "xml");
    VERIFY(it.attributes()["version"] == "1.1");
    VERIFY(it.attributes()["encoding"] == "UTF-8");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kPlainText && it->second == "\n");

    ++it;
    VERIFY(it->first == uxs::db::xml::token_t::kStartElement);
    std::string element(it->second);

    VERIFY(element == "root");
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
ADD_TEST_CASE("", "xml reader and writer", test_string_xml_2);
