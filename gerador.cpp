/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Programa auxiliar �s listas																 *
*                                                                              *
*   Objetivo: Gera��o de oito mil n�meros inteiros para ordena��o,			    *
*				  e grava��o no arquivo ChavePri.dat.                              *
*                                                                              *
*   Nome: Alu�sio Moura C. dos Santos           Matr�cula: 12615               *
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
   FILE *arq = fopen("ChavePri.dat", "w");
   char *msg = "\n\n\t";

	if (arq == NULL)		// O arquivo n�o foi corretamente criado
   	strcat(msg, "\aERRO: Nao foi possivel criar o arquivo!");
   else						// O arquivo foi criado
   {
      randomize();
      for (int i=0; i<MAX; i++)		// Gera��o de mil inteiros aleat�rios
	   	fprintf(arq, "%d\n", rand());
      strcat(msg, "Foram gerados oito mil numeros inteiros em 'ChavePri.dat'.");
   } // Fim de if

   // Impress�o dos resultados
   strcat(msg, "\n\t[Pressione qualquer tecla para continuar...]\n");
   printf("%s", msg);
   getch();
   fclose(arq);
} // Fim de main

