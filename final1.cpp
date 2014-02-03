/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Projeto Final de Curso - Entrega: 04/07/2005                               *
*                                                                              *
*   Objetivo: Buscar em base de dados usando árvores Patrícia                  *
*   Programa 1/2: Controle de acervo bibliográfico                             *
*                                                                              *
*   Nome: Júlio César Campos                    Matrícula: 11882               *
*   Nome: Ramon Fortes Pereira                  Matrícula: 12638               *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Nome: Nathalia Gonçalves Candido            Matrícula: 12744               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
*                                                                              *
*******************************************************************************/

//--Bibliotecas---------------------------------------------------------------//
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//----------------------------------------------------------------------------//

//--Constantes----------------------------------------------------------------//
#define	MAX	100
#define	NOME	194
//----------------------------------------------------------------------------//

//--Estruturas----------------------------------------------------------------//
// Estrutura de Livro
typedef struct LIVRO
{
	long int			tombo;
	char				titulo[NOME];
   unsigned char	exemplares;
   short int 		anoPublicacao;
   unsigned char 	numEdicao;
   char 				nomeEditora[NOME];
} LIVRO;
// Estrutura de Autor
typedef struct AUTOR
{
	long int 		tombo;
	char 				nome[NOME];
} AUTOR;
// Estrutura de Assunto
typedef struct ASSUNTO
{
	long int 		tombo;
	char 				nome[NOME];
} ASSUNTO;
//----------------------------------------------------------------------------//

//--Protótipos----------------------------------------------------------------//
unsigned char 	Menu(void);
bool 				AlteraLivro(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int);
char 				BuscaLivro(LIVRO [], unsigned short int, long int);
bool 				Carrega(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int &);
void 				Insere(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int &);
void 				InsereLivro(LIVRO &, AUTOR &, ASSUNTO &);
void 				Listagem(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int);
void 				ListaLivro(LIVRO, AUTOR, ASSUNTO);
bool 				RemoveLivro(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int);
bool 				Salva(LIVRO [], AUTOR [], ASSUNTO [], unsigned short int);
//----------------------------------------------------------------------------//

//--Programa------------------------------------------------------------------//
void main(void)
{
	// Declaração de variáveis
	LIVRO acervo[MAX];
   AUTOR autores[MAX];
   ASSUNTO assuntos[MAX];
   unsigned char op;
   unsigned short int indice = 0;
   bool alterou = 0, ret;
   char *sair = "";

   // Carregamento da base de dados
	ret = Carrega(acervo, autores, assuntos, indice);
   if (ret)
      printf("\n\tBase de dados carregada de 'ACERVO.Dat' com sucesso!\n\tForam carregados %d registros.\n\t[Pressione qualquer tecla para continuar...]\n", indice);
   else
   	printf("\a\n\tNao foi possivel carregar a base de dados!\n\t[Pressione qualquer tecla para continuar...]\n");
   getch();

	// Estrutura do programa
	do
   {
		op = Menu();
   	switch (op)
      {
			case 1:	// Inserir Livro
         			Insere(acervo, autores, assuntos, indice);
                  alterou = 1;
                  break;

			case 2:	// Alterar Livro
         			ret = AlteraLivro(acervo, autores, assuntos, indice);
                  if (ret)
                  {
                  	alterou = ret;
                    	printf("\n\n\tRegistro devidamente alterado!\n\t[Pressione qualquer tecla para continuar...]\n");
                  } // Fim de if
                  else
							printf("\n\n\tNenhum registro foi alterado!\n\t[Pressione qualquer tecla para continuar...]\n");
                  getch();
                  break;

			case 3:	// Remover Livro
         			ret = RemoveLivro(acervo, autores, assuntos, indice);
                  if (ret)
                  {
                  	alterou = ret;
                    	printf("\n\n\tRegistro devidamente removido!\n\t[Pressione qualquer tecla para continuar...]\n");
                  } // Fim de if
                  else
							printf("\n\n\tNenhum registro foi removido!\n\t[Pressione qualquer tecla para continuar...]\n");
                  getch();
                  break;

			case 4:	// Listagem do acervo
         			Listagem(acervo, autores, assuntos, indice);
         			break;

         case 0:	// Sair
         			if (alterou)
                  {
                     do
                     {
                     	(void) getchar();
                  		printf("\n\tHa alteracoes nao salvas. Deseja salvar a base de dados (s/n)? ");
                        scanf(" %[^\n]", sair);
                     	strlwr(sair);
                     	if (strcmp(sair, "s") == 0)
                        {
                     		ret = Salva(acervo, autores, assuntos, indice);
			                  if (ret)
			                  	printf("\n\tBase de dados salva em 'ACERVO.Dat' com sucesso!\n\t[Pressione qualquer tecla para continuar...]\n");
                           else
			                  	printf("\a\n\tNao foi possivel salvar a base de dados!\n\t[Pressione qualquer tecla para continuar...]\n");
		                  	getch();
                        } // Fim de if
                        else if (strcmp(sair, "n") != 0)
                        	printf("\a\n\tOpcao invalida! Entre apenas com 's' ou 'n'!\n");
                     } while ((strcmp(sair, "s") != 0) && (strcmp(sair, "n") != 0));
                  } // Fim de if
         			break;

        	default:	// Opção inválida
           			printf("\a\n\tOpcao invalida! Entre apenas com valores entre 1 e 4!\n\t[Pressione qualquer tecla para continuar...]\n");
						getch();
                  break;
      } // Fim de switch
   } while (op != 0);

} // Fim de main

