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

///funcao para fazer a fatoracao LU da matriz
double fatoracaoLU(double *L, double *U, double *matriz, unsigned int tamanho){
	//se no metodo de gauss a matriz restante tiver uma linha que tem apenas 0,
	//entao a matriz nao eh inversivel

	///inicializando a matriz L e a matriz U
	for(int i = 0; i < tamanho; i++){
		for(int j = 0; j < tamanho; j ++){
			U[(i*tamanho) + j] = matriz[(i*tamanho) + j];
			L[(i*tamanho) + j] = 0;
			if(i == j){
				L[(i*tamanho) + j] = 1;
			}
		}
	}

	///APLICANDO METODO DE GAUSS EM U
	int pivo;
	int linha;
	int coluna;
	///este for faz iterar para cada coluna
	for(int i = 1; i < tamanho; i++){
		pivo = i-1;
		coluna = i-1;
		///este for faz iterar para cada linha
		for(int k = i; k < tamanho; k++){
			linha = k*tamanho;
			///obtendo o fator para multiplicar a linha anterior e subtrair da 
			///linha atual
			//TODO: Se o pivo for 0, tenho que tentar trocar as linhas
			//para poder fazer a divisao abaixo:
			//Se nao ela vai dar NaN
			double fator = U[linha+coluna]/U[tamanho*pivo+pivo];
			L[linha+coluna] = fator;
			///este for faz a subtracao para cada el da linha
			for(int j = 0; j < tamanho; j++){
				U[linha+j] = U[linha+j]-(U[tamanho*pivo+j]*fator);
			}
		}
	}
	//TODO: testar se a matriz U ficou com alguma linha que eh apenas 0,
	// entao a matriz nao eh inversivel e devemos sair do programa
	return 0;
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

///fazendo a fatoracao L U
double *L = NULL;
double *U = NULL;
if ( ! (L = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ){
	printf("Erro: afalha na alocacao da matriz L, terminando o programa.\n");
	exit(0);
}
if ( ! (U = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ){
	printf("Erro: afalha na alocacao da matriz U, terminando o programa.\n");
	exit(0);
}

double tempo_LU = fatoracaoLU(L, U, matriz, tamanho_matriz);
///testa se inversivel
if(tempo_LU == -1){
	printf("Erro: a matriz nao eh inversivel");
	exit(1);
}

//double tempo_iter = retrosubstituicao();


if(entrada != NULL){
	fclose(entrada);
}
if(saida != NULL){
	fclose(saida);
}

}
