# operating-system-exercises
Resolution of exercises proposed by Professor Raphael Guerra for Operating Systems discipline

parte 1

1 - Faça um programa que crie "N" threads ("N" é um dado de entrada lido do teclado). Cada thread, quando executar, deverá imprimir na tela "Eu sou a thread <nome> e meu ID é YY", onde YY é o ID que o sistema operacional dá para a thread (tipo pthread_t, primeiro campo da função pthread_create). Explique as saídas do programa quando este é executado. NÃO USE VARIÁVEIS GLOBAIS!!!

Dica 1: para imprimir uma variável do tipo pthread_t use %u.  
Dica 2: quando criar uma thread com nome "thread_01", passe o nome completo como parâmetro no formato string.
Dica 3: pegue a ID da thread usando a função pthread_self().

2 - Tanto pthread_self() quanto gettid() retornam o ID da thread. Altere o programa do exercício 1 para imprimir a ID da thread usando ambas funções. Elas retornam o mesmo valor? Se não, pesquise qual a diferença entre elas.

Dica: Para chamar gettid, use instrução syscall(SYS_gettid) e faça include da biblioteca <sys/syscall.h>



parte 2

Implementar o seguinte programa:

+ Criar um vetor global de inteiros de tamanho 100.000

+ Preencher o vetor com números aleatórios entre 1 e 100

+ Criar 2 threads
	* T1 remove números pares do vetor
	* T2 remove números múltiplos de 5 do vetor
	
+ Remova do fim para o começo do vetor e mantenha a ordem original dos elementos

+ Teste o programa com e sem semáforos, observe o resultado e explique o que está acontecendo

+ Construa uma função que avalia se o resultado final está correto



parte 3 (meio ponto extra na média)

Faça o programa acima com processos. 

+ Use fork() para criar um processo filho que removerá os múltiplos de 5. 

+ Processo pai remove pares e verifica se resultado está correto no final. Pai precisa sincronizar para esperar filho antes de verificar resultado.

+ É necessário usar memória compartilhada. PDF explica como fazer.
