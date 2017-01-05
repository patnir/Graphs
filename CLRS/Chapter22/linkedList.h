// Node user-defined data type

typedef struct _Node{
	unsigned int value;
	struct _Node *next;
} Node;

Node *createNode2(int val, Node *next) {
	Node *curr = (Node *)malloc(sizeof(Node));
	curr->value = val;
	curr->next = next;
	return curr;
}

Node *createNode1(int val) {
	Node *curr = (Node *)malloc(sizeof(Node));
	if (curr == NULL) {
		fprintf(stderr, "Error creating node1\n");
	}
	curr->value = val;
	curr->next = NULL;
	return curr;
}

void destroyNode(Node *node) {
	free(node);
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
		list->size += 1;
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
	list->head = ret->next;
	ret->next = NULL;
	list->size -= 1;
	return ret;
}

void destroyLinkedList(LinkedList *list) {
	while (list->head != NULL) {
    	Node *tmp = list->head->next;
    	list->head->next = NULL;
    	free(list->head);
		list->head = tmp;
   	}

   	free(list);
 }

 void printLinkedList(LinkedList *list) {
 	Node *curr = list->head;
 	//fprintf(stdout, "Linked list size: %d\n", list->size);
 	while (curr != NULL) {
 		fprintf(stdout, "%d->", curr->value);
 		curr = curr->next;
 	}
 	fprintf(stdout, "\\\n");
 }