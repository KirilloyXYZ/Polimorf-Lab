#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int vectorInit(Vector* v, const Algebra* type, size_t n)
{
    if (v == NULL || type == NULL) return 1;

    void* new_data = NULL;
    if (n > 0)
    {
        new_data = malloc(n * type->elementSize);
        if (new_data == NULL) return 1;
    }

    free(v->data);
    v->data = new_data;
    v->size = n;
    v->type = type;

    return 0;
}

void vectorFree(Vector* v)
{
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->type = NULL;
}

void vectorPrint(const Vector* v)
{
    if(v == NULL || v->type == NULL)
    {

        printf("The vector is undefined");
        return;

    }

        printf("[");
        const char* separator = "";
        for(size_t i = 0; i < v->size ; ++i)
        {

            printf("%s",separator);
            const char* p = (const char*)v->data + i * v->type->elementSize; 

            v->type->print(p);
            

            separator = ", ";

        }

        printf("]\n");
        
}

int vectorAdd(const Vector* a, const Vector* b, Vector* out)
{
    if(a == NULL || b == NULL || out == NULL || a->type != b->type || a->size != b->size) return 1;
    else
    {

        vectorFree(out);  
        if(vectorInit(out, a->type, a->size) != 0) return 1;

        for(size_t i = 0; i < a->size; ++i)
        {
            const char* pa = (const char*)a->data + i * a->type->elementSize;
            const char* pb = (const char*)b->data + i * a->type->elementSize;
            char* pout = (char*)out->data + i * a->type->elementSize;

            a->type->add(pa, pb, pout);
        }
     
    }
        
    return 0;
}

int vectorDot(const Vector* a, const Vector* b, void* out) 
{
    if(a == NULL || b == NULL || out == NULL || a->type != b->type || a->size != b->size) return 1;
    else
    {
        
        a->type->defaultInit(out);

        char* tmp = malloc(a->type->elementSize);
        if(tmp == NULL) return 1;
        
        
        for(size_t i = 0; i < a->size; ++i)
        {
            const char* pa = (const char*)a->data + i * a->type->elementSize;
            const char* pb = (const char*)b->data + i * a->type->elementSize;
            
            a->type->multiply(pa, pb, tmp);
            a->type->add(out, tmp, out);
            
        }

        free(tmp);
        return 0;

    }

}

int vectorRead(Vector* v)
{

    if(!v || !v->type || !v->type->read) return 1;

    printf("Enter the dimension of the vector\n");
    size_t n = 0;

    if(scanf("%zu", &n) != 1) return 1;

    if(vectorInit(v, v->type, n) != 0) return 1;
    if(n == 0) return 0;

    printf("Enter %zu element(s) (format depends on the selected type)\n", n);

    for(size_t i = 0; i < n; ++i)
    {

        void* dst = (char*)v->data + i * v->type->elementSize;
        if (v->type->read(dst) != 0)
        {
            return 1;
        }

    }

    return 0;
         
}