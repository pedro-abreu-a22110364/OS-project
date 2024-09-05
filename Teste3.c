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
#include <semaphore.h>
#include <pthread.h>

/**************************
*						  *
* 		Dados Thread	  *
* 						  *
**************************/
typedef struct
{
	char opc;
	long tempo;

} dadosThread;

/**************************
* 						  *
* Constantes e Variaveis  *
*						  *
**************************/
//numero de peças necessarias para produzir
#define NR_ESCAPE 1
#define NR_JANELAS 5
#define NR_PORTAS 4
#define NR_CHASSIS 1
#define NR_PNEUS 4
#define NR_JANTES 4
#define NR_PORCAS 16
#define NR_VALVULAS 16
#define NR_CILINDROS 4
#define NR_CAIXA 1
#define NR_RADIADORES 2
#define NR_CARROCARIA 1
#define NR_RODAS 4
#define NR_MOTOR 1

//numero de peças em stock
int nrEscape = 0;
int nrJanelas = 0;
int nrPortas = 0;
int nrChassis = 0;
int nrPneus = 0;
int nrJantes = 0;
int nrPorcas = 0;
int nrValvulas = 0;
int nrCilindros = 0;
int nrCaixa = 0;
int nrRadiadores = 0;
int nrCarrocaria = 0;
int nrRodas = 0;
int nrMotor = 0;

//dados da thread (opc e tempo)
dadosThread* dadosEscape;
dadosThread* dadosJanelas;
dadosThread* dadosPortas;
dadosThread* dadosChassis;
dadosThread* dadosPneus;
dadosThread* dadosJantes;
dadosThread* dadosPorcas;
dadosThread* dadosValvulas;
dadosThread* dadosCilindros;
dadosThread* dadosCaixa;
dadosThread* dadosRadiadores;

//threads
pthread_t threadEscape;
pthread_t threadJanelas;
pthread_t threadPortas;
pthread_t threadChassis;
pthread_t threadPneus;
pthread_t threadJantes;
pthread_t threadPorcas;
pthread_t threadValvulas;
pthread_t threadCilindros;
pthread_t threadCaixa;
pthread_t threadRadiadores;

//semaforos
sem_t* semEscape;
sem_t* semJanelas;
sem_t* semPortas;
sem_t* semChassis;
sem_t* semPneus;
sem_t* semJantes;
sem_t* semPorcas;
sem_t* semValvulas;
sem_t* semCilindros;
sem_t* semCaixa;
sem_t* semRadiadores;

//lista de espera com tempos
long listaEscape[100];
long listaJanelas[100];
long listaPortas[100];
long listaChassis[100];
long listaPneus[100];
long listaJantes[100];
long listaPorcas[100];
long listaValvulas[100];
long listaCilindros[100];
long listaCaixa[100];
long listaRadiadores[100];

//variaveis posiçao
int posAtualEscape = 0;
int posFinalEscape = 0;
int posAtualJanelas = 0;
int posFinalJanelas = 0;
int posAtualPortas = 0;
int posFinalPortas = 0;
int posAtualChassis = 0;
int posFinalChassis = 0;
int posAtualPneus = 0;
int posFinalPneus = 0;
int posAtualJantes = 0;
int posFinalJantes = 0;
int posAtualPorcas = 0;
int posFinalPorcas = 0;
int posAtualValvulas = 0;
int posFinalValvulas = 0;
int posAtualCilindros = 0;
int posFinalCilindros = 0;
int posAtualCaixa = 0;
int posFinalCaixa = 0;
int posAtualRadiadores = 0;
int posFinalRadiadores = 0;

