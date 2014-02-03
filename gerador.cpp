/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Programa auxiliar às listas																 *
*                                                                              *
*   Objetivo: Geração de oito mil números inteiros para ordenação,			    *
*				  e gravação no arquivo ChavePri.dat.                              *
*                                                                              *
*   Nome: Aluísio Moura C. dos Santos           Matrícula: 12615               *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
*                                                                              *
*******************************************************************************/

// Declaração de bibliotecas
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------//

// Declaração de constantes
#define MAX 8000

//----------------------------------------------------------------------------//

// Programa main
void main()
{
	// Declaração de variáveis
   FILE *arq = fopen("ChavePri.dat", "w");
   char *msg = "\n\n\t";

	if (arq == NULL)		// O arquivo não foi corretamente criado
   	strcat(msg, "\aERRO: Nao foi possivel criar o arquivo!");
   else						// O arquivo foi criado
   {
      randomize();
      for (int i=0; i<MAX; i++)		// Geração de mil inteiros aleatórios
	   	fprintf(arq, "%d\n", rand());
      strcat(msg, "Foram gerados oito mil numeros inteiros em 'ChavePri.dat'.");
   } // Fim de if

   // Impressão dos resultados
   strcat(msg, "\n\t[Pressione qualquer tecla para continuar...]\n");
   printf("%s", msg);
   getch();
   fclose(arq);
} // Fim de main

