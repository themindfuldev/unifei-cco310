/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 7 - Entrega: 22/06/2005                                              *
*                                                                              *
*   Objetivo: Ordenar 10 arquivos utilizando Ordena��o Multidirecional         *
*                                                                              *
*   Nome: Ramon Fortes Pereira                  Matr�cula: 12638               *
*   Nome: Tiago Romero Garcia                   Matr�cula: 12643               *
*   Nome: Nathalia Gon�alves Candido            Matr�cula: 12744               *
*   Ci�ncia da Computa��o - Universidade Federal de Itajub�                    *
*                                                                              *
*******************************************************************************/

/********************************Bibliotecas***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
/******************************************************************************/

/********************************Constantes************************************/
#define	numero_de_arquivos		10
#define  MAX							2 * numero_de_arquivos
#define	maximo_16bits				65536
#define	numeros_por_vetor			500
#define	quantidade_de_numeros   numeros_por_vetor * numero_de_arquivos
#define	vazio							-1
#define	tamanho_nome_arquivos	80
#define	nome_de_arquivo			"Arquivo"
#define	extensao						".dat"
#define	nome_arquivo_final		"ArqFinal.Dat"
/******************************************************************************/

/*********************************Prot�tipos***********************************/
void Ordenacao(int vet[MAX], FILE *arquivo[numero_de_arquivos]);
void Multid(int raiz, int vet[MAX], FILE *arquivo[numero_de_arquivos]);
void ArvoreHeap(int raiz, int vet[MAX]);
int carrega_vetor(char caminho[], int vetor[], int tamanho_vetor);
int aleatorio_16bits(void);
void vetor_de_aleatorio(int vetor[], const int quantidade);
void zera_vetor(int vetor[], const int quantidade);
int grava_vetor(char caminho[], FILE *&arquivo, int vetor[], int tamanho_vetor);
void gera_10_vetores_aleatorios(int vetor[numero_de_arquivos][numeros_por_vetor]);
void Heap(int inferior, int superior, int numeros[]);
void Heap_Sort(int numeros[], int quantidade);
void ordenando_vetores(int vetor[numero_de_arquivos][numeros_por_vetor]);
void gera_nome_arquivo(char nomes[numero_de_arquivos][tamanho_nome_arquivos]);
void gera_10_arquivos(char nomes[numero_de_arquivos][tamanho_nome_arquivos], FILE *arquivo[]);
int LeDado(FILE *arquivo);
void Imprime(void);
void GravaDados(FILE *arquivo, int dado);
/******************************************************************************/

/*********************************Principal************************************/
void main ()
{
   // Para armazenar os nomes do arquivos
   char nomes_arquivos[numero_de_arquivos][tamanho_nome_arquivos];
   // Para armazenar os arquivos
 	FILE *arquivos[numero_de_arquivos];
	// �rvore heap
   int vet[MAX];

   // Gera 10 arquivos com n�meros aleat�rios pr�-ordenados
   gera_10_arquivos(nomes_arquivos, arquivos);

	// Ordena estes 10 arquivos por Ordena��o Multidirecional
   Ordenacao(vet, arquivos);

	// Imprime
   Imprime();
}
/******************************************************************************/

/*********************************aleatorio************************************/
// Devolve um n�mero aleat�rio de 16 bits
int aleatorio_16bits(void)
{
   int aleatorio;
   aleatorio = rand() % maximo_16bits;
   return aleatorio;
}

/*****************************vetor_de_aleatorio*******************************/
// Devolve um vetor com n�meros aleat�rio de 16 bits sem ser repetido
void vetor_de_aleatorio(int vetor[], const int quantidade)
{
   // Para marcar quais n�meros j� for�o gerados
   int numeros_gerados[maximo_16bits];
   // Para receber o n�mero aleat�rio
   int aleatorio;
   // Para contar as casas do vetor
   int contador;

   // Zera o vetor
   zera_vetor(numeros_gerados, maximo_16bits);

   // Inicializa o gerador de aleat�rios
   randomize();

   // Enquanto n�o completar o vetor
	for (contador = 0; contador < quantidade; contador++)
   {
      // Pega um n�mero aleat�rio
      aleatorio = aleatorio_16bits();

      // Se o n�mero n�o existir dentro do vetor ele guarda
      if (numeros_gerados[aleatorio] == vazio)
      {
      	vetor[contador] = aleatorio;
         numeros_gerados[aleatorio] = 1;
      }
      // Se ja existir ele volta o contador
      else
      	contador--;
   }
}

