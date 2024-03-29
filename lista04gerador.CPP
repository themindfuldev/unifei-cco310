/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Programa auxiliar �s listas																 *
*                                                                              *
*   Objetivo: Gera��o de oito mil n�meros inteiros para ordena��o,			    *
*				  e grava��o nos arquivos Arquivo_01.dat e Arquivo_02.dat          *
*                                                                              *
*   Nome: Andr� Carvalho Silveira               Matr�cula: 12616               *
*   Nome: Tiago Romero Garcia                   Matr�cula: 12643               *
*   Ci�ncia da Computa��o - Universidade Federal de Itajub�                    *
*                                                                              *
*******************************************************************************/

// Declara��o de bibliotecas
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------//

// Declara��o de constantes
#define MAX 8000

//----------------------------------------------------------------------------//

// Programa main
void main()
{
	// Declara��o de vari�veis
   FILE *arq1, *arq2;
   char *msg = "\n";

   arq1 = fopen("Arquivo_01.dat", "w");
   arq2 = fopen("Arquivo_02.dat", "w");

	if ((arq1 == NULL) || (arq2 == NULL))		// O arquivo n�o foi corretamente criado
   	strcat(msg, "\aERRO: Nao foi possivel criar os arquivos!");
   else													// O arquivo foi criado
   {
      randomize();
      for (int i=0; i<MAX; i++)					// Gera��o de mil inteiros aleat�rios
	   	fprintf(arq1, "%d\n", rand());
      strcat(msg, "\n\tForam gerados oito mil numeros inteiros em 'Arquivo_01.dat'.");
      for (int i=0; i<MAX; i++)					// Gera��o de mil inteiros aleat�rios
	   	fprintf(arq2, "%d\n", rand());
      strcat(msg, "\n\tForam gerados oito mil numeros inteiros em 'Arquivo_02.dat'.");
   } // Fim de if

   // Impress�o dos resultados
   strcat(msg, "\n\t[Pressione qualquer tecla para continuar...]\n");
   printf("%s", msg);
   getch();
   fclose(arq1);
   fclose(arq2);   
} // Fim de main

