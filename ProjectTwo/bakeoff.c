#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PANTRY 1
#define NUM_REFRIG 2
#define NUM_BAKER 3

typedef struct {
    sem_t flour;
    sem_t sugar;
    sem_t yeast;
    sem_t baking_soda;
    sem_t salt;
    sem_t cinnamon;
} Pantry;

typedef struct{
    sem_t eggs;
    sem_t milk;
    sem_t butter;
} Refrig;

typedef struct{
    sem_t mixer;
    sem_t bowl;
    sem_t spoon;
    sem_t oven;
    Pantry pantry[NUM_PANTRY];
    Refrig refrig[NUM_REFRIG];
}Kitchen;

void initKitchen(Kitchen* kitchen){
    sem_open(&kitchen-> mixer, 0, 2);
    sem_open(&kitchen-> bowl, 0, 3);
    sem_open(&kitchen-> spoon, 0, 5);
    sem_open(&kitchen-> oven, 0, 1);

    for(int i = 0; i < NUM_PANTRY; i++){
        sem_open(&kitchen->pantry[i].flour, 0, 1);
        sem_open(&kitchen->pantry[i].sugar, 0, 1);
        sem_open(&kitchen->pantry[i].yeast, 0, 1);
        sem_open(&kitchen->pantry[i].baking_soda, 0, 1);
        sem_open(&kitchen->pantry[i].salt, 0, 1);
        sem_open(&kitchen->pantry[i].cinnamon, 0, 1);
    }

    for (int i = 0; i < NUM_REFRIG; i++) {
        sem_open(&kitchen->refrig[i].eggs, 0, 1);
        sem_open(&kitchen->refrig[i].milk, 0, 1);
        sem_open(&kitchen->refrig[i].butter, 0, 1);
    }
}

//Cookies: Flour, Sugar, Milk, Butter 
void cookie(Kitchen* kitchen, int pantryId, int refrigId, int id){
    printf("Baker %d: Gathering ingredients for baking cookies...\n", id);
    printf("Baker %d is using pantry %d.\n",id , pantryId);
    printf("Baker %d is using refig %d.\n",id , refrigId);
    
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->refrig[refrigId].milk);
    sem_wait(&kitchen->refrig[refrigId].butter);

    printf("Baker %d: Gathered (Flour, Sugar, Milk, Butter).\n", id);

    // Simulate baking process

    // Gather kitchen appliances

    // release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->refrig[refrigId].milk);
    sem_post(&kitchen->refrig[refrigId].butter);
    printf("Baker %d: Returned (Flour, Sugar, Milk, Butter).\n", id);
    };

void* bakersBake(void* arg){
    Kitchen* kitchen = (Kitchen*)arg;
    int id = *(int*)arg;
    int refigNum = id % NUM_REFRIG;
    int pantryNum = id % NUM_PANTRY;
    
    cookie(kitchen, pantryNum, refigNum, id);

    return NULL;
}

int main(){
    pthread_t baker[NUM_BAKER];
    Kitchen kitchen; 
    int bakerIds[NUM_BAKER];

    initKitchen(&kitchen);

    //create baker threads
    for(int i = 0; i < NUM_BAKER; i++){
        bakerIds[i] = i + 1;
        pthread_create(&baker[i], NULL, bakersBake, &bakerIds[i]);
    }

    // Wait for all baker threads to finish
    for (int i = 0; i < NUM_BAKER; i++) {
        pthread_join(baker[i], NULL);
    }
    // Close all semaphores
    sem_close(&kitchen.mixer);
    sem_close(&kitchen.bowl);
    sem_close(&kitchen.spoon);
    sem_close(&kitchen.oven);

    for (int i = 0; i < NUM_PANTRY; i++) {
        sem_close(&kitchen.pantry[i].flour);
        sem_close(&kitchen.pantry[i].sugar);
        sem_close(&kitchen.pantry[i].yeast);
        sem_close(&kitchen.pantry[i].baking_soda);
        sem_close(&kitchen.pantry[i].salt);
        sem_close(&kitchen.pantry[i].cinnamon);
    }

    for (int i = 0; i < NUM_REFRIG; i++) {
        sem_close(&kitchen.refrig[i].eggs);
        sem_close(&kitchen.refrig[i].milk);
        sem_close(&kitchen.refrig[i].butter);
    }
    return 0;
}