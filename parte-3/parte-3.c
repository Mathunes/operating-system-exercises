#include <stdio.h>          /* printf(), scanf()                            */
#include <stdlib.h>         /* rand(), exit(), NULL                         */
#include <unistd.h>         /* fork()                                       */
#include <time.h>           /* time()                                       */
#include <sys/shm.h>        /* shmget(), shmat()                            */
#include <sys/wait.h>       /* wait()                                       */
#include <sys/sem.h>        /* semget(), semctl()                           */

#define MAX     100000
#define SETVAL  16 // Comando utilizado como parametro semclt para indicar a passagem de valor para o semaforo

typedef struct vetor {
    int dados[MAX];
} Vetor;

Vetor *vetor, vetorComparacao;
struct sembuf sem_op; //especificacao das operacoes a serem realizadas no semaforo

void preencherVetor() {
    srand(time(NULL));
    int aux;
    for (int i = 0; i < MAX; i++) {
        aux = 1 + (rand() % 100);
        vetor[0].dados[i] = aux;
        vetorComparacao.dados[i] = aux;
    }
}

void deslocarElementos(int posicao) {
    for (int i = posicao; i < MAX-1; i++) {
        vetor[0].dados[i] = vetor[0].dados[i+1];
    }
}

void removerMultiplosDeCinco(int sem_set_id) {

    sem_op.sem_num = 0; //Numero (indice) do semaforo
    sem_op.sem_op = -1; //Operacao a ser executada (-1 subtracao, 1 soma, 0 suspender)
    sem_op.sem_flg = 0; //Sinalizadores
    semop(sem_set_id, &sem_op, 1);
    
    for (int i = MAX-1; i >= 0; i--) {
        if ((vetor[0].dados[i] % 5) == 0) {
            deslocarElementos(i);
        }
    }
    
    sem_op.sem_num = 0;
    sem_op.sem_op = 1; 
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

void removerPares(int sem_set_id) {
    
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);

    for (int i = MAX-1; i >= 0; i--) {
        if ((vetor[0].dados[i] % 2) == 0) {
            deslocarElementos(i);
        }
    }

    sem_op.sem_num = 0;
    sem_op.sem_op = 1; 
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

//========================================= VETOR DE VERIFICAÇÂO - DADOS REMOVIDOS COM UM PROCESSO =========================================

void deslocarElementos2(int posicao) {
    for (int i = posicao; i < MAX-1; i++) {
        vetorComparacao.dados[i] = vetorComparacao.dados[i+1];
    }
}

void removerVetorComparacao() {
    for (int i = MAX-1; i >= 0; i--) {
        if (((vetorComparacao.dados[i] % 2) == 0) || ((vetorComparacao.dados[i] % 5) == 0)) {
            deslocarElementos2(i);
        }
    }
}

//==========================================================================================================================================

int compararVetores() {
    for (int i = 0; i < MAX; i++) {
        if (vetor[0].dados[i] != vetorComparacao.dados[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    //Obtendo identificador do segmento de memoria compartilhada
    int shm_id = shmget(0, 400000, IPC_CREAT | 0600); //0 é a chave que os processos utilizaram para acessar a memoria compartilhada / tamanho / Direitos ou permissao de acesso do segmento

    //Obtendo endereço de memoria compartilhada
    char *shm_adrr = shmat(shm_id, NULL, 0);

    //Obtendo id do semaforo
    int sem_set_id = semget(0, 1, IPC_CREAT | 0600); // ou chave gerada por ftok

    //Definindo o valor do semaforo
    semctl(sem_set_id, 0, SETVAL, 1); // Global para o sistema (fornecido por SysV IPC - mecanismo de comunicacao)

    //Gravando o vetor na memoria compartilhada
    vetor = (Vetor*)shm_adrr;

    preencherVetor();

    int pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            removerMultiplosDeCinco(sem_set_id);
            exit(0);
        default:
            removerPares(sem_set_id);
            wait(0); //Esperando o filho
    }

    removerVetorComparacao();

    if(compararVetores()) {
        printf("Iguais\n");
    } else {
        printf("Diferentes\n");
    }

    return 0;
}