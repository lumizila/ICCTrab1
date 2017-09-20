#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

///funcao dada pelo professor para capturar o tempo
double timestamp(void){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

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

///funcao para imprimir a matriz no stdout
void imprimeMatriz(double *mat, unsigned int n, double tempo_LU, double tempo_iter, double tempo_residuo){

	///imprimindo os tempos	e tamanho da matriz
	printf("# Tempo LU: %f\n# Tempo iter: %f\n# Tempo residuo: %f\n#\n%d\n", tempo_LU, tempo_iter, tempo_residuo, n);
 
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

///funcao para imprimir a matriz em um arquivo
void imprimeMatrizArquivo(double *mat, unsigned int n, double tempo_LU, double tempo_iter, double tempo_residuo, FILE *saida){

	///imprimindo os tempos	e tamanho da matriz
	fprintf(saida, "sdlka");
	fprintf(saida, "# Tempo LU: %f\n# Tempo iter: %f\n# Tempo residuo: %f\n#\n%d\n", tempo_LU, tempo_iter, tempo_residuo, n);

	unsigned int iterador = 0;
	unsigned int fim =  n*n;
	while( iterador != fim ) {
		fprintf(saida, "%f ", mat[iterador]);
		iterador++;
		if((iterador%n) == 0){
			fprintf(saida, "\n");
		}
	}
}

///funcao para fazer a fatoracao LU da matriz
double fatoracaoLU(double *L, double *U, double *matriz, unsigned int tamanho){
	//se no metodo de gauss a matriz restante tiver uma linha que tem apenas 0,
	//entao a matriz nao eh inversivel
	///capturando o tempo inicial
	double tempo_inicial = timestamp();
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
			//Se o pivo for 0, tenho que tentar trocar a linha do pivo com alguma abaixo dela
			//se nao encontrar nenhuma linha pra trocar entao essa matriz nao eh inversivel?
			//TODO: Confirmar isso com o professor
			//if(U[tamanho*pivo+pivo] == 0){
				//verifica todas as linhas abaixo do pivo para alguma linha que tenha
				//um elemento diferente de 0 na posicao do pivo. 
				//dai se tiver, eu troco de linhas na matriz U, na matriz L e na matriz identidade tbm ??

			//}
			///obtendo o fator para multiplicar a linha anterior e subtrair da 
			///linha atual 
			double fator = U[linha+coluna]/U[tamanho*pivo+pivo];
			L[linha+coluna] = fator;
			///este for faz a subtracao para cada el da linha
			for(int j = 0; j < tamanho; j++){
				U[linha+j] = U[linha+j]-(U[tamanho*pivo+j]*fator);
			}
		}
	}

	///testando se a matriz U ficou com alguma linha que eh apenas 0,
	///entao a matriz nao eh inversivel e devemos sair do programa
	bool linha_nula = true;
	for(int linha = 0; linha < tamanho; linha++){
		linha_nula = true;
		for(int coluna = 0; coluna < tamanho; coluna++){
			if(U[linha*tamanho+coluna] != 0){
				linha_nula = false;
			}
		}
		///se a boolean que indica se a linha eh nula tem valor verdadeiro
		///significa que a matriz na eh inversivel
		if(linha_nula){
			return -1;
		}
	}

	///capturando variacao de tempo
	tempo_inicial = timestamp() - tempo_inicial;
	return tempo_inicial;
}

double retrosubstituicao(double *L, double *U, double *Inversa, unsigned int tamanho){
	///capturando o tempo inicial
	double tempo_inicial = timestamp();

	double *identidade = NULL;
	if ( ! (identidade = (double *) malloc(tamanho*tamanho*sizeof(double))) ){
		printf("Erro: afalha na alocacao da matriz identidade, terminando o programa.\n");
		exit(0);
	}

	///gerando a matriz identidade
	for(int i = 0; i < tamanho; i++){
		for(int j = 0; j < tamanho; j ++){
			identidade[(i*tamanho) + j] = 0;
			if(i == j){
				identidade[(i*tamanho) + j] = 1;
			}
		}
	}

	///agora que temos a matriz identidade, a L e a U
	///de forma que matriz*inversa = identidade, eh possivel
	///calcular a inversa coluna por coluna na forma:
	///matriz*inversa[coluna x] = identidade[coluna x]
	///assim temos que Ax=b e A=LU, portanto L(Ux) = b, Ly = b e Ux = y

	///criando o vetor y para salvar as informacoes
	double y[tamanho];
	double x[tamanho];

	double multi;

	///este for eh para cada coluna da identidade
	for(int i = 0; i < tamanho; i++){
		///inicializando o vetor y e o vetor x
		for(int m = 0; m < tamanho; m++){
			y[m] = 0;
			x[m] = 0;
		}

		///Ly = b

		///este for eh para cada linha de y
		///faz-se a substituicao 
		for(int j = 0; j < tamanho; j++){
				///este for opera a multiplicacao entre a matriz L e o vetor y
				multi = 0;
				for(int k = 0; k < j; k++){
					multi = multi + L[tamanho*j+k]*y[k];
				}
				
				//y[j] = identidade[tamanho*j+i]/L[tamanho*j+j]
				//y[j] = (identidade[tamanho*j+i]-(L[tamanho*j]*y[0]))/L[tamamnho*j+j]
				//y[2] = (identidade[tamanho*j+i]-(L[tamanho*j]*y[0]+L[tamanho*j+1]*y[1]))/ L[tamanho*j+2]
 				//...

				y[j] = (identidade[tamanho*j+i] - multi) / L[tamanho*j+j];
		}

		///Ux = y

		///agora que tenho o valor de y referente a coluna i da identidade,
		///eh possivel calcular o vetor x referente a coluna i da identidade
		///com retrosubstituicao

		///para cada linha de x, comecando de baixo pra cima
		for(int j = (tamanho-1); j >= 0; j--){
			///este for opera a multiplicacao entre U e x
			multi = 0;
			for(int k = (tamanho-1); k > j; k--){
				multi = multi + U[tamanho*j+k]*x[k];
			}
			x[j] = (y[j] - multi) / U[tamanho*j+j]; 

			///colocando os resultados de x na matriz inversa
			Inversa[tamanho*j+i] = x[j];
		}
	}

	///capturando variacao de tempo
	tempo_inicial = timestamp() - tempo_inicial;
	return tempo_inicial;
}

