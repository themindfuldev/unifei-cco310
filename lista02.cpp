/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 2 - Entrega: 06/04/2005                                              *
*                                                                              *
*   Objetivo: Testar e comparar a efici�ncia dos m�todos de classifica��o:     *
*				  Bubble Sort, Shake Sort, Comb Sort e Quick Sort                  *
*                                                                              *
*   Nome: Alu�sio Moura C. dos Santos           Matr�cula: 12615               *
*   Nome: Tiago Romero Garcia                   Matr�cula: 12643               *
*   Ci�ncia da Computa��o - Universidade Federal de Itajub�                    *
*                                                                              *
*******************************************************************************/

// Convers�o do floating point para o modo 16-bit
extern _floatconvert;
#pragma extref _floatconvert

//----------------------------------------------------------------------------//

// Declara��o de bibliotecas
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----------------------------------------------------------------------------//

// Declara��o de constantes
#define MAX 8000
#define METODOS 4

//----------------------------------------------------------------------------//

// Declara��o de estruturas
typedef struct ARQ
{
	int chavePrimaria;
   int ordemRegistro;
} ARQ;

typedef struct RELATORIO
{
	bool ativo;
   char nome[20];
	unsigned long comp;
   unsigned long perm;
	float tempo;
} RELATORIO;

typedef struct PILHA
{
	int l;
   int r;
} PILHA;

//----------------------------------------------------------------------------//

// Declara��o de prot�tipos de fun��es
int  Menu(void);
FILE *AbreArquivoARQ(void);
FILE *AbreArquivoREL(bool);
int  GravaArquivoREL(FILE *, RELATORIO []);
int  LeArquivoARQ(FILE *, ARQ []);
int  LeArquivoREL(FILE *, RELATORIO []);
int  RelatorioEstatistico(void);
int  RelatorioGrafico(void);
int  IniciaModoGrafico(void);
void GeraFatorEscala(RELATORIO [], unsigned long &, unsigned long &, unsigned long &);
int  BubbleSort(void);
int  ShakeSort(void);
int  CombSort(void);
int  QuickSort(void);

//----------------------------------------------------------------------------//

