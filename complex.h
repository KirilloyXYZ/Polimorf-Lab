#ifndef COMPLEX_H
#define COMPLEX_H

#include "algebra.h"

typedef struct Complex
{
    double re;
    double im;
} Complex;

const Algebra* getComplexAlgebra(void);


#endif 
