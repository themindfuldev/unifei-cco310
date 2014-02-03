/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 4 - Entrega: 06/05/2005                                              *
*                                                                              *
*   Objetivo: Testar e comparar a eficiência dos métodos de classificação:     *
*				  Fusão de duas vias, Fusão alternativa, Merge Sort e Radix Sort.  *
*                                                                              *
*   Nome: André Carvalho Silveira               Matrícula: 12616               *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
*                                                                              *
*******************************************************************************/

// Declaração de bibliotecas
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----------------------------------------------------------------------------//

// Declaração de constantes
#define MAX 8000
#define METODOS 4

//----------------------------------------------------------------------------//

// Declaração de estruturas
typedef struct ARQ
{
	int chavePrimaria;
   int ordemRegistro;
} ARQ;

typedef struct RELATORIO
{
	bool ativo;
   char nome[20];
   char arquivo[20];
	unsigned long comp;
	float tempo;
   char teorico[200];
} RELATORIO;

typedef struct NO
{
	int info, prox;
} NO;

//----------------------------------------------------------------------------//

// Declaração de protótipos de funções
int  Menu(void);
FILE *AbreArquivo(void);
int  GravaArquivo(FILE *, ARQ [], int);
int  LeArquivo(FILE *, ARQ []);
int  Relatorio(RELATORIO []);
int  FusaoDireta(RELATORIO &);
int  FusaoAlternativa(RELATORIO &);
int  Intercalacao(RELATORIO &);
int  RadixSort(RELATORIO &);
int  FusaoDuasVias(int, int, ARQ []);
int  Merge(int, int, int, ARQ []);
int  FusaoDuasViasREL(int, int, ARQ [], RELATORIO &);
int  MergeREL(int, int, int, ARQ [], RELATORIO &);

//----------------------------------------------------------------------------//

