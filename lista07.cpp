/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 7 - Entrega: 22/06/2005                                              *
*                                                                              *
*   Objetivo: Ordenar 10 arquivos utilizando Ordenação Multidirecional         *
*                                                                              *
*   Nome: Ramon Fortes Pereira                  Matrícula: 12638               *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Nome: Nathalia Gonçalves Candido            Matrícula: 12744               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
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

/*********************************Protótipos***********************************/
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
	// Árvore heap
   int vet[MAX];

   // Gera 10 arquivos com números aleatórios pré-ordenados
   gera_10_arquivos(nomes_arquivos, arquivos);

	// Ordena estes 10 arquivos por Ordenação Multidirecional
   Ordenacao(vet, arquivos);

	// Imprime
   Imprime();
}
/******************************************************************************/

/*********************************aleatorio************************************/
// Devolve um número aleatório de 16 bits
int aleatorio_16bits(void)
{
   int aleatorio;
   aleatorio = rand() % maximo_16bits;
   return aleatorio;
}

/*****************************vetor_de_aleatorio*******************************/
// Devolve um vetor com números aleatório de 16 bits sem ser repetido
void vetor_de_aleatorio(int vetor[], const int quantidade)
{
   // Para marcar quais números já forão gerados
   int numeros_gerados[maximo_16bits];
   // Para receber o número aleatório
   int aleatorio;
   // Para contar as casas do vetor
   int contador;

   // Zera o vetor
   zera_vetor(numeros_gerados, maximo_16bits);

   // Inicializa o gerador de aleatórios
   randomize();

   // Enquanto não completar o vetor
	for (contador = 0; contador < quantidade; contador++)
   {
      // Pega um número aleatório
      aleatorio = aleatorio_16bits();

      // Se o número não existir dentro do vetor ele guarda
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

   // Cria um arquivo binário pra escrita
	arquivo = fopen(caminho, "w+b");

   // Verifica se o arquivo foi realmente criado
   // Senão retorna falso
   if (!arquivo)
   	return 0;

	// Grava no arquivo o vetor
   for (contador = 0; contador < tamanho_vetor; contador++)
   	fwrite(&vetor[contador], sizeof(int), 1, arquivo);

   // Indicando o final do arquivo com o valor de vazio
   for (contador = 0; contador < tamanho_vetor; contador++)
 		fwrite(&fim_de_arquivo, sizeof(int), 1, arquivo);

   // Rebobina o cabeçote do arquivo
   rewind(arquivo);

   // Retorna true
   return 1;
}

/*************************gera_10_vetores_aleatorios***************************/
void gera_10_vetores_aleatorios(int vetor[numero_de_arquivos][numeros_por_vetor])
{
	// Vetor que comportará todos os números de uma só vez
   int aleatorios[quantidade_de_numeros];
   // Para varrer o vetor que contém todos os números
   int contador = 0, i, j;

   // Gera todos os números aleatórios de uma só vez,
   // só pra não ter números repetidos
   vetor_de_aleatorio(aleatorios, quantidade_de_numeros);

   // Agora vamos separar em vetores menores de tamanho = quantidade

   // Para separar em "10 (numero_de_arquivos)" vetores
   for (i = 0; i < numero_de_arquivos; i++)
   {
   	// Para percorrer os vetores por completo
		for (j = 0; j < numeros_por_vetor; j++)
	   {
      	vetor[i][j] = aleatorios[contador];
         // Para varrer o vetor que contém todos os números
         contador++;
   	}
   }
}

/***********************************Heap_Sort**********************************/
void Heap_Sort(int numeros[], int quantidade)
{
	// Limites
	int inferior, superior;

   // Estrutura de temporária
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
// Cria uma árvore heap
void Heap(int inferior, int superior, int numeros[])
{
	// Estrutura temporária
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
	// Para usar o heap em todo o vetores, só q um por vez
	int temporario[numeros_por_vetor];
   // Contadores
   int i, j;

   // Para rodar todo o vetor de vetor
   for(i = 0; i < numero_de_arquivos; i++)
   {
  	  	// Para percorrer os vetores por completo
		for (j = 0; j < numeros_por_vetor; j++)
      	temporario[j] = vetor[i][j];

      // Agora ordenamos o temporário
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
   // Vetor temporário para armazenar no arquivo
	int temp[numeros_por_vetor];
   int i, j;

	// Gera 10 vetores em um só com números aleatórios
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
	// Para pegar o número
	int dado;
   // Verifica se o arquivo foi realmente criado
   // Senão retorna falso
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
// Função Ordenacao: Método da Ordenação Multidirecional
void Ordenacao(int vet[MAX], FILE *arquivo[numero_de_arquivos])
{
   FILE *arquivo_final;
	int i, pos;

   // Abre o arquivo final
   arquivo_final = fopen(nome_arquivo_final, "w+b");

   // Rebobina o arquivo
   rewind(arquivo_final);

	// Carrega os nós folhas
	for (i = numero_de_arquivos; i < MAX; i++)
	   vet[i] = LeDado(arquivo[i - numero_de_arquivos]);

	// Forma a primeira árvore completa
   for (i = numero_de_arquivos - 1; i > 0; i--)
   {
   	Multid(i, vet, arquivo);
      ArvoreHeap(i, vet);
   }

   // Ordenação Multidirecional
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
// Puxa os nós da árvore até ler um próximo dado de algum arquivo
void Multid(int raiz, int vet[MAX], FILE *arquivo[numero_de_arquivos])
{
	// Se for folha
	if (raiz >= numero_de_arquivos)
   {
   	vet[raiz] = LeDado(arquivo[raiz - numero_de_arquivos]);
      return;
   }

   // Se não for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se não tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1)
   {
   	vet[raiz] = -1;
      return;
   }

   // Se só tiver o filho direito
   if (vet[fE] == -1)
   {
   	vet[raiz] = vet[fD];
      Multid(fD, vet, arquivo);
      return;
   }

   // Se só tiver o filho esquerdo
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
// Faz uma Heap decrescente na árvore
void ArvoreHeap(int raiz, int vet[MAX])
{
	// Se for folha
	if (raiz >= numero_de_arquivos) return;

   // Se não for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se não tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1) return;

   int troca;

   // Se só tiver o filho direito
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

   // Se só tiver o filho esquerdo
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

   // Cria um arquivo binário pra escrita
	arquivo = fopen(caminho, "rb");

   // Verifica se o arquivo foi realmente criado
   // Senão retorna falso
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
   // Definindo variáveis locais
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

