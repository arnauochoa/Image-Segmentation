//
// Created by Arnau Ochoa Bañuelos on 2020-01-09.
//

#ifndef IMAGE_SEGMENTATION_STACK_H
#define IMAGE_SEGMENTATION_STACK_H

#endif //IMAGE_SEGMENTATION_STACK_H

// A structure to represent a stack
typedef struct {
    int top;
    unsigned capacity;
    int* array;
} Stack;

// function to create a stack of given capacity. It initializes size of
// stack as 0
Stack *createStack(unsigned capacity)
// Stack is full when top is equal to the last index
int isFull(Stack* stack);
// Stack is empty when top is equal to -1
int isEmpty(Stack* stack);
// Function to add an item to stack. It increases top by 1
void push(Stack* stack, int item);
// Function to remove an item from stack. It decreases top by 1
int pop(Stack* stack);
// Function to return the top from stack without removing it
int peek(Stack* stack);