// Programa main
void main()
{
   // Declaração de variáveis
   int op, ret;
   RELATORIO rel[METODOS];

   // Inicialização da estrutura de relatório
   strcpy(rel[0].nome, "Fusao Direta");
   strcpy(rel[1].nome, "Fusao Altern.");
   strcpy(rel[2].nome, "Merge Sort");
   strcpy(rel[3].nome, "Radix Sort");
   for (int i=0; i < METODOS; i++)
   	rel[i].ativo = false;

	// Estrutura do programa
	do
   {
		op = Menu();
   	switch (op)
      {
         case 1: // Ordenar o arquivo 01 por Fusao de Duas Vias
         	ret = FusaoDireta(rel[0]);
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 2: // Ordenar o arquivo 02 por Fusao Alternativa
         	ret = FusaoAlternativa(rel[1]);
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 3: // Ordenar os arquivos 01 e 02 por Merge Sort
         	ret = Intercalacao(rel[2]);
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 4: // Ordenar o arquivo 01 por Radix Sort
         	ret = RadixSort(rel[3]);
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 5: // Gerar o relatório estatístico
				ret = Relatorio(rel);
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
				printf("\n\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         case 0: // Sair
            printf("\tFinalizando o programa...\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         default: // Opção inválida
            printf("\t\aERRO: Opcao invalida...\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;
   	} // Fim de switch
	} while(op); // Fim de do while

} // Fim de main

//----------------------------------------------------------------------------//

// Função Menu: Imprime o menu
int Menu(void)
{
   // Declaração de variáveis
	int op;

   // Menu
	clrscr();
	printf("\n     Lista 4 - Classificacao e Pesquisa de Dados / Prof. Sdnei Brito");
	printf("\n     Nome: Andre Carvalho Silveira      -  Matricula: 12616  -  CCO 2004");
	printf("\n     Nome: Tiago Romero Garcia          -  Matricula: 12643  -  CCO 2004");
   printf("\n\n\n\tMenu de Opcoes:");
   printf("\n\t  1. Ordenar o arquivo 01 por Fusao de Duas Vias");
   printf("\n\t  2. Ordenar o arquivo 02 por Fusao Alternativa");
   printf("\n\t  3. Ordenar os arquivos 01 e 02 por Merge Sort");
   printf("\n\t  4. Ordenar o arquivo 01 por Radix Sort");
   printf("\n\t  5. Gerar o relatorio estatistico");
   printf("\n\t  0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);
   printf("\n\n");

   // Retorno da opcao escolhida
   return op;

} // Fim de Menu

//----------------------------------------------------------------------------//

// Função FusaoDireta: Ordena o arquivo 01 por Fusao de Duas Vias
int FusaoDireta(RELATORIO &rel0)
{
   // Declaração de variáveis
   ARQ vetor[MAX];
   FILE *arquivo;
   clock_t t1, t2;
   int nteo;
   char teo[20];

   // Inicialização de rel0
   rel0.ativo = true;
   rel0.comp = 0;
   strcpy(rel0.arquivo, "01");
   nteo = 2*MAX - 1;
   itoa(nteo, teo, 10);
   strcpy(rel0.teorico, teo);

	// Arquivo de dados
   // Abertura
   arquivo = fopen("Arquivo_01.dat", "r");
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivo(arquivo, vetor);

   // Primeira ordenação - captura do tempo
   t1 = clock();

   //-- Início do algoritmo
   FusaoDuasVias(0, MAX-1, vetor);
	//-- Fim do algoritmo

	t2 = clock();

   // Inserção das estatísticas de tempo
   rel0.tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivo(arquivo, vetor);

   // Segunda ordenação - captura do número de comparações

   //-- Início do algoritmo
   FusaoDuasViasREL(0, MAX-1, vetor, rel0);
	//-- Fim do algoritmo

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Gravação dos resultados
   // Abertura
   arquivo = fopen("Arquivo_01Final.dat", "w");
   if (!arquivo)
      return 0;

	// Gravação
	GravaArquivo(arquivo, vetor, MAX);

	// Fechamento do arquivo de resultados
   fclose(arquivo);

	return 1;

} // Fim de FusaoDireta

//----------------------------------------------------------------------------//

// Função FusaoDuasVias
int FusaoDuasVias(int l, int r, ARQ vetor[MAX])
{
	// Declaração de variáveis
   int meio;

   if (l < r)
   {
   	meio = (l+r)/2;
      FusaoDuasVias(l, meio, vetor);
      FusaoDuasVias(meio+1, r, vetor);
      Merge(l, meio, r, vetor);
   } // Fim de if

   return 1;

} // Fim de FusaoDuasVias

//----------------------------------------------------------------------------//

// Função Merge
int Merge(int l, int meio, int r, ARQ vetor[MAX])
{
	// Declaração de variáveis
   ARQ aux[MAX];
   int i, j, k;

   i = j = l;
   k = meio + 1;
   while ((j <= meio)&&(k <= r))
   {
   	if (vetor[j].chavePrimaria < vetor[k].chavePrimaria)
      {
      	aux[i] = vetor[j];
         i++;
         j++;
      } // Fim de if
      else
      {
      	aux[i] = vetor[k];
         i++;
         k++;
      } // Fim de else
   } // Fim de while

   while (j <= meio)
   {
   	aux[i] = vetor[j];
      i++;
      j++;
   } // Fim de while

   for (i=l; i < k; i++)
   	vetor[i] = aux[i];

   return 1;

} // Fim de Merge

//----------------------------------------------------------------------------//

// Função FusaoDuasViasREL
int FusaoDuasViasREL(int l, int r, ARQ vetor[MAX], RELATORIO &rel0)
{
	// Declaração de variáveis
   int meio;

   if (l < r)
   {
   	meio = (l+r)/2;
      FusaoDuasViasREL(l, meio, vetor, rel0);
      FusaoDuasViasREL(meio+1, r, vetor, rel0);
      MergeREL(l, meio, r, vetor, rel0);
   } // Fim de if

   return 1;

} // Fim de FusaoDuasViasREL

//----------------------------------------------------------------------------//

// Função MergeREL
int MergeREL(int l, int meio, int r, ARQ vetor[MAX], RELATORIO &rel0)
{
	// Declaração de variáveis
   ARQ aux[MAX];
   int i, j, k;

   i = j = l;
   k = meio + 1;
   while ((j <= meio)&&(k <= r))
   {
   	if (vetor[j].chavePrimaria < vetor[k].chavePrimaria)
      {
      	aux[i] = vetor[j];
         i++;
         j++;
      } // Fim de if
      else
      {
      	aux[i] = vetor[k];
         i++;
         k++;
      } // Fim de else
      rel0.comp++;
   } // Fim de while

   while (j <= meio)
   {
   	aux[i] = vetor[j];
      i++;
      j++;
   } // Fim de while

   for (i=l; i < k; i++)
   	vetor[i] = aux[i];

   return 1;

} // Fim de MergeREL

//----------------------------------------------------------------------------//

// Função FusaoAlternativa: Ordena o arquivo 02 por Fusao Alternativa
int FusaoAlternativa(RELATORIO &rel1)
{
   // Declaração de variáveis
   ARQ aux[MAX], vetor[MAX];
   FILE *arquivo;
   clock_t t1, t2;
   int tam, i, j, k, l1, l2, r1, r2, nteo;
   char teo[20];

   // Inicialização de rel1
   rel1.ativo = true;
   rel1.comp = 0;
   strcpy(rel1.arquivo, "02");
   nteo = 2*MAX - 1;
   itoa(nteo, teo, 10);
   strcpy(rel1.teorico, teo);

	// Arquivo de dados
   // Abertura
   arquivo = fopen("Arquivo_02.dat", "r");
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivo(arquivo, vetor);

   // Primeira ordenação - captura do tempo
   t1 = clock();

   //-- Início do algoritmo
   tam = 1;
   while (tam < MAX)
   {
   	l1 = 0;
      k = 0;
      while (l1+tam < MAX)
      {
      	l2 = l1 + tam;
         r1 = l2 - 1;
         if (l2+tam-1 < MAX)
         	r2 = l2+tam-1;
         else
         	r2 = MAX-1;

         for (i=l1, j=l2; (i <= r1)&&(j <= r2); k++)
         {
         	if (vetor[i].chavePrimaria <= vetor[j].chavePrimaria)
            {
            	aux[k] = vetor[i];
               i++;
            } // Fim de if
            else
            {
            	aux[k] = vetor[j];
               j++;
            } // Fim de else
         } // Fim de for

         while (i <= r1)
         {
         	aux[k] = vetor[i];
            i++;
            k++;
         } // Fim de while

         while (j <= r2)
         {
         	aux[k] = vetor[j];
            j++;
            k++;
         } // Fim de while

         l1 = r2+1;
      } // Fim de while

      for (i=l1; k < MAX; i++)
      {
      	aux[k] = vetor[i];
         k++;
      } // Fim de for

      for (i=0; i < MAX; i++)
      	vetor[i] = aux[i];

      tam *= 2;
   } // Fim de while
	//-- Fim do algoritmo
	t2 = clock();

   // Inserção das estatísticas de tempo
   rel1.tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivo(arquivo, vetor);

   // Segunda ordenação - captura do número de comparações

   //-- Início do algoritmo
   tam = 1;
   while (tam < MAX)
   {
   	l1 = 0;
      k = 0;
      while (l1+tam < MAX)
      {
      	l2 = l1 + tam;
         r1 = l2 - 1;
         if (l2+tam-1 < MAX)
         	r2 = l2+tam-1;
         else
         	r2 = MAX-1;

         for (i=l1, j=l2; (i <= r1)&&(j <= r2); k++)
         {
         	if (vetor[i].chavePrimaria <= vetor[j].chavePrimaria)
            {
            	aux[k] = vetor[i];
               i++;
            } // Fim de if
            else
            {
            	aux[k] = vetor[j];
               j++;
            } // Fim de else
            rel1.comp++;
         } // Fim de for

         while (i <= r1)
         {
         	aux[k] = vetor[i];
            i++;
            k++;
         } // Fim de while

         while (j <= r2)
         {
         	aux[k] = vetor[j];
            j++;
            k++;
         } // Fim de while

         l1 = r2+1;
      } // Fim de while

      for (i=l1; k < MAX; i++)
      {
      	aux[k] = vetor[i];
         k++;
      } // Fim de for

      for (i=0; i < MAX; i++)
      	vetor[i] = aux[i];

      tam *= 2;
   } // Fim de while
	//-- Fim do algoritmo

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Gravação dos resultados
   // Abertura
   arquivo = fopen("Arquivo_02Final.dat", "w");
   if (!arquivo)
      return 0;

	// Gravação
	GravaArquivo(arquivo, vetor, MAX);

	// Fechamento do arquivo de resultados
   fclose(arquivo);

	return 1;

} // Fim de FusaoAlternativa

//----------------------------------------------------------------------------//

// Função Intercalacao: Ordena os arquivos 01 e 02 por Merge Sort
int Intercalacao(RELATORIO &rel2)
{
   // Declaração de variáveis
   ARQ vetor1[MAX], vetor2[MAX], final[2*MAX];
   FILE *arquivo1, *arquivo2;
   clock_t t1, t2;
   int al, bl, ak, bk, ck, nteo;
   char teo[20];

   // Inicialização de rel2
   rel2.ativo = true;
   rel2.comp = 0;
   strcpy(rel2.arquivo, "01 e 02");
   nteo = MAX * (2 - 1);
   itoa(nteo, teo, 10);
   strcpy(rel2.teorico, teo);

	// Arquivo de dados
   // Abertura
   arquivo1 = fopen("Arquivo_01Final.dat", "r");
   arquivo2 = fopen("Arquivo_02Final.dat", "r");
   if ((!arquivo1)||(!arquivo2))
      return 0;

	// Leitura
   LeArquivo(arquivo1, vetor1);
   LeArquivo(arquivo2, vetor2);

   // Primeira ordenação - captura do tempo
   t1 = clock();

   //-- Início do algoritmo
   al = bl = MAX;
   ak = bk = 0;
   for (ck=0; (ck <= al) && (bk <= bl); ck++)
   {
   	if (vetor1[ak].chavePrimaria < vetor2[bk].chavePrimaria)
      {
      	final[ck] = vetor1[ak];
         ak++;
      } // Fim de if
      else
      {
      	final[ck] = vetor2[bk];
         bk++;
      } // Fim de else
   } // Fim de for
   while (ak <= al)
   {
		final[ck] = vetor1[ak];
      ck++;
      ak++;
   } // Fim de while
   while (bk <= bl)
   {
		final[ck] = vetor2[bk];
      ck++;
      bk++;
   } // Fim de while
	//-- Fim do algoritmo
	t2 = clock();

   // Inserção das estatísticas de tempo
   rel2.tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivo(arquivo1, vetor1);
   LeArquivo(arquivo2, vetor2);   

   // Segunda ordenação - captura do número de comparações

   //-- Início do algoritmo
   al = bl = MAX;
   ak = bk = 0;
   for (ck=0; (ck < al) && (bk < bl); ck++)
   {
   	if (vetor1[ak].chavePrimaria < vetor2[bk].chavePrimaria)
      {
      	final[ck] = vetor1[ak];
         ak++;
      } // Fim de if
      else
      {
      	final[ck] = vetor2[bk];
         bk++;
      } // Fim de else
      rel2.comp++;
   } // Fim de for
   while (ak <= al)
   {
		final[ck] = vetor1[ak];
      ck++;
      ak++;
   } // Fim de while
   while (bk <= bl)
   {
		final[ck] = vetor2[bk];
      ck++;
      bk++;
   } // Fim de while
	//-- Fim do algoritmo

	// Fechamento do arquivo de dados
   fclose(arquivo1);
   fclose(arquivo2);

	// Gravação dos resultados
   // Abertura
   arquivo1 = fopen("Arquivo_03Final.dat", "w");
   if (!arquivo1)
      return 0;

	// Gravação
	GravaArquivo(arquivo1, final, 2*MAX);

	// Fechamento do arquivo de resultados
   fclose(arquivo1);

	return 1;

} // Fim de Intercalacao

//----------------------------------------------------------------------------//

// Função RadixSort: Ordena o arquivo 01 por Radix Sort
int RadixSort(RELATORIO &rel3)
{
   // Declaração de variáveis
   ARQ vetor[MAX];
   NO item[MAX];
   FILE *arquivo;
   clock_t t1, t2;
   int exp, final[10], i, inicio[10], j, p, q, primeiro, y, nteo;
   int m = 5;
   char teo[10];

   // Inicialização de rel1
   rel3.ativo = true;
   rel3.comp = 0;
   strcpy(rel3.arquivo, "01");
   strcpy(rel3.teorico, "\n\t\tNao faz sentido o numero de comparacoes para este metodo.\n\t\tA quantidade de operacoes de distribuicao de chaves e: ");
   nteo = MAX * m;
   itoa(nteo, teo, 10);
   strcat(rel3.teorico, teo);


	// Arquivo de dados
   // Abertura
   arquivo = fopen("Arquivo_01.dat", "r");
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivo(arquivo, vetor);

   // Primeira ordenação - captura do tempo
   t1 = clock();

   //-- Início do algoritmo

	// Inicializa a lista
   for (i=0; i < MAX-1; i++)
   {
   	item[i].info = vetor[i].chavePrimaria;
      item[i].prox = i+1;
   } // Fim de for

   item[MAX-1].info = vetor[MAX-1].chavePrimaria;
   item[MAX-1].prox = -1;
   primeiro = 0;		// Início da lista encadeada

   // Pressupondo o valor de cinco dígitos
   for (int k=1; k <= m; k++)
   {
   	for (i=0; i < 10; i++)
      {
      	// Inicializa os índices para o controle das filas
         final[i] = -1;
         inicio[i] = -1;
      } // Fim de for

      // Processa cada elemento da lista
      while (primeiro != -1)
      {
      	p = primeiro;
         primeiro = item[primeiro].prox;
         y = item[p].info;

         // Extrai o k-ésimo dígito
         exp = pow(10, k-1);
         j = (y/exp) % 10;

         // Insere y na fila j
         q = final[j];

         if (q == -1)
         	inicio[j] = p;
         else
         	item[q].prox = p;
         final[j] = p;
      } // Fim de while

      // Agora cada elemento está na fila correta, com base no dígito k
      // Buscando o primeiro elemento
      for (j = 0; (j < 10) && (inicio[j] == -1); j++);
     	primeiro = inicio[j];

      // Associa os elementos dos escaninhos restantes
      while (j <= 9)
      {
       	// Encontra o próximo
         for (i = j+1; (i < 10) && (inicio[i] == -1); i++);
        	if (i <= 9)
         {
           	p = i;
            item[final[j]].prox = inicio[i];
         } // Fim de if
         j = i;
      } // Fim de while
	   item[final[p]].prox = -1;
   } // Fim de for

   // Copia de volta para o vetor original
	for (i=0; i < MAX; i++)
   {
   	vetor[i].chavePrimaria = item[primeiro].info;
      primeiro = item[primeiro].prox;
   } // Fim de for

	//-- Fim do algoritmo
	t2 = clock();

   // Inserção das estatísticas de tempo
   rel3.tempo = (t2-t1)/CLK_TCK;

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Gravação dos resultados
   // Abertura
   arquivo = fopen("Arquivo_04.dat", "w");
   if (!arquivo)
      return 0;

	// Gravação
	GravaArquivo(arquivo, vetor, MAX);

	// Fechamento do arquivo de resultados
   fclose(arquivo);

	return 1;

} // Fim de RadixSort

//----------------------------------------------------------------------------//

// Função LeArquivo: Lê um arquivo de dados
int LeArquivo(FILE *arquivo, ARQ vetor[MAX])
{
   // Declaração de variáveis
   int inc=0;

	// Posicionamento do arquivo
   fseek(arquivo, SEEK_SET, 0);

	// Criação do vetor
   for (int pos=0; pos<MAX; pos++)
   {
   	fscanf(arquivo, "%d ", &vetor[pos].chavePrimaria);
      vetor[pos].ordemRegistro = inc++;
   } // Fim de for

   return 1;

} // Fim de LeArquivo

//----------------------------------------------------------------------------//

// Função GravaArquivo: Grava um novo arquivo de dados
int GravaArquivo(FILE *arquivo, ARQ vetor[MAX], int tam)
{
	// Posicionamento do arquivo
   fseek(arquivo, SEEK_SET, 0);

	// Gravação do vetor
   for (int pos=0; pos<tam; pos++)
   	fprintf(arquivo, "%d\n", vetor[pos].chavePrimaria);

   return 1;

} // Fim de GravaArquivo

//----------------------------------------------------------------------------//

// Função Relatorio: Gera o relatório estatístico
int Relatorio(RELATORIO rel[METODOS])
{

	printf("Metodo\t\tArquivo\t\tComp\t\tTempo\t\tTeorico");

   // Impressão da estatísticas
   for (int i=0; i<METODOS; i++)
   {
	   printf("\n\n%s", rel[i].nome);
	   if (rel[i].ativo)
	   {
      	printf("\t%s", rel[i].arquivo);
         if (rel[i].comp != 0)
         	printf("\t\t%ld", rel[i].comp);
         else
		      printf("\t\t----");
         if (rel[i].tempo != 0)
		      printf("\t\t%.4f", rel[i].tempo);
         else
		      printf("\t\t----");
			printf("\t\t%s", rel[i].teorico);
	   } // Fim de if
	   else
			printf("\tMetodo ainda nao avaliado.");
	} // Fim de for

   printf("\n\nComparacao estatistica:\n\n\tHa uma disparidade entre o numero de comparacoes teorico e experimental nos metodos de Fusao de Duas Vias e Fusao de Duas Vias alternativa. O modelo\nexperimental apresentou um maior numero de comparacoes que o teorico, devido\nao esforco computacional do algoritmo ser maior do que o previsto. Em relacao\nao metodo de Intercalacao, o modelo teorico atingiu as expectativas. Para o\nmetodo Radix Sort nao faz sentido falar em numero de comparacoes; entretanto,\ntemos o modelo teorico do numero de operacoes de distribuicao de chaves.");  

	return 1;

} // Fim de RelatorioEstatistico

