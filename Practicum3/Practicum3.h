#ifndef Practicum3_H
#define Practicum3_H

#include <stdbool.h>

typedef struct ActorData ActorData;
typedef struct MovieData MovieData;
typedef struct ActorNode ActorNode;
typedef struct MovieNode MovieNode;

struct ActorData {
    char* name;
};

struct ActorNode {
    ActorNode* prev;
    ActorData* actor;
    ActorNode* next;
};

struct MovieData {
    char* name;
    int year;
    ActorNode* actors;
};

struct MovieNode {
    MovieData* movie;
    MovieNode* next;
};

void searchCoactor(ActorData actor, MovieNode* input, ActorNode* output);
void searchActors(const char letter, ActorNode* input, ActorNode** output);
void searchMovies(const char letter, MovieNode* input, MovieNode** output);
bool deleteMovie(MovieData movie, MovieNode** head);
void deleteActorGlobal(ActorData actor, MovieNode** movieHead, ActorNode** actorHead);
void deleteActorMovie(ActorData actor, MovieData* movie);
ActorNode* insertActor(ActorData* actor, ActorNode** head);
void insertMovie(MovieData* movie, MovieNode** head);
ActorData* newActor(const char* name);
MovieData* newMovie(const char* name, int year, ActorData *actors[], int sizeOfArray, ActorNode** head);

#endif