/********************************zera_vetor************************************/
void zera_vetor(int vetor[], const int quantidade)
{
	int i;

   for (i = 0; i < quantidade; i++)
     vetor[i] = vazio;
}

/*********************************grava_vetor**********************************/
int grava_vetor(char caminho[], FILE *&arquivo, int vetor[], int tamanho_vetor)
{
   // Para indicar fim do arquivo
 	const int fim_de_arquivo = vazio;
	int contador;

   // Cria um arquivo bin�rio pra escrita
	arquivo = fopen(caminho, "w+b");

   // Verifica se o arquivo foi realmente criado
   // Sen�o retorna falso
   if (!arquivo)
   	return 0;

	// Grava no arquivo o vetor
   for (contador = 0; contador < tamanho_vetor; contador++)
   	fwrite(&vetor[contador], sizeof(int), 1, arquivo);

   // Indicando o final do arquivo com o valor de vazio
   for (contador = 0; contador < tamanho_vetor; contador++)
 		fwrite(&fim_de_arquivo, sizeof(int), 1, arquivo);

   // Rebobina o cabe�ote do arquivo
   rewind(arquivo);

   // Retorna true
   return 1;
}

/*************************gera_10_vetores_aleatorios***************************/
void gera_10_vetores_aleatorios(int vetor[numero_de_arquivos][numeros_por_vetor])
{
	// Vetor que comportar� todos os n�meros de uma s� vez
   int aleatorios[quantidade_de_numeros];
   // Para varrer o vetor que cont�m todos os n�meros
   int contador = 0, i, j;

   // Gera todos os n�meros aleat�rios de uma s� vez,
   // s� pra n�o ter n�meros repetidos
   vetor_de_aleatorio(aleatorios, quantidade_de_numeros);

   // Agora vamos separar em vetores menores de tamanho = quantidade

   // Para separar em "10 (numero_de_arquivos)" vetores
   for (i = 0; i < numero_de_arquivos; i++)
   {
   	// Para percorrer os vetores por completo
		for (j = 0; j < numeros_por_vetor; j++)
	   {
      	vetor[i][j] = aleatorios[contador];
         // Para varrer o vetor que cont�m todos os n�meros
         contador++;
   	}
   }
}

/***********************************Heap_Sort**********************************/
void Heap_Sort(int numeros[], int quantidade)
{
	// Limites
	int inferior, superior;

   // Estrutura de tempor�ria
   int temporaria;

   superior = quantidade;
   inferior = (quantidade / 2) + 1;

   while (inferior > 0)
   {
		inferior--;
   	Heap(inferior,superior,numeros);
   }

   while (superior > 0)
   {
   	// Troca
      temporaria = numeros[0];
		numeros[0] = numeros[superior];
		numeros[superior] = temporaria;

      superior--;

     	Heap(inferior, superior, numeros);
   }
}

/***********************************Heap***************************************/
// Cria uma �rvore heap
void Heap(int inferior, int superior, int numeros[])
{
	// Estrutura tempor�ria
   int temporaria;
	// Contadores
   int contador, contador2;

   // Inicializa contadores
   contador = inferior;
  	contador2 = 2 * inferior;

   // Armazena o primeiro elemento
  	temporaria = numeros[ inferior ];

   if ((contador2 < superior) && (numeros[contador2] < numeros[contador2 + 1]))
   	contador2++;

   while ((contador2 <= superior) && (temporaria < numeros[contador2]))
   {
   	numeros[contador] = numeros[contador2];
      contador = contador2;
      contador2 = 2 * contador2;

      if ((contador2 < superior) && (numeros[contador2] < numeros[contador2+1]))
      	contador2++;
   }
   numeros[contador] = temporaria;

}

