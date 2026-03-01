#include <assert.h>
#include <stddef.h>

#include "vector.h"
#include "real.h"
#include "complex.h"

int main(void) {
    
    {
        const Algebra* R = getRealAlgebra();
        Real a = {2.0}, b = {-5.0}, out;

        R->add(&a, &b, &out);
        assert(out.value == -3.0);

        R->multiply(&a, &b, &out);
        assert(out.value == -10.0);

        out.value = 123.0;
        R->defaultInit(&out);
        assert(out.value == 0.0);
    }

    {
        const Algebra* C = getComplexAlgebra();
        Complex a = {1.0, 2.0}, b = {3.0, 4.0}, out;

        C->add(&a, &b, &out);
        assert(out.re == 4.0);
        assert(out.im == 6.0);

        C->multiply(&a, &b, &out);
        assert(out.re == -5.0);
        assert(out.im == 10.0);

        out.re = 7.0; out.im = 8.0;
        C->defaultInit(&out);
        assert(out.re == 0.0);
        assert(out.im == 0.0);
    }

    {
        const Algebra* R = getRealAlgebra();
        Vector* a = vectorCreate();
        Vector* b = vectorCreate();
        Vector* out = vectorCreate();

        assert(a && b && out);

        assert(vectorInit(a, R, 2) == 0);
        assert(vectorInit(b, R, 2) == 0);
        assert(vectorInit(out, R, 0) == 0);

        Real t;

        t.value = 1.0;  assert(vectorSet(a, 0, &t) == 0);
        t.value = 2.0;  assert(vectorSet(a, 1, &t) == 0);

        t.value = 10.0; assert(vectorSet(b, 0, &t) == 0);
        t.value = 20.0; assert(vectorSet(b, 1, &t) == 0);

        assert(vectorAdd(a, b, out) == 0);
        assert(vectorSize(out) == 2);

        Real r;
        assert(vectorGet(out, 0, &r) == 0); 
        assert(r.value == 11.0);
        
        assert(vectorGet(out, 1, &r) == 0); 
        assert(r.value == 22.0);

        assert(vectorAdd(NULL, b, out) == 1);
        assert(vectorAdd(a, NULL, out) == 1);
        assert(vectorAdd(a, b, NULL) == 1);

        Vector* c = vectorCreate();
        assert(c);
        assert(vectorInit(c, getComplexAlgebra(), 2) == 0);
        assert(vectorAdd(a, c, out) == 1);

        assert(vectorInit(b, R, 3) == 0);
        assert(vectorAdd(a, b, out) == 1);

        vectorDestroy(a);
        vectorDestroy(b);
        vectorDestroy(c);
        vectorDestroy(out);
    }

    {
        const Algebra* R = getRealAlgebra();
        Vector* a = vectorCreate();
        Vector* b = vectorCreate();
        assert(a && b);

        Real res;
        assert(vectorInit(a, R, 2) == 0);
        assert(vectorInit(b, R, 2) == 0);

        Real t;
        t.value = 1.0; assert(vectorSet(a, 0, &t) == 0);
        t.value = 2.0; assert(vectorSet(a, 1, &t) == 0);

        t.value = 3.0; assert(vectorSet(b, 0, &t) == 0);
        t.value = 4.0; assert(vectorSet(b, 1, &t) == 0);

        assert(vectorDot(a, b, &res) == 0);
        assert(res.value == 11.0);

        assert(vectorDot(NULL, b, &res) == 1);
        assert(vectorDot(a, NULL, &res) == 1);
        assert(vectorDot(a, b, NULL) == 1);

        Vector* c = vectorCreate();
        assert(c);
        assert(vectorInit(c, getComplexAlgebra(), 2) == 0);
        Complex outC;
        assert(vectorDot(a, c, &outC) == 1);

        assert(vectorInit(b, R, 3) == 0);
        assert(vectorDot(a, b, &res) == 1);

        vectorDestroy(a);
        vectorDestroy(b);
        vectorDestroy(c);
    }

    

    return 0;
} 