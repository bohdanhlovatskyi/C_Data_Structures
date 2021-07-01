
typedef struct table_item {
    char *text;
    char *rplm;
} TableItem;

// linked list that will be used as an collision solver for
// hash table (contains list of pointers to hash table items:
// strings and their replacements)
typedef struct node {
    struct node *next;
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
    LinkedList **overflow_buckets;
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
LinkedList *allocate_list(void);
LinkedList** create_overflow_buckets(Table *table);
void free_overflow_buckets(Table *table);
void print_lst(LinkedList *lst);
void print_overflow_buckets(Table *table);
void ll_add(LinkedList *lst, TableItem *item);
void free_linked_list(LinkedList *lst);
void handle_collision(Table *table, unsigned index, TableItem *item);
