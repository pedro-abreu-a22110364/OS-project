/**************************
* 						  *
* 		Constantes 		  *
*						  *
**************************/
#define NUM_PROCESSES 1
#define TOT_COUNT 1000
#define TEM_ERRO -1
#define CALCULO 0
#define FILHO 0
#define FEITO 1

/**************************
*						  *
* 		Includes 		  *
* 						  *
**************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>

/******************************************************************************
* 																			  *
*							 FUNCAO randNumGen 								  *
*																		      *
* 						Gerador de numero aleatorios						  *
*																			  *
******************************************************************************/
double randNumGen(){
	int random_value = rand(); //Gera numero aleatorio

	//Mete o valor gerado entre 0 and 1
	double unit_random = random_value / (double) RAND_MAX;

	return unit_random;
}

/******************************************************************************
* 																			  *
* 								FUNCAO fazCalculo 							  *
* 																			  *
* 							Algoritmo ficticio de teste 					  *
*																			  *
* 									Parametros: 							  *
* 					it_inicio - Numero onde comecam as iteracoes			  *
*					 it_fim - Numero onde terminam as iteracoes 			  *
* 				num_magico - Numero que se adicionar em cada iteracao 		  *
* 																			  *
******************************************************************************/
long fazCalculo(long it_inicio, long it_fim, long num_magico)
{
	double calculo = 0; // resultado do calculo
	double x, y; // variaveis que irao guardar numeros aleatorios

	//Formula de calculo fornecido
	for (long i = it_inicio; i < it_fim; i++) {
		// gera um numero random dependente da iteracao
		srand(i);
		x = randNumGen();
		y = randNumGen();
		double result = sqrtf((x * x) + (y * y));
		if (result < 1)
			calculo += num_magico;
	}
	return calculo;
}

/******************************************************************************
* 																			  *
* 									FUNCAO main 							  *
* 																			  *
******************************************************************************/
int main () {

	//Variaveis
	long nrIteracoes = 0, nrMagico = 0, iniIt = 0, fimIt = 0, fimItFixo = 0;
	long ret, fd;
	int pid = -1;
	long * res;

	printf("Quantas iteracoes?\n");
	scanf("%ld",&nrIteracoes);

	fimItFixo = nrIteracoes/4;
	fimIt = fimItFixo;

	printf("Qual o numero magico?\n");
	scanf("%ld",&nrMagico);

	//Criar memoria partilhada
	fd = shm_open("/shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("shm");
		exit(1);
	}

	//Definir tamanho da memoria partilhada
	ret = ftruncate(fd, sizeof(int));
	if (ret == -1)
	{
		perror("shm");
		exit(2);
	}

	//Mapear memoria
	res = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (res == MAP_FAILED)
	{
		perror("shm-mmap");
		exit(3);
	}


	*res = 0;
	for (int i = 0; i < 4; i++) {
		pid = fork();

		if (pid < 0)
		{
			printf("erro");
			exit(1);
		}

		if (pid == 0)
		{
			*res += fazCalculo(iniIt,fimIt,nrMagico);

			exit(0);
		}


		iniIt += fimItFixo;
		fimIt += fimItFixo;
	}

	for (int i = 0; i < 4; i++) {
		wait(NULL);
	}

	printf("Valor calculado: %ld\n",*res);
}
