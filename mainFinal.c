#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*BinaryOperationPtr)(const void*, const void*, void*);
typedef void (*InitOperationPtr)(void*); // Зачем он нужен
typedef void (*PrintOperationPtr)(const void*);
typedef int  (*ReadOperationPtr)(void*);

typedef struct Algebra // Fieldinfo
{ 
    BinaryOperationPtr add;        
    BinaryOperationPtr multiply;   
    InitOperationPtr   defaultInit;            // чтобы положить правильный ноль в комплексные и в ещественные вектора  переименовать (присваивать)
    PrintOperationPtr  print;
    ReadOperationPtr   read;      
    size_t elementSize; 
} Algebra;

typedef struct Real
{
    double value; // не обязательно через структуру
}Real;

void real_add(const void* a, const void* b, void* out) // это сложение ОДНОЙ координаты (одного элемента), а не всего вектора.
{                                                           // слитное написание имя функции
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

int real_read(void* out)
{
    Real* r = (Real*)out;
    return (scanf("%lf", &r->value) == 1) ? 0 : 1;
}

void printReal(const void* x)
{
    const Real* r = (const Real*)x;
    printf("%f",r->value);
}

static Algebra* realAlgebra = NULL; // чтобы нельзя было при помощи extern обратиться к этой переменной из другого файла
const Algebra* getRealAlgebra(void)
{
    if(!realAlgebra)
    {

    realAlgebra = malloc(sizeof(*realAlgebra));
    if (!realAlgebra) return NULL; 
    realAlgebra->add = real_add;
    realAlgebra->print = printReal;
    realAlgebra->read  = real_read;
    realAlgebra->multiply = real_multiply;
    realAlgebra->elementSize = sizeof(Real);
    realAlgebra->defaultInit = real_zero;
    
    }
    return realAlgebra; // lazy initialization

}

typedef struct Complex
{
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

int complex_read(void* out)
{
    Complex* c = (Complex*)out;
    return (scanf("%lf %lf", &c->re, &c->im) == 2) ? 0 : 1;
}

void printComplex(const void* x)
{
    const Complex* c = (const Complex*)x;
    if(c->im >= 0) printf("%f+%fi",c->re, c->im);
    else printf("%f%fi",c->re, c->im);

}

static Algebra* complexAlgebra = NULL;
const Algebra* getComplexAlgebra(void) // почему const 
{
    if(!complexAlgebra)
    {
        complexAlgebra = malloc(sizeof(*complexAlgebra)); // почему нету * перед complexAlgebra
        if(!complexAlgebra) return NULL;
        complexAlgebra->add = complex_add;
        complexAlgebra->print = printComplex;
        complexAlgebra->read  = complex_read;
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
    const Algebra* type;     // type (Algebra*) — чтобы понимать, какого типа эти координаты и какие операции использовать
} Vector;

void vector_init(Vector* v, const Algebra* type) /* Подготовка к работе вектора. Vector a; vector_init(&a, getRealAlgebra());*/
{
    v->data = NULL; // Почему на NULL? Для чего
    v->size = 0;
    v->type = type;
}

int vector_allocation(Vector* v, size_t n)
{
    if(v == NULL || v->type == NULL) return 1;

    void* new_data = NULL;
    if( n > 0)
    {
        new_data = malloc(n * v->type->elementSize);
        if (new_data == NULL) return 1;
        
    }

    free(v->data);
    v->data = new_data;
    v->size = n;

    return 0;
}

void vector_free(Vector* v)
{
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->type = NULL;
}

void vector_print(const Vector* v)
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
            const char* p = (const char*)v->data + i * v->type->elementSize; // вынести функцию принта в филд

            v->type->print(p);
            

            separator = ", ";
        }

        printf("]\n");
        
}

int vector_add(const Vector* a, const Vector* b, Vector* out)
{
    if(a == NULL || b == NULL || out == NULL || a->type != b->type || a->size != b->size) return 1;
    else
    {
        vector_free(out); // если out был не пустой надо старое удалить иначе в инит мы тупо перезапишем и затрем старое и будет утечка
        vector_init(out, a->type); // проверка на пустоту

        if(vector_allocation(out, a->size) != 0) return 1;

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

int vector_dot(const Vector* a, const Vector* b, void* out) // переименровать void* в element ptr
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

int vector_read(Vector* v)
{
    if(!v || !v->type || !v->type->read) return 1;

    printf("Enter the dimension of the vector\n");
    size_t n = 0;

    if(scanf("%zu", &n) != 1) return 1;

    if(vector_allocation(v, n) != 0) return 1;
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
    

int main()
{
    Vector A, B; // маленькими буквами ( типы с больших букв, переменные и функции с маленьких)

    A.data = NULL; // Делаем так а не vector_free так как free(мусорный_адрес) => краш ,vector_free безопасен только если v->data либо NULL, либо адрес, который реально вернул malloc
    A.size = 0;
    A.type = NULL;

    B.data = NULL;
    B.size = 0;
    B.type = NULL;

    while (1)

    {
        printf("\n=== MENU ===\n");
        printf("1. Create/Recreate A (choose type + read)\n");
        printf("2. Create/Recreate B (choose type + read)\n");
        printf("3. Print A\n");
        printf("4. Print B\n");
        printf("5. Compute A + B and print\n");
        printf("6. Compute A * B and print\n");
        printf("0. Exit\n");
        printf("> ");

        int choice = -1;
        if(scanf("%d", &choice) != 1) 
        {
            printf("Input error.\n");
            break;
        }

        switch (choice)
        {
            case 0:
                vector_free(&A);
                vector_free(&B);
                return 0;

            case 1:
            {
                int t = 0;
                printf("Choose type for A: 1) Real  2) Complex\n> ");
                if(scanf("%d", &t) != 1 || (t != 1 && t != 2))
                {
                    printf("Invalid type.\n");
                    break;
                }

                vector_free(&A);

                if(t == 1) vector_init(&A, getRealAlgebra());
                else        vector_init(&A, getComplexAlgebra());
                if(vector_read(&A) != 0) 
                {
                    printf("Failed to read A.\n");
                    vector_free(&A);
                }
                break;
            }

            case 2: 
            {
                int t = 0;
                printf("Choose type for B: 1) Real  2) Complex\n> ");
                if(scanf("%d", &t) != 1 || (t != 1 && t != 2)) 
                {
                    printf("Invalid type.\n");
                    break;
                }

                vector_free(&B);

                if(t == 1) vector_init(&B, getRealAlgebra());
                else        vector_init(&B, getComplexAlgebra());

                if (vector_read(&B) != 0) 
                {
                    printf("Failed to read B.\n");
                    vector_free(&B);
                }
                break;
            }

            case 3:
                if (!A.type) printf("A is not created yet.\n");
                else 
                { 
                    printf("A = "); vector_print(&A);
                }
                break;

            case 4:
                if(!B.type) printf("B is not created yet.\n");
                else 
                { 
                    printf("B = "); vector_print(&B);
                }
                break;

            case 5:
            {
                if (!A.type || !B.type) 
                {
                    printf("Create A and B first.\n");
                    break;
                }

                Vector C;

                C.data = NULL;
                C.size = 0;
                C.type = NULL;

                if(vector_add(&A, &B, &C) != 0)
                {
                    printf("Amount error\n");
                }

                else
                {
                    printf("A + B = ");
                    vector_print(&C);
                }

                vector_free(&C);
                break;

            }

            case 6:
            {
                if (!A.type || !B.type) 
                {
                    printf("Create A and B first.\n");
                    break;
                }

                void* res = malloc(A.type->elementSize);
                if (!res)
                {
                    printf("Memory error.\n");
                    break;
                }

                if (vector_dot(&A, &B, res) != 0)
                {
                    printf("Different dimensions\n");
                    free(res);
                    break;
                }

                else
                {
                    printf("A * B = ");
                    A.type->print(res);      
                    printf("\n");
                }

                free(res);
                break;
            }

            default:
                printf("Unknown option.\n");
                break;

        }
    }

    vector_free(&A);
    vector_free(&B);
    return 0;
}

// библиотека гугл тест или самому написать тесты на ASERT