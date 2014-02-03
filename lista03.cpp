/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 3 - Entrega: 20/04/2005                                              *
*                                                                              *
*   Objetivos:                                                                 *
*     1. Testar e comparar a efici�ncia dos m�todos de classifica��o:          *
*	      Sele��o Direta e Heap Sort.												       *
*		2. Computar um relat�rio comparativo entre todos os m�todos anteriores.  *
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
#define METODOS 2
#define TOTAL 9

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
int  LeArquivoREL(FILE *, RELATORIO [], int, int);
int  RelatorioEstatistico(void);
int  RelatorioComparativo(void);
int  RelatorioGrafico(void);
int  IniciaModoGrafico(void);
void GeraFatorEscala(RELATORIO [], unsigned long &, unsigned long &, unsigned long &);
int  fatorial(int);
int  SelecaoDireta(void);
int  HeapSort(void);
int  Heap(int, int, ARQ []);
int  HeapComp(int, int, ARQ [], RELATORIO &);

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
         case 1: // Ordenar o arquivo de dados por Sele��o Direta
         	ret = SelecaoDireta();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 2: // Ordenar o arquivo de dados por Heap Sort
         	ret = HeapSort();
            if (!ret) printf("\n\t\aERRO: Nao foi possivel carregar o arquivo.");
            else printf("\tMetodo executado com sucesso, e suas estatisticas devidamente geradas.");
				printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			   getch();
            break;

         case 3: // Gerar o relat�rio estat�stico
				ret = RelatorioEstatistico();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
				printf("\n\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
            break;

         case 4: // Gerar o relat�rio gr�fico
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

         case 5: // Gerar o relat�rio comparativo de todos os m�todos
				ret = RelatorioComparativo();
            if (!ret) printf("\t\aERRO: Nao foi possivel carregar o arquivo de relatorio.");
				printf("\n\n\t[Pressione qualquer tecla para continuar...]\n");
            getch();
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
	printf("\n     Lista 3 - Classificacao e Pesquisa de Dados / Prof. Sdnei Brito");
	printf("\n     Nome: Aluisio Moura C. dos Santos  -  Matricula: 12615  -  CCO 2004");
	printf("\n     Nome: Tiago Romero Garcia          -  Matricula: 12643  -  CCO 2004");
   printf("\n\n\n\tMenu de Opcoes:");
   printf("\n\t  1. Ordenar o arquivo de dados por Selecao Direta");
   printf("\n\t  2. Ordenar o arquivo de dados por Heap Sort");
   printf("\n\t  3. Gerar o relatorio estatistico");
   printf("\n\t  4. Gerar o relatorio grafico");
   printf("\n\t  5. Gerar o relatorio comparativo de todos os metodos");
   printf("\n\t  0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);
   printf("\n\n");

   // Retorno da opcao escolhida
   return op;

} // Fim de Menu

//----------------------------------------------------------------------------//

// Fun��o SelecaoDireta: M�todo de ordena��o "Sele��o Direta"
int SelecaoDireta(void)
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   int k;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel, 0, METODOS);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista3.dat", "w");

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
   for (int i=0; i < MAX-1; i++)
   {
   	k = i;
      temp.chavePrimaria = vetor[i].chavePrimaria;
      temp.ordemRegistro = vetor[i].ordemRegistro;
      for (int j=i+1; j < MAX; j++)
      {
      	if (vetor[j].chavePrimaria < temp.chavePrimaria)
         {
         	k = j;
            temp.chavePrimaria = vetor[k].chavePrimaria;
            temp.ordemRegistro = vetor[k].ordemRegistro;
         } // Fim de ir
      } // Fim de for
      vetor[k].chavePrimaria = vetor[i].chavePrimaria;
		vetor[k].ordemRegistro = vetor[i].ordemRegistro;
      vetor[i].chavePrimaria = temp.chavePrimaria;
		vetor[i].ordemRegistro = temp.ordemRegistro;
   } // Fim de for
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[0].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
   for (int i=0; i < MAX-1; i++)
   {
   	k = i;
      temp.chavePrimaria = vetor[i].chavePrimaria;
      temp.ordemRegistro = vetor[i].ordemRegistro;
      for (int j=i+1; j < MAX; j++)
      {
      	if (vetor[j].chavePrimaria < temp.chavePrimaria)
         {
         	k = j;
            temp.chavePrimaria = vetor[k].chavePrimaria;
            temp.ordemRegistro = vetor[k].ordemRegistro;
            rel[0].comp++;
         } // Fim de ir
      } // Fim de for
      vetor[k].chavePrimaria = vetor[i].chavePrimaria;
		vetor[k].ordemRegistro = vetor[i].ordemRegistro;
      vetor[i].chavePrimaria = temp.chavePrimaria;
		vetor[i].ordemRegistro = temp.ordemRegistro;
      rel[0].perm++;
   } // Fim de for


	for (int i=0; i<MAX; i++)
   {
   	printf("%d  ", vetor[i].chavePrimaria);
      if (i%10==0) printf("\n");
   }
   printf("\n%d\t%d\t%f\n", rel[0].comp, rel[0].perm, rel[0].tempo);

   
	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de SelecaoDireta

