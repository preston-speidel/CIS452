#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PANTRY 1
#define NUM_REFRIG 2
#define NUM_BAKER 3

// ANSI color codes
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define RESET   "\x1B[0m"

// Function to get a thread's color based on its ID
const char* getThreadColor(int id) {
    switch (id % 6) {
        case 0: return RED;
        case 1: return GREEN;
        case 2: return YELLOW;
        case 3: return BLUE;
        case 4: return MAGENTA;
        case 5: return CYAN;
        default: return RESET;
    }
}

typedef struct {
    sem_t flour;
    sem_t sugar;
    sem_t yeast;
    sem_t baking_soda;
    sem_t salt;
    sem_t cinnamon;
} Pantry;

typedef struct {
    sem_t eggs;
    sem_t milk;
    sem_t butter;
} Refrig;

typedef struct {
    sem_t mixer;
    sem_t bowl;
    sem_t spoon;
    sem_t oven;
    Pantry pantry[NUM_PANTRY];
    Refrig refrig[NUM_REFRIG];
} Kitchen;

// Struct to pass arguments to threads
typedef struct {
    Kitchen* kitchen;
    int id;
} ThreadArgs;

void initKitchen(Kitchen* kitchen) {
    sem_init(&kitchen->mixer, 0, 2);  // 2 mixers available
    sem_init(&kitchen->bowl, 0, 3);   // 3 bowls available
    sem_init(&kitchen->spoon, 0, 5);  // 5 spoons available
    sem_init(&kitchen->oven, 0, 1);   // 1 oven available

    // Initialize pantry semaphores
    for (int i = 0; i < NUM_PANTRY; i++) {
        sem_init(&kitchen->pantry[i].flour, 0, 1);
        sem_init(&kitchen->pantry[i].sugar, 0, 1);
        sem_init(&kitchen->pantry[i].yeast, 0, 1);
        sem_init(&kitchen->pantry[i].baking_soda, 0, 1);
        sem_init(&kitchen->pantry[i].salt, 0, 1);
        sem_init(&kitchen->pantry[i].cinnamon, 0, 1);
    }

    // Initialize refrigerator semaphores
    for (int i = 0; i < NUM_REFRIG; i++) {
        sem_init(&kitchen->refrig[i].eggs, 0, 1);
        sem_init(&kitchen->refrig[i].milk, 0, 1);
        sem_init(&kitchen->refrig[i].butter, 0, 1);
    }
}

void cookie(Kitchen* kitchen, int pantryId, int refrigId, int id) {
    const char* color = getThreadColor(id);

    printf("%sBaker %d: Gathering ingredients for baking cookies...\n", color, id);
    printf("%sBaker %d is using pantry %d.\n", color, id, pantryId);
    printf("%sBaker %d is using refrig %d.\n", color, id, refrigId);

    // Cookies: Flour, Sugar, Milk, Butter 
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->refrig[refrigId].milk);
    sem_wait(&kitchen->refrig[refrigId].butter);

    printf("%sBaker %d: Gathered (Flour, Sugar, Milk, Butter).\n", color, id);

    // Simulate baking process
    sem_wait(&kitchen->mixer);
    sem_wait(&kitchen->bowl);
    sem_wait(&kitchen->spoon);
    sem_wait(&kitchen->oven);
    printf("%sBaker %d made cookies\n", color, id);

    // Release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->refrig[refrigId].milk);
    sem_post(&kitchen->refrig[refrigId].butter);
    sem_post(&kitchen->mixer);
    sem_post(&kitchen->bowl);
    sem_post(&kitchen->spoon);
    sem_post(&kitchen->oven);
    printf("%sBaker %d: Returned (Flour, Sugar, Milk, Butter).\n", color, id);
    printf("%sBaker %d: Returned (Mixer, Bowl, Spoon, Oven).\n%s", color, id, RESET);
}

