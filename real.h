#ifndef REAL_H
#define REAL_H

#include "algebra.h"

typedef struct Real
{
    double value; 
}Real;

const Algebra* getRealAlgebra(void);

#endif
