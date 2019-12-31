#include <stdio.h>          /* printf(), scanf()                            */
#include <stdlib.h>         /* rand(), exit(), NULL                         */
#include <pthread.h>        /* pthread_t, pthread_attr_init()               */
#include <time.h>           /* time()                                       */
#include <semaphore.h>      /* sem_t(), sem_open(), sem_wait(), sem_post()  */

#define MAX 100000

int vetor[MAX];
int ultimo = MAX;

sem_t s; //Declarando semaforo

void deslocarElemento(int posicao) {
    for (int i = posicao; i < ultimo-1; i++) {
        vetor[i] = vetor[i+1];
    }
}

void *removerPares() {
    for (int i = MAX-1; i >= 0; i--) {
        sem_wait(&s); //Decrementa semaforo e bloqueia thread (se houver)
        if ((vetor[i] % 2) == 0) {
            deslocarElemento(i);
        }
        sem_post(&s); //Incrementa semaforo e acorda thread (se houver)
    }
    pthread_exit(NULL);
}

void *removerMultiplosDe5() {
    for (int i = MAX-1; i >= 0; i--) {
        sem_wait(&s); //Decremente semaforo e bloqueia thread
        if ((vetor[i] % 5) == 0) {
            deslocarElemento(i);
        }
        sem_post(&s); //Incrementa semaforo e acorda thread
    }
    pthread_exit(NULL);
}

//========================================= VETOR DE VERIFICAÇÂO - DADOS REMOVIDOS SEM THREADS =========================================
int vetorVerificacao[MAX];
int ultimo2 = MAX;

void deslocarElementoVerificacao(int posicao) {
    for (int i = posicao; i < ultimo2-1; i++) {
        vetorVerificacao[i] = vetorVerificacao[i+1];
    }    
}

void removerElementosVerificacao() {
    for (int i = MAX-1; i >= 0; i--) {
        if (((vetorVerificacao[i] % 5) == 0) || ((vetorVerificacao[i] % 2) == 0)) {
            deslocarElementoVerificacao(i);
        }
    }
}
//======================================================================================================================================

void preencherVetores() {
    srand(time(NULL));
    int aux;
    for (int i = 0; i < MAX; i++) {
        aux = 1 + rand() % 100;
        vetor[i] = aux;
        vetorVerificacao[i] = aux;
    }
}

int compararVetores() {
    for (int i = 0; i < MAX; i++) {
        if (vetor[i] != vetorVerificacao[i]) {
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

    preencherVetores();

    removerElementosVerificacao();

    pthread_create(&(thread[0]), &attr, removerPares, NULL);
    pthread_create(&(thread[1]), &attr, removerMultiplosDe5, NULL);

    pthread_attr_destroy(&attr);
    
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    if (compararVetores()) {
        printf("Iguais\n");
    } else {
        printf("Diferentes\n");
    }

    pthread_exit(NULL);
    return 0;
}