void pancakes(Kitchen* kitchen, int pantryId, int refrigId, int id){
const char* color = getThreadColor(id);

    printf("%sBaker %d: Gathering ingredients for baking Pancakes...\n", color, id);
    printf("%sBaker %d is using pantry %d.\n", color, id, pantryId);
    printf("%sBaker %d is using refrig %d.\n", color, id, refrigId);

    //Pancakes: Flour, Sugar, Baking soda, Salt, Egg, Milk, Butter
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->pantry[pantryId].baking_soda);
    sem_wait(&kitchen->pantry[pantryId].salt);
    sem_wait(&kitchen->refrig[refrigId].eggs);
    sem_wait(&kitchen->refrig[refrigId].milk);
    sem_wait(&kitchen->refrig[refrigId].butter);

    printf("%sBaker %d: Gathered (Flour, Sugar, Baking soda, Salt, Egg, Milk, Butter).\n", color, id);

    // Simulate baking process
    sem_wait(&kitchen->mixer);
    sem_wait(&kitchen->bowl);
    sem_wait(&kitchen->spoon);
    sem_wait(&kitchen->oven);
    printf("%sBaker %d made pancakes\n", color, id);

    // Release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->pantry[pantryId].baking_soda);
    sem_post(&kitchen->pantry[pantryId].salt);
    sem_post(&kitchen->refrig[refrigId].eggs);
    sem_post(&kitchen->refrig[refrigId].milk);
    sem_post(&kitchen->refrig[refrigId].butter);
    sem_post(&kitchen->mixer);
    sem_post(&kitchen->bowl);
    sem_post(&kitchen->spoon);
    sem_post(&kitchen->oven);
    printf("%sBaker %d: Returned (Flour, Sugar, Baking soda, Salt, Egg, Milk, Butter).\n", color, id);
    printf("%sBaker %d: Returned (Mixer, Bowl, Spoon, Oven).\n%s", color, id, RESET);
}

void pizza(Kitchen* kitchen, int pantryId, int refrigId, int id){
    const char* color = getThreadColor(id);

    printf("%sBaker %d: Gathering ingredients for baking pizza...\n", color, id);
    printf("%sBaker %d is using pantry %d.\n", color, id, pantryId);
    printf("%sBaker %d is using refrig %d.\n", color, id, refrigId);

    //pizza dough: Yeast, Sugar, Salt
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->pantry[pantryId].salt);

    printf("%sBaker %d: Gathered (Yeast, Sugar, Salt).\n", color, id);

    // Simulate baking process
    sem_wait(&kitchen->mixer);
    sem_wait(&kitchen->bowl);
    sem_wait(&kitchen->spoon);
    sem_wait(&kitchen->oven);
    printf("%sBaker %d made pizza\n", color, id);

    // Release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->pantry[pantryId].salt);
    sem_post(&kitchen->mixer);
    sem_post(&kitchen->bowl);
    sem_post(&kitchen->spoon);
    sem_post(&kitchen->oven);
    printf("%sBaker %d: Returned (Yeast, Sugar, Salt).\n", color, id);
    printf("%sBaker %d: Returned (Mixer, Bowl, Spoon, Oven).\n%s", color, id, RESET);
}

void pretzels(Kitchen* kitchen, int pantryId, int refrigId, int id){
    const char* color = getThreadColor(id);

    printf("%sBaker %d: Gathering ingredients for pretzels...\n", color, id);
    printf("%sBaker %d is using pantry %d.\n", color, id, pantryId);
    printf("%sBaker %d is using refrig %d.\n", color, id, refrigId);

    //Soft Pretzels: Flour, Sugar, Salt, Yeast, Baking Soda, Egg
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->pantry[pantryId].salt);
    sem_wait(&kitchen->pantry[pantryId].yeast);
    sem_wait(&kitchen->pantry[pantryId].baking_soda);
    sem_wait(&kitchen->refrig[refrigId].eggs);

    printf("%sBaker %d: Gathered (Flour, Sugar, Salt, Yeast, Baking Soda, Egg).\n", color, id);

    // Simulate baking process
    sem_wait(&kitchen->mixer);
    sem_wait(&kitchen->bowl);
    sem_wait(&kitchen->spoon);
    sem_wait(&kitchen->oven);
    printf("%sBaker %d made pretzels \n", color, id);

    // Release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->pantry[pantryId].salt);
    sem_post(&kitchen->pantry[pantryId].yeast);
    sem_post(&kitchen->pantry[pantryId].baking_soda);
    sem_post(&kitchen->refrig[refrigId].eggs);
    sem_post(&kitchen->mixer);
    sem_post(&kitchen->bowl);
    sem_post(&kitchen->spoon);
    sem_post(&kitchen->oven);
    printf("%sBaker %d: Returned (Flour, Sugar, Salt, Yeast, Baking Soda, Egg).\n", color, id);
    printf("%sBaker %d: Returned (Mixer, Bowl, Spoon, Oven).\n%s", color, id, RESET);
}

