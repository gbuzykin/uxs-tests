#include "test_suite.h"
#include "test_types.h"

#include "uxs/cli/parser.h"
#include "uxs/pool_allocator.h"

#include <cctype>
#include <cmath>
#include <cstdio>
#include <map>
#include <set>
#include <vector>

using namespace uxs_test_suite;

/*static*/ int64_t T::not_empty_count = 0;
/*static*/ int64_t T::instance_count = 0;

/*static*/ const TestCase* TestCase::first_avail = nullptr;

const std::vector<std::string_view> g_include_test_category = {"brute"};
const std::vector<std::string_view> g_exclude_test_category = {};

const std::vector<std::string_view> g_include_test_group = {};
const std::vector<std::string_view> g_exclude_test_group = {};

const std::map<std::string_view, std::string_view> g_friendly_text = {
    {"", "Sanity Tests"},
    {"1-bruteforce", "Bruteforce Tests"},
    {"2-perf", "Performance Tests (ns/iter)"},
    {"3-info", "Information"},
};

using TestCategory = std::map<std::string_view, std::vector<const TestCase*>>;

std::map<std::string_view, TestCategory> g_test_table;
unsigned g_proc_num = 1;
std::string g_testdata_path;

std::string uxs_test_suite::report_error(const char* file, int line, const char* msg) {
    char buf[256];
#if defined(_MSC_VER) && _MSC_VER <= 1800
    return std::string(
        buf, std::min(sizeof(buf), static_cast<size_t>(_snprintf_s(buf, sizeof(buf), "%s:%d: %s", file, line, msg))));
#else
    return std::string(
        buf, std::min(sizeof(buf), static_cast<size_t>(std::snprintf(buf, sizeof(buf), "%s:%d: %s", file, line, msg))));
#endif
}

