#ifndef moviesActors_H
#define moviesActors_H

#include <stdio.h>
#include "movies.h"
#include "actors.h"

/**
 * Deletes the Actor reference in a movie
 * 
 * @param actor Actor to delete
 * @param movie Address of the movie
*/
void deleteActorMovie(ActorData actor, MovieData* movie) {
    ActorNode* toDelete = deleteActor(actor, &movie->actors);
    toDelete = NULL;
}

/** 
 * @param list List of movies
 * @param output Array of List of movies per beginning character
*/
void buildIndex(MovieNode* list, MovieNode* output[26]) {
    for (size_t i = 0; i < 26; i++)
    {
        searchMovies(i+65, list, &output[i]);
        if(!output[i]) {
            output[i] = NULL;
        }
    }
}

void printMovieNoActorInline(MovieNode* list) {
      MovieNode* temp = list;
    while (temp)
    {
        printf("%s, %d; ", temp->movie->name, temp->movie->year);
        temp = temp->next;
    }
}

#endif