// Programa main
void main()
{
   // Declara��o de vari�veis
   int op, ret;

	// Estrutura do programa
	do
   {
		op = Menu();
   	switch (op)
      {
         case 1: // Ordenar o arquivo de dados por Bubble Sort
         	ret = BubbleSort();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 2: // Ordenar o arquivo de dados por Shake Sort
         	ret = ShakeSort();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 3: // Ordenar o arquivo de dados por Comb Sort
         	ret = CombSort();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 4: // Ordenar o arquivo de dados por Quick Sort
         	ret = QuickSort();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 5: // Gerar o relat�rio estat�stico
				ret = RelatorioEstatistico();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
				printf("\n\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         case 6: // Gerar o relat�rio gr�fico
				ret = RelatorioGrafico();
            if (!ret)
            {
            	printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
					printf("\n\t[Pressione qualquer tecla para continuar...]\n");
				   getch();
            } // Fim de if
            else if (ret==-1)
            {
					printf("\n\t[Pressione qualquer tecla para continuar...]\n");
				   getch();
            } // Fim de if
            break;

         case 0: // Sair
            printf("\tFinalizando o programa...\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         default: // Op��o inv�lida
            printf("\t\aERRO: Opcao invalida...\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;
   	} // Fim de switch
	} while(op); // Fim de do while

} // Fim de main

//----------------------------------------------------------------------------//

// Fun��o Menu: Imprime o menu
int Menu(void)
{
   // Declara��o de vari�veis
	int op;

   // Menu
	clrscr();
	printf("\n     Lista 2 - Classificacao e Pesquisa de Dados / Prof. Sdnei Brito");
	printf("\n     Nome: Aluisio Moura C. dos Santos  -  Matricula: 12615  -  CCO 2004");
	printf("\n     Nome: Tiago Romero Garcia          -  Matricula: 12643  -  CCO 2004");
   printf("\n\n\n\tMenu de Opcoes:");
   printf("\n\t  1. Ordenar o arquivo de dados por Bubble Sort");
   printf("\n\t  2. Ordenar o arquivo de dados por Shake Sort");
   printf("\n\t  3. Ordenar o arquivo de dados por Comb Sort");
   printf("\n\t  4. Ordenar o arquivo de dados por Quick Sort");
   printf("\n\t  5. Gerar o relatorio estatistico");
   printf("\n\t  6. Gerar o relatorio grafico");
   printf("\n\t  0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);
   printf("\n\n");

   // Retorno da opcao escolhida
   return op;

} // Fim de Menu

//----------------------------------------------------------------------------//

// Fun��o BubbleSort: M�todo de ordena��o "Bubble Sort"
int BubbleSort()
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   bool troca;
	int k, m, pos;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista2.dat", "w");

   // Inicializa��o de rel[0]
   rel[0].ativo = true;
   rel[0].comp = rel[0].perm = 0;

	// Arquivo de dados
   // Abertura
   arquivo = AbreArquivoARQ();
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivoARQ(arquivo, vetor);

   // Primeira ordena��o - captura do tempo
   t1 = clock();
   troca = true;
   m = MAX;
   k = 0;
	while (troca)
   {
   	troca = false;
      for (pos=0; pos < m; pos++)
      {
      	if (vetor[pos].chavePrimaria > vetor[pos+1].chavePrimaria)
         {
         	temp.chavePrimaria = vetor[pos].chavePrimaria;
            temp.ordemRegistro = vetor[pos].ordemRegistro;
            vetor[pos].chavePrimaria = vetor[pos+1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+1].ordemRegistro;
            vetor[pos+1].chavePrimaria = temp.chavePrimaria;
				vetor[pos+1].ordemRegistro = temp.ordemRegistro;
            troca = true;
            k = pos;
         } // Fim de if
      } // Fim de for
      m = k;
   } // Fim de while
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[0].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
   troca = true;
   m = MAX;
   k = 0;
	while (troca)
   {
   	troca = false;
      for (pos=0; pos < m; pos++)
      {
      	if (vetor[pos].chavePrimaria > vetor[pos+1].chavePrimaria)
         {
         	temp.chavePrimaria = vetor[pos].chavePrimaria;
            temp.ordemRegistro = vetor[pos].ordemRegistro;
            vetor[pos].chavePrimaria = vetor[pos+1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+1].ordemRegistro;
            vetor[pos+1].chavePrimaria = temp.chavePrimaria;
				vetor[pos+1].ordemRegistro = temp.ordemRegistro;
            troca = true;
            k = pos;
            rel[0].perm++;
         } // Fim de if
         rel[0].comp++;
      } // Fim de for
      m = k;
   } // Fim de while

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de BubbleSort

//----------------------------------------------------------------------------//

// Fun��o ShakeSort: M�todo de ordena��o "Shake Sort"
int ShakeSort(void)
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   int k, l, r, pos;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista2.dat", "w");

   // Inicializa��o de rel[1]
   rel[1].ativo = true;
   rel[1].comp = rel[1].perm = 0;

	// Arquivo de dados
   // Abertura
   arquivo = AbreArquivoARQ();
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivoARQ(arquivo, vetor);

   // Primeira ordena��o - captura do tempo
   t1 = clock();
	l = 0;
   r = k = MAX;
   do
   {
		for (pos=l; pos < r; pos++)
	   {
	     	if (vetor[pos].chavePrimaria > vetor[pos+1].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos].chavePrimaria;
	         temp.ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = vetor[pos+1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+1].ordemRegistro;
	         vetor[pos+1].chavePrimaria = temp.chavePrimaria;
				vetor[pos+1].ordemRegistro = temp.ordemRegistro;
	         k = pos;
	      } // Fim de if
	   } // Fim de for
      r = k;
		for (pos=r; pos > l; pos--)
	   {
	     	if (vetor[pos-1].chavePrimaria > vetor[pos].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos-1].chavePrimaria;
	         temp.ordemRegistro = vetor[pos-1].ordemRegistro;
	         vetor[pos-1].chavePrimaria = vetor[pos].chavePrimaria;
				vetor[pos-1].ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = temp.chavePrimaria;
				vetor[pos].ordemRegistro = temp.ordemRegistro;
	         k = pos;
	      } // Fim de if
	   } // Fim de for
      l = k;
   } while (l < r);
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[1].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
	l = 0;
   r = k = MAX;
   do
   {
		for (pos=l; pos < r; pos++)
	   {
	     	if (vetor[pos].chavePrimaria > vetor[pos+1].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos].chavePrimaria;
	         temp.ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = vetor[pos+1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+1].ordemRegistro;
	         vetor[pos+1].chavePrimaria = temp.chavePrimaria;
				vetor[pos+1].ordemRegistro = temp.ordemRegistro;
	         k = pos;
         	rel[1].perm++;
	      } // Fim de if
        	rel[1].comp++;
	   } // Fim de for
      r = k;
		for (pos=r; pos > l; pos--)
	   {
	     	if (vetor[pos-1].chavePrimaria > vetor[pos].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos-1].chavePrimaria;
	         temp.ordemRegistro = vetor[pos-1].ordemRegistro;
	         vetor[pos-1].chavePrimaria = vetor[pos].chavePrimaria;
				vetor[pos-1].ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = temp.chavePrimaria;
				vetor[pos].ordemRegistro = temp.ordemRegistro;
	         k = pos;
         	rel[1].perm++;
	      } // Fim de if
        	rel[1].comp++;
	   } // Fim de for
      l = k;
   } while (l < r);

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de ShakeSort

