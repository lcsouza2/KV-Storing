#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "logging.h"
#include "avl_tree.h"

// LOW LEVEL FUNCTIONS =============================
int _get_height(AVLNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

AVLNode *_create_node(int value) {
    AVLNode *node = malloc(sizeof(AVLNode));
    if (!node) {
        debug("Failed to allocate memory for AVLNode.");
        return NULL;
    }
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLNode *_simple_left_rotate(AVLNode *node) {
    AVLNode *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    int l_height = _get_height(node->left);
    int r_height = _get_height(node->right);
    int new_root_r_height = _get_height(new_root->right);
    node->height = 1 + (l_height > r_height ? l_height : r_height);
    new_root->height = 1 + (l_height > new_root_r_height ? l_height : new_root_r_height);

    return new_root;
}

AVLNode *_simple_right_rotate(AVLNode *node) {
    AVLNode *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;

    int l_height = _get_height(node->left);
    int r_height = _get_height(node->right);
    int new_root_l_height = _get_height(new_root->left);
    node->height = 1 + (l_height > r_height ? l_height : r_height);
    new_root->height = 1 + (new_root_l_height > r_height ? new_root_l_height : r_height);

    return new_root;
}

AVLNode *_double_left_rotate(AVLNode *node) {
    node->right = _simple_right_rotate(node->right);
    return _simple_left_rotate(node);
}

AVLNode *_double_right_rotate(AVLNode *node) {
    node->left = _simple_left_rotate(node->left);
    return _simple_right_rotate(node);
}

AVLNode *_insert(AVLNode *node, int value) {
    if (node == NULL) return _create_node(value);

    if (value > node->value) node->right = _insert(node->right, value);
    else if (value < node->value) node->left = _insert(node->left, value);
    else {
        debug("Value already exists in the AVLNode.");
        return node;
    }

    int l_height = _get_height(node->left);
    int r_height = _get_height(node->right);

    node->height = 1 + (l_height > r_height ? l_height : r_height);
    int balance_factor = _get_height(node->left) - _get_height(node->right);
    if (balance_factor > 1 && value < node->left->value) return _simple_right_rotate(node);
    if (balance_factor < -1 && value > node->right->value) return _simple_left_rotate(node);
    if (balance_factor > 1 && value > node->left->value) return _double_right_rotate(node);
    if (balance_factor < -1 && value < node->right->value) return _double_left_rotate(node);

    return node;
}

void _free_tree(AVLNode *node) {
    if (node == NULL) return;
    _free_tree(node->left);
    _free_tree(node->right);
    free(node);
}
// LOW LEVEL FUNCTIONS =============================


// INTERFACE FUNCTIONS =============================
AVLTree *create_avl_tree() {
    AVLTree *tree = malloc(sizeof(AVLTree));
    if (!tree) {
        debug("Failed to allocate memory for AVLTree.");
        return NULL;
    }
    tree->root = NULL;
    tree->bytes_allocated = sizeof(AVLTree);
    return tree;
}

AVLTree *insert_avl_tree(AVLTree *tree, int value) {
    if (tree == NULL) return NULL;
    tree->root = _insert(tree->root, value);
    return tree;
}


void print_tree_hierarchical(AVLNode *node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) printf("    ");
    printf("%d, h: %d\n", node->value, node->height);
    print_tree_hierarchical(node->right, level + 1);
    print_tree_hierarchical(node->left, level + 1);

}

void free_avl_tree(AVLTree *tree) {
    if (tree == NULL) return;
    _free_tree(tree->root);
    free(tree);
}
// INTERFACE FUNCTIONS =============================
