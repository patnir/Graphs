typedef struct _Queue{
	Node *head;
	Node *tail;
	int size;
} Queue;

Queue *createQueue() {
	Queue *q = (Queue *)malloc(sizeof(Queue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

void enqueue(Queue *q, int val) {
	Node *node = createNode1(val);

	if (q->size == 0) {
		q->head = node;
		q->tail = node;
		q->size += 1;
		return;
	}

	q->size += 1;
	q->tail->next = node;
	q->tail = node;
}


void dequeue(Queue *q) {
	if (q->size == 0) {
		return;
	}

	q->size -= 1;

	Node *tmp = q->head;
	q->head = q->head->next;
	if (q->head == NULL) {
		q->tail = NULL;
	}
	tmp->next = NULL;

	free(tmp);

	return;
}

Node *retDequeue(Queue *q) {
	if (q->size == 0) {
		return NULL;
	}

	q->size -= 1;

	Node *tmp = q->head;
	q->head = q->head->next;
	if (q->head == NULL) {
		q->tail = NULL;
	}
	tmp->next = NULL;

	return tmp;
}

void printQueue(Queue *q) {
 	Node *curr = q->head;
 	fprintf(stdout, "Queue size: %d\n", q->size);
 	while (curr != NULL) {
 		fprintf(stdout, "%d->", curr->value);
 		curr = curr->next;
 	}
 	fprintf(stdout, "\\\n");
}