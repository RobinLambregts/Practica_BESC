#include "Practicum3.h"
#include "moviesActors.h"
#include "movies.h"
#include "actors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    ActorData *inputActors1[] = {
        newActor("DiCaprio"),
        newActor("Pitt"),
        newActor("Clooney"),
        newActor("Hanks"),
        newActor("Damon"),
        newActor("Depp"),
        newActor("Freeman"),
        newActor("Norton")
    };
    ActorData *inputActors2[] = {
        newActor("Pacino"),
        newActor("De Niro"),
        newActor("Freeman"),
        newActor("Nicholson"),
        newActor("Oldman"),
        newActor("Bale"),
        newActor("Cruise"),
        newActor("Ford"),
        newActor("Spacey"),
        newActor("Norton")
    };
    ActorData *inputActors3[] = {
        newActor("Eastwood"),
        newActor("Gyllenhaal"),
        newActor("Hoffman"),
        newActor("Redford"),
        newActor("Spacey"),
        newActor("Norton")
    };

    MovieNode** headMovie = malloc(sizeof(MovieNode*));
    *headMovie = NULL;
    ActorNode** headActor = malloc(sizeof(ActorNode*));
    *headActor = NULL;

    MovieData* inputMovies[] = {
        newMovie("Awesome", 2002, inputActors1, sizeof(inputActors1)/sizeof(inputActors1[0]), headActor),
        newMovie("Test", 2005, inputActors2, sizeof(inputActors2)/sizeof(inputActors2[0]), headActor),
        newMovie("Test", 1998, inputActors3, sizeof(inputActors3)/sizeof(inputActors3[0]), headActor)
    };

    for (size_t i = 0; i < sizeof(inputMovies)/sizeof(inputMovies[0]); i++)
    {
        insertMovie(inputMovies[i], headMovie);
    }    

    deleteActorMovie((ActorData){"Clooney"}, (*headMovie)->movie);

    MovieNode** test = malloc(sizeof(MovieNode*) * 26);
    for (size_t i = 0; i < 26; i++)
    {
        test[i] = NULL;
    }    

    buildIndex(*headMovie, test);

    printf("All Movies:");
    printMovies(*headMovie);
    printf("\nAll Actors: \n");
    printActors(*headActor);


    printf("\nIndex: ");
    for (int i = 0; i < 26; i++)
    {
        printf("\n%c: ", i+65);
        printMovieNoActorInline(test[i]);       
    }
    

    return 0;
}