/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 1 - Entrega: 21/03/2005                                              *
*                                                                              *
*   Objetivo: Testar e comparar a efici�ncia dos m�todos de classifica��o:     *
*				  Inser��o direta, Inser��o bin�ria e Shell sort						 *
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
#define METODOS 3

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
int  InsercaoBinaria(void);
int  InsercaoDireta(void);
int  ShellSort(void);

//----------------------------------------------------------------------------//

// Programa main
void main(void)
{
   // Declara��o de vari�veis
   int op, ret;

	// Estrutura do programa
	do
   {
		op = Menu();
   	switch (op)
      {
         case 1: // Ordenar o arquivo de dados por Insercao Direta
         	ret = InsercaoDireta();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 2: // Ordenar o arquivo de dados por Insercao Binaria
         	ret = InsercaoBinaria();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 3: // Ordenar o arquivo de dados por Shell Sort
         	ret = ShellSort();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\n\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 4: // Gerar o relat�rio estat�stico
				ret = RelatorioEstatistico();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         case 5: // Gerar o relat�rio estat�stico
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
            }
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
	printf("\n     Lista 1 - Classificacao e Pesquisa de Dados / Prof. Sdnei Brito");
	printf("\n     Nome: Aluisio Moura C. dos Santos  -  Matricula: 12615  -  CCO 2004");
	printf("\n     Nome: Tiago Romero Garcia          -  Matricula: 12643  -  CCO 2004");
   printf("\n\n\n\tMenu de Opcoes:");
   printf("\n\t  1. Ordenar o arquivo de dados por Insercao Direta");
   printf("\n\t  2. Ordenar o arquivo de dados por Insercao Binaria");
   printf("\n\t  3. Ordenar o arquivo de dados por Shell Sort");
	printf("\n\t  4. Gerar o relatorio estatistico");
	printf("\n\t  5. Gerar o relatorio grafico");
   printf("\n\t  0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);
   printf("\n\n");

   // Retorno da opcao escolhida
   return op;

} // Fim de Menu

//----------------------------------------------------------------------------//

// Fun��o InsercaoBinaria: M�todo de classifica��o "Inser��o Bin�ria"
int InsercaoBinaria(void)
{
   // Declara��o de vari�veis
	int dir, esq, inicio, pos;
   ARQ troca, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[3];
   clock_t t1, t2;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista1.dat", "w");

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
   for (int seg=1; seg<MAX; seg++)
   {
   	esq = 0;
      dir = seg;
		inicio = (esq+dir)/2;

      while (esq<dir)
      {
      	if (vetor[seg].chavePrimaria<vetor[inicio].chavePrimaria)
         	dir = inicio;
			else
         	esq = inicio+1;
			inicio = (esq+dir)/2;
      } // Fim de while

		if (inicio<seg)
      {
	    	troca.chavePrimaria = vetor[seg].chavePrimaria;
	      troca.ordemRegistro = vetor[seg].ordemRegistro;
	    	for (pos = seg; pos>esq; pos--)
	      {
	         vetor[pos].chavePrimaria = vetor[pos-1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos-1].ordemRegistro;
	      } // Fim de for
	      vetor[inicio].chavePrimaria = troca.chavePrimaria;
	      vetor[inicio].ordemRegistro = troca.ordemRegistro;
      } // Fim de if
   } // Fim de for
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[1].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
   for (int seg=1; seg<MAX; seg++)
   {
   	if (vetor[seg].chavePrimaria<vetor[seg-1].chavePrimaria)
      {
	   	esq = 0;
	      dir = seg;
			inicio = (esq+dir)/2;
	      rel[1].comp++;

	      while (esq<dir)
	      {
	      	if (vetor[seg].chavePrimaria<vetor[inicio].chavePrimaria)
	         	dir = inicio;
				else
	         	esq = inicio+1;
				inicio = (esq+dir)/2;
		      rel[1].comp++;
	      } // Fim de while
      } // Fim de if

		if (inicio<seg)
		{
	    	troca.chavePrimaria = vetor[seg].chavePrimaria;
	      troca.ordemRegistro = vetor[seg].ordemRegistro;
	    	for (pos = seg; pos>esq; pos--)
	      {
	         vetor[pos].chavePrimaria = vetor[pos-1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos-1].ordemRegistro;
            rel[1].perm++;
	      } // Fim de for
	      vetor[inicio].chavePrimaria = troca.chavePrimaria;
	      vetor[inicio].ordemRegistro = troca.ordemRegistro;
         rel[1].perm++;
      } // Fim de if

   } // Fim de for

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de InsercaoBinaria

//----------------------------------------------------------------------------//

// Fun��o InsercaoDireta: M�todo de classifica��o "Inser��o Direta"
int InsercaoDireta(void)
{
   // Declara��o de vari�veis
	int inicio, pos;
   ARQ troca, vetor[MAX];
   FILE *arquivo, *relat;
   RELATORIO rel[3];
   clock_t t1, t2;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista1.dat", "w");

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
   for (int seg=1; seg<MAX; seg++)
   {
   	inicio = seg;

      while ((vetor[seg].chavePrimaria<vetor[inicio-1].chavePrimaria)&&(inicio>0))
			inicio--;

		if (inicio<seg)
      {
      	troca.chavePrimaria = vetor[seg].chavePrimaria;
         troca.ordemRegistro = vetor[seg].ordemRegistro;
      	for (pos = seg; pos>inicio; pos--)
         {
            vetor[pos].chavePrimaria = vetor[pos-1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos-1].ordemRegistro;
         } // Fim de for
         vetor[inicio].chavePrimaria = troca.chavePrimaria;
         vetor[inicio].ordemRegistro = troca.ordemRegistro;
      } // Fim de if

   } // Fim de for
   t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[0].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
   for (int seg=1; seg<MAX; seg++)
   {
   	inicio = seg;

      while ((vetor[seg].chavePrimaria<vetor[inicio-1].chavePrimaria)&&(inicio>0))
      {
			inicio--;
	      rel[0].comp++;
      } // Fim de while

		if (inicio<seg)
      {
      	troca.chavePrimaria = vetor[seg].chavePrimaria;
         troca.ordemRegistro = vetor[seg].ordemRegistro;
      	for (pos = seg; pos>inicio; pos--)
         {
            vetor[pos].chavePrimaria = vetor[pos-1].chavePrimaria;
				vetor[pos].ordemRegistro = vetor[pos-1].ordemRegistro;
            rel[0].perm++;
         } // Fim de for
         vetor[inicio].chavePrimaria = troca.chavePrimaria;
         vetor[inicio].ordemRegistro = troca.ordemRegistro;
         rel[0].perm++;
      } // Fim de if

   } // Fim de for

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

   return 1;

} // Fim de InsercaoDireta

//----------------------------------------------------------------------------//

// Fun��o ShellSort: M�todo de classifica��o "Shell Sort"
int ShellSort(void)
{
   // Declara��o de vari�veis
	int inc, np, nptemp, nseg, pos1, pos2, seg;
   ARQ vetor[MAX], troca;
   FILE *arquivo, *relat;
   RELATORIO rel[3];
   clock_t t1, t2;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista1.dat", "w");

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

	// Leitura do valor de np
   do
   {
	   printf("\tInsira o valor de np (incremento inicial = 2^np): ");
	   scanf("%d", &np);
      if ((np<0)||(np>10))
      {
      	printf("\n\tValor invalido! Digite um valor entre 0 e 10!\n\t[Pressione qualquer tecla para continuar...]\n\n\n");
         getch();
      } // Fim de if
   } while ((np<0)||(np>10));

   // Primeira ordena��o - captura do tempo
   t1 = clock();
	for (nptemp = np; nptemp>=0; nptemp--)
   {
   	inc = pow(2, nptemp);
   	for (nseg = 0; nseg < inc; nseg++)
		{
			for (seg = nseg+inc; seg < MAX; seg+=inc)
			{
				pos1 = nseg;
            pos2 = seg-inc;
	      	troca.chavePrimaria = vetor[seg].chavePrimaria;
	         troca.ordemRegistro = vetor[seg].ordemRegistro;

            while ((pos2 >= seg)&&(pos1 == seg))
            {
            	if (troca.chavePrimaria < vetor[pos2].chavePrimaria)
               {
               	vetor[pos2+inc].chavePrimaria = vetor[pos2].chavePrimaria;
               	vetor[pos2+inc].ordemRegistro = vetor[pos2].ordemRegistro;
                  pos2 = pos2-inc;
               } // Fim de if
               else
               	pos1 = pos2+inc;
            } // Fim de while

            vetor[pos2].chavePrimaria = troca.chavePrimaria;
            vetor[pos2].ordemRegistro = troca.ordemRegistro;

         } // Fim de for
      } // Fim de for
   } // Fim de for
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[2].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
	for (nptemp = np; nptemp>=0; nptemp--)
   {
   	inc = pow(2, nptemp);
   	for (nseg = 0; nseg < inc; nseg++)
		{
			for (seg = nseg+inc; seg < MAX; seg+=inc)
			{
				pos1 = nseg;
            pos2 = seg-inc;
	      	troca.chavePrimaria = vetor[seg].chavePrimaria;
	         troca.ordemRegistro = vetor[seg].ordemRegistro;

            while ((pos2 >= nseg)&&(pos1 == nseg))
            {
            	if (troca.chavePrimaria < vetor[pos2].chavePrimaria)
               {
               	vetor[pos2+inc].chavePrimaria = vetor[pos2].chavePrimaria;
               	vetor[pos2+inc].ordemRegistro = vetor[pos2].ordemRegistro;
                  pos2 = pos2-inc;
                  rel[2].perm++;
               } // Fim de if
               else
               	pos1 = pos2+inc;
               rel[2].comp++;
            } // Fim de while

            vetor[pos1].chavePrimaria = troca.chavePrimaria;
            vetor[pos1].ordemRegistro = troca.ordemRegistro;
            rel[2].perm++;
         } // Fim de for
      } // Fim de for
   } // Fim de for

	// Fechamento do arquivo de dados
   fclose(arquivo);

	for (int u=0; u<MAX; u++) printf("%d\t", vetor[u].chavePrimaria);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de ShellSort

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
		return fopen("Lista1.dat", "r");

	// Abrir para grava��o
	// Declara��o de vari�veis
	FILE *arq;
   RELATORIO rel[METODOS];

   arq = fopen("Lista1.dat", "r");

	// Cria��o de um novo arquivo relat�rio
 	if (!arq)
   {
    	fclose(arq);
      printf("\tCriando arquivo relatorio...\n");
		arq = fopen("Lista1.dat", "w");
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
      	case 0:	strcpy(string, "Insercao_Direta");
         			break;

      	case 1:	strcpy(string, "InsercaoBinaria");
         			break;

      	case 2:	strcpy(string, "Shell_Sort");
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