/******************************************************************************
* 																			  *
* 						FUNCAO inicializarDados 							  *
* 																			  *
* 				Esta funcao inicializa todos os dados com opc				  *
* 																			  *
******************************************************************************/
void inicializarDados(){
	dadosThread* dadosEscape = malloc(sizeof (dadosThread));
	dadosEscape->opc = 'e';

	dadosThread* dadosJanelas = malloc(sizeof (dadosThread));
	dadosJanelas->opc = 'j';

	dadosThread* dadosPortas = malloc(sizeof (dadosThread));
	dadosPortas->opc = 'p';

	dadosThread* dadosChassis = malloc(sizeof (dadosThread));
	dadosChassis->opc = 'c';

	dadosThread* dadosPneus = malloc(sizeof (dadosThread));
	dadosPneus->opc = 'u';

	dadosThread* dadosJantes = malloc(sizeof (dadosThread));
	dadosJantes->opc = 't';

	dadosThread* dadosPorcas = malloc(sizeof (dadosThread));
	dadosPorcas->opc = 'o';

	dadosThread* dadosValvulas = malloc(sizeof (dadosThread));
	dadosValvulas->opc = 'v';

	dadosThread* dadosCilindros = malloc(sizeof (dadosThread));
	dadosCilindros->opc = 'l';

	dadosThread* dadosCaixa = malloc(sizeof (dadosThread));
	dadosCaixa->opc = 'x';

	dadosThread* dadosRadiadores = malloc(sizeof (dadosThread));
	dadosRadiadores->opc = 'r';
}

/******************************************************************************
* 																			  *
* 						FUNCAO inicializarThread 							  *
* 																			  *
* 				Esta funcao inicializa todas as threads						  *
* 																			  *
******************************************************************************/
void inicializarThread(){
	pthread_create(&threadEscape, NULL, adicionarStock, (void*)dadosEscape);
	pthread_create(&threadJanelas, NULL, adicionarStock, (void*)dadosJanelas);
	pthread_create(&threadPortas, NULL, adicionarStock, (void*)dadosPortas);
	pthread_create(&threadChassis, NULL, adicionarStock, (void*)dadosChassis);
	pthread_create(&threadPneus, NULL, adicionarStock, (void*)dadosPneus);
	pthread_create(&threadJantes, NULL, adicionarStock, (void*)dadosJantes);
	pthread_create(&threadPorcas, NULL, adicionarStock, (void*)dadosPorcas);
	pthread_create(&threadValvulas, NULL, adicionarStock, (void*)dadosValvulas);
	pthread_create(&threadCilindros, NULL, adicionarStock, (void*)dadosCilindros);
	pthread_create(&threadCaixa, NULL, adicionarStock, (void*)dadosCaixa);
	pthread_create(&threadRadiadores, NULL, adicionarStock, (void*)dadosRadiadores);
}


