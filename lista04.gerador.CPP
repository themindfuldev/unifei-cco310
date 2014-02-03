/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Programa auxiliar às listas																 *
*                                                                              *
*   Objetivo: Geração de oito mil números inteiros para ordenação,			    *
*				  e gravação nos arquivos Arquivo_01.dat e Arquivo_02.dat          *
*                                                                              *
*   Nome: André Carvalho Silveira               Matrícula: 12616               *
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
   FILE *arq1, *arq2;
   char *msg = "\n";

   arq1 = fopen("Arquivo_01.dat", "w");
   arq2 = fopen("Arquivo_02.dat", "w");

	if ((arq1 == NULL) || (arq2 == NULL))		// O arquivo não foi corretamente criado
   	strcat(msg, "\aERRO: Nao foi possivel criar os arquivos!");
   else													// O arquivo foi criado
   {
      randomize();
      for (int i=0; i<MAX; i++)					// Geração de mil inteiros aleatórios
	   	fprintf(arq1, "%d\n", rand());
      strcat(msg, "\n\tForam gerados oito mil numeros inteiros em 'Arquivo_01.dat'.");
      for (int i=0; i<MAX; i++)					// Geração de mil inteiros aleatórios
	   	fprintf(arq2, "%d\n", rand());
      strcat(msg, "\n\tForam gerados oito mil numeros inteiros em 'Arquivo_02.dat'.");
   } // Fim de if

   // Impressão dos resultados
   strcat(msg, "\n\t[Pressione qualquer tecla para continuar...]\n");
   printf("%s", msg);
   getch();
   fclose(arq1);
   fclose(arq2);   
} // Fim de main

