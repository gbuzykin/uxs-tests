#include "test_suite.h"
#include "test_types.h"
#include "util/pool_allocator.h"
#include "util/string_view.h"

#include <cctype>
#include <map>
#include <set>
#include <vector>

using namespace util_test_suite;

/*static*/ int64_t T::not_empty_count = 0;
/*static*/ int64_t T::instance_count = 0;

/*static*/ const TestCase* TestCase::first_avail = nullptr;

const std::vector<std::string_view> g_include_test_category = {"brute"};
const std::vector<std::string_view> g_exclude_test_category = {};

const std::vector<std::string_view> g_include_test_group = {"string"};
const std::vector<std::string_view> g_exclude_test_group = {};

const std::map<std::string_view, std::string_view> g_friendly_text = {
    {"", "Sanity Tests"},
    {"1-bruteforce", "Bruteforce Tests"},
    {"2-perf", "Performance Tests"},
    {"3-info", "Information"},
};

using TestCategory = std::map<std::string_view, std::vector<const TestCase*>>;

std::map<std::string_view, TestCategory> g_test_table;

std::string util_test_suite::report_error(const char* file, int line, const char* msg) {
    std::stringstream ss;
    ss << file << ':' << line << ": " << msg;
    return ss.str();
}

namespace {

void dump_and_destroy_global_pool() {
    auto *desc = util::pool::global_pool_desc(), *desc0 = desc;
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
                total_use_count += static_cast<util::pool::part_hdr_t*>(node)->use_count;
                node = node->next;
            } while (node != &desc->partitions);

            if (partition_count != 1 || free_count + total_use_count + 1 != node_count_per_partition) {
                std::cout << std::endl
                          << "------ WARNING! Unallocated objects of size " << (desc->size_and_alignment & 0xffff)
                          << " and alignment " << (desc->size_and_alignment >> 16) << std::endl;
                std::cout << "-- free_count = " << free_count << std::endl;
                std::cout << "-- partition_count = " << partition_count << std::endl;
                std::cout << "-- total_use_count = " << total_use_count << std::endl;
                std::cout << "-- node_count_per_partition = " << node_count_per_partition << std::endl;
            }
        }

        desc = desc->next_pool;
    } while (desc != desc0);

    util::pool::reset_global_pool();
}

void organize_test_cases() {
    auto is_matched = [](std::string_view name, const std::vector<std::string_view>& tbl) {
        for (const auto& sub : tbl) {
            if (name.find(sub) != std::string_view::npos) { return true; }
        }
        return false;
    };

    for (const TestCase* test = TestCase::first_avail; test; test = test->next_avail) {
        if ((test->category.empty() || g_include_test_category.empty() ||
             is_matched(test->category, g_include_test_category)) &&
            !is_matched(test->category, g_exclude_test_category) &&
            (g_include_test_group.empty() || is_matched(test->group_name, g_include_test_group)) &&
            !is_matched(test->group_name, g_exclude_test_group)) {
            g_test_table[test->category][test->group_name].push_back(test);
        }
    }
}

std::string get_friendly_text(std::string_view name) {
    auto it = g_friendly_text.find(name);
    if (it != g_friendly_text.end()) { return std::string(it->second); }
    return std::string(name);
};

void perform_common_test_cases(std::string_view tbl_name, const TestCategory& category) {
    std::string title = get_friendly_text(tbl_name);
    std::cout << std::endl << "----------- " << get_friendly_text(title) << " -----------" << std::endl;
    for (const auto& group : category) {
        std::cout << "-- " << get_friendly_text(group.first) << " ... " << std::flush;
        size_t test_count = group.second.size(), n = 1;
        for (const auto* test : group.second) {
            std::string n_test_str = std::to_string(n) + '/' + std::to_string(test_count);
            std::cout << n_test_str << std::flush;
            test->test();
            std::cout << std::string(n_test_str.size(), '\b') << std::flush;
            ++n;
        }
        std::cout << "OK!            " << std::endl;
    }
}

void perform_tabular_test_cases(std::string_view tbl_name, const TestCategory& category) {
    std::set<std::string> column_names;
    std::map<std::string, std::map<std::string, int>> table;
    size_t test_count = category.size(), n = 1;
    std::string title = get_friendly_text(tbl_name);

    std::cout << std::endl;

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

        std::string n_test_str = "-- " + title + " ... " + std::to_string(n) + '/' + std::to_string(test_count);
        std::cout << n_test_str << std::flush;

        table[std::string(it, name.end())][column_tag] = test->test();
        column_names.emplace(std::move(column_tag));

        std::cout << std::string(n_test_str.size(), '\b') << std::flush;
        ++n;
    }

    // Print table :

    size_t most_long_name = title.size();
    for (const auto& row : table) {
        if (row.first.size() > most_long_name) { most_long_name = row.first.size(); }
    }

    auto print_hor_line = [most_long_name, &column_names]() {
        std::cout << '+' << std::string(most_long_name + 2, '-') << '+';
        for (const auto& col_name : column_names) {
            size_t col_width = std::max<size_t>(20, col_name.size());
            std::cout << std::string(col_width + 2, '-') << '+';
        }
        std::cout << std::endl;
    };

    std::ios::fmtflags ios_flags(std::cout.flags());

    print_hor_line();

    std::cout << "| " << std::setw(most_long_name + 1) << std::left << title << std::setw(0) << '|';
    for (const auto& col_name : column_names) {
        size_t col_width = std::max<size_t>(20, col_name.size());
        std::cout << std::setw(col_width + 1) << std::right << col_name << std::setw(0) << " |";
    }
    std::cout << std::endl;

    print_hor_line();

    for (const auto& row : table) {
        std::cout << "| " << std::setw(most_long_name + 1) << std::left << row.first << std::setw(0) << '|';
        int first_val = 0;
        for (const auto& col_name : column_names) {
            size_t col_width = std::max<size_t>(20, col_name.size());
            auto it = row.second.find(col_name);
            if (it != row.second.end()) {
                int val = it->second, ratio = 100;
                if (first_val == 0) {
                    first_val = val;
                } else {
                    ratio = static_cast<int>(.5 + static_cast<double>(100 * val) / first_val);
                }
                std::string sval = std::to_string(val) + " (" + std::to_string(ratio) + "%)";
                std::cout << std::setw(col_width + 1) << std::right << sval << std::setw(0) << " |";
            } else {
                std::cout << std::setw(col_width + 1) << std::right << '-' << std::setw(0) << " |";
            }
        }
        std::cout << std::endl;
    }

    print_hor_line();
    std::cout.flags(ios_flags);
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
        std::cout << std::endl << "FAILED! (" << ex.what() << ")            " << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
#if _ITERATOR_DEBUG_LEVEL != 0
    std::cout << "Iterator debugging enabled!" << std::endl;
#endif  // _ITERATOR_DEBUG_LEVEL != 0

    organize_test_cases();
    perform_test_cases();

    if (T::instance_count != 0) {
        std::cout << std::endl << "------ WARNING! Undestroyed T objects" << std::endl;
        std::cout << "-- T::not_empty_count = " << T::not_empty_count << std::endl;
        std::cout << "-- T::instance_count = " << T::instance_count << std::endl;
    }
    dump_and_destroy_global_pool();

    return 0;
}
