#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define HASHSIZE 101

int main(int argc, char *argv[]) {
    Table *table = create_table();

    char *test_str[] = {"Bonjour", "Test", "Test", "zkza", "Tfrt"};
    char *repl_str[] = {"Check", "Done", "Done", "collision", "collision"};
    int test_size = 5;

    for (int i = 0; i < test_size; i++) {
        add(table, test_str[i], repl_str[i]);
    }

    print_overflow_buckets(table);

    for (int i = 0; i < test_size; i++) {
        TableItem *item = lookup(table, test_str[i]);
        if (item != NULL) {
            printf("The answer is: %s | %s\n", item->text, item->rplm);
        } else {
            printf("Null pointer is returned\n");
        }
    }

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
    for (int i = 0; i < strlen(str); i++)
        res += str[i];

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

    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}


/* frees the memory, allocated for the table (also for its elements) */
void free_table(Table *table) {
    for (int i = 0; i < table->size; i++) {
        TableItem *cur_itm = table->items[i];
        if (cur_itm != NULL)
            free(table->items[i]);
    }
    free(table->items);
    free_overflow_buckets(table);
    free(table);
}

Node *allocate_node(void) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: could not allocate memory for node");
        exit(EXIT_FAILURE);
    }

    return node;
}

LinkedList *allocate_list(void) {
    LinkedList *lst = (LinkedList *) malloc(sizeof(LinkedList));
    if (lst == NULL) {
        fprintf(stderr, "Error: could not allocate memory for linked list");
        exit(EXIT_FAILURE);
    }

    return lst;
}

void ll_add(LinkedList *lst, TableItem *item) {
    Node *node = allocate_node();
    node->item = item;
    node->next = NULL;

    if (lst == NULL) {
        lst->head = node;
    } else {
        node->next = lst->head;
        lst->head = node;
    }
    print_lst(lst);
    ++lst->size;
}


void free_linked_list(LinkedList *lst) {
    Node *current, *next;

    current = lst->head;
    next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(lst);
}

LinkedList** create_overflow_buckets(Table *table) {
    LinkedList **buckets = (LinkedList **) calloc(table->size, sizeof(LinkedList*));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;

    return buckets;
}


void free_overflow_buckets(Table *table) {
    LinkedList **buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        if (buckets[i] != NULL)
            free_linked_list(buckets[i]);

    free(buckets);
} 

void print_overflow_buckets(Table *table) {
    LinkedList **buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        if (buckets[i] != NULL) {
            printf("Idx: %d |", i);
            print_lst(buckets[i]);
        }

    printf("\n");
}

void print_lst(LinkedList *lst) {
    Node *current;


    printf("Size of lst: %d |", lst->size);
    current = lst->head;
    while (current != NULL) {
        printf("%s:%s -> ", current->item->text, current->item->rplm);
        current = current->next;
    }
    printf("Null \n");
}


void handle_collision(Table *table, unsigned index, TableItem *item) {
    LinkedList *lst = table->overflow_buckets[index];

    if (lst == NULL) {
        lst = allocate_list();
        Node *node = allocate_node();
        node->item = item;
        lst->head = node;
        table->overflow_buckets[index] = lst;
    } else
        ll_add(lst, item);
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
        if (strcmp(cur_item->text, str) == 0) {
            printf("Element is already in hash table: %s\n", str);
        } else {
            printf("The collision has occured: %s\n", str);
            handle_collision(table, idx, item);
        }
    }
}


/* conducts amortised O(1) lookup in a hash table */
TableItem *lookup(Table *table, char *str) {
    int index = hash(str);

    TableItem *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];
    if (head == NULL) {
        return item;
    }

    Node *current = head->head;

    // Ensure that we move to items which are not NULL
    while (item != NULL) {
        if (strcmp(item->text, str) == 0)
            return item;
        if (current == NULL)
            return NULL;
        item = current->item;
        current = current->next;
    }
    return NULL;
}