//----------------------------------------------------------------------------//

// Fun��o HeapSort: M�todo de ordena��o "HeapSort"
int HeapSort(void)
{
   // Declara��o de vari�veis
   ARQ temp, vetor[MAX];
   FILE *arquivo, *relat;
  	RELATORIO rel[METODOS];
   clock_t t1, t2;
   int l, r;

	// Arquivo relat�rio
   // Abertura
   relat = AbreArquivoREL(false);
   if (!relat)
   	return 0;

	// Leitura
	LeArquivoREL(relat, rel, 0, METODOS);

   // Modo de grava��o
   fclose(relat);
   relat = fopen("Lista3.dat", "w");

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
   l = MAX/2 + 1;
   r = MAX;
   while (l > 1)
   {
   	l--;
      Heap(l, r, vetor);
   } // Fim de while
   while (r > 1)
   {
   	temp.chavePrimaria = vetor[1].chavePrimaria;
   	temp.ordemRegistro = vetor[1].ordemRegistro;
   	vetor[1].chavePrimaria = vetor[r].chavePrimaria;
   	vetor[1].ordemRegistro = vetor[r].ordemRegistro;
   	vetor[r].chavePrimaria = temp.chavePrimaria;
   	vetor[r].ordemRegistro = temp.ordemRegistro;
      r--;
      Heap(l, r, vetor);
   } // Fim de while
	t2 = clock();

   // Inser��o das estat�sticas de tempo
   rel[1].tempo = (t2-t1)/CLK_TCK;

	// Nova leitura do arquivo de dados
   LeArquivoARQ(arquivo, vetor);

   // Segunda ordena��o - captura do n�mero de compara��es e permutas
   l = MAX/2 + 1;
   r = MAX;
   while (l > 1)
   {
   	l--;
      HeapComp(l, r, vetor, rel[1]);
   } // Fim de while
   while (r > 1)
   {
   	temp.chavePrimaria = vetor[1].chavePrimaria;
   	temp.ordemRegistro = vetor[1].ordemRegistro;
   	vetor[1].chavePrimaria = vetor[r].chavePrimaria;
   	vetor[1].ordemRegistro = vetor[r].ordemRegistro;
   	vetor[r].chavePrimaria = temp.chavePrimaria;
   	vetor[r].ordemRegistro = temp.ordemRegistro;
      rel[1].perm++;
      r--;
      HeapComp(l, r, vetor, rel[1]);
   } // Fim de while

	// Fechamento do arquivo de dados
   fclose(arquivo);

	// Grava��o e fechamento do arquivo relat�rio
   GravaArquivoREL(relat, rel);
   fclose(relat);

	return 1;

} // Fim de HeapSort

//----------------------------------------------------------------------------//

// Fun��o Heap: Forma um heap
int Heap(int l, int r, ARQ vetor[])
{
	// Declara��o de vari�veis
   int i, j;
   ARQ temp;

	// Inicializa��o de vari�veis
   i = l;
   j = 2*l;
   temp.chavePrimaria = vetor[l].chavePrimaria;
	temp.ordemRegistro = vetor[l].ordemRegistro;
   if ((j < r) && (vetor[j].chavePrimaria < vetor[j+1].chavePrimaria))
   {
		j++;
   } // Fim de if
   while ((j <= r) && (temp.chavePrimaria < vetor[j].chavePrimaria ))
   {
   	vetor[i].chavePrimaria = vetor[j].chavePrimaria;
		vetor[i].ordemRegistro = vetor[j].ordemRegistro;

      i = j;
      j = 2*j;
	   if ((j < r) && (vetor[j].chavePrimaria < vetor[j+1].chavePrimaria))
      {
			j++;
	   } // Fim de if
   } // Fim de while
   vetor[i].chavePrimaria = temp.chavePrimaria;
   vetor[i].ordemRegistro = temp.ordemRegistro;

	return 1;

} // Fim de Heap

//----------------------------------------------------------------------------//

