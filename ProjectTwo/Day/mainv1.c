#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct{
    //Recipe list 
    // ingrients 
    //appliances
} Baker;

typedef struct {
    bool flour;
    bool sugar;
    bool yeast;
    bool bakingSoda;
    bool salt;
    bool cinnamon;
} Pantry;

typedef struct {
    bool eggs;
    bool milk;
    bool butter;
} frig;

typedef struct {
    int mixer;
    int bowl;
    int spoon;
    int oven;
    Pantry* pantry;
    frig* frigs;
} Kitchen;

Kitchen kitchen;
sem_t kitchenSemaphore;
FILE *logFile;

void setupKitchen(){
    kitchen.mixer = 2;
    kitchen.bowl = 3;
    kitchen.spoon = 5;
    kitchen.oven = 1;
    
    kitchen.pantry = malloc(sizeof(Pantry));
    kitchen.frigs = malloc(2 * sizeof(frig));

    kitchen.pantry->flour = true;
    kitchen.pantry->sugar = true;
    kitchen.pantry->yeast = true;
    kitchen.pantry->bakingSoda = true;
    kitchen.pantry->salt = true;
    kitchen.pantry->cinnamon = true;

    kitchen.frigs[0].eggs = true;
    kitchen.frigs[0].milk = true;
    kitchen.frigs[0].butter = true;

    kitchen.frigs[1].eggs = true;
    kitchen.frigs[1].milk = true;
    kitchen.frigs[1].butter = true;
}

void* accessKitchen(void* arg) {
    int threadId = *(int*)arg;
    free(arg);
    sem_wait(&kitchenSemaphore);

    fprintf(logFile, "Baker %d entered the kitchen.\n", threadId);
    fprintf(logFile, "Items available to chef %d: Mixer=%d, Bowl=%d, Spoon=%d, Oven=%d\n",
            threadId, kitchen.mixer, kitchen.bowl, kitchen.spoon, kitchen.oven);

    if (kitchen.mixer > 0 || kitchen.bowl > 0 || kitchen.spoon > 0 || kitchen.oven > 0) {
        if (kitchen.mixer > 0) {
            kitchen.mixer--;
            fprintf(logFile, "Baker %d took a mixer. Remaining: %d\n", threadId, kitchen.mixer);
        } else if (kitchen.bowl > 0) {
            kitchen.bowl--;
            fprintf(logFile, "Baker %d took a bowl. Remaining: %d\n", threadId, kitchen.bowl);
        } else if (kitchen.spoon > 0) {
            kitchen.spoon--;
            fprintf(logFile, "Baker %d bought spoon. Remaining: %d\n", threadId, kitchen.spoon);
        } else if (kitchen.oven > 0) {
            kitchen.oven--;
            fprintf(logFile, "Baker %d is using the oven. Remaining: %d\n", threadId, kitchen.oven);
        } else {
            fprintf(logFile, "Baker %d found no items.\n", threadId);
        }
    }
    

    fprintf(logFile, "Baker %d entered the kitchen.\n", threadId);

    // Check pantry ingredients
    fprintf(logFile, "Pantry items: Flour=%d, Sugar=%d, Yeast=%d, BakingSoda=%d, Salt=%d, Cinnamon=%d\n",
            kitchen.pantry->flour, kitchen.pantry->sugar, kitchen.pantry->yeast,
            kitchen.pantry->bakingSoda, kitchen.pantry->salt, kitchen.pantry->cinnamon);

    // Check refrigerator ingredients
    fprintf(logFile, "Refrigerator 1: Eggs=%d, Milk=%d, Butter=%d\n",
            kitchen.frigs[0].eggs, kitchen.frigs[0].milk, kitchen.frigs[0].butter);
    fprintf(logFile, "Refrigerator 2: Eggs=%d, Milk=%d, Butter=%d\n",
            kitchen.frigs[1].eggs, kitchen.frigs[1].milk, kitchen.frigs[1].butter);

    // Simulate taking ingredients
    if (kitchen.pantry->flour) {
        kitchen.pantry->flour = false;
        fprintf(logFile, "Baker %d took Flour from the pantry.\n", threadId);
    }

    if (kitchen.frigs[0].eggs) {
        kitchen.frigs[0].eggs = false;
        fprintf(logFile, "Baker %d took Eggs from Refrigerator 1.\n", threadId);
    }

    fprintf(logFile, "Baker %d left the kitchen.\n\n", threadId);
    fflush(logFile);
    sem_post(&kitchenSemaphore);
    return NULL;
}

int main() {
    logFile = fopen("sharedMem4.txt", "w");
    if (!logFile) {
        perror("Failed to open log file");
        return 1;
    }

    int numThreads = 5;
    pthread_t threads[numThreads];
    sem_init(&kitchenSemaphore, 0, 1);
    setupKitchen();

    for (int i = 0; i < numThreads; i++) {
        int* threadId = malloc(sizeof(int));
        *threadId = i + 1;
        pthread_create(&threads[i], NULL, accessKitchen, threadId);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&kitchenSemaphore);
    free(kitchen.pantry);
    free(kitchen.frigs);

    fprintf(logFile, "Final stock: Mixers=%d, Bowl=%d, Spoon=%d, Oven=%d\n",
            kitchen.mixer, kitchen.bowl, kitchen.spoon, kitchen.oven);
    fclose(logFile);
    return 0;
}
