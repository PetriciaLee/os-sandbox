#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 100

int brojevi[100];  
int N;              
int M;              
int red_na_potezu;  

void *dretva1(void *arg) {
    int m = *(int *)arg;  
    for (int j = 0; j < m; j++) {
        
        while (red_na_potezu != 0) { /* radni wait */ }

        for (int i = 0; i < N; i++) {
            brojevi[i] = rand() % MAX_VAL;
            printf("%d. %d\n", i, brojevi[i]);
        }

        red_na_potezu = 1;  
    }
    return NULL;
}

void *dretva2(void *arg) {
    int m = *(int *)arg;

    for (int j = 0; j < m; j++) {

        while (red_na_potezu != 1) { /* radni wait */ }

        int zbroj = 0;
        for (int i = 0; i < N; i++)
            zbroj += brojevi[i];

        printf("Zbroj = %d\n", zbroj);

        red_na_potezu = 0;  
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Upotreba: %s N M\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    srand(time(NULL));

    red_na_potezu = 0;  

    pthread_t t1, t2;
    pthread_create(&t1, NULL, dretva1, &M);
    pthread_create(&t2, NULL, dretva2, &M);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}