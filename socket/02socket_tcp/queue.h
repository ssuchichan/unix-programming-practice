#ifndef QUEUE_H
#define QUEUE_H

struct Node {
    int data;
    struct Node* next;
};

void push_queue(struct Node* queue, int data);
int pop_queue(struct Node* queue);
int is_empty(struct Node* queue);

#endif
