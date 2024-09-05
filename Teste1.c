#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void dormida (long tempo)
{
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = tempo;
	nanosleep(&ts, NULL);
}

int main(int argc, char **argv){
	int nrClientes,nrFifo;
	char comando[500];

	printf("Quantos clientes?\n");
	scanf("%d",&nrClientes);

	if (argc >= 2)
	{
		sprintf (comando, "/bin/bash ./testscript.sh %s &", argv[1]);
		system(comando);
	}
	else
	{
		printf ("Numero de argumentos invalido\n");
		exit(EXIT_SUCCESS);
	}


	for(nrFifo = 1; nrFifo <= nrClientes; nrFifo++) {
		char mypath[20];

			sprintf(mypath,"%s%d.txt","fifo",nrFifo);

			mkfifo(mypath, 0666);

			printf("FIFO %s criado com sucesso!\n",mypath);
	}

	for (int i = 1; i <= nrClientes; i++) {
		char nomeFifo[20];
		char msg[20];
		int pid = -1;
		long time;

		FILE * stream;
		char c;

		sprintf(nomeFifo,"%s%d.txt","fifo",i);

		pid = fork();

		if (pid < 0) {
			printf("erro");
			exit(1);
		}

		if (pid == 0) {
			stream = fopen(nomeFifo,"r");

					if (stream == NULL)
						printf("Erro na abertura do ficheiro");

					fscanf(stream,"%s %ld",msg,&time);

					dormida(time);

					printf("Recebeu mensagem %s %ld de %s\n",msg,time,nomeFifo);

					fclose(stream);

					exit(0);
		}

	}

	while (wait(NULL) >= 0) {

	}

	return 0;
}
