#include "rbtree_tests.h"

bool uxs_test_suite::rbtree_check_balance(uxs::rbtree_node_t* node, int& black, int cur_black,
                                          uxs::rbtree_node_t::color_t color) {
    if ((color != uxs::rbtree_node_t::color_t::black) && (node->color != uxs::rbtree_node_t::color_t::black)) {
        return false;
    }
    if (node->color == uxs::rbtree_node_t::color_t::black) { ++cur_black; }

    if (node->left) {
        if (!rbtree_check_balance(node->left, black, cur_black, node->color)) { return false; }
    } else if (black > 0) {
        if (cur_black != black) { return false; }
    } else {
        black = cur_black;
    }

    if (node->right) {
        if (!rbtree_check_balance(node->right, black, cur_black, node->color)) { return false; }
    } else if (black > 0) {
        if (cur_black != black) { return false; }
    } else {
        black = cur_black;
    }

    return true;
}