//----------------------------------------------------------------------------//

// Fun��o CombSort: M�todo de ordena��o "Comb Sort"
int CombSort(void)
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   bool troca;
   int h, pos;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista2.dat", "w");

   // Inicializa��o de rel[2]
   rel[2].ativo = true;
   rel[2].comp = rel[2].perm = 0;

	// Arquivo de dados
   // Abertura
   arquivo = AbreArquivoARQ();
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivoARQ(arquivo, vetor);

   // Primeira ordena��o - captura do tempo
   t1 = clock();
	h = MAX;
   do
   {
   	h = h/1.3;
      if ((h == 9)||(h == 10))
      	h = 11;
      troca = false;

		for (pos=0; pos < MAX-h; pos++)
	   {
	     	if (vetor[pos].chavePrimaria > vetor[pos+h].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos].chavePrimaria;
	         temp.ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = vetor[pos+h].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+h].ordemRegistro;
	         vetor[pos+h].chavePrimaria = temp.chavePrimaria;
				vetor[pos+h].ordemRegistro = temp.ordemRegistro;
	         troca = true;
	      } // Fim de if
	   } // Fim de for
   } while ((troca == true)||(h > 1));
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[2].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
	h = MAX;
   do
   {
   	h = h/1.3;
      if ((h == 9)||(h == 10))
      	h = 11;
      troca = false;

		for (pos=0; pos < MAX-h; pos++)
	   {
	     	if (vetor[pos].chavePrimaria > vetor[pos+h].chavePrimaria)
	      {
	        	temp.chavePrimaria = vetor[pos].chavePrimaria;
	         temp.ordemRegistro = vetor[pos].ordemRegistro;
	         vetor[pos].chavePrimaria = vetor[pos+h].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos+h].ordemRegistro;
	         vetor[pos+h].chavePrimaria = temp.chavePrimaria;
				vetor[pos+h].ordemRegistro = temp.ordemRegistro;
	         troca = true;
            rel[2].perm++;
	      } // Fim de if
         rel[2].comp++;
	   } // Fim de for
   } while ((troca == true)||(h > 1));

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de CombSort

//----------------------------------------------------------------------------//

