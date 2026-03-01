#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include "algebra.h"

typedef struct Vector Vector;

int vectorInit(Vector* v, const Algebra* type, size_t n);
void vectorFree(Vector* v);

void vectorPrint(const Vector* v);

int vectorAdd(const Vector* a, const Vector* b, Vector* out);
int vectorDot(const Vector* a, const Vector* b, void* out);

int vectorRead(Vector* v);

Vector* vectorCreate(void);
void vectorDestroy(Vector* v);

int vectorIsInitialized(const Vector* v);
size_t vectorSize(const Vector* v);
size_t vectorElementSize(const Vector* v);

int vectorSet(Vector* v, size_t i, const void* value);
int vectorGet(const Vector* v, size_t i, void* out_value);

void vectorPrintValue(const Vector* v, const void* value);

#endif 
