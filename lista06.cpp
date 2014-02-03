/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 6 - Entrega: 13/06/2005                                              *
*                                                                              *
*   Objetivo: Testar e comparar a eficiência dos métodos de pesquisa em tabelas*
*                                                                              *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Nome: Nathalia Gonçalves Candido            Matrícula: 12744               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
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
// Protótipos

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
unsigned long int zAlfanumerica(char []);
unsigned long int Rotacao(unsigned long int);
void GeraEstatistica(short int colisoes[H][GG], ESTAT [], ESTAT [], short int &melH);

//----------------------------------------------------------------------------//
// Funções

void main(void) {

	// Declaração de variáveis
   short int i, j, k, colisoes[2*H][GG], ocorr[N][GG], melH = 0, end;
   unsigned long int aprox[GG], chave;
   TABELA tabela[N];
   ESTAT h[H], g[GG];
	char sentenca[30] = "";

	// Inicialização de variáveis
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
        colisoes[i][j] = 0;

   //---------------------------------------------------------------------------
   // Tabelas

	for (k = 0; k < 2*H; k++) {

	   for (i = 0; i < N; i++)
	   	for (j = 0; j < GG; j++)
	        tabela[i].end[j] = -1;

		// Rotação
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

	            } // Fim do switch
            } // Fim do for
	      } // Fim do for

	      else {

            switch (k % 4) {

			   	case 0:	tabela[i].end[0] = hDivisao((short int) tabela[i].chave);
               			break;

               case 1:  tabela[i].end[0] = hMeioQb((short int) tabela[i].chave);
                 			break;

               case 2:	tabela[i].end[0] = hMult((short int) tabela[i].chave);
                			break;

               case 3:	tabela[i].end[0] = hAleat((short int) tabela[i].chave);

            } // Fim do switch
         } // Fim do else
	   } // Fim do for

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

	      } // Fim do for

	      if ((i + 1) % 270 == 0) {

	      	printf("\n[Pressione qualquer tecla para continuar...]");
	      	getch();

	      } // Fim do if
	   } // Fim do for

	  	printf("\n[Fim da tabela]");
	   getch();

	   // Colisões
		// Zera número de ocorrências
	   for (i = 0; i < N; i++)
	   	for (j = 0; j < GG; j++)
	        ocorr[i][j] = 0;

		// Computa as ocorrências
	   for (i = 0; i < N; i++)
			for (j = 0; j < GG; j++)
	      	if (tabela[i].end[j] != -1)
	         	ocorr[tabela[i].end[j]][j]++;

		// Computa as colisões
	   for (j = 0; j < GG; j++){
	   	for (i = 0; i < N; i++){
	      	if (ocorr[i][j] > 1)
	         	colisoes[k][j] += (short int) (ocorr[i][j] - 1);

         } // Fim do for
      } // Fim do for
   } // Fim de for dos métodos


	// Tabela final
	GeraEstatistica(colisoes, h, g, melH);

   // Método da Chaves Alfanuméricas
   chave = zAlfanumerica(sentenca);

	if (melH > 3) chave = Rotacao(chave);

	switch (melH % 4) {
		case 0:	end = hDivisao(chave);
      			break;
      case 1:	end = hMeioQb(chave);
      			break;
		case 2:	end = hMult(chave);
      			break;
      case 3:	end = hAleat(chave);
      			break;
   } // Fim do switch

	printf("\nUtilizando z(x) de Conversao com deslocamento de bits:\n\tSentenca: %s\n", sentenca);
   printf("\nCalculando o endereco para esta chave com o melhor h(x): %s:", h[melH % 4].nome);
   if (melH > 3)	printf(" com %s", g[0].nome);
   printf("\n\tEndereco: %d\n", end);

   getch();

} // Fim de main

//----------------------------------------------------------------------------//
// Função GeraChaves: Gera chaves de 32 bits não-repetidas

void GeraChaves(TABELA tabela[N]) {

	// Declaração de variáveis
   short int i, j;
	unsigned long int temp;
   bool check;

   randomize();

   for (i = 0; i < N; i++) {

       do {

       	 check = 0;
	   	 temp = _lrand();

	       for (j = 0; (j < i) && (check == 0); j++) {

	       	if (temp == tabela[j].chave)
			  		check = 1;

	       } // Fim do for

       } while (check == 1);

       tabela[i].chave = temp;

   } // Fim do for

} // Fim da Função GeraChaves()

//----------------------------------------------------------------------------//
// Função hDivisao

short int hDivisao(unsigned long int x) {

	return (short int) ((x % N) + 1);

} // Fim da Função hDivisao

//----------------------------------------------------------------------------//
// Função hMeioQb