// Fun��o QuickSort: M�todo de ordena��o "Quick Sort"
int QuickSort(void)
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
   PILHA pilha[100];
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   int i, j, l, r, pivo, topo;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista2.dat", "w");

   // Inicializa��o de rel[3]
   rel[3].ativo = true;
   rel[3].comp = rel[3].perm = 0;

	// Arquivo de dados
   // Abertura
   arquivo = AbreArquivoARQ();
   if (!arquivo)
      return 0;

	// Leitura
   LeArquivoARQ(arquivo, vetor);

   // Primeira ordena��o - captura do tempo
   t1 = clock();
	pilha[0].l = 0;
   pilha[0].r = MAX-1;
   topo = 0;
   do
   {
   	l = pilha[topo].l;
      r = pilha[topo].r;
      topo--;
      do
      {
      	i = l;
         j = r;
         pivo = vetor[(l+r)/2].chavePrimaria;
	      do
	      {
	      	while (vetor[i].chavePrimaria < pivo) i++;
	      	while (vetor[j].chavePrimaria > pivo) j--;
	         if (i <= j)
	         {
		        	temp.chavePrimaria = vetor[i].chavePrimaria;
		         temp.ordemRegistro = vetor[i].ordemRegistro;
		         vetor[i].chavePrimaria = vetor[j].chavePrimaria;
					vetor[i].ordemRegistro = vetor[j].ordemRegistro;
		         vetor[j].chavePrimaria = temp.chavePrimaria;
					vetor[j].ordemRegistro = temp.ordemRegistro;
	            i++;
	            j--;
	         } // Fim de if
	      } while (i <= j);

         if (i < r)
         {
         	topo++;
            pilha[topo].l = i;
            pilha[topo].r = r;
         } // Fim de if
         r = j;
      } while (l < r);
   } while (topo >= 0);
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[3].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
	pilha[0].l = 0;
   pilha[0].r = MAX-1;
   topo = 0;
   do
   {
   	l = pilha[topo].l;
      r = pilha[topo].r;
      topo--;
      do
      {
      	i = l;
         j = r;
         pivo = vetor[(l+r)/2].chavePrimaria;
	      do
	      {
	      	while (vetor[i].chavePrimaria < pivo)
            {
            	i++;
               rel[3].comp++;
            } // Fim de while
	      	while (vetor[j].chavePrimaria > pivo)
            {
            	j--;
               rel[3].comp++;
            } // Fim de while
	         if (i <= j)
	         {
		        	temp.chavePrimaria = vetor[i].chavePrimaria;
		         temp.ordemRegistro = vetor[i].ordemRegistro;
		         vetor[i].chavePrimaria = vetor[j].chavePrimaria;
					vetor[i].ordemRegistro = vetor[j].ordemRegistro;
		         vetor[j].chavePrimaria = temp.chavePrimaria;
					vetor[j].ordemRegistro = temp.ordemRegistro;
	            i++;
	            j--;
               rel[3].perm++;
	         } // Fim de if
	      } while (i <= j);

         if (i < r)
         {
         	topo++;
            pilha[topo].l = i;
            pilha[topo].r = r;
         } // Fim de if
         r = j;
      } while (l < r);
   } while (topo >= 0);

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de QuickSort

//----------------------------------------------------------------------------//

// Fun��o AbreArquivoARQ: Abre o arquivo de dados
FILE *AbreArquivoARQ(void)
{
   return fopen("ChavePri.dat", "r");

} // Fim de AbreArquivoARQ

//----------------------------------------------------------------------------//

// Fun��o AbreArquivoREL: Abre/cria o arquivo de relat�rio
FILE *AbreArquivoREL(bool leitura)
{
	// Abrir para leitura
	if (leitura)
		return fopen("Lista2.dat", "r");

	// Abrir para grava��o
	// Declara��o de vari�veis
	FILE *arq;
   RELATORIO rel[METODOS];

   arq = fopen("Lista2.dat", "r");

	// Cria��o de um novo arquivo relat�rio
 	if (!arq)
   {
    	fclose(arq);
      printf("\tCriando arquivo relatorio...\n");
		arq = fopen("Lista2.dat", "w");
      for (int i=0; i<METODOS; i++)
      {
        	rel[i].ativo = false;
         rel[i].comp = rel[i].perm = rel[i].tempo = 0;
      } // Fim de for
      GravaArquivoREL(arq, rel);
   } // Fim de if

   // Retorno do arquivo
   return arq;

} // Fim de AbreArquivoREL

//----------------------------------------------------------------------------//

// Fun��o LeArquivoARQ: L� o arquivo de dados
int LeArquivoARQ(FILE *arquivo, ARQ vetor[MAX])
{
   // Declara��o de vari�veis
   int inc=0;

	// Posicionamento do arquivo
   fseek(arquivo, SEEK_SET, 0);

	// Cria��o do vetor
   for (int pos=0; pos<MAX; pos++)
   {
   	fscanf(arquivo, "%d ", &vetor[pos].chavePrimaria);
      vetor[pos].ordemRegistro = inc++;
   } // Fim de for

   return 1;

} // Fim de LeArquivoARQ

//----------------------------------------------------------------------------//

// Fun��o LeArquivoREL: L� o arquivo relat�rio do disco
int LeArquivoREL(FILE *arquivo, RELATORIO rel[METODOS])
{
	for (int i=0; i<METODOS; i++)
    	fscanf(arquivo, "%d %s %ld %ld %f", &rel[i].ativo, &rel[i].nome, &rel[i].comp, &rel[i].perm, &rel[i].tempo);

	return 1;

} // Fim de LeArquivoREL

