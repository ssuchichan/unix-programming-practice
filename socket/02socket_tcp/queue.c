#include "queue.h"
#include <stdlib.h>
#include <string.h>

void push_queue(struct Node* queue, int data) {
    struct Node* q = queue;
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    memset(n, 0, sizeof(struct Node));
    n->data = data;
    while(q->next != NULL) {
        q = q->next;
    }
    q->next = n;
    n->next = NULL;
}

int pop_queue(struct Node* queue) {
    if (is_empty(queue)) {
        return -1;
    }

    struct Node* p = queue->next;
    int data = p->data;
    queue->next = p->next;
    free(p);

    return data;
}

int is_empty(struct Node* queue) {
    return !(queue->next) ? 1 : 0;
}
