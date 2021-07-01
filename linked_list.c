/* Produced logs:
initial lst:
Size of lst: 5 |0 -> 8 -> 2 -> 4 -> 1 -> Null 
Size of lst: 5 |0 -> 8 -> 10 -> 4 -> 1 -> Null 
Size of lst: 4 |0 -> 8 -> 4 -> 1 -> Null 
Size of lst: 4 |0 -> 8 -> 4 -> 1 -> Null 
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node* next;
} node;

/* is there really a need for it? I guess yep, because
it create an additional level of abstraction that might
be usefule, speaking of organisation of program structure */
typedef struct ll
{
    struct node* root;
    int size;
} List;

node *init(int a);
List *init_ll(void);
void rm_lst(List *lst);
void print_lst(List *lst, FILE *fp);
void add_node(List *lst, int a);
int insert_node(List *lst, int idx, int a);
int rm_node(List *lst, int a);

int main(int argc, char *argv[]) {
    int arr[5] = {1, 4, 2, 8, 0};
    int test_size = 5;
    List *test_lst = init_ll();
    FILE *logs;
    if (argc > 1 && *argv[1] == 'd')
        logs = stdout;
    else
        logs = fopen("logs.txt", "a");

    for (int i = 0; i < test_size; i++) {
        add_node(test_lst, arr[i]);
    }

    fprintf(logs, "initial lst:\n");
    print_lst(test_lst, logs);

    insert_node(test_lst, 2, 10);
    print_lst(test_lst, logs);

    rm_node(test_lst, 10);
    print_lst(test_lst, logs);

    // tries to remove node that does not exist
    rm_node(test_lst, 100);
    print_lst(test_lst, logs);

    rm_lst(test_lst);
    fclose(logs);

    // print_lst(test_lst); this will give segmentation fault, as expected
    // system("leaks ./a.out"); with `export MallocStackLogging=1` prints out
    // the stack trace of memory allocation (on heap);

    if (argc > 1 && *argv[1] == 'd')
        fscanf(stdin, "c");  // in debug mode this allows for the orogramme not to stop
        // -> run the leaks command on a process that is still run.
}


/* initialises a new node with value a */
node *init(int a) {
    node *initialised_node;

    initialised_node = (node *) malloc(sizeof(node));
    if (initialised_node == NULL) {
        fprintf(stderr, "Could not init a node\n");
        exit(EXIT_FAILURE);
    }
    initialised_node->data = a;
    initialised_node->next = NULL;
    
    return initialised_node;
}


/* initialises a new empty list */
List *init_ll(void) {
    List *init_lst;
    init_lst = (List *) malloc(sizeof(List));
    if (init_lst == NULL) {
        fprintf(stderr, "Could not init a lst\n");
        exit(EXIT_FAILURE);
    }
    init_lst->root = NULL;
    init_lst->size = 0;

    return init_lst;
}


/* de-allocates all the memory allocated for a list and its nodes */
void rm_lst(List *lst) {
    node *current, *next;

    current = lst->root;
    next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(lst);
}


/* adds node with a value a */
void add_node(List *lst, int a) {
    node *node_to_add = init(a);

    if (lst->root == NULL) {
        lst->root = node_to_add;
    } else {
        node_to_add->next = lst->root;
        lst->root = node_to_add;
    }
    ++lst->size;
}


/* inserts node at a given idx.
Raises an error, if the idx is out of bounds */
int insert_node(List *lst, int idx, int a) {

    if (idx < 0 || idx > lst->size) {
        fprintf(stderr, "Idx does not fit, could not insert a node\n");
        // exit(EXIT_FAILURE);
        return 1;
    }

    int cr_idx = 0;
    node *current;

    current = lst->root;
    while (cr_idx != idx && current != NULL) {
        current = current->next;
        cr_idx++;
    }

    current->data = a;

    return 0;
}

/* removes node with value a from a list */
int rm_node(List *lst, int a) {
    node *current, *next;
    int cr_idx = 0;
    int to_return = -1;

    current = lst->root;
    while (current->next != NULL && (current->next)->data != a) {
        current = current->next;
        cr_idx++;
    }

    if (cr_idx == lst->size - 1) {
        fprintf(stderr, "There is no such node\n");
        return to_return;
    } else {
        to_return = current->data;
        current->next = (current->next)->next;
        --lst->size;
        return to_return;
    }
}


/* prints out the elements of linked list,
separated by -> ; If ones want it to print
into stdout (terminal), just pass a null
pointer to it*/
void print_lst(List *lst, FILE *fp) {
    node *current;

    if (!fp)
      fp = stdout;

    fprintf(fp, "Size of lst: %d |", lst->size);
    current = lst->root;
    while (current != NULL) {
        fprintf(fp, "%d -> ", current->data);
        current = current->next;
    }
    fprintf(fp, "Null \n");
}