//----------------------------------------------------------------------------//

// Fun��o GravaArquivoREL: Grava o arquivo relat�rio no disco
int GravaArquivoREL(FILE *arquivo, RELATORIO rel[METODOS])
{
	// Declara��o de vari�veis
   char string[20]="";

	for (int i=0; i<METODOS; i++)
   {
   	switch (i)
      {
      	case 0:	strcpy(string, "Bubble_Sort");
         			break;

      	case 1:	strcpy(string, "Shake_Sort");
         			break;

      	case 2:	strcpy(string, "Comb_Sort");
         			break;

      	case 3:	strcpy(string, "Quick_Sort");
         			break;
      } // Fim de switch
   	fprintf(arquivo, "%d %s %ld %ld %f\n", rel[i].ativo, string, rel[i].comp, rel[i].perm, rel[i].tempo);
   } // Fim de for

	return 1;

} // Fim de GravaArquivoREL

//----------------------------------------------------------------------------//

// Fun��o RelatorioEstatistico: Gera o relat�rio estat�stico
int RelatorioEstatistico(void)
{
	// Declara��o de vari�veis
	FILE *relat;
   RELATORIO rel[METODOS];

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(true);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Impress�o da estat�sticas
   for (int i=0; i<METODOS; i++)
   {
	   printf("\n\n\t  %s:\n", rel[i].nome);
	   if (rel[i].ativo)
	   {
			printf("\n\t\tComparacoes: %ld", rel[i].comp);
	      printf("\n\t\tPermutas: %ld", rel[i].perm);
         if (rel[i].tempo != 0)
		      printf("\n\t\tTempo de execucao: %.4f segundos", rel[i].tempo);
         else
		      printf("\n\t\tTempo de execucao: devido a velocidade do computador,\n\t\tnao foi possivel detectar diferenca de tempo para este metodo.");
	   } // Fim de if
	   else
			printf("\n\t\tMetodo ainda nao avaliado. Execute-o para avaliacao.");
      if ((i%2==1)&&(i!=METODOS-1)) // Pagina��o
      {
			printf("\n\n\t[Pressione qualquer tecla para ver mais...]\n");
         getch();
      } // Fim de if
	} // Fim de for

	// Fechamento do arquivo relat�rio
   fclose(relat);

	return 1;

} // Fim de RelatorioEstatistico

//----------------------------------------------------------------------------//

// Fun��o RelatorioGrafico: Gera o relatorio gr�fico
int RelatorioGrafico(void)
{
	// Declara��o de vari�veis
	FILE *relat;
   RELATORIO rel[METODOS];
	char *string="", *string2="";
   unsigned long escalaComp, escalaPerm, escalaTempo;
   int x0, xi, xg;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(true);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

	// Inicializa��o do modo gr�fico
   if (IniciaModoGrafico()==0)	// Erro de gr�fico
      return -1;

	// Escala
  	GeraFatorEscala(rel, escalaComp, escalaPerm, escalaTempo);
   for (int i=0; i<METODOS; i++)
   {
   	rel[i].comp /= escalaComp;
      rel[i].perm /= escalaPerm;
      rel[i].tempo *= escalaTempo;
   } // Fim de for

   // Gera��o do gr�fico
   for (int i=0; i<METODOS; i++)
   {
		// Constante para a itera��o
      x0 = 550/(METODOS+1);
      xi = x0*(i+1);
      xg = x0/4;

		// Compara��es
   	setfillstyle(2, 2);
      ultoa(400-rel[i].comp, string, 10);
	   bar3d(xi+3, atoi(string), xi+xg-3, 400, 20, 1);

      // Permuta��es
   	setfillstyle(3, 3);
      ultoa(400-rel[i].perm, string, 10);
	   bar3d(xi+xg+3, atoi(string), xi+xg*2-3, 400, 20, 1);

      // Tempo
   	setfillstyle(4, 4);
	   string = ecvt(400-rel[i].tempo, 3, NULL, NULL);
	   bar3d(xi+xg*2+3, atoi(string), xi+xg*3-3, 400, 20, 1);

      // M�todo
	   outtextxy(xi, 440, rel[i].nome);
   } // Fim de for

   // Eixos X e Y
   outtextxy(1000, 1000, "99999"); // Escreve o lixo de mem�ria fora da tela
   line(50, 100, 50, 420);
   line(50, 420, 600, 420);
   outtextxy(20, 100, "300");
   outtextxy(20, 150, "250");
   outtextxy(20, 200, "200");
   outtextxy(20, 250, "150");
   outtextxy(20, 300, "100");
   outtextxy(20, 350, "50");
   outtextxy(20, 400, "0");

	// Legenda
   rectangle(380, 0, 610, 100);

   setfillstyle(2, 2);
   bar3d(395, 10, 415, 30, 0, 1);
   ultoa(escalaComp, string2, 10);
   string = "Comparacoes  / ";
   strcat(string, string2);
   outtextxy(425, 18, string);

   outtextxy(1000, 1000, "99999999999"); // Escreve o lixo de mem�ria fora da tela

   setfillstyle(3, 3);
   bar3d(395, 40, 415, 60, 0, 1);
   ultoa(escalaPerm, string2, 10);
   string = "Permutas     / ";
   strcat(string, string2);
   outtextxy(425, 48, string);

   outtextxy(1000, 1000, "99999999999"); // Escreve o lixo de mem�ria fora da tela

   setfillstyle(4, 4);
   bar3d(395, 70, 415, 90, 0, 1);
   ultoa(escalaTempo, string2, 10);
   string = "Tempo  (ms)  * ";
   strcat(string, string2);
   outtextxy(425, 78, string);

	getch();

   // Fechamento do modo gr�fico
   closegraph();

   return 1;

} // Fim de RelatorioGrafico