/******************************************************************************
* 																			  *
* 							FUNCAO inicializarSem 							  *
* 																			  *
* 					Esta funcao inicializa todos os semaforos				  *
* 																			  *
******************************************************************************/
void inicializarSem(){
	semEscape = sem_open("/escape", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semJanelas = sem_open("/janelas", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semPortas = sem_open("/portas", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semChassis = sem_open("/chassis", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semPneus = sem_open("/pneus", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semJantes = sem_open("/jantes", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semPorcas = sem_open("/porcas", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semValvulas = sem_open("/valvulas", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semCilindros = sem_open("/cilindros", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semCaixa = sem_open("/caixa", O_CREAT, S_IRUSR |S_IWUSR, 1);
	semRadiadores = sem_open("/radiadores", O_CREAT, S_IRUSR |S_IWUSR, 1);
}

/******************************************************************************
* 																			  *
* 							FUNCAO unlinkSem 								  *
* 																			  *
* 				Esta funcao da unlink em todos os semaforos					  *
* 																			  *
******************************************************************************/
void unlinkSem(){
	sem_unlink("/escape");
	sem_unlink("/janelas");
	sem_unlink("/portas");
	sem_unlink("/chassis");
	sem_unlink("/pneus");
	sem_unlink("/jantes");
	sem_unlink("/porcas");
	sem_unlink("/valvulas");
	sem_unlink("/cilindros");
	sem_unlink("/caixa");
	sem_unlink("/radiadores");
}

/******************************************************************************
* 																			  *
* 								FUNCAO dormida	 							  *
* 																			  *
* 		Esta funcao suspende a execucao do progama durante 10 no máximo 	  *
* 		Parametro tempo - Valor de espera em 10*milisegundos     			  *
* 																			  *
******************************************************************************/
void dormida (long tempo)
{
 long tmp = tempo*10;
 struct timespec ts;
 ts.tv_sec = tmp/1000;
 ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) *1000000;
 nanosleep(&ts, NULL);
}

/******************************************************************************
* 																			  *
* 						FUNCAO verificarAutomovel 							  *
* 																			  *
* 	Esta funcao verifica se temos peças suficientes para montar o automovel   *
* 																			  *
******************************************************************************/
void verificarAutomovel () {
	if (nrCarrocaria >= NR_CARROCARIA && nrRodas >= NR_RODAS && nrMotor >= NR_MOTOR) {
		nrCarrocaria--;
		nrRodas -= 4;
		nrMotor--;

		printf("Produziu A\n");
		fflush(stdout);
		fflush(stdin);
	}
}

/******************************************************************************
* 																			  *
* 						FUNCAO verificarCarrocaria 							  *
* 																			  *
* 	Esta funcao verifica se temos peças suficientes para montar a carroçaria  *
* 																			  *
******************************************************************************/
void verificarCarrocaria () {
	if (nrEscape >= NR_ESCAPE && nrJanelas >= NR_JANELAS && nrPortas >= NR_PORTAS && nrChassis >= NR_CHASSIS) {

		nrEscape -= NR_ESCAPE;
		nrJanelas -= NR_JANELAS;
		nrPortas -= NR_PORTAS;
		nrChassis -= NR_CHASSIS;

		nrCarrocaria++;
		printf("Produziu C\n");
		fflush(stdout);
		fflush(stdin);

		verificarAutomovel();
	}
}

/******************************************************************************
* 																			  *
* 							FUNCAO verificarRodas 							  *
* 																			  *
* 	Esta funcao verifica se temos peças suficientes para montar as rodas	  *
* 																			  *
******************************************************************************/
void verificarRodas () {
	if (nrPneus >= NR_PNEUS && nrJantes >= NR_JANTES && nrPorcas >= NR_PORCAS) {

		nrPneus -= NR_PNEUS;
		nrJantes -= NR_JANTES;
		nrPorcas -= NR_PORCAS;

		nrRodas += 4;
		printf("Produziu R\n");
		fflush(stdout);
		fflush(stdin);

		verificarAutomovel();
	}
}

/******************************************************************************
* 																			  *
* 							FUNCAO verificarMotor 							  *
* 																			  *
* 		Esta funcao verifica se temos peças suficientes para montar o motor   *
* 																			  *
******************************************************************************/
void verificarMotor () {
	if (nrValvulas >= NR_VALVULAS && nrCilindros >= NR_CILINDROS && nrCaixa >= NR_CAIXA && nrRadiadores >= NR_RADIADORES) {

		nrValvulas -= NR_VALVULAS;
		nrCilindros -= NR_CILINDROS;
		nrCaixa -= NR_CAIXA;
		nrRadiadores -= NR_RADIADORES;

		nrMotor++;
		printf("Produziu M\n");
		fflush(stdout);
		fflush(stdin);

		verificarAutomovel();
	}
}

/******************************************************************************
* 																			  *
* 							FUNCAO adicionarStock 							  *
* 																			  *
* 			Esta funcao adiciona peças e verifica se existem peças			  *
* 				suficientes para a fabricação de sub-fábricas				  *
* 																			  *
******************************************************************************/
void * adicionarStock(void * dados) {
	dadosThread * dadosTemp = (dadosThread*) dados;

	switch (dadosTemp->opc) {

	case 'e':
		sem_wait(semEscape);
		//printf("semEscape a bloquear\n");

		dadosTemp->tempo = listaEscape[posAtualEscape];

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrEscape++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semEscape);
		//printf("semEscape a desbloquear\n");

		posAtualEscape++;
		if (posAtualEscape == sizeof(listaEscape)) {
			posAtualEscape = 0;
		}
		break;

	case 'j':
		sem_wait(semJanelas);
		//printf("semJanelas a bloquear\n");

		dadosTemp->tempo = listaJanelas[posAtualJanelas];

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrJanelas++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semJanelas);
		//printf("semJanelas a desbloquear\n");

		posAtualJanelas++;
		if (posAtualJanelas == sizeof(listaJanelas)) {
			posAtualJanelas = 0;
		}
		break;

	case 'p':
		sem_wait(semPortas);
		//printf("semPortas a bloquear\n");

		dadosTemp->tempo = listaPortas[posAtualPortas];

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrPortas++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semPortas);
		//printf("semPortas a desbloquear\n");

		posAtualPortas++;
		if (posAtualPortas == sizeof(listaPortas)) {
			posAtualPortas = 0;
		}
		break;

	case 'c':
		sem_wait(semChassis);
		//printf("semChassis a bloquear\n");

		dadosTemp->tempo = listaChassis[posAtualChassis];

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrChassis++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semChassis);
		//printf("semChassis a desbloquear\n");

		posAtualChassis++;
		if (posAtualChassis == sizeof(listaChassis)) {
			posAtualChassis = 0;
		}
		break;

	case 'u':
		sem_wait(semPneus);
		//printf("semPneus a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrPneus++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semPneus);
		//printf("semPneus a desbloquear\n");

		posAtualPneus++;
		if (posAtualPneus == sizeof(listaPneus)) {
			posAtualPneus = 0;
		}
		break;

	case 't':
		sem_wait(semJantes);
		//printf("semJantes a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrJantes++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semJantes);
		//printf("semJantes a desbloquear\n");

		posAtualJantes++;
		if (posAtualJantes == sizeof(listaJantes)) {
			posAtualJantes = 0;
		}
		break;

	case 'o':
		sem_wait(semPorcas);
		//printf("semPorcas a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrPorcas++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semPorcas);
		//printf("semPorcas a desbloquear\n");

		posAtualPorcas++;
		if (posAtualPorcas == sizeof(listaPorcas)) {
			posAtualPorcas = 0;
		}
		break;

	case 'v':
		sem_wait(semValvulas);
		//printf("semValvulas a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrValvulas++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semValvulas);
		//printf("semValvulas a desbloquear\n");

		posAtualValvulas++;
		if (posAtualValvulas == sizeof(listaValvulas)) {
			posAtualValvulas = 0;
		}
		break;

	case 'l':
		sem_wait(semCilindros);
		//printf("semCilindros a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrCilindros++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semCilindros);
		//printf("semCilindros a desbloquear\n");

		posAtualCilindros++;
		if (posAtualCilindros == sizeof(listaCilindros)) {
			posAtualCilindros = 0;
		}
		break;

	case 'x':
		sem_wait(semCaixa);
		//printf("semCaixa a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrCaixa++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semCaixa);
		//printf("semCaixa a desbloquear\n");

		posAtualCaixa++;
		if (posAtualCaixa == sizeof(listaCaixa)) {
			posAtualCaixa = 0;
		}
		break;

	case 'r':
		sem_wait(semRadiadores);
		//printf("semRadiadores a bloquear\n");

		//printf("%c vai dormir %ld\n",dadosTemp->opc,dadosTemp->tempo);
		dormida(dadosTemp->tempo);
		//printf("%c dormiu %ld\n",dadosTemp->opc,dadosTemp->tempo);

		nrRadiadores++;

		printf("Produziu %c\n",dadosTemp->opc);
		fflush(stdout);
		fflush(stdin);

		sem_post(semRadiadores);
		//printf("semRadiadores a desbloquear\n");

		posAtualRadiadores++;
		if (posAtualRadiadores == sizeof(listaRadiadores)) {
			posAtualRadiadores = 0;
		}
		break;
	}

	verificarCarrocaria();
	verificarRodas();
	verificarMotor();

	free(dadosTemp);
}

/******************************************************************************
* 																			  *
* 									FUNCAO main 							  *
* 																			  *
******************************************************************************/
int main(int argc, char **argv) {
	char opc;
	long tempo;

	inicializarDados();

	inicializarThread();

	unlinkSem();

	inicializarSem();

	do {

		scanf(" %c%ld",&opc,&tempo);

		if (tempo <= 0 || tempo > 999) {
			opc = 'a';
		}

		switch (opc) {

			case 'e':
				//adicionar os elementos por ordem no array
				listaEscape[posFinalEscape] = tempo;
				posFinalEscape++;

				//verificar se chegou ao fim do array
				if (posFinalEscape == sizeof(listaEscape)) {
					posFinalEscape = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'j':
				//adicionar os elementos por ordem no array
				listaJanelas[posFinalJanelas] = tempo;
				posFinalJanelas++;

				//verificar se chegou ao fim do array
				if (posFinalJanelas == sizeof(listaJanelas)) {
					posFinalJanelas = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'p':
				//adicionar os elementos por ordem no array
				listaPortas[posFinalPortas] = tempo;
				posFinalPortas++;

				//verificar se chegou ao fim do array
				if (posFinalPortas == sizeof(listaPortas)) {
					posFinalPortas = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'c':
				//adicionar os elementos por ordem no array
				listaChassis[posFinalChassis] = tempo;
				posFinalChassis++;

				//verificar se chegou ao fim do array
				if (posFinalChassis == sizeof(listaChassis)) {
					posFinalChassis = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'u':
				//adicionar os elementos por ordem no array
				listaPneus[posFinalPneus] = tempo;
				posFinalPneus++;

				//verificar se chegou ao fim do array
				if (posFinalPneus == sizeof(listaPneus)) {
					posFinalPneus = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 't':
				//adicionar os elementos por ordem no array
				listaJantes[posFinalJantes] = tempo;
				posFinalJantes++;

				//verificar se chegou ao fim do array
				if (posFinalJantes == sizeof(listaJantes)) {
					posFinalJantes = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'o':
				//adicionar os elementos por ordem no array
				listaPorcas[posFinalPorcas] = tempo;
				posFinalPorcas++;

				//verificar se chegou ao fim do array
				if (posFinalPorcas == sizeof(listaPorcas)) {
					posFinalPorcas = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'v':
				//adicionar os elementos por ordem no array
				listaValvulas[posFinalValvulas] = tempo;
				posFinalValvulas++;

				//verificar se chegou ao fim do array
				if (posFinalValvulas == sizeof(listaValvulas)) {
					posFinalValvulas = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'l':
				//adicionar os elementos por ordem no array
				listaCilindros[posFinalCilindros] = tempo;
				posFinalCilindros++;

				//verificar se chegou ao fim do array
				if (posFinalCilindros == sizeof(listaCilindros)) {
					posFinalCilindros = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'x':
				//adicionar os elementos por ordem no array
				listaCaixa[posFinalCaixa] = tempo;
				posFinalCaixa++;

				//verificar se chegou ao fim do array
				if (posFinalCaixa == sizeof(listaCaixa)) {
					posFinalCaixa = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'r':
				//adicionar os elementos por ordem no array
				listaRadiadores[posFinalRadiadores] = tempo;
				posFinalRadiadores++;

				//verificar se chegou ao fim do array
				if (posFinalRadiadores == sizeof(listaRadiadores)) {
					posFinalRadiadores = 0;
				}

				pthread_create(&universalOpc, NULL, adicionarStock, (void*)dados);
				break;

			case 'q':
				dormida(tempo);
				exit(1);

			default:
				printf("Comando invalido\n");
				fflush(stdout);
				fflush(stdin);
			}

	} while(1);

}