short int hMeioQb(unsigned long int x) {

	return (short int) ((x * x) >> 17);

} // Fim da Função hMeioQb()

//----------------------------------------------------------------------------//
// Função hMult

short int hMult(unsigned long int x) {

	// Declaração de variáveis
	unsigned long a = 265435769;

	return (short int) ((a * x) >> 23);

} // Fim da Função hMult()

//----------------------------------------------------------------------------//
// Função hAleat

short int hAleat(unsigned long int x) {

	// Declaração de variáveis
	unsigned long int a = 265435769;
   int c;

   // Geração do número aleatório c
   srand(x);

   c = rand();

	return (short int) ((a * x + c) >> 23);

} // Fim da Função hAleat()

//----------------------------------------------------------------------------//
// Função gMeioQ

short int gMeioQ(unsigned long int x) {

	// Declaração de variáveis
	short int digitos;
	unsigned __int64 x2, div1, div2;

	// Quadrado de x
	x2 = x;
   x2 *= x2;

	digitos = (short int) log10(x2);

   // Divisores para obtenção do meio do quadrado
   div1 = pow(10, (digitos / 2) + (DIG / 2));
   div2 = pow(10, (digitos / 2) - (DIG / 2));

	return (short int) ((x2 % div1) / div2);

} // Fim da Função gMeioQ()

//----------------------------------------------------------------------------//
// Função gDobra

short int gDobra(unsigned long int x) {

	// Declaração de variáveis
	short int digitos, i, j;
   int div, soma = 0;
   char vet[20] = "", ped[4] = "", temp;

   // Dígitos de x
 	digitos = (short int) (log10(x) + 1);

	// Formação do vetor de char
   itoa(x, vet, 10);

	// Inversão do vetor
	for (i = 0; i < digitos/2; i++) {

     	temp = vet[i];
		vet[i] = vet[digitos-1-i];
		vet[digitos-1-i] = temp;

   } // Fim do for

	// Complemento com zeros
	while (digitos % DIG != 0) {

   	vet[digitos] = '0';
      digitos++;

   } // Fim do while

	// Extração das partições de DIG dígitos e soma
	for (j = 0; j < digitos; j += DIG) {

		for (i = 0; i < DIG; i++)
   		ped[i] = vet[i+j];

		soma += atoi(ped);

   } // Fim do for

	// Extração dos últimos DIG dígitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;

} // Fim da Função gDobra()

//----------------------------------------------------------------------------//
// Função gSoma

short int gSoma(unsigned long int x) {

	// Declaração de variáveis
	short int digitos, i, j;
   int div, soma = 0;
   char vet[10] = "", ped[4] = "";

   // Dígitos de x
 	digitos = (short int) (log10(x) + 1);

	// Formação do vetor de char
   itoa(x, vet, 10);

	// Extração das partições de DIG dígitos e soma
	for (j = 0; j < digitos; j += DIG) {

		for (i = 0; i < DIG; i++)
   		ped[i] = vet[i+j];

		ped[i] = '\0';
		soma += atoi(ped);

   } // Fim do for

	// Extração dos últimos DIG dígitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;

} // Fim da Função gSoma()

//----------------------------------------------------------------------------//
// Função gSub

short int gSub(unsigned long int x) {

	// Declaração de variáveis
   char vet[10] = "", ped[4] = "";

	// Formação do vetor de char
   itoa(x, vet, 10);

	// Extração dos números
	ped[0] = vet[0];
   ped[1] = vet[2];
	ped[2] = vet[3];
	ped[3] = vet[4];
   ped[4] = '\0';

   return (short int) atoi(ped);

} // Fim da Função gSub()

//----------------------------------------------------------------------------//
// Função gConversao - Conversão de base 11 para base 10

short int gConversao(unsigned long int x) {

	// Declaração de variáveis
	short int digitos, i;
   int div;
	unsigned __int64 soma = 0ui64;

   // Dígitos de x
	digitos = (short int) (log10(x) + 1);

   for (i = 0; i < digitos; i++) {

   	div = pow(10, digitos - i);
		soma += (x % div) / pow(10, digitos - 1 - i) * pow(11, digitos - 1 - i);

   } // Fim do for

	// Extração dos últimos DIG dígitos
   div = pow(10, DIG);
   soma %= div;

   return (short int) soma;

} // Fim da Função gConversao()

//----------------------------------------------------------------------------//
// Função Rotacao

