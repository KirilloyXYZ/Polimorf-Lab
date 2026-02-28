#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include "algebra.h"

typedef struct Vector 
{
    void* data;              
    size_t size;             
    const Algebra* type;     
} Vector;

int vectorInit(Vector* v, const Algebra* type, size_t n);
void vectorFree(Vector* v);

void vectorPrint(const Vector* v);

int vectorAdd(const Vector* a, const Vector* b, Vector* out);
int vectorDot(const Vector* a, const Vector* b, void* out);

int vectorRead(Vector* v);

#endif 
