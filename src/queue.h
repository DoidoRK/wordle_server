#include <iostream>
#include <cstdlib>

//Generic implementation for a queue.

// Define a struct to represent a node
typedef struct node {
    void *client_socket;
    struct node *next;
} node_t;

// Define a struct to represent a queue
typedef struct queue {
    node_t *head;
    node_t *tail;
} queue_t;

void init_queue(queue_t *queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

bool enqueue(queue_t *queue, void *client_socket) {
    // Create a new node
    node_t *newnode = (node_t *)malloc(sizeof(node_t));
    if (NULL == newnode) {
        return false;
    }
    newnode->client_socket = client_socket;
    newnode->next = NULL;

    // Connect the new node to the queue tail
    if (NULL != queue->tail) {
        queue->tail->next = newnode;
    }
    queue->tail = newnode;

    // If the queue was empty, make the new node the new head
    if (NULL == queue->head) {
        queue->head = newnode;
    }
    return true;
}

void* dequeue(queue_t *queue) {
    // Check if the queue is empty
    if (NULL == queue->head) {
        return NULL;
    }

    // Save the head of the queue
    node_t *tmp = queue->head;

    // Save the value to return
    void *result = tmp->client_socket;

    // Remove it from the list
    queue->head = queue->head->next;
    if (NULL == queue->head) {
        queue->tail = NULL;
    }
    free(tmp);

    return result;
}
