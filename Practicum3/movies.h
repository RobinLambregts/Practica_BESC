#ifndef movies_H
#define movies_H

/**
 * @param movie1 Address of movie1
 * @param movie2 Address of movie2
 * @return < 0 if movie1 comes before movie2, = 0 if movie1 is equivalent to movie2, > 0 if movie1 comes after movie2
*/
int compareMovies(MovieData* movie1, MovieData* movie2) {
    if(!movie2) { return -1; };
    int compare = strcmp(movie1->name, movie2->name);
    return compare == 0 ? (movie1->year - movie2->year) : compare;
}

/**
 * @param name of the movie
 * @param year Year Of Release
 * @param actors[] List of participating actors
 * @param sizeOfArray length of actors array
 * @param actors head of linked list of all actors
 * @param head head of global Actors list
 * @return Address of newly allocated MovieData
*/
MovieData* newMovie(const char* name, int year, ActorData *actors[], int sizeOfArray, ActorNode** head) {
    char* AllocName = malloc(sizeof(strlen(name) + 1));
    if(!AllocName){ return NULL; }
    strcpy(AllocName, name);
    ActorNode** list = malloc(sizeof(ActorNode*));
    if(!list){ 
        free(AllocName);
        return NULL;
     }
    *list = NULL;
    for (size_t i = 0; i < sizeOfArray; i++)
    {
        ActorNode* searchList = NULL;
        searchActors(actors[i]->name[0], *head, &searchList);
        while (searchList)
        {
            if(compareActors(actors[i], searchList->actor) == 0) 
            {
                insertActor(searchList->actor, list);
                goto nextIteration;
            }
            searchList = searchList->next;
        }
        ActorNode* insertedActor = insertActor(actors[i], head);
        insertActor(insertedActor->actor, list);     
        nextIteration: ; 
    }
    MovieData* data = malloc(sizeof(MovieData));
    if(!data){ 
        free(AllocName);
        free(list);
        return NULL; 
    }
    data->actors = *list;
    data->name = AllocName;
    data->year = year;
    return data;
}

/**
 * inserts the given movie in an alphabetically sorted linked list, if name matches, order based on ascending release year
 * returns early if duplicate is found
 * 
 * @param movie Address of movie
 * @param head Address of head of linked list
*/
void insertMovie(MovieData* movie, MovieNode** head) {
    MovieNode** listLooper = head;
    while (*listLooper)
    {
        int compare = compareMovies(movie, (*listLooper)->movie);
        if (compare == 0)
        {
            free(movie);
            return;
        }
        if (compare > 0)
        {
            listLooper = &(*listLooper)->next;
        }     
        break;   
    }
    MovieNode* address = malloc(sizeof(MovieNode));
    if(!address){
        return NULL;
    }
    address->movie = movie;
    address->next = *listLooper;
    *listLooper = address; 
}

/**
 * @param movie Movie to delete
 * @param head Address of head of linked list
 * @return true on succesfull delete, otherwise false 
*/
bool deleteMovie(MovieData movie, MovieNode** head) {
    MovieNode** listLooper = head;
    while (*listLooper && compareMovies(&movie, (*listLooper)->movie) != 0)
    {
        listLooper = &(*listLooper)->next;
    }
    if(*listLooper)
    {
        MovieNode* toDelete = *listLooper;
        *listLooper = (*listLooper)->next;
        free(toDelete->movie->name);
        free(toDelete->movie);
        free(toDelete);
        return true;
    }
    return false;
}

/**
 * @param letter Letter used in search
 * @param input List of Movies
 * @param output List of found movies
*/
void searchMovies(const char letter, MovieNode* input, MovieNode** output) {
    MovieNode* listLooper = input;
    while (listLooper && listLooper->movie->name[0] != letter)
    {
        listLooper = listLooper->next;
    }
    while (listLooper && listLooper->movie->name[0] == letter)
    {
        insertMovie(listLooper->movie, output);
        listLooper = listLooper->next;
    }    
}

void printMovies(MovieNode* list) {
    MovieNode* temp = list;
    while (temp)
    {
        printf("\n%s, %d, Actors: \n", temp->movie->name, temp->movie->year);
        printActors(temp->movie->actors);
        temp = temp->next;
    }
}

#endif