/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 6 - Entrega: 13/06/2005                                              *
*                                                                              *
*   Objetivo: Testar e comparar a efici�ncia dos m�todos de pesquisa em tabelas*
*                                                                              *
*   Nome: Tiago Romero Garcia                   Matr�cula: 12643               *
*   Nome: Nathalia Gon�alves Candido            Matr�cula: 12744               *
*   Ci�ncia da Computa��o - Universidade Federal de Itajub�                    *
*                                                                              *
*******************************************************************************/

//----------------------------------------------------------------------------//
// Bibliotecas

#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------//
// Constantes

#define N 1009
#define H 4
#define G 5
#define GG G+1
const short int DIG = log10(N) + 1;

//----------------------------------------------------------------------------//
// Estruturas

typedef struct ESTAT {
	char nome[10];
	short int colisoes;
} ESTAT;

typedef struct TABELA {
	unsigned long int chave;
   short int end[GG];
} TABELA;

//----------------------------------------------------------------------------//
// Prot�tipos

void GeraChaves(TABELA []);
short int hDivisao(unsigned long int);
short int hMeioQb(unsigned long int);
short int hMult(unsigned long int);
short int hAleat(unsigned long int);
short int gMeioQ(unsigned long int);
short int gDobra(unsigned long int);
short int gSoma(unsigned long int);
short int gSub(unsigned long int);
short int gConversao(unsigned long int x);
unsigned long int Rotacao(unsigned long int);
void GeraEstatistica(short int colisoes[H][GG], ESTAT [], ESTAT[]);

//----------------------------------------------------------------------------//
// Fun��es

void main(void) {
	// Declara��o de vari�veis
   short int i, j, k, colisoes[2*H][GG], ocorr[N][GG];
   unsigned long int aprox[GG];
   TABELA tabela[N];
   ESTAT h[H], g[GG];

	// Inicializa��o de vari�veis
   GeraChaves(tabela);
   strcpy(h[0].nome, "Divisao");
   strcpy(h[1].nome, "MeioQb ");
   strcpy(h[2].nome, "Mul    ");
   strcpy(h[3].nome, "Aleatorio");
   strcpy(g[0].nome, "Rotacao");
   strcpy(g[1].nome, "MeioQ  ");
   strcpy(g[2].nome, "Dobra  ");
   strcpy(g[3].nome, "Soma   ");
   strcpy(g[4].nome, "Sub    ");
   strcpy(g[5].nome, "Conversao");
   for (i = 0; i < 2*H; i++)
   	for (j = 0; j < GG; j++)
        colisoes[i][j] = -1;

   //---------------------------------------------------------------------------
   // Tabelas

	for (k = 0; k < 2*H; k++) {
	   for (i = 0; i < N; i++)
	   	for (j = 0; j < GG; j++)
	        tabela[i].end[j] = -1;

		if (k == 4)
		   for (i = 0; i < N; i++)
         	tabela[i].chave = Rotacao(tabela[i].chave);

	   for (i = 0; i < N; i++) {
	   	if ((log10(tabela[i].chave) + 1) > DIG) {
	      	aprox[1] = gMeioQ(tabela[i].chave);
	         aprox[2] = gDobra(tabela[i].chave);
	         aprox[3] = gSoma(tabela[i].chave);
	         aprox[4] = gSub(tabela[i].chave);
	         aprox[5] = gConversao(tabela[i].chave);
            for (j = 1; j < GG; j++) {
	            switch (k % 4) {
				   	case 0:	tabela[i].end[j] = hDivisao(aprox[j]);
	               			break;

	               case 1:  tabela[i].end[j] = hMeioQb(aprox[j]);
                  			break;

                  case 2:	tabela[i].end[j] = hMult(aprox[j]);
                  			break;

                  case 3:	tabela[i].end[j] = hAleat(aprox[j]);
	            }
            }
	      }
	      else {
            switch (k % 4) {
			   	case 0:	tabela[i].end[0] = hDivisao((short int) tabela[i].chave);
               			break;

               case 1:  tabela[i].end[0] = hMeioQb((short int) tabela[i].chave);
                 			break;

               case 2:	tabela[i].end[0] = hMult((short int) tabela[i].chave);
                			break;

               case 3:	tabela[i].end[0] = hAleat((short int) tabela[i].chave);
            }
         }
	   }

	   // Listagem
	   clrscr();
	   printf("\nTabela %d: ", k+1);
      if (k > 3) printf("%s e ", g[0].nome);
		printf("h(x): %s", h[k % 4].nome);
	   printf("\nChave\t\tDireto\t%s\t%s\t%s\t%s\t%s", g[1].nome, g[2].nome, g[3].nome, g[4].nome, g[5].nome);
	   getch();
	   for (i = 0; i < N; i++)	{
			printf("\n%10d", tabela[i].chave);
			for (j = 0; j < GG; j++) {
		      if (tabela[i].end[j] == -1) printf("\t----");
		      else printf("\t%4d", tabela[i].end[j]);
	      }
	      if ((i + 1) % 270 == 0) {
	      	printf("\n[Pressione qualquer tecla para continuar...]");
	      	getch();
	      }
	   }
	  	printf("\n[Fim da tabela]");
	   getch();

	   // Colis�es
		// Zera n�mero de ocorr�ncias
	   for (i = 0; i < N; i++)
	   	for (j = 0; j < GG; j++)
	        ocorr[i][j] = 0;

		// Computa as ocorr�ncias
	   for (i = 0; i < N; i++)
			for (j = 0; j < GG; j++)
	      	if (tabela[i].end[j] != -1)
	         	ocorr[tabela[i].end[j]][j]++;

		// Computa as colis�es
	   for (j = 0; j < GG; j++)
	   	for (i = 0; i < N; i++)
	      	if (ocorr[i][j] > 1) {
            	if (colisoes[k][j] == -1)	colisoes[k][j] = 0;
	         	colisoes[k][j] += (short int) (ocorr[i][j] - 1);
            }

   } // Fim de for dos m�todos

   //---------------------------------------------------------------------------
   // M�todo da rota��o

   //---------------------------------------------------------------------------
	// Tabela final

	GeraEstatistica(colisoes, h, g);

} // Fim de main

