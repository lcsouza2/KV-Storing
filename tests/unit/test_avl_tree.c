#include "logging.h"
#include "test_utils.h"
#include "avl_tree.h"
#include <string.h>
#include <stdlib.h>

int test_tree_insertion() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, "30", "thirty");
    insert_avl_tree(tree, "20", "twenty");
    insert_avl_tree(tree, "40", "forty");
    insert_avl_tree(tree, "10", "ten");
    insert_avl_tree(tree, "25", "twenty-five");

    ASSERT_TEST(tree->root != NULL, "Root should not be NULL after insertions.");
    ASSERT_TEST(strcmp(tree->root->value, "thirty") == 0, "Root value should be 'thirty'.");
    ASSERT_TEST(strcmp(tree->root->left->value, "twenty") == 0, "Left child of root should be 'twenty'.");
    ASSERT_TEST(strcmp(tree->root->right->value, "forty") == 0, "Right child of root should be 'forty'.");
    ASSERT_TEST(strcmp(tree->root->left->left->value, "ten") == 0, "Left child of left child of root should be 'ten'.");
    ASSERT_TEST(strcmp(tree->root->left->right->value, "twenty-five") == 0, "Right child of left child of root should be 'twenty-five'.");

    free_avl_tree(tree);
    info("test_tree_insertion passed.");
    return 0;
}

int test_tree_left_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, "10", "ten");
    insert_avl_tree(tree, "20", "twenty");
    insert_avl_tree(tree, "30", "thirty"); // This should trigger a left rotation

    ASSERT_TEST(strcmp(tree->root->value, "twenty") == 0, "Root value should be 'twenty' after left rotation.");
    ASSERT_TEST(strcmp(tree->root->left->value, "ten") == 0, "Left child of root should be 'ten' after left rotation.");
    ASSERT_TEST(strcmp(tree->root->right->value, "thirty") == 0, "Right child of root should be 'thirty' after left rotation.");

    free_avl_tree(tree);
    info("test_tree_left_rotation passed.");
    return 0;
}

int test_tree_right_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, "30", "thirty");
    insert_avl_tree(tree, "20", "twenty");
    insert_avl_tree(tree, "10", "ten"); // This should trigger a right rotation

    ASSERT_TEST(strcmp(tree->root->value, "twenty") == 0, "Root value should be 'twenty' after right rotation.");
    ASSERT_TEST(strcmp(tree->root->left->value, "ten") == 0, "Left child of root should be 'ten' after right rotation.");
    ASSERT_TEST(strcmp(tree->root->right->value, "thirty") == 0, "Right child of root should be 'thirty' after right rotation.");

    free_avl_tree(tree);
    info("test_tree_right_rotation passed.");
    return 0;
}

int test_tree_double_left_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, "10", "ten");
    insert_avl_tree(tree, "30", "thirty");
    insert_avl_tree(tree, "20", "twenty"); // This should trigger a double left rotation

    ASSERT_TEST(strcmp(tree->root->value, "twenty") == 0, "Root value should be 'twenty' after double left rotation.");
    ASSERT_TEST(strcmp(tree->root->left->value, "ten") == 0, "Left child of root should be 'ten' after double left rotation.");
    ASSERT_TEST(strcmp(tree->root->right->value, "thirty") == 0, "Right child of root should be 'thirty' after double left rotation.");

    free_avl_tree(tree);
    info("test_tree_double_left_rotation passed.");
    return 0;
}

int test_tree_double_right_rotation() {
    AVLTree *tree = create_avl_tree();

    insert_avl_tree(tree, "30", "thirty");
    insert_avl_tree(tree, "10", "ten");
    insert_avl_tree(tree, "20", "twenty"); // This should trigger a double right rotation

    ASSERT_TEST(strcmp(tree->root->value, "twenty") == 0, "Root value should be 'twenty' after double right rotation.");
    ASSERT_TEST(strcmp(tree->root->left->value, "ten") == 0, "Left child of root should be 'ten' after double right rotation.");
    ASSERT_TEST(strcmp(tree->root->right->value, "thirty") == 0, "Right child of root should be 'thirty' after double right rotation.");

    free_avl_tree(tree);
    info("test_tree_double_right_rotation passed.");
    return 0;
}