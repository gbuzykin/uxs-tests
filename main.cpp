#include "test_suite.h"
#include "test_types.h"
#include "util/pool_allocator.h"
#include "util/string_view.h"

#include <map>
#include <set>
#include <vector>

using namespace util_test_suite;

/*static*/ int64_t T::not_empty_count = 0;
/*static*/ int64_t T::instance_count = 0;
/*static*/ int64_t T::compare_less_count = 0;

/*static*/ int64_t T_NothrowDefaultCopyMove::not_empty_count = 0;
/*static*/ int64_t T_NothrowDefaultCopyMove::instance_count = 0;
/*static*/ int64_t T_NothrowDefaultCopyMove::compare_less_count = 0;

/*static*/ const TestCase* TestCase::first_avail = nullptr;

const std::set<std::string> g_include_test_cat = {};
const std::set<std::string> g_exclude_test_cat = {};

const std::set<std::string> g_include_test_group = {};
const std::set<std::string> g_exclude_test_group = {};

const std::map<std::string_view, std::string> g_friendly_text = {
    {"", "Sanity tests"},
    {"1-bruteforce", "Bruteforce tests"},
    {"2-perf", "Performance tests"},
    {"3-info", "Information"},
};

std::map<std::string_view, std::map<std::string_view, std::vector<const TestCase*>>> g_test_table;

std::string util_test_suite::report_error(const char* file, int line, const char* msg) {
    std::stringstream ss;
    ss << file << "(" << line << "): " << msg;
    return ss.str();
}

namespace {

void dump_and_destroy_global_pools() {
    for (auto item = util::pool_base::global_pool_list(); item; item = item->next) {
        auto desc = item->pool_desc();

        size_t free_count = 0;
        auto node = desc->free.next;
        while (node != &desc->free) {
            ++free_count;
            node = node->next;
        }

        size_t node_count_per_partition = desc->node_count_per_partition;
        size_t partition_count = 0;
        size_t total_use_count = 0;
        node = desc->partitions.next;
        while (node != &desc->partitions) {
            ++partition_count;
            total_use_count += static_cast<util::pool_base::part_hdr_t*>(node)->use_count;
            node = node->next;
        }

        std::cout << std::endl;
        std::cout << "------------------------ global pool for size " << (desc->size_and_alignment & 0xffff)
                  << " and alignment " << (desc->size_and_alignment >> 16) << std::endl;
        std::cout << "free_count = " << free_count << std::endl;
        std::cout << "partition_count = " << partition_count << std::endl;
        std::cout << "total_use_count = " << total_use_count << std::endl;
        std::cout << "node_count_per_partition = " << node_count_per_partition << " (" << free_count + total_use_count
                  << ")" << std::endl;

        item->reset();
    }
}

void organize_test_cases() {
    for (const TestCase* test = TestCase::first_avail; test; test = test->next_avail) {
        if ((g_include_test_cat.empty() ||  //
             g_include_test_cat.find(test->category) != g_include_test_cat.end()) &&
            g_exclude_test_cat.find(test->category) == g_exclude_test_cat.end() &&
            (g_include_test_group.empty() ||  //
             g_include_test_group.find(test->group_name) != g_include_test_group.end()) &&
            g_exclude_test_group.find(test->group_name) == g_exclude_test_group.end()) {
            g_test_table[test->category][test->group_name].push_back(test);
        }
    }
}

int perform_test_cases() {
    try {
        auto get_friendly_text = [](std::string_view name) {
            auto it = g_friendly_text.find(name);
            if (it != g_friendly_text.end()) { return std::string_view(it->second); }
            return name;
        };

        for (const auto& cat : g_test_table) {
            std::cout << std::endl
                      << "----------- " << get_friendly_text(cat.first).data() << " -----------" << std::endl;
            for (const auto& group : cat.second) {
                std::cout << "-- " << get_friendly_text(group.first).data() << " ... " << std::flush;
                for (const auto* test : group.second) { test->test(); }
                std::cout << "OK!   " << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cout << "FAILED! (" << ex.what() << ")" << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
#if _ITERATOR_DEBUG_LEVEL != 0
    std::cout << "Iterator debuging enabled!" << std::endl;
#endif

    organize_test_cases();
    perform_test_cases();

    std::cout << std::endl;
    std::cout << "T::not_empty_count = " << T::not_empty_count << std::endl;
    std::cout << "T::instance_count = " << T::instance_count << std::endl;
    std::cout << "T::compare_less_count = " << T::compare_less_count << std::endl;
    dump_and_destroy_global_pools();

    return 0;
}