//----------------------------------------------------------------------------//
// Fun��o GeraChaves: Gera chaves de 32 bits n�o-repetidas

void GeraChaves(TABELA tabela[N]) {
	// Declara��o de vari�veis
   short int i, j;
	unsigned long int temp;
   bool check;

   randomize();
   for (i = 0; i < N; i++) {
       do {
       	 check = 0;
	   	 temp = _lrand();
	       for (j = 0; (j < i) && (check == 0); j++) {
	       	if (temp == tabela[j].chave) {
			  		check = 1;
	         }
	       }
       } while (check == 1);
       tabela[i].chave = temp;
   }

} // Fim de GeraChaves

//----------------------------------------------------------------------------//
// Fun��o hDivisao

short int hDivisao(unsigned long int x) {
	return (short int) ((x % N) + 1);
} // Fim de hDivisao

//----------------------------------------------------------------------------//
// Fun��o hMeioQb

short int hMeioQb(unsigned long int x) {
	return (short int) ((x * x) >> 17);
} // Fim de hMeioQb

//----------------------------------------------------------------------------//
// Fun��o hMult

short int hMult(unsigned long int x) {
	// Declara��o de vari�veis
	unsigned long a = 265435769;

	return (short int) ((a * x) >> 23);
} // Fim de hMult

//----------------------------------------------------------------------------//
// Fun��o hAleat

short int hAleat(unsigned long int x) {
	// Declara��o de vari�veis
	unsigned long int a = 265435769;
   int c;

   // Gera��o do n�mero aleat�rio c
   srand(x);
   c = rand();

	return (short int) ((a * x + c) >> 23);
} // Fim de hAleat

//----------------------------------------------------------------------------//
// Fun��o gMeioQ

short int gMeioQ(unsigned long int x) {
	// Declara��o de vari�veis
	short int digitos;
	unsigned __int64 x2, div1, div2;

	// Quadrado de x
	x2 = x;
   x2 *= x2;
	digitos = (short int) log10(x2);

   // Divisores para obten��o do meio do quadrado
   div1 = pow(10, (digitos / 2) + (DIG / 2));
   div2 = pow(10, (digitos / 2) - (DIG / 2));

	return (short int) ((x2 % div1) / div2);
} // Fim de gMeioQ

//----------------------------------------------------------------------------//
// Fun��o gDobra

short int gDobra(unsigned long int x) {
	// Declara��o de vari�veis
	short int digitos, i, j;
   int div, soma = 0;
   char vet[20] = "", ped[4] = "", temp;

   // D�gitos de x
 	digitos = (short int) (log10(x) + 1);

	// Forma��o do vetor de char
   itoa(x, vet, 10);

	// Invers�o do vetor
	for (i = 0; i < digitos/2; i++) {
     	temp = vet[i];
		vet[i] = vet[digitos-1-i];
		vet[digitos-1-i] = temp;
   }

	// Complemento com zeros
	while (digitos % DIG != 0) {
   	vet[digitos] = '0';
      digitos++;
   }

	// Extra��o das parti��es de DIG d�gitos e soma
	for (j = 0; j < digitos; j += DIG) {
		for (i = 0; i < DIG; i++)
   		ped[i] = vet[i+j];
		soma += atoi(ped);
   }

	// Extra��o dos �ltimos DIG d�gitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;
} // Fim de gDobra

//----------------------------------------------------------------------------//
// Fun��o gSoma

short int gSoma(unsigned long int x) {
	// Declara��o de vari�veis
	short int digitos, i, j;
   int div, soma = 0;
   char vet[10] = "", ped[4] = "";

   // D�gitos de x
 	digitos = (short int) (log10(x) + 1);

	// Forma��o do vetor de char
   itoa(x, vet, 10);

	// Extra��o das parti��es de DIG d�gitos e soma
	for (j = 0; j < digitos; j += DIG) {
		for (i = 0; i < DIG; i++)
   		ped[i] = vet[i+j];
		ped[i] = '\0';
		soma += atoi(ped);
   }

	// Extra��o dos �ltimos DIG d�gitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;
} // Fim de gSoma

