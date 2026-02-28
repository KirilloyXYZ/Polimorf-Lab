#include <stdio.h>
#include <stdlib.h>

#include "real.h"

static void realAdd(const void* a, const void* b, void* out) 
{                                                           
    Real* res = (Real*)out;                            
    const Real* a1 = (const Real*)a;
    const Real* a2 = (const Real*)b;

    res->value = a1->value + a2->value;
}

static void realMultiply(const void* a, const void* b, void* out)
{
    Real* res = (Real*)out;
    const Real* a1 = (const Real*)a;
    const Real* a2 = (const Real*)b;

    res->value = a1->value * a2->value;
}

static void realZero(void* out)
{
    Real* res = (Real*)out;

    res->value = 0.0;
}

static int realRead(void* out)
{
    Real* r = (Real*)out;
    return (scanf("%lf", &r->value) == 1) ? 0 : 1;
}

static void printReal(const void* x)
{
    const Real* r = (const Real*)x;
    printf("%f",r->value);
}

static Algebra* realAlgebra = NULL;

const Algebra* getRealAlgebra(void)
{
    if(!realAlgebra)
    {

        realAlgebra = malloc(sizeof(*realAlgebra));
        if (!realAlgebra) return NULL; 
        realAlgebra->add = realAdd;
        realAlgebra->print = printReal;
        realAlgebra->read  = realRead;
        realAlgebra->multiply = realMultiply;
        realAlgebra->elementSize = sizeof(Real);
        realAlgebra->defaultInit = realZero;
    
    }

    return realAlgebra; 

}