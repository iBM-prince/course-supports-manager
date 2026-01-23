#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* Initialize an empty stack */
Stack* init_stack() {
    Stack *s = malloc(sizeof(Stack));
    if (s) s->top = NULL;
    return s;
}

/* Add a support to the top of the stack (Addition History) */
void push(Stack *s, Support data) {
    StackNode *newNode = malloc(sizeof(StackNode));
    if (!newNode) return;
    
    newNode->data = data;
    newNode->next = s->top;
    s->top = newNode;
}

/* Check if stack is empty */
int is_stack_empty(Stack *s) {
    return (s->top == NULL);
}

/* Free stack memory */
void free_stack(Stack *s) {
    while (s->top) {
        StackNode *temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    free(s);
}