//----------------------------------------------------------------------------//

// Fun��o IniciaModoGrafico: Inicializa o modo gr�fico
int IniciaModoGrafico(void)
{
	int gdriver = DETECT, gmode, errorcode;
   initgraph(&gdriver, &gmode, "C:\\BC5\\BGI");
	errorcode = graphresult();
   if (errorcode!=0)
   {
		printf("\tErro de grafico: %s", grapherrormsg(errorcode));
      return 0;
   } // Fim de if
   return 1;

} // Fim de IniciaModoGrafico

//----------------------------------------------------------------------------//

// Fun��o GeraFatorEscala
void GeraFatorEscala(RELATORIO rel[METODOS], unsigned long &escalaComp, unsigned long &escalaPerm, unsigned long &escalaTempo)
{
   // Declara��o de vari�veis
	unsigned long maxComp = rel[0].comp, maxPerm = rel[0].perm;
   float maxTempo = rel[0].tempo;

	// Inicializa��o dos fatores de escala
   escalaComp = escalaPerm = escalaTempo = 10;

	// Determina��o dos valores m�ximos de Compara��es, Permuta��es e Tempo
   for (int i=1; i<METODOS; i++)
   {
   	maxComp = ((maxComp < rel[i].comp)? rel[i].comp: maxComp);
   	maxPerm = ((maxPerm < rel[i].perm)? rel[i].perm: maxPerm);
   	maxTempo = ((maxTempo < rel[i].tempo)? rel[i].tempo: maxTempo);
   } // Fim de for

	// Determina��o do fator de escala de Compara��es
	while (!((maxComp/escalaComp < 300)&&(maxComp/escalaComp > 250)))
   {
   	if (maxComp/escalaComp > 300)
      	escalaComp *= 10;
      if (maxComp/escalaComp < 250)
      	escalaComp /= 2;
   } // Fim de while

	// Determina��o do fator de escala de Permuta��es
	while (!((maxPerm/escalaPerm < 300)&&(maxPerm/escalaPerm > 250)))
   {
   	if (maxPerm/escalaPerm > 300)
      	escalaPerm *= 10;
      if (maxPerm/escalaPerm < 250)
      	escalaPerm /= 2;
   } // Fim de while

	// Determina��o do fator de escala de Tempo
	while (!((maxTempo*escalaTempo < 300)&&(maxTempo*escalaTempo > 250)))
   {
   	if (maxTempo*escalaTempo > 300)
      	escalaTempo /= 2;
      if (maxTempo*escalaTempo < 250)
      	escalaTempo *= 10;
   } // Fim de while

} // Fim de GeraFatorEscala

