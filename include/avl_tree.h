#ifndef AVL_TREE_H
#define AVL_TREE_H
#endif

typedef struct AVLNode {
    int value;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
    int bytes_allocated;
} AVLTree;

AVLTree *create_avl_tree();

AVLTree *insert_avl_tree(AVLTree *tree, int value);

void print_tree_hierarchical(AVLNode *node, int level);

void free_avl_tree(AVLTree *tree);