namespace {

void dump_and_destroy_global_pool() {
    auto *desc = uxs::detail::g_global_pool.desc(), *desc0 = desc;
    do {
        size_t free_count = 0;
        auto node = desc->free.next;
        while (node != &desc->free) {
            ++free_count;
            node = node->next;
        }

        if (!dllist_is_empty(&desc->partitions)) {
            size_t node_count_per_partition = desc->node_count_per_partition;
            size_t partition_count = 0;
            size_t total_use_count = 0;
            node = desc->partitions.next;
            do {
                ++partition_count;
                total_use_count += static_cast<uxs::detail::pool_part_hdr_t*>(node)->use_count;
                node = node->next;
            } while (node != &desc->partitions);

            if (partition_count != 1 || free_count + total_use_count + 1 != node_count_per_partition) {
                uxs::stdbuf::out.endl();
                uxs::println("------ \033[0;35mWARNING!\033[0m Unallocated objects of size {} and alignment {}",
                             desc->size_and_alignment & 0xffff, desc->size_and_alignment >> 16);
                uxs::println("-- free_count = {}", free_count);
                uxs::println("-- partition_count = {}", partition_count);
                uxs::println("-- total_use_count = {}", total_use_count);
                uxs::println("-- node_count_per_partition = {}", node_count_per_partition);
            }
        }

        desc = desc->next_pool;
    } while (desc != desc0);

    uxs::detail::g_global_pool.reset(nullptr);
}

void organize_test_cases() {
    auto is_matched = [](std::string_view name, const std::vector<std::string_view>& tbl) {
        for (const auto& sub : tbl) {
            if (name.find(sub) != std::string_view::npos) { return true; }
        }
        return false;
    };

    for (const TestCase* test = TestCase::first_avail; test; test = test->next_avail) {
        if (test->category.empty() /* run sanity tests anyway */ ||
            ((g_include_test_category.empty() || is_matched(test->category, g_include_test_category)) &&
             !is_matched(test->category, g_exclude_test_category) &&
             (g_include_test_group.empty() || is_matched(test->group_name, g_include_test_group)) &&
             !is_matched(test->group_name, g_exclude_test_group))) {
            auto& tests = g_test_table[test->category][test->group_name];
            assert(std::find(tests.begin(), tests.end(), test) == tests.end());
            tests.push_back(test);
        }
    }
}

std::string get_friendly_text(std::string_view name) {
    auto it = g_friendly_text.find(name);
    if (it != g_friendly_text.end()) { return std::string(it->second); }
    return std::string(name);
}

void perform_common_test_cases(std::string_view tbl_name, const TestCategory& category) {
    const std::string title = get_friendly_text(tbl_name);
    uxs::stdbuf::out.endl();
    uxs::println("----------- {} -----------", get_friendly_text(title));
    for (const auto& group : category) {
        uxs::print("-- {} ... ", get_friendly_text(group.first)).flush();
        size_t test_count = group.second.size(), n = 1;
        for (const auto* test : group.second) {
            const std::string n_test_str = uxs::format("{}/{}", n, test_count);
            uxs::stdbuf::out.write(n_test_str).flush();
            VERIFY(test->test() == 0);
            uxs::print("{:\b>{}}", "", n_test_str.size()).flush();
            ++n;
        }
        uxs::stdbuf::out.write("\033[0;32mOK!\033[0m            ").endl();
    }
}

void perform_tabular_test_cases(std::string_view tbl_name, const TestCategory& category) {
    std::set<std::string> column_names;
    std::map<std::string, std::map<std::string, int>> table;
    size_t test_count = category.size(), n = 1;
    const std::string title = get_friendly_text(tbl_name);

    uxs::stdbuf::out.endl();

    // Collect table data :

    for (const auto& group : category) {
        const auto& name = group.first;
        if (name.empty() || group.second.empty()) { continue; }
        const auto* test = group.second.front();

        // Extract column tag <...>
        std::string column_tag;
        auto it = name.begin();
        if (*it == '<') {
            column_tag.reserve(32);
            while (++it != name.end() && *it != '>') { column_tag.push_back(*it); }
            while (++it != name.end() && std::isspace(static_cast<unsigned char>(*it))) {}
        }

        const std::string n_test_str = uxs::format("-- {} ... {}/{}", title, n, test_count);
        uxs::stdbuf::out.write(n_test_str).flush();

        table[std::string(it, name.end())][column_tag] = test->test();
        column_names.emplace(std::move(column_tag));

        uxs::print("{:\b>{}}", "", n_test_str.size()).flush();
        ++n;
    }

    // Print table :

    size_t most_long_name = title.size();
    for (const auto& row : table) {
        if (row.first.size() > most_long_name) { most_long_name = row.first.size(); }
    }

    auto print_hor_line = [most_long_name, &column_names]() {
        uxs::print("+{:->{}}+", "", most_long_name + 2);
        for (const auto& col_name : column_names) {
            uxs::print("{:->{}}+", "", std::max<size_t>(col_name.empty() ? 10 : 18, col_name.size()) + 2);
        }
        uxs::stdbuf::out.endl();
    };

    print_hor_line();

    uxs::print("| {: <{}}|", title, most_long_name + 1);
    for (const auto& col_name : column_names) {
        uxs::print("{: >{}} |", col_name, std::max<size_t>(col_name.empty() ? 10 : 18, col_name.size()) + 1);
    }
    uxs::stdbuf::out.endl();

    print_hor_line();

    for (const auto& row : table) {
        uxs::print("| {: <{}}|", row.first, most_long_name + 1);
        double first_val = 0;
        for (const auto& col_name : column_names) {
            size_t col_width = std::max<size_t>(col_name.empty() ? 10 : 18, col_name.size());
            auto it = row.second.find(col_name);
            if (it != row.second.end()) {
                double val = 0.001 * it->second, ratio = 100;
                if (first_val == 0) {
                    first_val = val;
                } else {
                    ratio = static_cast<int>(std::floor(.5 + static_cast<double>(100.0 * val) / first_val));
                }
                const std::string sval = col_name.empty() ? uxs::format("{:.1f}", val) :
                                                            uxs::format("{:.1f} ({}%)", val, ratio);
                uxs::print("\033[0;{}m{: >{}}\033[0m |", ratio >= 100 ? 32 : 31, sval, col_width + 1);
            } else {
                uxs::print("{: >{}} |", '-', col_width + 1);
            }
        }
        uxs::stdbuf::out.endl();
    }

    print_hor_line();
}

int perform_test_cases() {
    try {
        for (const auto& category : g_test_table) {
            const auto& name = category.first;
            if (name.find("perf") != std::string_view::npos || name.find("info") != std::string_view::npos) {
                perform_tabular_test_cases(name, category.second);
            } else {
                perform_common_test_cases(name, category.second);
            }
        }
    } catch (const std::exception& ex) {
        uxs::stdbuf::out.endl();
        uxs::println("\033[0;31mFAILED!\033[0m ({})            ", ex.what());
        return -1;
    }

    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
#if _ITERATOR_DEBUG_LEVEL != 0
    uxs::stdbuf::out.write("Iterator debugging enabled!").endl();
#endif  // _ITERATOR_DEBUG_LEVEL != 0
#if defined(_DEBUG_REDUCED_BUFFERS)
    uxs::stdbuf::out.write("Using reduced buffers!").endl();
#endif  // defined(_DEBUG_REDUCED_BUFFERS)

    bool show_help = false;
    auto cli = uxs::cli::command(argv[0])
               << uxs::cli::overview("Testing system for UXS library")
               << (uxs::cli::required({"-d"}) & uxs::cli::value("<test data path>", g_testdata_path)) %
                      "Test data directory path."
               << (uxs::cli::option({"-j", "--jobs="}) & uxs::cli::value("<N>", g_proc_num)) %
                      "Maximum job count to use."
               << uxs::cli::option({"-h", "--help"}).set(show_help) % "Display this information.";

    auto parse_result = cli->parse(argc, argv);
    if (show_help) {
        for (auto const* node = parse_result.node; node; node = node->get_parent()) {
            if (node->get_type() == uxs::cli::node_type::command) {
                uxs::stdbuf::out.write(static_cast<const uxs::cli::basic_command<char>&>(*node).make_man_page(true));
                break;
            }
        }
        return 0;
    } else if (parse_result.status != uxs::cli::parsing_status::ok) {
        switch (parse_result.status) {
            case uxs::cli::parsing_status::unknown_option: {
                uxs::println(uxs::stdbuf::err, "\033[0;31merror:\033[0m unknown command line option `{}`",
                             argv[parse_result.arg_count]);
            } break;
            case uxs::cli::parsing_status::invalid_value: {
                if (parse_result.arg_count < argc) {
                    uxs::println(uxs::stdbuf::err, "\033[0;31merror:\033[0m invalid command line argument `{}`",
                                 argv[parse_result.arg_count]);
                } else {
                    uxs::println(uxs::stdbuf::err, "\033[0;31merror:\033[0m expected command line argument after `{}`",
                                 argv[parse_result.arg_count - 1]);
                }
            } break;
            case uxs::cli::parsing_status::unspecified_option: {
                if (g_testdata_path.empty()) {
                    uxs::println(uxs::stdbuf::err, "\033[0;31merror:\033[0m unspecified test data path");
                }
            } break;
            default: break;
        }
        return -1;
    }

    g_proc_num = std::max(1u, g_proc_num);
    if (!g_testdata_path.empty() && g_testdata_path.back() != '/' && g_testdata_path.back() != '\\') {
        g_testdata_path.push_back('/');
    }

    uxs::println("Using up to {} threads", g_proc_num);

    organize_test_cases();
    perform_test_cases();

    if (T::instance_count != 0) {
        uxs::stdbuf::out.endl();
        uxs::println("------ \033[0;35mWARNING!\033[0m Undestroyed T objects");
        uxs::println("-- T::not_empty_count = {}", T::not_empty_count);
        uxs::println("-- T::instance_count = {}", T::instance_count);
    }
    dump_and_destroy_global_pool();

    return 0;
}
