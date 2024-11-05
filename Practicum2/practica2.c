#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define CONSUMER_AMOUNT 2          // Amount of Consumer Threads
#define PRODUCER_AMOUNT 2          // Amount of Producer Threads
#define PURCHASE_QUANTITY 10        // Max amount of purchaseQuantity
#define BUFFER_SIZE 100              // Size of buffers
#define MAX_SLEEP_TIME 5            // Max wait before consumer consumes again
#define BEGIN_STOCK_SIZE 20         //Initial stock size

struct product{
    int productId, productCount;
};

struct user{
    int userId, productId, productQuantity;
};

/* Typedef Struct Buffer:
    struct user buffer[] -> Array of elements Buffer_Size Long
    size_t head -> Element nr that is first of list
    size_t tail -> Element nr that is last of list
    phtread_mutex_t mutex -> Mutex when accessing buffer
*/
typedef struct {
    struct user buffer[BUFFER_SIZE];
    size_t head, tail;
    pthread_mutex_t mutex;
} Buffer;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Creating Buffers For Consumer And Producer
Buffer consumerBuffer = {.head = 0, .tail = 0, .mutex = PTHREAD_MUTEX_INITIALIZER};
Buffer producerBuffer = {.head = 0, .tail = 0, .mutex = PTHREAD_MUTEX_INITIALIZER};

//Creating overflowbuffer
Buffer notFulfilled = {.head = 0, .tail = 0, .mutex=PTHREAD_MUTEX_INITIALIZER};
bool isBusy[CONSUMER_AMOUNT];
bool stockHasChanged;

/* bool requestToBuffer
    Buffer* Buffer -> pointer to buffer
    Struct user Data -> Data to add to buffer
    Returns; True -> Succesfully added to buffer, False -> Failed to add to buffer
*/
bool requestToBuffer(Buffer* buffer, struct user data)
{
    pthread_mutex_lock(&buffer->mutex);
    if ((buffer->head + 1) % BUFFER_SIZE == buffer->tail)
    {
        pthread_mutex_unlock(&buffer->mutex);
        return false;
    }
    buffer->buffer[buffer->head] = data;
    buffer->head = (buffer->head + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&buffer->mutex);
    return true;
}

/* void getFromBuffer
    Buffer* buffer -> Buffer to read from
    struct user** data -> Address Pointing to an empty struct user data
    Returns -> data = NULL when nothing found; else returns retrieved data to **data
*/
void getFromBuffer(Buffer* buffer, struct user** data)
{
    pthread_mutex_lock(&buffer->mutex);
    if (buffer->head == buffer->tail)
    {
        *data = NULL;
        pthread_mutex_unlock(&buffer->mutex);
    } else
    {
        **data = buffer->buffer[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer->mutex);
    }   
}

/* consumerThread
    Taking in it's unique id
    Will randomly create request and push them to the buffer;
    after succesfull add will wait random amount of time untill next add
*/
void* consumerThread(void* args)
{
    int consumerId = *(int*)args;
    while (true)
    {   
        while (isBusy[consumerId])
        {   
            //Busy Waiting untill request is handeled
            usleep(100);
        }        
        struct user consumer = {consumerId, rand()%BEGIN_STOCK_SIZE, rand()%PURCHASE_QUANTITY}; 
        while (!requestToBuffer(&consumerBuffer, consumer))
        {
            usleep(1000);
        }
        isBusy[consumerId] = true;
         sleep(rand()%MAX_SLEEP_TIME);
    }
}

/* producerThread
    Taking in it's unique id
    Will randomly create request and push them to the buffer;
    after succesfull add will wait random amount of time untill next add 
*/
void* producerThread(void* args)
{
    int producerId = *(int*)args;
    while (true)
    {
        struct user producer = {producerId, rand()%BEGIN_STOCK_SIZE, rand()%PURCHASE_QUANTITY}; 
        while (!requestToBuffer(&producerBuffer, producer))
        {
            usleep(1000);
        }
         sleep(rand()%MAX_SLEEP_TIME);
    } 
}

//Initializes stock[] with id's matching it's position in the array, with a random int productCount
void initializeStock(struct product stock[])
{
    for (size_t i = 0; i < BEGIN_STOCK_SIZE; i++)
    {
        stock[i].productId=i;
        stock[i].productCount=rand()%BEGIN_STOCK_SIZE;
    }
    
}

/*  handleRequest
    buffer -> Buffer from which requests will be processed
    stock -> array of products that are in stock
    isConsumer -> Will consume goods, else will produce
    Will get last item from given buffer, and process it by either subtracting(isCustomer) or adding(!isCustomer) to stock
*/
void handleRequest(Buffer* buffer, struct product stock[],bool isConsumer)
{
    struct user request;
    struct user* requestP = &request;
    getFromBuffer(buffer, &requestP);
    if(requestP)
    {
        for (size_t i = 0; i < BEGIN_STOCK_SIZE; i++)
        {
            if (request.productId == stock[i].productId)
            {
                pthread_mutex_lock(&mutex);
                if (isConsumer)
                {
                    if (request.productQuantity <= stock[i].productCount)
                    {
                        stock[i].productCount -= request.productQuantity;
                        printf("Consumer %d has bought item %d, %d times\n", request.userId, request.productId, request.productQuantity);
                        isBusy[request.userId] = false;
                    } else
                    {
                        printf("Consumer %d couldn't buy item %d, %d times\n", request.userId, request.productId, request.productQuantity);
                        requestToBuffer(&notFulfilled, request);                     
                    }  
                    stockHasChanged = false;
                    pthread_mutex_unlock(&mutex);
                    return; 
                }else
                {
                    stock[i].productCount += request.productQuantity;
                    printf("Producer %d has stocked item %d, %d times\n", request.userId, request.productId, request.productQuantity);
                    pthread_mutex_unlock(&mutex);
                    stockHasChanged = true;   
                    return; 
                }
            }
        }
    }
}

/*  server; handels requests from both buffers
    Firstly, will create a stock and initialize it
    In loop, will request from buffer, and if valid request, will handle it
*/
void server(){
    struct product stock[BEGIN_STOCK_SIZE];
    initializeStock(stock);
    while (true)
    {   
        handleRequest(&producerBuffer, stock, false);
        if (stockHasChanged)
        {
            handleRequest(&notFulfilled, stock, true);
            handleRequest(&consumerBuffer, stock, true);
        }        
        usleep(10);
    }
}

int main(){
    //initializes randomised seed
    // srand(time(NULL));
    srand(1);
    //create thread arrays
    pthread_t consumerThreads[CONSUMER_AMOUNT];
    pthread_t producerThreads[PRODUCER_AMOUNT];
    //initializes thread arrays with unique id
    for (int i = 0; i < CONSUMER_AMOUNT; ++i) {
        int *id = &i;
        pthread_create(&consumerThreads[i], NULL, consumerThread, id);
    }

    for (int i = 0; i < PRODUCER_AMOUNT; ++i) {
        int *id = &i;
        pthread_create(&producerThreads[i], NULL, producerThread, id);
    }
    //run server
    server();
    return 0;
}