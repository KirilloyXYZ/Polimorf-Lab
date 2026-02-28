#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <stddef.h>

typedef void (*BinaryOperationPtr)(const void*, const void*, void*);
typedef void (*InitOperationPtr)(void*); 
typedef void (*PrintOperationPtr)(const void*);
typedef int  (*ReadOperationPtr)(void*);

typedef struct Algebra 
{ 
    BinaryOperationPtr add;        
    BinaryOperationPtr multiply;   
    InitOperationPtr   defaultInit;          
    PrintOperationPtr  print;
    ReadOperationPtr   read;      
    size_t elementSize; 
} Algebra;

#endif 
