#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

typedef struct table_item {
    char *text;
    char *rplm;
} TableItem;


// linked list that will be used as an collision solver for
// hash table (contains list of pointers to hash table items:
// strings and their replacements)
typedef struct node {
    struct llist *next;
    TableItem *item;
} Node;

typedef struct llist {
    Node *head;
    int size;
} LinkedList;


typedef struct table {
    // pointer to list of pointers with tableitems
    TableItem **items;
    // parallel list of of pointers to litsts that will 
    // be used in case of overflow
    // LinkedList **overflow_buckets;
    int size;
    int count; // check whether table is filled
} Table;


unsigned hash(char *str);
TableItem* create_item(char *str, char *rplm);
Table* create_table(void);
// void free_table_item(TableItem *item);
void free_table(Table *table);
void add(Table *table, char *str, char *rplm);
TableItem* lookup(Table *table, char *str);
Node* allocate_node(void);


int main(int argc, char *argv[]) {
    Table *table = create_table();

    char *test_str[] = {"Bon", "Test"};
    char *repl_str[] = {"Check", "Done"};
    int test_size = 2;

    for (int i = 0; i < test_size; i++) {
        add(table, test_str[i], repl_str[i]);
    }

    TableItem *item = lookup(table, test_str[0]);
    printf("The answer is: %s\n", item->rplm);

    free_table(table);

    if (argc != 1 && strcmp(argv[1], "d") == 0)
        fscanf(stdin, "c");

    return 0;
}

/* the simplest hash function:
add all the elements of a strings */
unsigned hash(char *str) {
    unsigned res = 0;

    // adding all the elements of string
    for (int i = 0; str[i]; i++)
        i += str[i];

    return res % HASHSIZE;
}


/* creates an item for a hash table from two given strings */
TableItem* create_item(char *str, char *rplm) {
    TableItem *item = (TableItem *) malloc(sizeof(TableItem));
    item->text = str;
    item->rplm = rplm;

    return item;
}


/* allocates memory for a table */
Table* create_table(void) {
    Table *table = (Table *) malloc(sizeof(Table));
    table->size = HASHSIZE;
    table->count = 0;
    table->items = (TableItem**) calloc(table->size, sizeof(TableItem*));
    for (int i =0; i < table->size; i++)
        table->items[i] = NULL;

    // todo:
    // table->overflow_buckets

    return table;
}


/* frees the memory, allocated for the table (also for its elements) */
void free_table(Table *table) {
    for (int i = 0; i < table->size; i++) {
        TableItem *cur_itm = table->items[i];
        if (cur_itm != NULL)
            free(table->items[i]);
    }
    //free(table->items);
    // TODO:
    // free(table->overflow_buckets);
    free(table);
}

Node *allocate_node(void) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: could not allocate memory for linked list");
        exit(EXIT_FAILURE);
    }

    return node;
}

void ll_add(LinkedList *lst, TableItem *item) {
    Node *node = allocate_node();
    node->item = item;
    node->next = NULL;

    if (lst == NULL) {
        lst->head = node;
    } else {
        // TODO why there is a problem
        node->next = lst->head;
        lst->head = node;
    }
    ++lst->size;
}

/* adds an element to the hash table */
void add(Table *table, char *str, char *rplm) {
    TableItem *item = create_item(str, rplm);
    unsigned idx = hash(str);

    TableItem *cur_item = table->items[idx];

    if (cur_item == NULL) {
        if (table->count == table->size) {
            fprintf(stderr, "Insertion error: hash table is full\n");
            // free_table_item(item);
            free(item);
            exit(EXIT_FAILURE);
        }

        table->items[idx] = item;
        ++table->count;
    } else {
        ; // TOOD add resolving of collision
    }
}


/* conducts amortised O(1) lookup in a hash table */
TableItem *lookup(Table *table, char *str) {
    int index = hash(str);

    TableItem *item = table->items[index];
    // todo handle collision via linked list

    return item;
}
