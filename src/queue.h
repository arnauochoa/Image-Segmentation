//
// Created by Arnau Ochoa Bañuelos on 2020-01-09.
//

#ifndef IMAGE_SEGMENTATION_QUEUE_H
#define IMAGE_SEGMENTATION_QUEUE_H

#endif //IMAGE_SEGMENTATION_QUEUE_H

// A structure to represent a queue
typedef struct
{
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;