double refinamento(double *matriz, double *Inversa, unsigned int tamanho_matriz, int iteracoes, double *tempo_iter, bool tem_saida, FILE *saida){
	int soma;
	///tempo medio para calcular a norma do residuo
	double tempo_medio;
	///R = I - A*inv(A)
	double *R = NULL;
	///I_aprox = matriz * Inversa
	double *I_aprox = NULL;
	double *identidade = NULL;

	if ( ! (identidade = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ){
		printf("Erro: afalha na alocacao da matriz identidade, terminando o programa.\n");
		exit(0);
	}

	if (!(R = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ) {
		printf("Erro: afalha na alocacao da matriz R, terminando o programa.\n");
		exit(0);
	}

	if (!(I_aprox = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ) {
		printf("Erro: afalha na alocacao da matriz I_aprox, terminando o programa.\n");
		exit(0);
	}

	///calculando I_aprox
	for(int i = 0; i < tamanho_matriz; i++) {
		for(int j = 0; j < tamanho_matriz; j++) {		
		    soma = 0;
		    for(int k = 0; k < tamanho_matriz; k++) {
				soma = soma + matriz[(i*tamanho_matriz) + k] * Inversa[(k*tamanho_matriz) + j];
		    }
			I_aprox[(i*tamanho_matriz) + j] = soma;
		}
	}

	///gerando a matriz identidade
	for(int i = 0; i < tamanho_matriz; i++){
		for(int j = 0; j < tamanho_matriz; j ++){
			identidade[(i*tamanho_matriz) + j] = 0;
			if(i == j){
				identidade[(i*tamanho_matriz) + j] = 1;
			}
		}
	}

	///calculando R
	for(int i = 0; i < tamanho_matriz; i++){
		for(int j = 0; j < tamanho_matriz; j++){
			R[(i*tamanho_matriz) + j] = identidade[(i*tamanho_matriz) + j] - I_aprox[(i*tamanho_matriz) + j];
		}
	}

	imprimeMatriz(R, tamanho_matriz, 0, 0, 0);

	///calcular norma de R
	///imprimir a norma de R na iteração
	///

	///TODO FAZER O REFINAMENTO AQUI
	///Para o refinamento temos que: matriz*Inversa Alterada(iter x) = Identidade Alterada(iter x)
	///Entao, para cada coluna da Inversa e da Identidade é preciso fazer um refinamento 
	///
	///TODO Inclui o tempo de somar o resultado do refinamento à solução original no tempo_iter

	return tempo_medio;
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
		//descomentar a linha de baixo para ver a matriz eleatoria criada
		//imprimeMatriz(matriz, tamanho_matriz);
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

double *Inversa = NULL;
if ( ! (Inversa = (double *) malloc(tamanho_matriz*tamanho_matriz*sizeof(double))) ){
	printf("Erro: afalha na alocacao da matriz I, terminando o programa.\n");
	exit(0);
}

double tempo_iter = retrosubstituicao(L, U, Inversa, tamanho_matriz);

///chamando a funcao de refinamento 
double tempo_residuo = refinamento(matriz, Inversa, tamanho_matriz, iteracoes, &tempo_iter, tem_saida, saida);

///TODO: testar se os prints estao de acordo com a especificacao
if(tem_saida){
	imprimeMatrizArquivo(Inversa, tamanho_matriz, tempo_LU, tempo_iter, tempo_residuo, saida);
}
else{
	imprimeMatriz(Inversa, tamanho_matriz, tempo_LU, tempo_iter, tempo_residuo);
}

///TODO: arrumar o Makefile para rodar o doxygen

///fechando os arquivos 
if(entrada != NULL){
	fclose(entrada);
}
if(saida != NULL){
	fclose(saida);
}

}
