#include "logging.h"
#include "test_utils.h"
#include "avl_tree.h"
#include <stdlib.h>

int test_tree_insertion() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, 30);
    insert_avl_tree(tree, 20);
    insert_avl_tree(tree, 40);
    insert_avl_tree(tree, 10);
    insert_avl_tree(tree, 25);

    ASSERT_TEST(tree->root != NULL, "Root should not be NULL after insertions.");
    ASSERT_TEST(tree->root->value == 30, "Root value should be 30.");
    ASSERT_TEST(tree->root->left->value == 20, "Left child of root should be 20.");
    ASSERT_TEST(tree->root->right->value == 40  , "Right child of root should be 40.");
    ASSERT_TEST(tree->root->left->left->value == 10, "Left child of left child of root should be 10.");
    ASSERT_TEST(tree->root->left->right->value == 25, "Right child of left child of root should be 25.");

    free_avl_tree(tree);
    info("test_tree_insertion passed.");
    return 0;
}

int test_tree_left_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, 10);
    insert_avl_tree(tree, 20);
    insert_avl_tree(tree, 30); // This should trigger a left rotation

    ASSERT_TEST(tree->root->value == 20, "Root value should be 20 after left rotation.");
    ASSERT_TEST(tree->root->left->value == 10, "Left child of root should be 10 after left rotation.");
    ASSERT_TEST(tree->root->right->value == 30, "Right child of root should be 30 after left rotation.");

    free_avl_tree(tree);
    info("test_tree_left_rotation passed.");
    return 0;
}

int test_tree_right_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, 30);
    insert_avl_tree(tree, 20);
    insert_avl_tree(tree, 10); // This should trigger a right rotation

    ASSERT_TEST(tree->root->value == 20, "Root value should be 20 after right rotation.");
    ASSERT_TEST(tree->root->left->value == 10, "Left child of root should be 10 after right rotation.");
    ASSERT_TEST(tree->root->right->value == 30, "Right child of root should be 30 after right rotation.");

    free_avl_tree(tree);
    info("test_tree_right_rotation passed.");
    return 0;
}

int test_tree_double_left_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, 10);
    insert_avl_tree(tree, 30);
    insert_avl_tree(tree, 20); // This should trigger a double left rotation

    ASSERT_TEST(tree->root->value == 20, "Root value should be 20 after double left rotation.");
    ASSERT_TEST(tree->root->left->value == 10, "Left child of root should be 10 after double left rotation.");
    ASSERT_TEST(tree->root->right->value == 30, "Right child of root should be 30 after double left rotation.");

    free_avl_tree(tree);
    info("test_tree_double_left_rotation passed.");
    return 0;
}

int test_tree_double_right_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, 30);
    insert_avl_tree(tree, 10);
    insert_avl_tree(tree, 20); // This should trigger a double right rotation

    ASSERT_TEST(tree->root->value == 20, "Root value should be 20 after double right rotation.");
    ASSERT_TEST(tree->root->left->value == 10, "Left child of root should be 10 after double right rotation.");
    ASSERT_TEST(tree->root->right->value == 30, "Right child of root should be 30 after double right rotation.");

    free_avl_tree(tree);
    info("test_tree_double_right_rotation passed.");
    return 0;
}