// Fun��o HeapComp: Forma um heap e salva as estat�sticas
int HeapComp(int l, int r, ARQ vetor[], RELATORIO &rel1)
{
	// Declara��o de vari�veis
   int i, j;
   ARQ temp;

	// Inicializa��o de vari�veis
   i = l;
   j = 2*l;
   temp.chavePrimaria = vetor[l].chavePrimaria;
	temp.ordemRegistro = vetor[l].ordemRegistro;
   if ((j < r) && (vetor[j].chavePrimaria < vetor[j+1].chavePrimaria))
   {
   	rel1.comp++;
		j++;
   } // Fim de if
   while ((j <= r) && (temp.chavePrimaria < vetor[j].chavePrimaria ))
   {
   	vetor[i].chavePrimaria = vetor[j].chavePrimaria;
		vetor[i].ordemRegistro = vetor[j].ordemRegistro;
      rel1.perm++;

      i = j;
      j = 2*j;
	   if ((j < r) && (vetor[j].chavePrimaria < vetor[j+1].chavePrimaria))
      {
	   	rel1.comp++;
			j++;
	   } // Fim de if
   } // Fim de while
   vetor[i].chavePrimaria = temp.chavePrimaria;
   vetor[i].ordemRegistro = temp.ordemRegistro;
   rel1.perm++;

	return 1;

} // Fim de Heap

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
		return fopen("Lista3.dat", "r");

	// Abrir para grava��o
	// Declara��o de vari�veis
	FILE *arq;
   RELATORIO rel[METODOS];

   arq = fopen("Lista3.dat", "r");

	// Cria��o de um novo arquivo relat�rio
 	if (!arq)
   {
    	fclose(arq);
      printf("\tCriando arquivo relatorio...\n");
		arq = fopen("Lista3.dat", "w");
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
int LeArquivoREL(FILE *arquivo, RELATORIO rel[METODOS], int base, int tot)
{
	for (int i=base; i<tot; i++)
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
      	case 0:	strcpy(string, "Selecao_Direta");
         			break;

      	case 1:	strcpy(string, "Heap_Sort");
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
	LeArquivoREL(relat, rel, 0, METODOS);

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
	LeArquivoREL(relat, rel, 0, METODOS);

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
	   bar3d(xi+xg*2+3, ceil(400-rel[i].tempo), xi+xg*3-3, 400, 20, 1);

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

//----------------------------------------------------------------------------//

// Fun��o RelatorioComparativo: Gera o relat�rio comparativo de todos os m�todos
int RelatorioComparativo(void)
{
	// Declara��o de vari�veis
	FILE *lista1, *lista2, *lista3;
   RELATORIO rel[TOTAL], melComp, melPerm;

	// Arquivo relat�rio
   // Abertura
   lista1 = fopen("Lista1.dat", "r");
	lista2 = fopen("Lista2.dat", "r");
	lista3 = fopen("Lista3.dat", "r");
   if ((!lista1)||(!lista2)||(!lista3))
   	return 0;

	// Leitura
	LeArquivoREL(lista1, rel, 0, 3);
	LeArquivoREL(lista2, rel, 3, 7);
	LeArquivoREL(lista3, rel, 7, TOTAL);

	strcpy(melComp.nome, rel[0].nome);
   melComp.comp = rel[0].comp;
	strcpy(melPerm.nome, rel[0].nome);
   melPerm.perm = rel[0].perm;
   for (int i=1; i<TOTAL; i++)
   {
		if (melComp.comp > rel[i].comp)
      {
			strcpy(melComp.nome, rel[i].nome);
		   melComp.comp = rel[i].comp;
      } // Fim de if
		if (melPerm.perm > rel[i].perm)
      {
			strcpy(melPerm.nome, rel[i].nome);
		   melPerm.perm = rel[i].perm;
      } // Fim de if
   } // Fim de for

   // Impress�o da estat�sticas
   clrscr();
   printf("Relatorio Comparativo");
   gotoxy(1, 3); printf("Nome");
	gotoxy(20, 3); printf("Comparacoes");
	gotoxy(35, 3); printf("Permutacoes");
	gotoxy(50, 3); printf("Tempo (s)");
   for (int i=0; i<TOTAL; i++)
   {
   	gotoxy(1, i+5); printf("%s", rel[i].nome);
		gotoxy(20, i+5); printf("%ld", rel[i].comp);
		gotoxy(35, i+5); printf("%ld", rel[i].perm);
      gotoxy(50, i+5);
      if (rel[i].tempo)
      	printf("%.3f", rel[i].tempo);
      else
      	printf("-----");
	} // Fim de for
   printf("\n\n\tO melhor em comparacoes eh o %s.", melComp.nome);
   printf("\n\tO melhor em permutacoes eh o %s.", melPerm.nome);

	// Fechamento do arquivo relat�rio
   fclose(lista1);
   fclose(lista2);
   fclose(lista3);

	return 1;
} // Fim de RelatorioComparativo

//----------------------------------------------------------------------------//

// Fun��o fatorial: Calcula o fatorial do n�mero
int fatorial(int x)
{
	int result = 1;
	for (int i = 2; i <= x; i++)
		result *= i;
	return result;
} // Fim de fatorial
