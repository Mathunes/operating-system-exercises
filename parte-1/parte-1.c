#include <stdio.h>          /* printf(), scanf()                            */
#include <pthread.h>        /* pthread_t, pthread_create()                  */
#include <stdlib.h>         /* rand(), exit(), NULL                         */
#include <string.h>         /* strcpy(), strcat()                           */
#include <unistd.h>         /* syscall()                                    */
#include <sys/syscall.h>    /* SYS_getiid                                   */

typedef struct NomeThread {
    char nome[10];
    char id[5];
} NomeThread;


void *exibir_nome(void* nome) {
    char *nome_thread = (char*)nome;
    printf("Eu sou a thread %s e meu ID retornado por pthread_self() e %lu e o ID retornado por getid() e %ld\n", nome_thread, pthread_self(), syscall(SYS_gettid));
    pthread_exit(NULL);
}

void definir_nome_thread(char *nome_thread, int id) {
    NomeThread novo_nome;

    strcpy(novo_nome.nome, "Thread_");
    sprintf(novo_nome.id, "%d", id);
    strcat(novo_nome.nome, novo_nome.id);
    strcpy(nome_thread, novo_nome.nome);   
}

int main() {
    pthread_t *thread;
    int n;
    
    char **vetor_nomes_threads; //Matriz de caracteres
    
    printf("Informe o numero de threads: ");
    scanf("%d", &n);

    thread = (pthread_t*)malloc(sizeof(pthread_t) * n); //Alocando espacos para threads dinamicamente

    vetor_nomes_threads = (char**)malloc(sizeof(char*) * n); //Alocando espacos para os nomes das threads dinamicamente

    for (int i = 0; i < n; i++) { //Preenchendo vetor de nomes com os nomes das threads
        vetor_nomes_threads[i] = (char*)malloc(sizeof(char) * 10);
        definir_nome_thread(vetor_nomes_threads[i], i+1);
    }
    
    for (int i = 0; i < n; i++) { //Criando threads
        int erro = pthread_create(&(thread[i]), NULL, exibir_nome, (void*)vetor_nomes_threads[i]);
    }
    
    pthread_exit(NULL);
    return 0;
}