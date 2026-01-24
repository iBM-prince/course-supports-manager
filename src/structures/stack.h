#ifndef STACK_H
#define STACK_H

#include "../supports/support.h"

/* Node structure for the stack */
typedef struct StackNode {
    Support data;
    struct StackNode *next;
} StackNode;

/* Stack structure */
typedef struct {
    StackNode *top;
} Stack;

/* Function prototypes */
Stack* init_stack();
void push(Stack *s, Support data);
int is_stack_empty(Stack *s);
void free_stack(Stack *s);

#endif