/*****************************ordenando_vetores********************************/
void ordenando_vetores(int vetor[numero_de_arquivos][numeros_por_vetor])
{
	// Para usar o heap em todo o vetores, s� q um por vez
	int temporario[numeros_por_vetor];
   // Contadores
   int i, j;

   // Para rodar todo o vetor de vetor
   for(i = 0; i < numero_de_arquivos; i++)
   {
  	  	// Para percorrer os vetores por completo
		for (j = 0; j < numeros_por_vetor; j++)
      	temporario[j] = vetor[i][j];

      // Agora ordenamos o tempor�rio
      Heap_Sort(temporario, numeros_por_vetor);

  	  	// E voltamos com o vetor ordenado para dentro do vetor de vetores
		for (j = 0; j < numeros_por_vetor; j++)
      	vetor[i][j] = temporario[j];
   }
}

/*****************************gera_nome_arquivo********************************/
void gera_nome_arquivo(char nomes[numero_de_arquivos][tamanho_nome_arquivos])
{
	int i;

	char temp[tamanho_nome_arquivos];

   // Cria todos os nomes dos arquivos
   for (i = 0; i < numero_de_arquivos; i++)
   {
      itoa((i+1),temp,10);
		strcpy(nomes[i],nome_de_arquivo);
		strcat(nomes[i],temp);
      strcat(nomes[i],extensao);
   }
}

/*****************************gera_10_arquivos*********************************/
void gera_10_arquivos(char nomes[numero_de_arquivos][tamanho_nome_arquivos], FILE *arquivo[])
{
	// Para armazenar os 10 vetores
	int vetor[numero_de_arquivos][numeros_por_vetor];
   // Vetor tempor�rio para armazenar no arquivo
	int temp[numeros_por_vetor];
   int i, j;

	// Gera 10 vetores em um s� com n�meros aleat�rios
	gera_10_vetores_aleatorios (vetor);
  	// Ordena cada um dos 10 vetores
   ordenando_vetores(vetor);
   // Gera o nome dos 10 arquivos
	gera_nome_arquivo(nomes);

	for (i = 0; i < numero_de_arquivos; i++)
   {
      for (j = 0; j < numeros_por_vetor; j++)
      	temp[j] = vetor[i][j];

		grava_vetor(nomes[i], arquivo[i], temp, numeros_por_vetor);
   }
}

/*********************************le_dado**************************************/
int LeDado (FILE *arquivo)
{
	// Para pegar o n�mero
	int dado;
   // Verifica se o arquivo foi realmente criado
   // Sen�o retorna falso
   if (!arquivo)
   	return -2;

	// Le no arquivo o vetor
 	fread(&dado, sizeof(int), 1, arquivo);

   return dado;
}

/******************************GravaDados**************************************/
void GravaDados(FILE *arquivo, int dado)
{
 	fwrite(&dado, sizeof(int), 1, arquivo);
}

/******************************Ordenacao***************************************/
// Fun��o Ordenacao: M�todo da Ordena��o Multidirecional
void Ordenacao(int vet[MAX], FILE *arquivo[numero_de_arquivos])
{
   FILE *arquivo_final;
	int i, pos;

   // Abre o arquivo final
   arquivo_final = fopen(nome_arquivo_final, "w+b");

   // Rebobina o arquivo
   rewind(arquivo_final);

	// Carrega os n�s folhas
	for (i = numero_de_arquivos; i < MAX; i++)
	   vet[i] = LeDado(arquivo[i - numero_de_arquivos]);

	// Forma a primeira �rvore completa
   for (i = numero_de_arquivos - 1; i > 0; i--)
   {
   	Multid(i, vet, arquivo);
      ArvoreHeap(i, vet);
   }

   // Ordena��o Multidirecional
	for (pos = 0; pos < quantidade_de_numeros; pos++)
   {
		GravaDados(arquivo_final, vet[1]);
      Multid(1, vet, arquivo);
      ArvoreHeap(1, vet);
   }

   // Fecha o arquivo
   fclose(arquivo_final);

} // Fim de Ordenacao

