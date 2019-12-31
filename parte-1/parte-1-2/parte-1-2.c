#include <stdio.h>          /* printf(), scanf()                            */
#include <pthread.h>        /* pthread_t, pthread_create()                  */
#include <stdlib.h>         /* rand(), exit(), NULL                         */
#include <string.h>         /* strcpy(), strcat()                           */
#include <unistd.h>         /* syscall()                                    */
#include <sys/syscall.h>    /* SYS_getiid                                   */

typedef struct nomeThread {
    char nome[10];
    char id[5];
} NomeThread;


void *exibirNome(void* nome) {
    char *nomeThread = (char*)nome;
    printf("Eu sou a thread %s e meu ID retornado por pthread_self() e %lu e o ID retornado por getid() e %ld\n", nomeThread, pthread_self(), syscall(SYS_gettid));
    pthread_exit(NULL);
}

void definirNomeThread(char *nomeThread, int id) {
    NomeThread novoNome;

    strcpy(novoNome.nome, "Thread_");
    sprintf(novoNome.id, "%d", id);
    strcat(novoNome.nome, novoNome.id);
    strcpy(nomeThread, novoNome.nome);   
}

int main() {
    pthread_t *thread;
    int N;
    
    char **vetorNomesThreads; //Matriz de caracteres
    
    printf("Informe o numero de threads: ");
    scanf("%d", &N);

    thread = (pthread_t*)malloc(sizeof(pthread_t) * N); //Alocando espacos para threads dinamicamente

    vetorNomesThreads = (char**)malloc(sizeof(char*) * N); //Alocando espacos para os nomes das threads dinamicamente

    for (int i = 0; i < N; i++) { //Preenchendo vetor de nomes com os nomes das threads
        vetorNomesThreads[i] = (char*)malloc(sizeof(char) * 10);
        definirNomeThread(vetorNomesThreads[i], i+1);
    }
    
    for (int i = 0; i < N; i++) { //Criando threads
        int erro = pthread_create(&(thread[i]), NULL, exibirNome, (void*)vetorNomesThreads[i]);
    }
    
    pthread_exit(NULL);
    return 0;
}