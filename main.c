#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

///funcao dada pelo professor para gerar matriz eleatoria
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

///funcao para ler a matriz de um arquivo de entrada
double *leMatriz(FILE *entrada, unsigned int *tamanho){

	double *mat = NULL;
	//recuperando o tamanho da matriz
	char buff[10];
	fscanf(entrada, "%s", buff);
	*tamanho = atoi(buff);

	///termina programa see alocacao falha
	if ( ! (mat = (double *) malloc(*tamanho**tamanho*sizeof(double))) ){
		printf("Erro: afalha na alocacao da matriz, terminando o programa.\n");
		exit(0);
	}
	
	unsigned int count = 0; 
	fscanf(entrada, "%s", buff);
	while(count != (*tamanho**tamanho) && !feof(entrada)){
		double el = atoi(buff);
		///salvando o elemento lido na matriz
		mat[count] = el;
		fscanf(entrada, "%s", buff);
		count ++;
	}
	return mat;
}

///funcao para imprimir a matriz
void imprimeMatriz(double *mat, unsigned int n){
	unsigned int iterador = 0;
	unsigned int fim =  n*n;
	while( iterador != fim ) {
		printf("%f ", mat[iterador]);
		iterador++;
		if((iterador%n) == 0){
			printf("\n");
		}
	}
}

///INICIO DO PROGRAMA PRINCIPAL
int main(int argc, char *argv[]){

///inicializando a seed de randomizacao
srand( 20172 );

///apontadores para arquivos de entrada/saida
FILE *entrada;
FILE *saida;

bool tem_entrada = false;
bool tem_saida = false;
bool tem_iteracoes = false;
bool eh_randomica = false;

int iteracoes = 0;
unsigned int tamanho_matriz = 0;

///a matriz na realidade sera um vetor onde o primeiro elemento
///fica na posicao 0 e o primeiro elemento da segunda linha
///fica na posicao n
double *matriz = NULL;

///esse for localiza os parametros passados para o argv 
///e faz as operacoes devidas de acordo com o parametro;
for(int i =0; i < argc; i++){
	//TODO: Ver com o professor pq tem duas opcoes -i
	if(strcmp(argv[i], "-i") == 0){
		///ler arquivo de entrada
		tem_entrada = true;
		entrada = fopen(argv[i+1], "r");
		if (entrada == NULL) {
			perror("Erro ao abrir o arquivo de entrada");
     		}
	}
	else if(strcmp(argv[i], "-o") == 0){
		///escrever no arquivo de saida
		tem_saida = true;
		saida = fopen(argv[i+1], "w");
		if (saida == NULL) {
			perror("Erro ao abrir o arquivo de saida");
		}
	}
	else if(strcmp(argv[i],"-r") == 0){
		///gerar matriz eleatoriamente
		eh_randomica = true;
		tamanho_matriz = atoi(argv[i+1]);
		matriz = generateSquareRandomMatrix(tamanho_matriz);
		imprimeMatriz(matriz, tamanho_matriz);
	}
	else if(strcmp(argv[i], "-it") == 0){
		///numero de iteracoes do refinamento
		tem_iteracoes = true;
		iteracoes = atoi(argv[i+1]);
	}
}

///checando o parametro obrigatorio
if(!tem_iteracoes){
	printf("Erro: O parametro -i k eh obrigatorio\n");
	printf("O formato da chamada eh:\ninvmat [-i arquivo_entrada] [-o arquivo_saida] [-r N] -i k\n");
	exit(0);
}

///lendo a matriz do arquivo de entrada
if(tem_entrada && (!eh_randomica)){
	matriz = leMatriz(entrada, &tamanho_matriz);
	imprimeMatriz(matriz, tamanho_matriz);
}

if(entrada != NULL){
	fclose(entrada);
}
if(saida != NULL){
	fclose(saida);
}

}
