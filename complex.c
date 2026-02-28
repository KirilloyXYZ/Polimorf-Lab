#include <stdio.h>
#include <stdlib.h>

#include "complex.h"

static void complexAdd(const void* a, const void* b, void* out)
{
    Complex* sum = (Complex*)out;
    const Complex* a1 = (const Complex*)a;
    const Complex* a2 = (const Complex*)b;

    sum->re = a1->re + a2->re;
    sum->im = a1->im + a2->im;
}

static void complexMultiply(const void* a, const void* b, void* out)
{
    Complex* product = (Complex*)out;
    const Complex* a1 = (const Complex*)a;
    const Complex* a2 = (const Complex*)b;

    product->re = (a1->re)*(a2->re) - (a1->im)*(a2->im);
    product->im = (a1->re)*(a2->im) + (a1->im)*(a2->re);
}

static void complexZero(void* out)
{
    Complex* res = (Complex*)out;

    res->re = 0.0;
    res->im = 0.0;
}

static int complexRead(void* out)
{
    Complex* c = (Complex*)out;
    return (scanf("%lf %lf", &c->re, &c->im) == 2) ? 0 : 1;
}

static void printComplex(const void* x)
{
    const Complex* c = (const Complex*)x;
    if(c->im >= 0) printf("%f+%fi",c->re, c->im);
    else printf("%f%fi",c->re, c->im);

}

static Algebra* complexAlgebra = NULL;

const Algebra* getComplexAlgebra(void) 
{
    if(!complexAlgebra)
    {

        complexAlgebra = malloc(sizeof(*complexAlgebra)); 
        if(!complexAlgebra) return NULL;
        complexAlgebra->add = complexAdd;
        complexAlgebra->print = printComplex;
        complexAlgebra->read  = complexRead;
        complexAlgebra->multiply = complexMultiply;
        complexAlgebra->defaultInit = complexZero;
        complexAlgebra->elementSize = sizeof(Complex);

    }

    return complexAlgebra;

}