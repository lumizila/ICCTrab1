#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//funcao dada pelo professor para gerar matriz eleatoria
double *generateSquareRandomMatrix( unsigned int n )
{
	double *mat = NULL;

	/* return NULL if memory allocation fails */
	if ( ! (mat = (double *) malloc(n*n*sizeof(double))) )
		return (NULL);

	/* generate a randomly initialized matrix in row-major order */
	double *ptr = mat;
	double *end = mat + n*n;

	double invRandMax = 1.0/(double)RAND_MAX;

	while( ptr != end ) {
		*ptr++ = (double)rand() * invRandMax;
	}

	return (mat);
}


//INICIO DO PROGRAMA PRINCIPAL
int main(int argc, char *argv[]){

//inicializando a seed de randomizacao
srand( 20172 );

//apontadores para arquivos de entrada/saida
FILE *entrada;
FILE *saida;
bool tem_entrada = false;
bool tem_saida = false;

unsigned int tamanho_matriz = 0;
double *matriz = NULL;

//esse for localiza os parametros passados para o argv 
//e faz as operacoes devidas de acordo com o parametro;
for(int i =0; i < argc; i++){
	if(strcmp(argv[i], "-i") == 0){
		//ler arquivo de entrada
		tem_entrada = true;
		entrada = fopen(argv[i+1], "r");
		if (entrada == NULL) {
			perror("Erro ao abrir o arquivo de entrada");
     		}
	}
	else if(strcmp(argv[i], "-o") == 0){
		//escrever no arquivo de saida
		tem_saida = true;
		saida = fopen(argv[i+1], "w");
		if (saida == NULL) {
			perror("Erro ao abrir o arquivo de saida");
		}
	}
	else if(strcmp(argv[i],"-r") == 0){
		//gerar matriz eleatoriamente
		tamanho_matriz = atoi(argv[i+1]);
		matriz = generateSquareRandomMatrix(tamanho_matriz);
	}
	else if(strcmp(argv[i], "-i") == 0){
		//numero de iteracoes do refinamento
	}
}

if(entrada != NULL){
	fclose(entrada);
}
if(saida != NULL){
	fclose(saida);
}

}
