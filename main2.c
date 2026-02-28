#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*BinaryOperationPtr)(const void*, const void*, void*);
typedef void (*InitOperationPtr)(void*);


typedef struct Algebra // Fieldinfo
{
    BinaryOperationPtr add;        
    BinaryOperationPtr multiply;   
    InitOperationPtr   defaultInit;   // чтобы положить правильный ноль в комплексные и вещественные вектора     
    size_t elementSize; // Зачем это поле
} Algebra;

typedef struct Real
{
    double value;
}Real;

void real_add(const void* a, const void* b, void* out) // это сложение ОДНОЙ координаты (одного элемента), а не всего вектора.
{
    Real* res = (Real*)out;                            // один скаляр + один скаляр = один скаляр
    const Real* a1 = (const Real*)a;
    const Real* a2 = (const Real*)b;

    res->value = a1->value + a2->value;
}

void real_multiply(const void* a, const void* b, void* out)
{
    Real* res = (Real*)out;
    const Real* a1 = (const Real*)a;
    const Real* a2 = (const Real*)b;

    res->value = a1->value * a2->value;
}

void real_zero(void* out)
{
    Real* res = (Real*)out;

    res->value = 0.0;
}

static Algebra* realAlgebra = NULL; // чтобы нельзя было при помощи extern обратиться к этой переменной из другого файла
const Algebra* getRealAlgebra(void)
{
    if(!realAlgebra)
    {

    realAlgebra = malloc(sizeof(*realAlgebra));
    if (!realAlgebra) return NULL; 
    realAlgebra->add = real_add;
    realAlgebra->multiply = real_multiply;
    realAlgebra->elementSize = sizeof(Real);
    realAlgebra->defaultInit = real_zero;
    
    }
    return realAlgebra; // lazy initialization

}

typedef struct Complex {
    double re;
    double im;
} Complex;

void complex_add(const void* a, const void* b, void* out)
{
    Complex* sum = (Complex*)out;
    const Complex* a1 = (const Complex*)a;
    const Complex* a2 = (const Complex*)b;

    sum->re = a1->re + a2->re;
    sum->im = a1->im + a2->im;
}

 void complex_multiply(const void* a, const void* b, void* out)
{
    Complex* product = (Complex*)out;
    const Complex* a1 = (const Complex*)a;
    const Complex* a2 = (const Complex*)b;

    product->re = (a1->re)*(a2->re) - (a1->im)*(a2->im);
    product->im = (a1->re)*(a2->im) + (a1->im)*(a2->re);
}

void complex_zero(void* out)
{
    Complex* res = (Complex*)out;

    res->re = 0.0;
    res->im = 0.0;
}

static Algebra* complexAlgebra = NULL;
const Algebra* getComplexAlgebra(void) // почему const 
{
    if(!complexAlgebra)
    {
        complexAlgebra = malloc(sizeof(*complexAlgebra)); // почему нету * перед complexAlgebra
        if(!complexAlgebra) return NULL;
        complexAlgebra->add = complex_add;
        complexAlgebra->multiply = complex_multiply;
        complexAlgebra->defaultInit = complex_zero;
        complexAlgebra->elementSize = sizeof(Complex);
    }
    return complexAlgebra;

}

typedef struct Vector 
{
    void* data;              // data указывает на блок памяти, где подряд лежат Real или Complex
    size_t size;             // size — сколько координат реально добавили
    size_t capacity;         // capacity — сколько координат помещается в выделенный блок без расширения !!!
    const Algebra* type;     // type (Algebra*) — чтобы понимать, какого типа эти координаты и какие операции использовать
} Vector;

void vector_init(Vector* v, const Algebra* type) /* Подготовка к работе вектора. Vector a; vector_init(&a, getRealAlgebra());*/
{
    v->data = NULL; // Почему на NULL? Для чего
    v->size = 0;
    v->capacity = 0;
    v->type = type;
}

void vector_free(Vector* v)
{
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    v->type = NULL;
}

int vector_reserve(Vector* v, size_t new_capacity) // vector_reserve(v, capacity * 2); // new_capacity = 8
{
    if(new_capacity <= v->capacity) return 0;
    else
    {
        
        void* ar = malloc(new_capacity * v->type->elementSize);
        if(ar == NULL) return 1;
        memcpy(ar, v->data, v->size * v->type->elementSize);
        free(v->data);
        v->data = ar;
        v->capacity = new_capacity;
        return 0;
    }
}

