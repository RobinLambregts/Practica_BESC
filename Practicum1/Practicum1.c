//Daan Hollands & Robin Lambregts
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BIGGEST_FLOAT 10.0  // Biggest float to be generated
#define DELTA 0.000001      // Max floating-point error in comparison
#define INPUT_SIZE 100      // The length of the input structs array

unsigned int seed = 0;

struct position
{
    float x;
    float y;
    float z;
};

// randomFloat: returns a randomized float between 0 and BIGGEST_FLOAT
float randomFloat()
{
    return (((float)rand() / (float)(RAND_MAX)) * BIGGEST_FLOAT);
}

/*
    compareFloat:
    float float1 -> Floating point number 1
    float float2 -> Floating point number 2

    returns: true when floats are equal within error delta, false when not equal
*/
bool compareFloat(float float1, float float2)
{
    return fabs(float1 - float2) < DELTA;
}

/*
    generateInputStructs:
    struct position* input -> location of the array of structs that need to be generated
*/
void generateInputStructs(struct position* input)
{
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        if (i % 50 == 0)
        {
            srand(seed);
        }
        (input + i)->x = randomFloat();
        (input + i)->y = randomFloat();
        (input + i)->z = randomFloat();

        /*
        input[i].x = randomFloat();
        input[i].y = randomFloat();
        input[i].z = randomFloat();
        */
    }
}

/*
    assignIndicesToStruct:
    struct position* input -> location of the array of structs that need to be generated
    struct position** indices -> location of the array of pointers to said structs
*/
void assignIndicesToStruct(struct position* input, struct position** indices)
{
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        *(indices + i) = input + i;

        /*
        indices[i] = input + i;
        */
    }
}

/*
    compareStructPosition:
    struct position struct1 -> first struct of type position
    struct position struct2 -> second struct of type position
    return -> true when all members match, else false
*/
bool compareStructPosition(struct position struct1, struct position struct2)
{
    int tested = 0;
    if (compareFloat(struct1.x, struct2.x))
    {
        tested++;
    }
    if (compareFloat(struct1.y, struct2.y))
    {
        tested++;
    }
    if (compareFloat(struct1.z, struct2.z))
    {
        tested++;
    }
    return (tested == 3);
}

/*
    checkForDuplicates
    struct position toCheck -> Given struct to compare
    struct position list[] -> List of structs to compare against
    struct position** returnAddress -> Location of the Address if a duplicate struct is found
    return -> true when duplicate found, else false
*/
bool checkForDuplicates(struct position toCheck, struct position* list, struct position** returnAddress)
{
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        struct position zeroStruct = { 0, 0, 0 };
        if (compareStructPosition(zeroStruct, *(list + i)))    // list[i]
        {
            memcpy(list + i, &toCheck, sizeof(struct position));
            break;
        }
        else if (compareStructPosition(toCheck, *(list + i)))    //list[i]
        {
            *returnAddress = list + i;
            return true;
        }
    }
    return false;
}

/*
    assignStructsNoDuplicates
    struct position input[] -> Given list of input structs
    struct position* indices[] -> List of references to output structs
    struct position output[] -> List of non-duplicate output structs
*/
void assignStructsNoDuplicates(struct position* input, struct position** indices, struct position* output)
{
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        struct position* tempAddress = NULL;
        struct position** tempAddressP = &tempAddress;
        if (checkForDuplicates(*(input + i), output, tempAddressP))
        {
            *(indices + i) = tempAddress;    //indices[i]
        }
        else
        {
            memcpy(output + i, input + i, sizeof(struct position));
            *(indices + i) = output + i;    //indices[i]
        }
    }
}

int main()
{
    seed = (unsigned int)time(NULL); // Set randomized seed
    srand(seed);

    struct position inputPosition[INPUT_SIZE] = { 0 }; // Defining & generating input
    generateInputStructs(inputPosition);

    struct position outputPosition[INPUT_SIZE] = { 0 };
    struct position* indices[INPUT_SIZE] = { NULL }; // Defining & assigning indices array

    // assignIndicesToStruct(inputPosition, indices);

    assignStructsNoDuplicates(inputPosition, indices, outputPosition);

    // Printing indices array
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        if (*(indices + i) != NULL)
        {
            printf("%i -> x:%f y:%f z:%f\n", i + 1, (*(indices + i))->x, (*(indices + i))->y, (*(indices + i))->z);
         // printf("%i -> x:%f y:%f z:%f\n", i + 1, indices[i].x, indices[i].y, indices[i].z;
        }
    }

    return 0;
}
