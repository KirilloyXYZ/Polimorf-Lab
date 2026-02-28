#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "real.h"
#include "complex.h"

int main()
{
    Vector a, b; 

    a.data = NULL; 
    a.size = 0;
    a.type = NULL;

    b.data = NULL;
    b.size = 0;
    b.type = NULL;

    while (1)

    {
        printf("\n=== MENU ===\n");
        printf("1. Create/Recreate a (choose type + read)\n");
        printf("2. Create/Recreate b (choose type + read)\n");
        printf("3. Print a\n");
        printf("4. Print b\n");
        printf("5. Compute a + b and print\n");
        printf("6. Compute a * b and print\n");
        printf("0. Exit\n");
        printf("> ");

        int choice = -1;
        if (scanf("%d", &choice) != 1) 
        {
            printf("Input error.\n");
            break;
        }

        switch (choice)
        {
            case 0:
                vectorFree(&a);
                vectorFree(&b);
                return 0;

            case 1:
            {
                int t = 0;
                printf("Choose type for A: 1) Real  2) Complex\n> ");
                if (scanf("%d", &t) != 1 || (t != 1 && t != 2))
                {
                    printf("Invalid type.\n");
                    break;
                }

                vectorFree(&a);

                if ((t == 1 && vectorInit(&a, getRealAlgebra(), 0) != 0) || (t == 2 && vectorInit(&a, getComplexAlgebra(), 0) != 0))
                {
                    printf("Memory error.\n");
                    vectorFree(&a);
                    break;
                }
                
                
                if (vectorRead(&a) != 0) 
                {
                    printf("Failed to read a.\n");
                    vectorFree(&a);
                }
                break;
            }

            case 2: 
            {
                int t = 0;
                printf("Choose type for b: 1) Real  2) Complex\n> ");
                if (scanf("%d", &t) != 1 || (t != 1 && t != 2)) 
                {
                    printf("Invalid type.\n");
                    break;
                }

                vectorFree(&b);

                if ((t == 1 && vectorInit(&b, getRealAlgebra(), 0) != 0) || (t == 2 && vectorInit(&b, getComplexAlgebra(), 0) != 0))
                {
                    printf("Memory error.\n");
                    vectorFree(&b);
                    break;
                }

                if (vectorRead(&b) != 0) 
                {
                    printf("Failed to read B.\n");
                    vectorFree(&b);
                }
                break;
            }

            case 3:
                if (!a.type) printf("a is not created yet.\n");
                else 
                { 
                    printf("a = "); vectorPrint(&a);
                }
                break;

            case 4:
                if(!b.type) printf("b is not created yet.\n");
                else 
                { 
                    printf("b = "); vectorPrint(&b);
                }
                break;

            case 5:
            {
                if (!a.type || !b.type) 
                {
                    printf("Create a and b first.\n");
                    break;
                }

                Vector c;

                c.data = NULL;
                c.size = 0;
                c.type = NULL;

                if(vectorAdd(&a, &b, &c) != 0)
                {
                    printf("Amount error\n");
                }

                else
                {
                    printf("a + b = ");
                    vectorPrint(&c);
                }

                vectorFree(&c);
                break;

            }

            case 6:
            {
                if (!a.type || !b.type) 
                {
                    printf("Create a and b first.\n");
                    break;
                }

                void* res = malloc(a.type->elementSize);
                if (!res)
                {
                    printf("Memory error.\n");
                    break;
                }

                if (vectorDot(&a, &b, res) != 0)
                {
                    printf("Different dimensions\n");
                    free(res);
                    break;
                }

                else
                {
                    printf("a * b = ");
                    a.type->print(res);      
                    printf("\n");
                }

                free(res);
                break;
            }

            default:
                printf("Unknown option.\n");
                break;

        } // switch
    } // while

    
    return 0; // не попадем 
}