void cinnamonRolls(Kitchen* kitchen, int pantryId, int refrigId, int id){
    const char* color = getThreadColor(id);

    printf("%sBaker %d: Gathering ingredients for baking Cinnamon Rolls...\n", color, id);
    printf("%sBaker %d is using pantry %d.\n", color, id, pantryId);
    printf("%sBaker %d is using refrig %d.\n", color, id, refrigId);

    //Cinnamon rolls: Flour, Sugar, Salt, Cinnamon, Butter, Eggs, 
    sem_wait(&kitchen->pantry[pantryId].flour);
    sem_wait(&kitchen->pantry[pantryId].sugar);
    sem_wait(&kitchen->pantry[pantryId].salt);
    sem_wait(&kitchen->pantry[pantryId].cinnamon);
    sem_wait(&kitchen->refrig[refrigId].butter);
    sem_wait(&kitchen->refrig[refrigId].eggs);

    printf("%sBaker %d: Gathered (Flour, Sugar, Salt, Cinnamon, Butter, Eggs, ).\n", color, id);

    // Simulate baking process
    sem_wait(&kitchen->mixer);
    sem_wait(&kitchen->bowl);
    sem_wait(&kitchen->spoon);
    sem_wait(&kitchen->oven);
    printf("%sBaker %d made Cinnamon Rolls \n", color, id);

    // Release semaphores
    sem_post(&kitchen->pantry[pantryId].flour);
    sem_post(&kitchen->pantry[pantryId].sugar);
    sem_post(&kitchen->pantry[pantryId].salt);
    sem_post(&kitchen->pantry[pantryId].yeast);
    sem_post(&kitchen->pantry[pantryId].baking_soda);
    sem_post(&kitchen->refrig[refrigId].eggs);
    sem_post(&kitchen->mixer);
    sem_post(&kitchen->bowl);
    sem_post(&kitchen->spoon);
    sem_post(&kitchen->oven);
    printf("%sBaker %d: Returned (Flour, Sugar, Salt, Cinnamon, Butter, Eggs, ).\n", color, id);
    printf("%sBaker %d: Returned (Mixer, Bowl, Spoon, Oven).\n%s", color, id, RESET);
}

void* bakersBake(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int pantryNum = args->id % NUM_PANTRY;
    int refrigNum = args->id % NUM_REFRIG;

    cookie(args->kitchen, pantryNum, refrigNum, args->id);
    pancakes(args->kitchen, pantryNum, refrigNum, args->id);
    pizza(args->kitchen, pantryNum, refrigNum, args->id);
    pretzels(args->kitchen, pantryNum, refrigNum, args->id);
    cinnamonRolls(args->kitchen, pantryNum, refrigNum, args->id);

    return NULL;
}

int main() {
    pthread_t baker[NUM_BAKER];
    Kitchen kitchen; 
    ThreadArgs args[NUM_BAKER];  // Proper argument passing

    initKitchen(&kitchen);

    // Create baker threads
    for (int i = 0; i < NUM_BAKER; i++) {
        args[i].kitchen = &kitchen;
        args[i].id = i + 1;  // IDs start at 1
        pthread_create(&baker[i], NULL, bakersBake, &args[i]);
    }

    // Wait for all baker threads to finish
    for (int i = 0; i < NUM_BAKER; i++) {
        pthread_join(baker[i], NULL);
    }

    // Destroy all semaphores
    sem_destroy(&kitchen.mixer);
    sem_destroy(&kitchen.bowl);
    sem_destroy(&kitchen.spoon);
    sem_destroy(&kitchen.oven);

    for (int i = 0; i < NUM_PANTRY; i++) {
        sem_destroy(&kitchen.pantry[i].flour);
        sem_destroy(&kitchen.pantry[i].sugar);
        sem_destroy(&kitchen.pantry[i].yeast);
        sem_destroy(&kitchen.pantry[i].baking_soda);
        sem_destroy(&kitchen.pantry[i].salt);
        sem_destroy(&kitchen.pantry[i].cinnamon);
    }

    for (int i = 0; i < NUM_REFRIG; i++) {
        sem_destroy(&kitchen.refrig[i].eggs);
        sem_destroy(&kitchen.refrig[i].milk);
        sem_destroy(&kitchen.refrig[i].butter);
    }

    return 0;
}