unsigned long int Rotacao(unsigned long int x) {

	// Declaração de variáveis
	short int ult;
   char vet[10] = "", temp;
   unsigned __int64 ret;

   // Dígitos de x
	ult = (short int) log10(x);

	// Formação do vetor de char
   itoa(x, vet, 10);

	// Rotação
  	temp = vet[0];
	vet[0] = vet[ult];
	vet[ult] = temp;

	// Só rotaciona quando se é permitido
	ret = atoi(vet);
   if (ret > pow(2, 32)) ret = x;

   return (unsigned long int) ret;

} // Fim da Função Rotacao()


//----------------------------------------------------------------------------//
// Função para Chaves Alfanuméricas

unsigned long int zAlfanumerica(char sentenca[30]) {

	// Definindo Variáveis Locais
   int number, soma = 0, i, j;

	randomize();

 	for (i = 0, j = 0; i < 30 && j < 5; i++, j++) {
  		number = (rand() % 26) + 97;

     	sentenca[i] = (char) number;

  	 	number = number << j;

     	soma = soma + number;

     	if (j == 4) j = -1;

  	} // Fim do for

  return soma;

} // Fim da Função zAlfanumerica()


//----------------------------------------------------------------------------//
// Função GeraEstatistica: Gera as estatísticas para os métodos

void GeraEstatistica(short int colisoes[H][GG], ESTAT h[H], ESTAT g[GG], short int &melH) {

	// Declaração de variáveis
   short int i, j, melhor, pior, melG, pioG, pioH;

	// Calcula o mínimo de colisões para cada H
	for (i = 0; i < 2*H; i++) {

	   for (j = 0; colisoes[i][j] == 0; j++);

		if (colisoes[i][j] != 0) {

      	h[i].colisoes = colisoes[i][j];

		   for (j = 0; j < GG; j++)
	      	if (colisoes[i][j] != 0) h[i].colisoes = min(h[i].colisoes, colisoes[i][j]);

      } // Fim do if
   } // Fim do for

	// Calcula o mínimo de colisões para cada G
	for (j = 1; j < GG; j++) {

		g[j].colisoes = colisoes[0][j];

      for (i = 0; i < 2*H; i++)
	   	g[j].colisoes = min(g[j].colisoes, colisoes[i][j]);

   } // Fim do for

   // Impressão dos resultados
	clrscr();
   printf("\nCOLISOES:\n");

	printf("\nMetodos de calculo h(x):");
	printf("\nMetodo\t\tMinimo de colisoes");
   melhor = pior = h[0].colisoes;
   melH = pioH = 0;

   for (i = 0; i < H; i++) {

   	melhor = min(melhor, h[i].colisoes);
      pior = max(pior, h[i].colisoes);

      if (melhor == h[i].colisoes)	melH = i;
      if (pior == h[i].colisoes)		pioH = i;

   	printf("\n%s:\t\t%d", h[i].nome, h[i].colisoes);

   } // Fim do for

	printf("\n\nMetodos de calculo h(x) apos %s:", g[0].nome);
	printf("\nMetodo\t\tMinimo de colisoes");

   for (i = H; i < 2*H; i++) {

   	melhor = min(melhor, h[i].colisoes);
      pior = max(pior, h[i].colisoes);

      if (melhor == h[i].colisoes)	melH = i;
      if (pior == h[i].colisoes)		pioH = i;

   	printf("\n%s:\t\t%d", h[i % 4].nome, h[i].colisoes);

   } // Fim do for

	printf("\n\nMetodos de aproximacao g(x):");
	printf("\nMetodo\t\tMinimo de colisoes");
   melhor = pior = g[1].colisoes;
   melG = pioG = 1;

   for (i = 1; i < GG; i++) {

   	melhor = min(melhor, g[i].colisoes);
      pior = max(pior, g[i].colisoes);

      if (melhor == g[i].colisoes)	melG = i;
      if (pior == g[i].colisoes)		pioG = i;

   	printf("\n%s:\t\t%d", g[i].nome, g[i].colisoes);

   } // Fim do for

   printf("\n\n[Pressione qualquer tecla para continuar...]");
   getch();

   clrscr();

	// Piores
   printf("\n\nOs piores metodos em colisoes foram:\n");

   printf("h(x): %s ", h[pioH % 4].nome);

   if (pioH > 3)	printf("com %s ", g[0].nome);

   printf("- %d colisoes\ng(x): %s - %d colisoes", h[pioH].colisoes, g[pioG].nome, g[pioG].colisoes);
	printf("\n\n");

	// Melhores
   printf("\n\nOs melhores metodos em colisoes foram:\n");

   printf("h(x): %s ", h[melH % 4].nome);

   if (melH > 3)	printf("com %s ", g[0].nome);

   printf("- %d colisoes\ng(x): %s - %d colisoes", h[melH].colisoes, g[melG].nome, g[melG].colisoes);
	printf("\n\n");

   getch();

} // Fim da Função GeraEstatistica()

