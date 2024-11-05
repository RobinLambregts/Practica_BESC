#ifndef actors_H
#define actors_H



/**
 * @param actor1 => Address of actor1
 * @param actor2 => Address of actor2
 * @return < 0 if actor1 comes before actor2, = 0 if actor1 is equivalent to actor2, > 0 if actor1 comes after actor2 
*/
int compareActors(ActorData* actor1, ActorData* actor2) {
    if(!actor2) { return -1; };
   return strcmp(actor1->name, actor2->name);
}

/**
 * @name name of actor
 * @return Address of newly allocated ActorData
*/
ActorData* newActor(const char* name) {
    char* AllocName = malloc(strlen(name) + 1);
    if(!AllocName){ return NULL; }
    strcpy(AllocName, name);
    ActorData* data = malloc(sizeof(ActorData));
    if(!data){
        free(AllocName);
        return NULL;
    }
    data->name=AllocName;
    return data;
}

/**
 * inserts the given actor in an alphabetically sorted linked list
 * 
 * @param actor Address of actor
 * @param head Address of head of linked list
 * @return The address of the inserted node
*/
ActorNode* insertActor(ActorData* actor, ActorNode** head) {
    ActorNode** listLooper = head;
    while (*listLooper)
    {
        int compare = compareActors(actor, (*listLooper)->actor);
        if (compare == 0)
        {
            free(actor);
            return *listLooper;
        }
        if(compare > 0)
        {
            listLooper = &(*listLooper)->next;
            continue;
        }
        break;
    }
    ActorNode* address = malloc(sizeof(ActorNode));
    if(!address) { return NULL; }
    address->actor = actor;
    address->prev = NULL;
    address->next = *listLooper;
    if (*listLooper)
    {
        address->prev = (*listLooper)->prev;
        (*listLooper)->prev = address;
    }
    (*listLooper) = address;
    return address;
}

/**
 * @param actor Actor to delete
 * @param head Address of head of linked list
 * @return Address of the actor unlinked out of the list
*/
ActorNode* deleteActor(ActorData actor, ActorNode** head) {
    ActorNode** listLooper = head;
    while (*listLooper && compareActors(&actor, (*listLooper)->actor) != 0)
    {
        listLooper = &(*listLooper)->next;
    }
    if (*listLooper)
    {
        ActorNode* toDelete = *listLooper;
        *listLooper = (*listLooper)->next;
        (*listLooper)->prev = *listLooper;
        return toDelete;
    }
    return NULL;
}

/**
 * Deletes the Actor from the global list, as well as any reference in a movie
 * 
 * @param actor Actor to delete
 * @param movieHead Address of the global movie list
 * @param actoHead Address of the global actor list
*/
void deleteActorGlobal(ActorData actor, MovieNode** movieHead, ActorNode** actorHead) {
    MovieNode** listLooper = movieHead;
    while (*listLooper)
    {
        deleteActorMovie(actor, (*listLooper)->movie);
        listLooper = &(*listLooper)->next;
    }
    ActorNode* toDelete = deleteActor(actor, actorHead);
    if(toDelete)
    {
        free(toDelete->actor->name);
        free(toDelete->actor);
        free(toDelete);
    }
}

/**
 * @param letter Letter used in search
 * @param input List of actors
 * @param output List of found actors
*/
void searchActors(const char letter, ActorNode* input, ActorNode** output) {
    ActorNode* listLooper = input;
    while (listLooper && listLooper->actor->name[0] < letter)
    {
        listLooper = listLooper->next;
    }
    while (listLooper && listLooper->actor->name[0] == letter)
    {
        insertActor(listLooper->actor, output);
        listLooper = listLooper->next;
    }
}

/**
 * @param actor Actor to search costars for
 * @param input List of movies to search through
 * @param output List of co-actors
*/
void searchCoactor(ActorData actor, MovieNode* input, ActorNode* output) {
    MovieNode* listLooper = input;
    while (listLooper)
    {
        ActorNode* movieActors = NULL;
        searchActors(actor.name[0], listLooper->movie->actors, &movieActors);
        if(movieActors){
            ActorNode* ActorsLooper = movieActors;
            while (ActorsLooper)
            {
                if(compareActors(&actor, ActorsLooper->actor) == 0) {
                    free(deleteActor(actor, &movieActors));
                    break;         
                }
                ActorsLooper = ActorsLooper->next;
            }
            while (ActorsLooper)
            {
                insertActor(ActorsLooper->actor, &output);
            }
        }
        listLooper = listLooper->next;
    }
}

void printActors(ActorNode* list) {
    ActorNode* temp = list;
    while (temp)
    {
        if(temp->actor){
            printf("%s\n", temp->actor->name);
        }
        temp = temp->next;
    }
}

#endif