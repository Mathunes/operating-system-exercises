#include <stdio.h>          /* printf(), scanf()                            */
#include <stdlib.h>         /* rand(), exit(), NULL                         */
#include <pthread.h>        /* pthread_t, pthread_attr_init()               */
#include <time.h>           /* time()                                       */
#include <semaphore.h>      /* sem_t(), sem_open(), sem_wait(), sem_post()  */

#define MAX 100000

int vetor[MAX];
int ultimo = MAX;

sem_t s; //Declarando semaforo

void deslocar_elemento(int posicao) {
    for (int i = posicao; i < ultimo-1; i++) {
        vetor[i] = vetor[i+1];
    }
}

void *remover_pares() {
    for (int i = MAX-1; i >= 0; i--) {
        sem_wait(&s); //Decrementa semaforo e bloqueia thread (se houver)
        if ((vetor[i] % 2) == 0) {
            deslocar_elemento(i);
        }
        sem_post(&s); //Incrementa semaforo e acorda thread (se houver)
    }
    pthread_exit(NULL);
}

void *remover_multiplos_de_cinco() {
    for (int i = MAX-1; i >= 0; i--) {
        sem_wait(&s); //Decremente semaforo e bloqueia thread
        if ((vetor[i] % 5) == 0) {
            deslocar_elemento(i);
        }
        sem_post(&s); //Incrementa semaforo e acorda thread
    }
    pthread_exit(NULL);
}

//========================================= VETOR DE VERIFICAÇÂO - DADOS REMOVIDOS SEM THREADS =========================================
int vetor_verificacao[MAX];
int ultimo2 = MAX;

void deslocar_elemento_verificacao(int posicao) {
    for (int i = posicao; i < ultimo2-1; i++) {
        vetor_verificacao[i] = vetor_verificacao[i+1];
    }    
}

void remover_elementos_verificacao() {
    for (int i = MAX-1; i >= 0; i--) {
        if (((vetor_verificacao[i] % 5) == 0) || ((vetor_verificacao[i] % 2) == 0)) {
            deslocar_elemento_verificacao(i);
        }
    }
}
//======================================================================================================================================

void preencher_vetores() {
    srand(time(NULL));
    int aux;
    for (int i = 0; i < MAX; i++) {
        aux = 1 + rand() % 100;
        vetor[i] = aux;
        vetor_verificacao[i] = aux;
    }
}

int comparar_vetores() {
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != vetor_verificacao[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    pthread_t thread[2];

    pthread_attr_t attr; //Atributo que sera usado para indicar que as threads são "juntaveis"
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); //Indicando que a thread é juntavel

    sem_init(&s, 0, 1); //Iniciando semaforo com valor 1

    preencher_vetores();

    remover_elementos_verificacao();

    pthread_create(&(thread[0]), &attr, remover_pares, NULL);
    pthread_create(&(thread[1]), &attr, remover_multiplos_de_cinco, NULL);

    pthread_attr_destroy(&attr);
    
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    if (comparar_vetores()) {
        printf("Iguais\n");
    } else {
        printf("Diferentes\n");
    }

    pthread_exit(NULL);
    return 0;
}