/******************************Multid******************************************/
// Puxa os n�s da �rvore at� ler um pr�ximo dado de algum arquivo
void Multid(int raiz, int vet[MAX], FILE *arquivo[numero_de_arquivos])
{
	// Se for folha
	if (raiz >= numero_de_arquivos)
   {
   	vet[raiz] = LeDado(arquivo[raiz - numero_de_arquivos]);
      return;
   }

   // Se n�o for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se n�o tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1)
   {
   	vet[raiz] = -1;
      return;
   }

   // Se s� tiver o filho direito
   if (vet[fE] == -1)
   {
   	vet[raiz] = vet[fD];
      Multid(fD, vet, arquivo);
      return;
   }

   // Se s� tiver o filho esquerdo
   if (vet[fD] == -1)
   {
   	vet[raiz] = vet[fE];
      Multid(fE, vet, arquivo);
      return;
   }

   // Se tiver ambos filhos
   if (vet[fE] < vet[fD])
   {
   	vet[raiz] = vet[fE];
      Multid(fE, vet, arquivo);
   }
   else
   {
   	vet[raiz] = vet[fD];
      Multid(fD, vet, arquivo);
   }
}

/******************************ArvoreHeap**************************************/
// Faz uma Heap decrescente na �rvore
void ArvoreHeap(int raiz, int vet[MAX])
{
	// Se for folha
	if (raiz >= numero_de_arquivos) return;

   // Se n�o for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se n�o tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1) return;

   int troca;

   // Se s� tiver o filho direito
   if (vet[fE] == -1)
   {
		if (vet[raiz] > vet[fD])
      {
      	troca = vet[raiz];
        	vet[raiz] = vet[fD];
         vet[fD] = troca;
      }
      return;
   }

   // Se s� tiver o filho esquerdo
   if (vet[fD] == -1)
   {
		if (vet[raiz] > vet[fE])
      {
      	troca = vet[raiz];
        	vet[raiz] = vet[fE];
         vet[fE] = troca;
      }
      return;
   }

   // Se tiver ambos filhos
   int menor = fE;

   if (vet[fE] > vet[fD]) menor++;

   if (vet[raiz] > vet[menor])
   {
    	troca = vet[raiz];
     	vet[raiz] = vet[menor];
      vet[menor] = troca;
      ArvoreHeap(menor, vet);
   }
   else ArvoreHeap(menor, vet);
}

/*******************************carrega_vetor**********************************/
int carrega_vetor(char caminho[], int vetor[], int tamanho_vetor)
{
	int contador;
   FILE *arquivo;

   // Cria um arquivo bin�rio pra escrita
	arquivo = fopen(caminho, "rb");

   // Verifica se o arquivo foi realmente criado
   // Sen�o retorna falso
   if (!arquivo)
   	return 0;

	// Grava no arquivo o vetor
   for (contador = 0; contador < tamanho_vetor; contador++)
   	fread(&vetor[contador], sizeof(int), 1, arquivo);

   // Fecha o arquivo
   fclose (arquivo);

   // Retorna true
   return 1;
}

/*********************************Imprime************************************/
void Imprime(void)
{
   // Definindo vari�veis locais
   int Arquivo_Final[quantidade_de_numeros], cont;

 	carrega_vetor(nome_arquivo_final, Arquivo_Final, quantidade_de_numeros);

   for (cont = 0; cont < quantidade_de_numeros; cont++)
   {
      if (cont % 200 == 0)
      {
      	if (cont == 0)
      	{
       		gotoxy (20,10);  printf ("PRESSIONE ENTER PARA CONTINUAR...");
      	}
      	getch();
      	clrscr();
      	printf ("\n   ****************************** ARQUIVO FINAL ************************\n\n");
      }
   	printf ("%d\t", Arquivo_Final[cont]);
   }
   getch();
   clrscr();
   gotoxy (25,12);	printf ("FIM DO ARQUIVO !!! ");
   getch();
}