//----------------------------------------------------------------------------//
// Função Menu: Cria um menu de opções
unsigned char Menu(void)
{
   // Declaração de variáveis
	int	op;

   // Menu
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tMenu de Opcoes:\n");
   printf("\n\t  * 1. Inserir um Livro");
   printf("\n\t  * 2. Alterar um Livro");
   printf("\n\t  * 3. Remover um Livro");
   printf("\n\t  * 4. Listagem do acervo");
   printf("\n\n\t  * 0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);

   // Retorno da opcao escolhida
   return (unsigned char) op;

} // Fim de Menu

//----------------------------------------------------------------------------//
// Função AlteraLivro: Altera um livro
bool AlteraLivro(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int indice)
{
	// Declaração de variáveis
   long int	reg, tombo;
	char 		*buscar = "";

	(void) getchar();
	printf("\n");
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tAlterar Livro");
	do
   {
      printf("\n\n\tEntre com o tombo: ");
      scanf("%ld", &tombo);

   	// Busca pelo índice do tombo digitado
      reg = BuscaLivro(acervo, indice, tombo);

      if (reg == -1)
      {
      	printf("\a\n\tTombo nao localizado!\n");
         do
			{
   	      (void) getchar();
		  		printf("\n\tDeseja procurar por um tombo valido (s/n)? ");
				gets(buscar);
		     	strlwr(buscar);
				if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
		        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
      } // Fim de if
      else
      {
			ListaLivro(acervo[reg], autores[reg], assuntos[reg]);
         do
			{
   	      (void) getchar();
		  		printf("\n\tTem certeza que deseja alterar este (s/n)? ");
				gets(buscar);
		     	strlwr(buscar);
				if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
		        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
         if (strcmp(buscar, "s") == 0)
         {
	      	strcpy(buscar, "n");
         	printf("\n\tEntre com os novos dados:\n\n");
   	   	InsereLivro(acervo[reg], autores[reg], assuntos[reg]);
            return 1;
         } // Fim de if
         else
         {
	         do
				{
			  		printf("\n\tDeseja procurar por um tombo valido (s/n)? ");
					gets(buscar);
			     	strlwr(buscar);
					if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
               {
			        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	   		      (void) getchar();
               } // Fim de if
		      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
         } // Fim de else
      } // Fim de else
   } while (strcmp(buscar, "s") == 0);

   return 0;

} // Fim de AlteraLivro

//----------------------------------------------------------------------------//
// Função BuscaLivro: Busca por um índice relacionado ao tombo inserido
char BuscaLivro(LIVRO autores[MAX], unsigned short int indice, long int tombo)
{
	// Declaração de variáveis
	unsigned short int	atual;

	// Retorna o índice se achar o tombo
	for (atual = 0; atual < indice; atual++)
   	if (autores[atual].tombo == tombo)
      	return atual;

   // Retorna -1 se não achar
   return -1;

} // Fim de BuscaLivro

//----------------------------------------------------------------------------//
// Função Carrega: Carrega o acervo do arquivo ACERVO.Dat
bool Carrega(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int &indice)
{
	// Declaração de variáveis
	FILE *arquivo;
   unsigned short int atual;

	// Abertura do arquivo
   arquivo = fopen("ACERVO.Dat", "r");
   if (!arquivo) return 0;

	// Arquivo de acervo
   indice = 0;
   fread(&acervo[indice], sizeof(LIVRO), 1, arquivo);
	while (acervo[indice].tombo != -1)
   {
      indice++;
	   fread(&acervo[indice], sizeof(LIVRO), 1, arquivo);
   } // Fim de for

	for (atual = 0; atual < indice; atual++)
	   fread(&assuntos[atual], sizeof(ASSUNTO), 1, arquivo);

	for (atual = 0; atual < indice; atual++)
	   fread(&autores[atual], sizeof(AUTOR), 1, arquivo);

   fclose(arquivo);
   return 1;

} // Fim de Carrega

//----------------------------------------------------------------------------//
// Função Insere: Insere um livro
void Insere(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int &indice)
{
	// Declaração de variáveis
	char	ret;

	(void) getchar();
   printf("\n");
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tInserir Livro");
   printf("\n\tOBS: So podem ser inseridos em sentencas: caracteres alfanumericos,\n\t     numeros, espacos e sinais de pontuacao como . , ; : - ? ! ' ");
  	do
   {
  		printf("\n\n\t  Numero de tombo     : "); scanf("%ld", &acervo[indice].tombo);
      ret = BuscaLivro(acervo, indice, acervo[indice].tombo);
	  	if (ret != -1)
      {
	     	printf("\a\n\tNumero de tombo ja existente!\n\t[Pressione qualquer tecla para continuar...]\n");
         getch();
      } // Fim de if
	  	if (acervo[indice].tombo < 0)
      {
	     	printf("\a\n\tO numero de tombo deve ser maior ou igual a zero!\n\t[Pressione qualquer tecla para continuar...]\n");
         getch();
      } // Fim de if
	} while ((ret != -1) || (acervo[indice].tombo < 0));
   assuntos[indice].tombo = autores[indice].tombo = acervo[indice].tombo;
   InsereLivro(acervo[indice], autores[indice], assuntos[indice]);
   indice++;

	printf("\n\n\tRegistro devidamente inserido!\n\t[Pressione qualquer tecla para continuar...]\n");
   getch();

} // Fim de InsereLivros

//----------------------------------------------------------------------------//
// Função InsereLivro: Insere um livro
void InsereLivro(LIVRO &livro, AUTOR &autor, ASSUNTO &assunto)
{
	// Declaração de variáveis
   unsigned long int	leitura;

	do
   {
	   printf("\t  Titulo do livro     : "); scanf(" %[^\n]", livro.titulo);
   	if (strlen(livro.titulo) >= NOME)
      {
       	printf("\a\n\tO comprimento maximo da sentenca e %d caracteres!\n\t[Pressione qualquer tecla para continuar...]\n\n", NOME);
         getch();
      } // Fim de if
	} while (strlen(livro.titulo) >= NOME);
	do
   {
	   printf("\t  Assunto             : "); scanf(" %[^\n]", assunto.nome);
   	if (strlen(assunto.nome) >= NOME)
      {
       	printf("\a\n\tO comprimento maximo da sentenca e %d caracteres!\n\t[Pressione qualquer tecla para continuar...]\n\n", NOME);
         getch();
      } // Fim de if
	} while (strlen(assunto.nome) >= NOME);
	do
   {
	   printf("\t  Autor               : "); scanf(" %[^\n]", autor.nome);
   	if (strlen(autor.nome) >= NOME)
      {
       	printf("\a\n\tO comprimento maximo da sentenca e %d caracteres!\n\t[Pressione qualquer tecla para continuar...]\n\n", NOME);
         getch();
      } // Fim de if
	} while (strlen(autor.nome) >= NOME);
  	do
   {
	   printf("\t  Numero de exemplares: "); scanf("%ld", &leitura);
   	if ((leitura < 0) || (leitura > 255))
      {
       	printf("\a\n\tO numero de exemplares deve ser entre 0 e 255!\n\t[Pressione qualquer tecla para continuar...]\n\n");
         getch();
      } // Fim de if
      else
	      livro.exemplares = (unsigned char) leitura;
	} while ((leitura < 0) || (leitura > 255));
   do
   {
   	printf("\t  Ano de publicacao   : "); scanf("%hd", &leitura);
   	if (abs(leitura) > 32767)
      {
       	printf("\a\n\tO ano de publicacao deve ser entre -32,767 e 32,767!\n\t[Pressione qualquer tecla para continuar...]\n\n");
         getch();
      } // Fim de if
      else
	      livro.anoPublicacao = (short int) leitura;
	} while (abs(leitura) > 32767);
  	do
   {
	   printf("\t  Numero da edicao    : "); scanf("%hd", &leitura);
   	if ((leitura < 0) || (leitura > 255))
      {
       	printf("\a\n\tO numero da edicao deve ser entre 0 e 255!\n\t[Pressione qualquer tecla para continuar...]\n\n");
         getch();
      } // Fim de if
      else
	      livro.numEdicao = (unsigned char) leitura;
	} while ((leitura < 0) || (leitura >= 255));
	do
   {
	  	printf("\t  Nome da editora     : "); scanf(" %[^\n]", livro.nomeEditora);
   	if (strlen(livro.nomeEditora) >= NOME)
      {
       	printf("\a\n\tO comprimento maximo da sentenca e %d caracteres!\n\t[Pressione qualquer tecla para continuar...]\n\n", NOME);
         getch();
      } // Fim de if
	} while (strlen(livro.nomeEditora) >= NOME);
} // Fim de InsereLivro

//----------------------------------------------------------------------------//
// Função Listagem: Lista o acervo
void Listagem(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int indice)
{
	// Declaração de variáveis
	unsigned short int	atual, mostra;

	(void) getchar();
	printf("\n");
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tListagem\n\n");
   if (indice == 0)
   	printf("\n\t  Nao ha registros armazenados!\n");
	for (atual = 0, mostra = 0; atual < indice; atual++, mostra++)
   {
		// Descarta os registros removidos
   	while (acervo[atual].tombo == -1) atual++;

      // Imprime os restantes
      if (atual < indice)
      {
	      printf("\n\tRegistro %hd:", mostra);
	      ListaLivro(acervo[atual], autores[atual], assuntos[atual]);
	      if (atual % 2 == 0)
         {
         	getch();
            clrscr();
         } // Fim de if
      } // Fim de if
   } // Fim de while

   printf("\n\n\tFim da listagem.\n\t[Pressione qualquer tecla para continuar...]\n");
   getch();

} // Fim de Listagem

//----------------------------------------------------------------------------//
// Função ListaLivro: Lista um livro
void ListaLivro(LIVRO livro, AUTOR autor, ASSUNTO assunto)
{
	// Declaração de variáveis
   short int	i, lim;

  	printf("\n\t  Numero de tombo     : %lu", livro.tombo);
   lim = (short int) strlen(livro.titulo);
   if (lim < 46)
	   printf("\n\t  Titulo do livro     : %s", livro.titulo);
   else
   {
	   printf("\n\t  Titulo do livro     : ");
		for (i = 0; i < lim; i++)
      {
      	printf("%c", livro.titulo[i]);
         if ((i + 1) % 47 == 0)
         	printf("\n\t                        ");
      } // Fim de for
   } // Fim de else
   lim = (short int) strlen(assunto.nome);
   if (lim < 46)
	   printf("\n\t  Assunto             : %s", assunto.nome);
   else
   {
	   printf("\n\t  Assunto             : ");
		for (i = 0; i < lim; i++)
      {
      	printf("%c", assunto.nome[i]);
         if ((i + 1) % 47 == 0)
         	printf("\n\t                        ");
      } // Fim de for
   } // Fim de else
   lim = (short int) strlen(autor.nome);
   if (lim < 46)
	   printf("\n\t  Autor               : %s", autor.nome);
   else
   {
	   printf("\n\t  Autor               : ");
		for (i = 0; i < lim; i++)
      {
      	printf("%c", autor.nome[i]);
         if ((i + 1) % 47 == 0)
           	printf("\n\t                        ");
      } // Fim de for
   } // Fim de else
   printf("\n\t  Numero de exemplares: %hu", livro.exemplares);
   printf("\n\t  Ano de publicacao   : %hd", livro.anoPublicacao);
   printf("\n\t  Numero da edicao    : %hu", livro.numEdicao);
   lim = (short int) strlen(livro.nomeEditora);
   if (lim < 46)
  		printf("\n\t  Nome da editora     : %s\n", livro.nomeEditora);
   else
   {
	   printf("\n\t  Nome da editora     : ");
		for (i = 0; i < lim; i++)
      {
      	printf("%c", livro.nomeEditora[i]);
         if ((i + 1) % 47 == 0)
         	printf("\n\t                        ");
      } // Fim de for
   } // Fim de else

} // Fim de ListaLivro

//----------------------------------------------------------------------------//
// Função RemoveLivro: Remove um livro
bool RemoveLivro(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int indice)
{
	// Declaração de variáveis
   long int reg, tombo;
	char *buscar = "";

	(void) getchar();
	printf("\n");
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tRemover Livro");
	do
   {
      printf("\n\n\tEntre com o tombo: ");
      scanf("%ld", &tombo);

   	// Busca pelo índice do tombo digitado
      reg = BuscaLivro(acervo, indice, tombo);

      if (reg == -1)
      {
      	printf("\a\n\tTombo nao localizado!\n");
         do
			{
   	      (void) getchar();
		  		printf("\n\tDeseja procurar por um tombo valido (s/n)? ");
				gets(buscar);
		     	strlwr(buscar);
				if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
		        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
      } // Fim de if
      else
      {
			ListaLivro(acervo[reg], autores[reg], assuntos[reg]);
         do
			{
   	      (void) getchar();
		  		printf("\n\tTem certeza que deseja remover este (s/n)? ");
				gets(buscar);
		     	strlwr(buscar);
				if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
		        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
         if (strcmp(buscar, "s") == 0)
         {
				acervo[reg].tombo = autores[reg].tombo = assuntos[reg].tombo = -1;
            return 1;
         } // Fim de if
         else
         {
	         do
				{
			  		printf("\n\tDeseja procurar por um tombo valido (s/n)? ");
					gets(buscar);
			     	strlwr(buscar);
					if ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0))
               {
			        	printf("\a\n\t  Opcao invalida! Entre apenas com 's' ou 'n'!\n\t  [Pressione qualquer tecla para continuar...]\n");
	   		      (void) getchar();
               }
		      } while ((strcmp(buscar, "s") != 0) && (strcmp(buscar, "n") != 0));
         } // Fim de else
      } // Fim de else
   } while (strcmp(buscar, "s") == 0);

   return 0;

} // Fim de RemoveLivro

//----------------------------------------------------------------------------//
// Função Salva: Salva o acervo no arquivo ACERVO.Dat
bool Salva(LIVRO acervo[MAX], AUTOR autores[MAX], ASSUNTO assuntos[MAX], unsigned short int indice)
{
	// Declaração de variáveis
	FILE 						*arquivo;
   LIVRO 					temp;
   unsigned short int	atual;

	// Abertura do arquivo
   arquivo = fopen("ACERVO.Dat", "w");
   if (!arquivo) return 0;

   // Inicialização dos marcadores
   temp.tombo = -1;

	// Arquivo de acervo
	for (atual = 0; atual < indice; atual++)
   {
		// Descarta os registros removidos
   	while (acervo[atual].tombo == -1) atual++;

      // Grava os restantes
      if (atual < indice)
		   fwrite(&acervo[atual], sizeof(LIVRO), 1, arquivo);
   } // Fim de for
   fwrite(&temp, sizeof(LIVRO), 1, arquivo);

	// Arquivo de assuntos
	for (atual = 0; atual < indice; atual++)
   {
		// Descarta os registros removidos
   	while (assuntos[atual].tombo == -1) atual++;

      // Grava os restantes
      if (atual < indice)
		   fwrite(&assuntos[atual], sizeof(ASSUNTO), 1, arquivo);
   } // Fim de for

	// Arquivo de Autores
	for (atual = 0; atual < indice; atual++)
   {
		// Descarta os registros removidos
   	while (autores[atual].tombo == -1) atual++;

      // Grava os restantes
      if (atual < indice)
		   fwrite(&autores[atual], sizeof(AUTOR), 1, arquivo);
   } // Fim de for

   fclose(arquivo);
   return 1;

} // Fim de Salva