//----------------------------------------------------------------------------//
// Fun��o gSub

short int gSub(unsigned long int x) {
	// Declara��o de vari�veis
   char vet[10] = "", ped[4] = "";

	// Forma��o do vetor de char
   itoa(x, vet, 10);

	// Extra��o dos n�meros
	ped[0] = vet[0];
   ped[1] = vet[2];
	ped[2] = vet[3];
	ped[3] = vet[4];
   ped[4] = '\0';

   return (short int) atoi(ped);
} // Fim de gSub

//----------------------------------------------------------------------------//
// Fun��o gConversao - Convers�o de base 11 para base 10

short int gConversao(unsigned long int x) {
	// Declara��o de vari�veis
	short int digitos, i;
   int div;
	unsigned __int64 soma = 0ui64;

   // D�gitos de x
	digitos = (short int) (log10(x) + 1);

   for (i = 0; i < digitos; i++) {
   	div = pow(10, digitos - i);
		soma += (x % div) / pow(10, digitos - 1 - i) * pow(11, digitos - 1 - i);
   }

	// Extra��o dos �ltimos DIG d�gitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;
} // Fim de gConversao

//----------------------------------------------------------------------------//
// Fun��o Rotacao

unsigned long int Rotacao(unsigned long int x) {
	// Declara��o de vari�veis
	short int ult;
   char vet[10] = "", temp;
   unsigned __int64 ret;

   // D�gitos de x
	ult = (short int) log10(x);

	// Forma��o do vetor de char
   itoa(x, vet, 10);

	// Rota��o
  	temp = vet[0];
	vet[0] = vet[ult];
	vet[ult] = temp;

	// S� rotaciona quando se � permitido
	ret = atoi(vet);
   if (ret > pow(2, 32)) ret = x;

   return (unsigned long int) ret;
} // Fim de Rotacao

//----------------------------------------------------------------------------//
// Fun��o GeraEstatistica: Gera as estat�sticas para os m�todos

void GeraEstatistica(short int colisoes[H][GG], ESTAT h[H], ESTAT g[GG]) {
	// Declara��o de vari�veis
   short int i, j, melhor, melH, melG;

	// Calcula o m�nimo de colis�es para cada H
	for (i = 0; i < 2*H; i++) {
	   for (j = 0; colisoes[i][j] == -1; j++);
		if (colisoes[i][j] != -1) {
      	h[i].colisoes = colisoes[i][j];
		   for (j = 0; j < GG; j++)
	      	if (colisoes[i][j] != -1) h[i].colisoes = min(h[i].colisoes, colisoes[i][j]);
      }
   }

	// Calcula o m�nimo de colis�es para cada G
	for (j = 1; j < GG; j++) {
		g[j].colisoes = colisoes[0][j];
	   for (i = 0; i < 2*H; i++)
	   	g[j].colisoes = min(g[j].colisoes, colisoes[i][j]);
   }

   // Impress�o dos resultados
	clrscr();
   printf("\nColisoes:\n");

	printf("\nMetodos de calculo h(x):");
	printf("\nMetodo\t\tMinimo de colisoes");
   melhor = h[0].colisoes;
   melH = 0;
   for (i = 0; i < H; i++) {
   	melhor = min(melhor, h[i].colisoes);
      if (melhor == h[i].colisoes)	melH = i;
   	printf("\n%s:\t%d", h[i].nome, h[i].colisoes);
   }

	printf("\n\nMetodos de calculo h(x) apos %s:", g[0].nome);
	printf("\nMetodo\t\tMinimo de colisoes");
   for (i = H; i < 2*H; i++) {
   	melhor = min(melhor, h[i].colisoes);
      if (melhor == h[i].colisoes)	melH = i;
   	printf("\n%s:\t%d", h[i % 4].nome, h[i].colisoes);
   }

	printf("\n\nMetodos de aproximacao g(x):");
	printf("\nMetodo\t\tMinimo de colisoes");
   melhor = g[1].colisoes;
   melG = 1;
   for (i = 1; i < GG; i++) {
   	melhor = min(melhor, g[i].colisoes);
      if (melhor == g[i].colisoes)	melG = i;
   	printf("\n%s:\t%d", g[i].nome, g[i].colisoes);
   }

   printf("\n\n[Pressione qualquer tecla para continuar...]");
   getch();

   printf("\n\nOs melhores metodos em colisoes foram:\n");
   printf("h(x): %s ", h[melH % 4].nome);
   if (melH > 3)	printf("com %s ", g[0].nome);
   printf("- %d colisoes\ng(x): %s - %d colisoes", h[melH].colisoes, g[melG].nome, g[melG].colisoes);
	printf("\n\n");
   getch();

} // Fim de GeraEstatistica

