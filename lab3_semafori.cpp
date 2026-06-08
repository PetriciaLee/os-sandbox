#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

long long globalni_broj = 0;

sem_t sem_prazan; 
sem_t sem_pun;    

typedef struct {
    int id;
} dretva_podaci;

void* dretva_generiraj(void* arg) {
    int* parametri = (int*)arg;
    int ukupno_zadataka = parametri[0];
    int broj_radnika = parametri[1];

    printf("Dretva koja generira zadatke pocela je s radom. Broj zadataka=%d\n", ukupno_zadataka);

    for (int i = 0; i < ukupno_zadataka; i++) {
        sem_wait(&sem_prazan); 

        globalni_broj = ((long long)rand() % 1000000000) + 1;
        printf("Generiran broj %lld\n", globalni_broj);
        
        usleep(10000); 

        sem_post(&sem_pun); 
    }

    for (int i = 0; i < broj_radnika; i++) {
        sem_wait(&sem_prazan);
        globalni_broj = -1;
        sem_post(&sem_pun);
    }

    return NULL;
}

void* dretva_racunaj(void* arg) {
    int id = ((dretva_podaci*)arg)->id;
    printf("Dretva %d pocela je s radom.\n", id);

    while (1) {
        sem_wait(&sem_pun); 

        long long lokalni_broj = globalni_broj;

        if (lokalni_broj == -1) {
            sem_post(&sem_prazan); 
            break;
        }

        printf("Dretva %d. preuzela zadatak %lld\n", id, lokalni_broj);
        
        sem_post(&sem_prazan); 

        unsigned long long zbroj = 0;
        for (long long i = 1; i <= lokalni_broj; i++) {
            zbroj += i;
        }     
        
        printf("Dretva %d. zadatak=%lld zbroj=%llu\n", id, lokalni_broj, zbroj);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uporaba: %s <broj_radnih_dretvi (m)> <broj_zadataka (n)>\n", argv[0]);
        return 1;
    }

    int m = atoi(argv[1]); 
    int n = atoi(argv[2]); 

    if (m <= 0 || n <= 0) {
        printf("Argumenti moraju biti pozitivni cijeli brojevi veći od nule.\n");
        return 1;
    }

    srand(time(NULL));
    
    sem_init(&sem_prazan, 0, 1); 
    sem_init(&sem_pun, 0, 0);    

    pthread_t dretva_gen;
    
    pthread_t* radne_dretve = (pthread_t*)malloc(m * sizeof(pthread_t));
    dretva_podaci* podaci = (dretva_podaci*)malloc(m * sizeof(dretva_podaci));

    if (radne_dretve == NULL || podaci == NULL) {
        perror("Greška kod alokacije memorije");
        return 1;
    }

    int gen_parametri[2] = {n, m};
    pthread_create(&dretva_gen, NULL, dretva_generiraj, gen_parametri);

    for (int i = 0; i < m; i++) {
        podaci[i].id = i + 1;
        pthread_create(&radne_dretve[i], NULL, dretva_racunaj, &podaci[i]);
    }

    pthread_join(dretva_gen, NULL);
    for (int i = 0; i < m; i++) {
        pthread_join(radne_dretve[i], NULL);
    }

    sem_destroy(&sem_prazan);
    sem_destroy(&sem_pun);
    free(radne_dretve);
    free(podaci);

    return 0;
}
