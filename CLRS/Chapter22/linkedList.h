// Node user-defined data type

typedef struct Node{
	unsigned int value;
	struct Node *next;
} Node;

Node *createNode2(int val, Node *next) {
	Node *curr = (Node *)malloc(sizeof(Node));
	curr->value = val;
	curr->next = next;
	return curr;
}

Node *createNode1(int val) {
	Node *curr = (Node *)malloc(sizeof(Node));
	curr->value = val;
	curr->next = NULL;
	return curr;
}

// Linked List user-defined data type

typedef struct LinkedList{
	Node *head;
	int size;
} LinkedList;

LinkedList *createLinkedList() {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));

	list->head = NULL;
	list->size = 0;

	return list;
}

void pushNode(LinkedList *list, int val) {
	Node *node = createNode1(val);

	if (list->size == 0) {
		list->head = node;
		return;
	}

	node->next = list->head;
	list->head = node;
	list->size += 1;
	return;
}

Node *popNode(LinkedList *list) {
	if (list->size == 0) {
		return NULL;
	}
	Node *ret = list->head;
	list->head = list->head->next;
	ret->next = NULL;
	list->size -= 1;
	return ret;
}

void destroyLinkedList(LinkedList *list) {
	while (list->head != NULL) {
      // whatever we need from head, we have to gather them first
      // should not access anything in head after we free it
      Node *tmp = list->head->next;
      free(list->head);
      list->head = tmp;
   }
 }

 void printLinkedList(LinkedList *list) {
 	Node *curr = list->head;
 	while (curr != NULL) {
 		fprintf(stdout, "%d->", curr->value);
 	}
 	fprintf(stdout, "\\\n");
 }