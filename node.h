
typedef struct _Node {
    int value;
    struct _Node *next;
    struct _Node *prev; 
} Node;


typedef struct _LinkedList {
    int size;
    Node *head;
    Node *tail;
} LinkedList;

LinkedList* createLinkedList();
void pushFront(LinkedList *list, int data);