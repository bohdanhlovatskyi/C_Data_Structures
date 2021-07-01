#include <stdio.h>
#include <stdlib.h>

#define TESTSIZE 6

typedef struct node {
    int data;
    struct node *left_child;
    struct node *right_child;
} node;

typedef struct bin_tree {
    node *root;
    int size;
} Tree;


node *init_tree_node(int a);
Tree *init_tree(void);
int deallocate_tree(Tree *tree);
int print_tree(node *root, int depth);
void add_node(Tree *tree, int node_value);
int binary_search(Tree *tree, int value);


int main(int argc, char *argv[]) {
    int arr[TESTSIZE] = {1, 4, 2, 9, -3, -9};
    int res;
    Tree *test_tree = init_tree();

    for (int i = 0; i < TESTSIZE; i++) {
        add_node(test_tree, arr[i]);
    }

    print_tree(test_tree->root, 0);
    res = binary_search(test_tree, 2);
    printf("After searching 2 (is in the tree), res is: %d\n", res);

    res = binary_search(test_tree, 100);
    printf("After searching 100 (is not in the tree), res is: %d\n", res);

    deallocate_tree(test_tree);

    return 0;
}


/* initialises a node with value a */
node *init_tree_node(int a) {
    node *initialised_node;

    initialised_node = (node *) malloc(sizeof(node));
    if (initialised_node == NULL) {
        fprintf(stderr, "Could not allocate memory for a tree node\n");
        exit(EXIT_FAILURE);
    }
    initialised_node->data = a;
    initialised_node->left_child = NULL;
    initialised_node->right_child = NULL;

    return initialised_node;
}

Tree *init_tree(void) {
    Tree *tree;

    tree = (Tree *) malloc(sizeof(tree));
    if (tree == NULL) {
        fprintf(stderr, "Could not allocate memory for a tree\n");
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;

    return tree;
}


// --------- delete tree ------------------------
void _dealocate_node(node *ndp);

int deallocate_tree(Tree *tree) {
    if (tree->root != NULL)
        _dealocate_node(tree->root);
        free(tree->root);
        free(tree);
        
    return 0;
}

void _dealocate_node(node *ndp) {
    if (ndp->left_child) {
        _dealocate_node(ndp->left_child);
        free(ndp->left_child);
    }

    if (ndp->right_child) {
        _dealocate_node(ndp->right_child);
        free(ndp->right_child);
    }
}

// --------- add node ------------------------
// helper function for add_node
void _add_recurse(node *tree_node, int node_value);

void add_node(Tree *tree, int node_value) {

    if (tree->root == NULL) {
        tree->root = init_tree_node(node_value);
        // printf("The root node is added\n");
    } else {
        _add_recurse(tree->root, node_value);
        ++tree->size;
        // printf("The node is added\n");
    }
}


void _add_recurse(node *tree_node, int node_value) {
    if (tree_node->data < node_value) {
        if (tree_node->left_child == NULL) {
            tree_node->left_child = init_tree_node(node_value); 
        } else {
            _add_recurse(tree_node->left_child, node_value);
        }
    } else if (tree_node->right_child == NULL)
        tree_node->right_child = init_tree_node(node_value);
      else
        _add_recurse(tree_node->right_child, node_value);
}


/* takes root node as its arg, because the recursion
is used */
int print_tree(node *root, int depth) {
    
    if (root == NULL) {
        return 0;
    }

    print_tree(root->left_child, depth + 1);
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%2d\n", root->data);
    print_tree(root->right_child, depth + 1);

    return 0;
}

/* conducts binary search via binary tree;
return : 0 if an element is in binary tree
         1 if not; */
int binary_search(Tree *tree, int value) {
    node *current;

    current = tree->root;
    while (current != NULL) {
        if (current->data > value)
            current = current->right_child;
        else if (current->data < value)
            current = current->left_child;
        else
            return 0;
    }

    return 1;
}