int vector_push_back(Vector* v, const void* elem)
{
    
    if(v == NULL || elem == NULL || v->type == NULL) return 1;
    if(v->size == v->capacity)
    {
        size_t new_cap;
        if(v->capacity == 0) new_cap = 1;
        else new_cap = 2 * (v->capacity);
        if(vector_reserve(v, new_cap) == 1) return 1;
        char* temp = (char*)v->data;
        char* end = temp + v->size * v->type->elementSize;
        memcpy(end, elem, v->type->elementSize);
        v->size++;
        return 0;

    }
    char* temp = (char*)v->data;
    char* end = temp + v->size * v->type->elementSize;
    memcpy(end, elem, v->type->elementSize);
    v->size++;
    return 0;
}

int vector_get(const Vector* v, size_t index, void* out) // out это адрес обьекта Complex или Real куда функция копирует index элемент
{
    if(v == NULL || out == NULL || v->type == NULL || (index >= v->size) || v->data == NULL) return 1;
    const char* src = (const char*)v->data + index * v->type->elementSize;
    memcpy(out, src, v->type->elementSize);
    
    return 0;
}

int vector_set(Vector* v, size_t index, const void* elem)
{
    if(v == NULL || elem == NULL || index >= v->size || v->type == NULL || v->data == NULL) return 1;
    char* dst = (char*)v->data + index * v->type->elementSize;
    memcpy(dst, elem, v->type->elementSize);

    return 0;
}

void printReal(const void* x)
{
    const Real* r = (const Real*)x;
    printf("%f",r->value);
}

void printComplex(const void* x)
{
    const Complex* c = (const Complex*)x;
    if(c->im >= 0) printf("%f+%fi",c->re, c->im);
    else printf("%f%fi",c->re, c->im);

}

void vector_print(const Vector* v)
{
    if(v == NULL || v->type == NULL)
    {
        printf("The vector is undefined");
        return;
    }

        printf("[");
        
        for(size_t i = 0; i < v->size ; ++i)
        {
            const char* p = (const char*)v->data + i * v->type->elementSize;

            if (v->type == getRealAlgebra()) 
            {
                printReal(p);
            }

            else if (v->type == getComplexAlgebra()) 
            {
                printComplex(p);
            }

            else
            {
                printf("Error");
            }

        if (i + 1 < v->size) printf(", ");
            
        }

        printf("]\n");
        
}

int vector_add(const Vector* a, const Vector* b, Vector* out)
{
    if(a == NULL || b == NULL || out == NULL || a->type != b->type || a->size != b->size) return 1;
    else
    {
        vector_init(out, a->type);
        if (vector_reserve(out, a->size)) return 1;

        unsigned char* tmp = malloc(a->type->elementSize);
        if (!tmp) return 1;
        for(size_t i = 0; i < a->size; ++i)
        {
            const char* pa = (const char*)a->data + i * a->type->elementSize;
            const char* pb = (const char*)b->data + i * b->type->elementSize;

            
            a->type->add(pa, pb, tmp);
            if (vector_push_back(out, tmp))
            { 
                free(tmp);
                return 1;
            } 
            
            
        }
        free(tmp);
        
                
    }
    
    
    return 0;
}

int vector_dot(const Vector* a, const Vector* b, void* out)
{
    if(a == NULL || b == NULL || out == NULL || a->type != b->type || a->size != b->size) return 1;
    else
    {
        vector_init(out, a->type);
        if (vector_reserve(out, a->size)) return 1;

        unsigned char* tmp = malloc(a->type->elementSize);

        if (!tmp) return 1;
        for(size_t i = 0; i < a->size; ++i)
        {
            const char* pa = (const char*)a->data + i * a->type->elementSize;
            const char* pb = (const char*)b->data + i * b->type->elementSize;

            a->type->multiply(pa, pb, tmp);

            if (vector_push_back(out, tmp))
            { 
                free(tmp);
                return 1;
            } 


        }
        
        free(tmp);

    }


}


    










int main()
{

    return 0;
}