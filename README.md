# ICCTrab1
Primeiro trabalho de ICC 

Devemos encontrar a inversa de uma matriz, utilizando fatoracao LU.
Uma boa fonte para entender o processo de fatoracao eh o video deste link:
https://www.youtube.com/watch?v=dza5JTvMpzk&fref=gc&dti=352699571413581

*Importante: algumas matrizes nao tem inversa, devemos testar e dar uma mensagem de erro
para matrizes deste tipo, em caso de erros, uma mensagem explicando o ocorrido deve ser impressa em stderr e a execução do programa deve ser encerrada com código diferente de 0.* 

Um pseudo codigo para o programa:

```
M = incia_matriz() // pode ser randomica ou vir de um arquivo de entrada
if(!testa_inversivel(M))
  print("esta matriz nao eh inversivel")
  exit
else
  L = nova_matriz();
  U = nova_matriz();
  fatoracao_LU(M, L, U, tempo_LU);
  Inversa = nova_matriz();
  resolve_sistema_linear(L, U, Inversa, tempo_iter);
  refinamento(Inversa, tempo_residuo);
  imprime_matriz(Inversa, tempo_LU, tempo_iter, tempo_residuo);
  desaloca_matriz(M);
  desaloca_matriz(L);
  desaloca_matriz(U);```
  
