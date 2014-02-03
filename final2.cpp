/*******************************************************************************
*                                                                              *
*   CCO310 - Classifica��o de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Projeto Final de Curso - Entrega: 04/07/2005                               *
*                                                                              *
*   Objetivo: Buscar em base de dados usando �rvores Patr�cia                  *
*   Programa 2/2: Busca no acervo usando �rvores Patr�cia                      *
*                                                                              *
*   Nome: J�lio C�sar Campos                    Matr�cula: 11882               *
*   Nome: Ramon Fortes Pereira                  Matr�cula: 12638               *
*   Nome: Tiago Romero Garcia                   Matr�cula: 12643               *
*   Nome: Nathalia Gon�alves Candido            Matr�cula: 12744               *
*   Ci�ncia da Computa��o - Universidade Federal de Itajub�                    *
*                                                                              *
*******************************************************************************/

//--Bibliotecas---------------------------------------------------------------//
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//----------------------------------------------------------------------------//

//--Constantes----------------------------------------------------------------//
#define	ALFABETO	 49
#define  NOME      194
#define  MAX		 65535
//----------------------------------------------------------------------------//

//--Estruturas----------------------------------------------------------------//
// Estrutura de Livro
typedef struct LIVRO
{
	long int					tombo;
	char						titulo[NOME];
   unsigned char			exemplares;
   short int				anoPublicacao;
   unsigned char			numEdicao;
   char						nomeEditora[NOME];
} LIVRO;
// Estrutura de Autor
typedef struct AUTOR
{
	long int					tombo;
	char						nome[NOME];
} AUTOR;
// Estrutura de Assunto
typedef struct ASSUNTO
{
	long int					tombo;
	char						nome[NOME];
} ASSUNTO;
// �ltimo n� da �rvore patr�cia
typedef struct FINAL
{
	char						nome[NOME];
   long int					endereco;
   FINAL						*prox;
} FINAL;
// N� da �rvore patr�cia
typedef struct NO
{
	char						letra;
   FINAL						*folha;
   NO							*ponte[ALFABETO];
} NO;
// Fila
typedef struct FILA
{
   NO							*ponte[MAX];
   unsigned short int	base, limite, p, u;
} FILA;
//----------------------------------------------------------------------------//

//--Prot�tipos----------------------------------------------------------------//
unsigned char			Menu(void);
NO							*Aloca(void);
FINAL						*AlocaFolha(void);
void						BuscaAcervo(char [], NO *, FILE *, long int, long int, unsigned short int);
void 						BuscaAssuntos(char [], NO *, FILE *, long int, long int, unsigned short int);
void			  			BuscaAutores(char [], NO *, FILE *, long int, long int, unsigned short int);
NO 						*BuscaSubArvore(NO *, int);
ASSUNTO					BuscaTomboAssunto(long int, FILE *, unsigned short int);
AUTOR 					BuscaTomboAutor(long int, FILE *, unsigned short int);
LIVRO 					BuscaTomboLivro(long int, FILE *, unsigned short int);
NO							*Busca(NO *, char []);
int						Converte(char);
void						CriaArvores(FILE *, NO *&, NO *&, NO *&, long int &, long int &, long int &, unsigned short int &);
void						Desaloca(NO	*&);
void						Desdobra(NO *, NO *&, char [], long int);
char 						InsereNaFila(FILA &, NO *);
void						InserirPatricia(NO *&, char [], long int);
void						ListaLivro(LIVRO, AUTOR, ASSUNTO);
int						Profundidade(NO *, NO *, char []);
NO 						*RemoveDaFila(FILA &);
bool 						SalvaArvores(NO *, NO *, NO *, long int, long int, long int, unsigned short int);
unsigned short int	Sucessor(FILA fila, unsigned short int px);
//----------------------------------------------------------------------------//

//--Principal-----------------------------------------------------------------//
void main()
{
	// Declara��o de vari�veis
   NO							*rAcervo = NULL, *rAssuntos = NULL, *rAutores = NULL;
   FILE						*arquivo;
   char						nome[NOME];
   long int					endAcervo, endAssuntos, endAutores;
   unsigned char			op;
	unsigned short int	indice;

	// Abertura do arquivo
   arquivo = fopen("ACERVO.Dat", "r");
   if (!arquivo)
   {
		printf("\a\n\tNao foi possivel abrir arquivo de dados!\n\t[Pressione qualquer tecla para continuar...]\n");
      getch();
	   return;
   } // Fim de if

   // Carregamento da base de dados
	CriaArvores(arquivo, rAcervo, rAssuntos, rAutores, endAcervo, endAssuntos, endAutores, indice);
   printf("\n\tArvores criadas a partir de 'ACERVO.Dat' com sucesso!\n\t[Pressione qualquer tecla para continuar...]\n");
   getch();

	// Estrutura do programa
	do
   {
		op = Menu();
   	switch (op)
      {
			case 1:	// Consulta por titulo
					 	printf("\n\n\tEntre com o titulo do livro: ");
					   scanf(" %[^\n]", nome);
					  	BuscaAcervo(nome, rAcervo, arquivo, endAssuntos, endAutores, indice);
                  break;

			case 2:	// Consulta por autor
					 	printf("\n\n\tEntre com o nome do autor: ");
					   scanf(" %[^\n]", nome);
					  	BuscaAutores(nome, rAutores, arquivo, endAcervo, endAssuntos, indice);
                  break;

			case 3:	// Consulta por assunto
					 	printf("\n\n\tEntre com o nome do autor: ");
					   scanf(" %[^\n]", nome);
					  	BuscaAssuntos(nome, rAssuntos, arquivo, endAcervo, endAutores, indice);
                  break;

         case 0:	// Sair
         			break;

        	default:	// Op��o inv�lida
           			printf("\a\n\tOpcao invalida! Entre apenas com valores entre 1 e 4!\n\t[Pressione qualquer tecla para continuar...]\n");
						getch();
                  break;
      } // Fim de switch
   } while (op != 0);

   // Salvamento os arquivos de �ndices
  	remove("INDICE.DAT");
   if (!SalvaArvores(rAcervo, rAssuntos, rAutores, endAcervo, endAssuntos, endAutores, indice))
   {
		printf("\a\n\tNao foi possivel salvar as arvores!\n\t[Pressione qualquer tecla para continuar...]\n");
	   getch();
	} // Fim de if

   // Desalocagem da mem�ria
   Desaloca(rAcervo);
   Desaloca(rAssuntos);
   Desaloca(rAutores);

	// Fechamento do arquivo
   fclose(arquivo);

	printf("\n\tMemoria desalocada e arquivos de indices armazenados em discos!\n\t[Pressione qualquer tecla para finalizar...]\n");
	getch();

} // Fim de main

//----------------------------------------------------------------------------//
// Fun��o Menu: Cria um menu de op��es
unsigned char Menu(void)
{
   // Declara��o de vari�veis
	int op;

   // Menu
	clrscr();
	printf("\n     Projeto Final de Curso - Classificacao e Pesquisa de Dados");
   printf("\n\n\n\tMenu de Opcoes:\n");
   printf("\n\t  * 1. Consulta por titulo");
   printf("\n\t  * 2. Consulta por autor");
   printf("\n\t  * 3. Consulta por assunto");
   printf("\n\n\t  * 0. Sair");
   printf("\n\n\tDigite a opcao desejada: ");
   scanf("%d", &op);

   // Retorno da opcao escolhida
   return (unsigned char) op;

} // Fim de Menu

//----------------------------------------------------------------------------//
// Fun��o Aloca: Aloca para a estrutura do tipo NO
NO	*Aloca(void)
{
	// Declara��o de vari�veis
	NO		*novo;
   char	i;

   // Aloca a mem�ria no sistema
	novo = (NO *) malloc(sizeof(NO));

	// Inicializa o n�
   novo->folha = NULL;
   novo->letra = '\0';
	for (i = 0; i < ALFABETO; i++)
	   novo->ponte[i] = NULL;

	// Devolve o espa�o alocado
	return novo;

} // Fim de Aloca

//----------------------------------------------------------------------------//
// Fun��o AlocaFolha: Aloca para a estrutura do tipo FINAL
FINAL	*AlocaFolha(void)
{
	// Declara��o de vari�veis
	FINAL	*novo;

   // Aloca a mem�ria no sistema
   novo = (FINAL *) malloc(sizeof(FINAL));

   // Inicializa o n�
  	novo->endereco = -1;
   novo->prox = NULL;
  	strcpy(novo->nome, "");

   // Devolve o espa�o alocado
   return novo;

} // Fim de AlocaFolha

//----------------------------------------------------------------------------//
// Fun��o Busca: Busca pelo n� que determinado pelo nome recebido
NO *Busca(NO *raiz, char nome[NOME])
{
	// Declara��o de vari�veis
	int	tamanho, i;
	NO		*procurado;

   // Pega o tamanho do nome
   tamanho = strlen(nome);

   // inicializa o n�
   procurado = raiz;

   // Pega letra por letra do nome
   for (i = 0; i < tamanho; i++)
   {
   	// Ve se existe aquele n�
      if (procurado->ponte[Converte(nome[i])])
   	 	procurado = procurado->ponte[Converte(nome[i])]; // Pega o n� da letra
      // Se n�o existir sai do for
      else break;
   } // Fim de for

   // Verifica se for igual a raiz
   if (procurado->letra == '\0')
   	return NULL;

   return procurado;
} // Fim de Busca

//----------------------------------------------------------------------------//
// Fun��o BuscaAcervo: Faz uma busca por titulo do livro no acervo
void BuscaAcervo(char nome[NOME], NO *raiz, FILE *arquivo, long int endAssuntos, long int endAutores, unsigned short int indice)
{
	// Declara��o de vari�veis
   NO			*ultimo;
   FINAL		*procurado;
   long int	end;
   LIVRO		livro;
   ASSUNTO	assunto;
   AUTOR		autor;
   char		i = 1;

	// Busca algum NO com a informa��o buscada
   ultimo = Busca(raiz, nome);

	// Se n�o achou
   if (!ultimo)
   {
		printf("\n\tNao consta no acervo um livro com este titulo!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de if

   procurado = ultimo->folha;

	// Se achou e � FINAL
   if (procurado)
   {
		while (procurado)
      {
			end = procurado->endereco;
	      fseek(arquivo, end, SEEK_SET);
		   fread(&livro, sizeof(LIVRO), 1, arquivo);
	      fseek(arquivo, endAssuntos, SEEK_SET);
	      assunto = BuscaTomboAssunto(livro.tombo, arquivo, indice);
	      fseek(arquivo, endAutores, SEEK_SET);
	      autor = BuscaTomboAutor(livro.tombo, arquivo, indice);
	      printf("\n\tRegistro %d:\n", i++);
			ListaLivro(livro, autor, assunto);
	      printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			getch();
         procurado = procurado->prox;
      } // Fim de while;
   } // Fim de if
	else
   {
		printf("\n\tSentenca insuficiente para localizar um livro!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de else

} // Fim de BuscaAcervo

//----------------------------------------------------------------------------//
void BuscaAssuntos(char nome[NOME], NO *raiz, FILE *arquivo, long int endAcervo, long int endAutores, unsigned short int indice)
{
	// Declara��o de vari�veis
   NO			*ultimo;
   FINAL		*procurado;
   long int	end;
   LIVRO		livro;
   ASSUNTO	assunto;
   AUTOR		autor;
   char		i = 1;

	// Busca algum NO com a informa��o buscada
   ultimo = Busca(raiz, nome);

	// Se n�o achou
   if (!ultimo)
   {
		printf("\n\tNao consta no acervo um livro com este titulo!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de if

   procurado = ultimo->folha;

	// Se achou e � FINAL
   if (procurado)
   {
		while (procurado)
      {
			end = procurado->endereco;
	      fseek(arquivo, end, SEEK_SET);
		   fread(&assunto, sizeof(ASSUNTO), 1, arquivo);
	      fseek(arquivo, endAcervo, SEEK_SET);
	      livro = BuscaTomboLivro(assunto.tombo, arquivo, indice);
	      fseek(arquivo, endAutores, SEEK_SET);
	      autor = BuscaTomboAutor(assunto.tombo, arquivo, indice);
	      printf("\n\tRegistro %d:\n", i++);
			ListaLivro(livro, autor, assunto);
	      printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			getch();
         procurado = procurado->prox;
      } // Fim de while;
   } // Fim de if
	else
   {
		printf("\n\tSentenca insuficiente para localizar um livro!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de else

} // Fim de BuscaAssuntos

//----------------------------------------------------------------------------//
void BuscaAutores(char nome[NOME], NO *raiz, FILE *arquivo, long int endAcervo, long int endAssuntos, unsigned short int indice)
{
	// Declara��o de vari�veis
   NO			*ultimo;
   FINAL		*procurado;
   long int	end;
   LIVRO		livro;
   ASSUNTO	assunto;
   AUTOR		autor;
   char		i = 1;

	// Busca algum NO com a informa��o buscada
   ultimo = Busca(raiz, nome);

	// Se n�o achou
   if (!ultimo)
   {
		printf("\n\tNao consta no acervo um livro com este titulo!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de if

   procurado = ultimo->folha;

	// Se achou e � FINAL
   if (procurado)
   {
		while (procurado)
      {
			end = procurado->endereco;
	      fseek(arquivo, end, SEEK_SET);
		   fread(&autor, sizeof(AUTOR), 1, arquivo);
	      fseek(arquivo, endAcervo, SEEK_SET);
	      livro = BuscaTomboLivro(autor.tombo, arquivo, indice);
	      fseek(arquivo, endAssuntos, SEEK_SET);
	      assunto = BuscaTomboAssunto(autor.tombo, arquivo, indice);
	      printf("\n\tRegistro %d:\n", i++);
			ListaLivro(livro, autor, assunto);
	      printf("\n\t[Pressione qualquer tecla para continuar...]\n");
			getch();
         procurado = procurado->prox;
      } // Fim de while;
   } // Fim de if
	else
   {
		printf("\n\tSentenca insuficiente para localizar um livro!\n\t[Pressione qualquer tecla para continuar...]\n");
		getch();
      return;
   } // Fim de else

} // Fim de BuscaAutores

//----------------------------------------------------------------------------//
// Fun��o BuscaSubArvore: Busca a i-�sima sub-�rvore de uma �rvore
NO *BuscaSubArvore(NO *r, int i)
{
   return r->ponte[i];
} // Fim de BuscaSubArvore

//----------------------------------------------------------------------------//
// Fun��o BuscaTomboLivro: A partir de um tombo, busca o livro relacionado
LIVRO BuscaTomboLivro(long int tombo, FILE *arquivo, unsigned short int indice)
{
	// Declara��o de vari�veis
	unsigned short int	atual;
	LIVRO  					livro;

	for (atual = 0; atual < indice; atual++)
   {
	   fread(&livro, sizeof(LIVRO), 1, arquivo);
    	if (livro.tombo == tombo) return livro;
   } // Fim de for

   strcpy(livro.titulo, "Nao encontrado");
   return livro;
} // Fim de BuscaTomboLivro

//----------------------------------------------------------------------------//
// Fun��o BuscaTomboAssunto: A partir de um tombo, busca o assunto relacionado
ASSUNTO BuscaTomboAssunto(long int tombo, FILE *arquivo, unsigned short int indice)
{
	// Declara��o de vari�veis
	unsigned short int	atual;
	ASSUNTO					assunto;

	for (atual = 0; atual < indice; atual++)
   {
	   fread(&assunto, sizeof(ASSUNTO), 1, arquivo);
    	if (assunto.tombo == tombo) return assunto;
   } // Fim de for

   strcpy(assunto.nome, "Nao encontrado");
   return assunto;
} // Fim de BuscaTomboAssunto

//----------------------------------------------------------------------------//
// Fun��o BuscaTomboAutor: A partir de um tombo, busca o assunto relacionado
AUTOR BuscaTomboAutor(long int tombo, FILE *arquivo, unsigned short int indice)
{
	// Declara��o de vari�veis
	unsigned short int	atual;
	AUTOR						autor;

	for (atual = 0; atual < indice; atual++)
   {
	   fread(&autor, sizeof(AUTOR), 1, arquivo);
    	if (autor.tombo == tombo) return autor;
   } // Fim de for

   strcpy(autor.nome, "Nao encontrado");
   return autor;
} // Fim de BuscaTomboAutor

//----------------------------------------------------------------------------//
// Fun��o Converte: Gera um �nico endere�o para cada letra
int Converte(char c)
{
	// De 0 a 25 s�o as letras
	// Verificar se � uma letra
	if (isalpha(c))
   {
   	// Verifica se for min�scula transforma pra mai�sculo
      if (islower(c))
      {
      	// Transforma para mai�scula
         c = (char)toupper(c);
         // E retorna
	      return (c - 65);
      }
      // Se for mai�sculo
      return (c - 65);
   }

 	// 26 � o espa�o
   // Verifica se � espa�o
   if (c == ' ')
   	return 26;

 	// De 27 a 36 s�o os n�meros
   // Verifica se � n�mero
   if (isdigit(c))
   	return (c - 21);

   // De 37 a 50 s�o os n�meros
   // Verifica se � pontua��o
 	switch(c)
	{
     	case '.': return 37;
      case ';': return 38;
      case ':': return 39;
      case ',': return 40;
      case '?': return 41;
      case '!': return 42;
      case '"': return 43;
      case '-': return 44;
      case '+': return 45;
      case '/': return 46;
      case '(': return 47;
      case ')': return 48;
	   default : return -1;
   } // Fim de switch

} // Fim de Converte

//----------------------------------------------------------------------------//
// Fun��o CriaArvores: Cria as �rvores Patr�cia
void CriaArvores(FILE *arquivo, NO *&rAcervo, NO *&rAssuntos, NO *&rAutores, long int &endAcervo, long int &endAssuntos, long int &endAutores, unsigned short int &indice)
{
	// Declara��o de vari�veis
   unsigned short int	atual;
   long int					anterior;
   LIVRO						livro;
   ASSUNTO					assunto;
   AUTOR						autor;

   // Marca��o do endere�o do in�cio do Acervo
   endAcervo = ftell(arquivo);

	// Cria��o das �rvores
   indice = 0;
	anterior = ftell(arquivo);
   fread(&livro, sizeof(LIVRO), 1, arquivo);
	while (livro.tombo != -1)
   {
    	InserirPatricia(rAcervo, strupr(livro.titulo), anterior);
      indice++;
		anterior = ftell(arquivo);
	   fread(&livro, sizeof(LIVRO), 1, arquivo);
   } // Fim de while

   // Marca��o do endere�o do in�cio dos Assuntos
   endAssuntos = ftell(arquivo);

	for (atual = 0; atual < indice; atual++)
   {
		anterior = ftell(arquivo);
	   fread(&assunto, sizeof(ASSUNTO), 1, arquivo);
    	InserirPatricia(rAssuntos, strupr(assunto.nome), anterior);
   } // Fim de for

   // Marca��o do endere�o do in�cio dos Autores
   endAutores = ftell(arquivo);

	for (atual = 0; atual < indice; atual++)
   {
		anterior = ftell(arquivo);
	   fread(&autor, sizeof(AUTOR), 1, arquivo);
    	InserirPatricia(rAutores, strupr(autor.nome), anterior);
   } // Fim de for

} // Fim de CriaArvores

//----------------------------------------------------------------------------//
// Fun��o Desaloca: Desaloca a mem�ria usada pela �rvore patricia
void Desaloca(NO *&raiz)
{
	// Declara��o de vari�veis
	int i, j, k = 0;
   NO *remove, *antes;		// ponteiros auxiliares

   if (!raiz) return;     // finaliza caso n�o exista a arvore patricia

   antes = remove = raiz;
   // Executa o la�o enquanto a raiz contiver sub�rvores
	while (raiz->ponte[k] && k < ALFABETO)
   {
   	// Verificando se o n� atual cont�m sub�rvores
	  	for (i = 0; i < ALFABETO; i++)
      {
      	// Finaliza o for se encontrar uma sub�rvore na posi��o "i"
   		if (remove->ponte[i] != NULL)
         {
            j = i;  // Armazenando a posi��o da sub�rvore em "j"
         	break;
         } // Fim de if
	  	} // Fim de for

      // Se "i" for igual ao tamanho do ALFABETO, ent�o o n� n�o cont�m sub�rvores
      // Assim, remove-se este n�
	   if (i == ALFABETO)
      {
         antes->ponte[j] = NULL;	// Alterando o ponteiro do n� anterior para NULL
		   free(remove);          	// Removendo o n�
         antes = remove = raiz; 	// Reinicializando a busca pelo pr�ximo n� a remover
   	} // Fim de if

      // Se o n� contiver sub�rvore, ent�o caminha-se at� ela
      else
      {
      	antes = remove;
	      remove = remove->ponte[i];
      } // Fim de else

      // Se todas as sub�rvores da raiz "k" foram removidas, ent�o executa a
      // Remo��o das pr�ximas sub�rvores k.
      if (raiz->ponte[k] == NULL)
      {
         antes = remove = raiz;
         // buscando a pr�xima sub�rvore da raiz
         while (raiz->ponte[k] == NULL) k++;
      } // Fim de if
	} // Fim de while

   // Desalocando a raiz
   free(raiz);

} // Fim de Desaloca

//----------------------------------------------------------------------------//
// Fun��o Desdobra: Auxilia o InserePatricia na hora de colocar dois nomes
// diferentes mas com representantes iguais na arvore Patr�cia.. a� ele desdobra
void Desdobra(NO *raiz, NO *&ultimo, char nome[NOME], long int endereco)
{
   // Declara��o de vari�veis
	int 	indice_entra, indice_ja_ta, tamanho_entra, tamanho_ja_ta;
   FINAL	*folha;
   NO		*auxiliar;

	// Inicializa��o de vari�veis
	tamanho_entra = strlen(nome) - 1;
   tamanho_ja_ta = strlen(ultimo->folha->nome) - 1;

   // Pega o �ndice da letra que � diferente dentro da string q vai ser colocada
   indice_ja_ta = indice_entra = Profundidade(raiz, ultimo, nome);

   while ((ultimo->letra != nome[indice_entra])	&&	(indice_entra <= tamanho_entra))
   	indice_entra++;

   while ((ultimo->letra != ultimo->folha->nome[indice_ja_ta])	&&	(indice_ja_ta <= tamanho_ja_ta))
   	indice_ja_ta++;

   // Verifica se ta no final da string que ja existe
   if (indice_ja_ta == tamanho_ja_ta)
   {
   	// Se ja estiver eu so aloca mais um n� pra q vai entrar
	   // Cria
      indice_entra++;
   	auxiliar = Aloca();
	   auxiliar->letra = nome[indice_entra];
      // Liga
	   ultimo->ponte[Converte(auxiliar->letra)] = auxiliar;

	   // Cria o n� folha
	   folha = AlocaFolha();
	 	auxiliar->folha = folha;
		// Preenche a folha
	   strcpy(folha->nome, nome);
	   folha->endereco = endereco;
      return;
    } // Fim de if

  	indice_entra++;
  	indice_ja_ta++;

	// Enquanto as duas palavras forem iguais (e verifica os limites da string)
   while ((nome[indice_entra] == ultimo->folha->nome[indice_ja_ta]) && ((indice_ja_ta <= tamanho_ja_ta) && (indice_entra <= tamanho_entra)))
   {
      // Pega o n� folha
   	folha = ultimo->folha;
      ultimo->folha = NULL;
      // Cria o pr�ximo n�
      auxiliar = Aloca();
	   auxiliar->letra = nome[indice_entra];
      // Liga
	   ultimo->ponte[Converte(auxiliar->letra)] = auxiliar;

	   // Cria o n� folha
	 	auxiliar->folha = folha;
		// Vai pro �ltimo de novo
      ultimo = auxiliar;
	   // Verifica se ta no final da string que ja existi
   	if ((indice_ja_ta == tamanho_ja_ta)	&&	(indice_entra < tamanho_entra))
	   {
	   	// Se ja estiver eu so aloca mais um n� pra q vai entrar
		   // Cria
   	   indice_entra++;
   		auxiliar = Aloca();
		   auxiliar->letra = nome[indice_entra];
	      // Liga
		   ultimo->ponte[Converte(auxiliar->letra)] = auxiliar;

		   // Cria o n� folha
		   folha = AlocaFolha();
	 		auxiliar->folha = folha;
			// Preenche a folha
		   strcpy(folha->nome,nome);
	   	folha->endereco = endereco;
	      return;
      } // Fim de if

	   // Verifica se a string q entra � uma substring q ja existi mas nao est� completa na arvore
   	if ((indice_entra == tamanho_entra ) && (indice_ja_ta < tamanho_ja_ta))
	   {
         // E coloca a folha antiga no novo n�
		   // Cria
   	   indice_ja_ta++;
   		auxiliar = Aloca();
		   auxiliar->letra = ultimo->folha->nome[indice_ja_ta];
	      // Liga
		   ultimo->ponte[Converte(auxiliar->letra)] = auxiliar;

		   // Coloca o n� folha
	 		auxiliar->folha = ultimo->folha;

  			// Coloca o nome e o endere�o que entra no �ltimo
         // Preenche com os novos dados
         folha = AlocaFolha();
		   strcpy(folha->nome, nome);
	   	folha->endereco = endereco;
         ultimo->folha = folha;
	   	return;
      } // Fim de if

   	// Percorre o restante
      indice_ja_ta++;
		indice_entra++;
   } // Fim de while

   if (indice_entra <= tamanho_entra)
   {
	   // Se forem diferentes
	  	// Se nem entraram no while
		auxiliar = Aloca();
   	auxiliar->letra = nome[indice_entra];
		// Liga
		ultimo->ponte[Converte(auxiliar->letra)] = auxiliar;

   	// Coloca o n� folha
	   // Preenche com os novos dados
   	folha = AlocaFolha();
	   strcpy(folha->nome, nome);
 		folha->endereco = endereco;
	   auxiliar->folha = folha;
   } // Fim de if

} // Fim de Desdobra

//----------------------------------------------------------------------------//
// Fun��o InsereNaFila: Insere um valor na fila
char InsereNaFila(FILA &fila, NO *p)
{
  // Verificando Overflow
  if (Sucessor(fila, fila.p) == fila.u) return -1;

  // Inserindo um elemento
  fila.ponte[fila.p] = p;
  fila.p = Sucessor(fila, fila.p);
  return 1;

} // Fim de InsereNaFila

//----------------------------------------------------------------------------//
// Fun��o InserirPatricia: Cria uma �rvore patr�cia e insere nela
void InserirPatricia(NO	*&raiz, char nome[NOME], long int endereco)
{
	// Declara��o de vari�veis
   NO		*auxiliar, *procurado;
   FINAL	*folha;

   // Se n�o existir raiz
   if (!raiz)
   {
      // Cria a raiz
   	raiz = Aloca();
      // Cria o espa�o para colocar a primeira letra
      auxiliar = Aloca();
      auxiliar->letra = nome[0];
      // Liga a ra�z no n� da primeira letra
      raiz->ponte[Converte(auxiliar->letra)] = auxiliar;

      // Coloca o n� folha
		auxiliar->folha = AlocaFolha();
      // Preenche a folha
      strcpy(auxiliar->folha->nome, nome);
      auxiliar->folha->endereco = endereco;
      return;
   } // Fim de if

   // Se ja existir raiz
   // Procura a primeira letra pra ver se existir
	procurado = Busca(raiz, nome);
   if (procurado)
	{
         // No caso de ser uma simples substring e n�o conter o n� folha
         if (procurado->folha == NULL)
         {
         	// Aloca o n� folha
	  	      folha = AlocaFolha();
			   // Coloca o n� folha
				procurado->folha = folha;
		      // Preenche a folha
		      strcpy(folha->nome, nome);
		      folha->endereco = endereco;
            return;
         } // Fim de if
         // No caso de ja existir um n� folha
         else
         {
  	         // As strings podem ser iguais
            if (strcmp(procurado->folha->nome, nome) == 0)
            {
            	// Ent�o criamos uma lista encadeada de folhas C
              	// Aloca o n� folha
		  	      folha = procurado->folha;

				   // Coloca o n� folha
               while (folha->prox)
               	folha = folha->prox;

					folha->prox = AlocaFolha();
		   	   // Preenche a folha
		      	strcpy(folha->prox->nome, nome);
			      folha->prox->endereco = endereco;
               return;
            } // Fim de if
            // Se forem diferentes
            else
            {
					// Desdobra
               Desdobra(raiz, procurado, nome, endereco);
               return;
            } // Fim de else
         } // Fim de else
    } // Fim de if

    // Senao, criamos
    else
    {
	    // Cria o espa�o para colocar a primeira letra
   	 auxiliar = Aloca();
	    auxiliar->letra = nome[0];
	 	 // Verifica se � a �ltima letra
	    // Liga a ra�z no n� da primeira letra
	    raiz->ponte[Converte(auxiliar->letra)] = auxiliar;

	    // Cria o n� folha
	    folha = AlocaFolha();
	 	 auxiliar->folha = folha;
		 // Preenche a folha
	    strcpy(folha->nome, nome);
	    folha->endereco = endereco;
    } // Fim de else

} // Fim de InserirPatricia

//----------------------------------------------------------------------------//
// Fun��o ListaLivro: Lista um livro
void ListaLivro(LIVRO livro, AUTOR autor, ASSUNTO assunto)
{
	// Declara��o de vari�veis
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
// Fun��o Profundidade: Retorna em que lugar o caracter est� na string
int Profundidade(NO *raiz, NO *ultimo, char nome[NOME])
{
	// Declara��o de vari�veis
   unsigned int	i;
   NO					*temp;

   temp = raiz;
	for (i = 0; i < strlen(nome); i++)
   {
      temp = temp->ponte[Converte(nome[i])];
   	if ((nome[i] == ultimo->letra) && (ultimo == temp)) return i;
   } // Fim de for
   return 0;
} // Fim de Profundidade

//----------------------------------------------------------------------------//
// Fun��o RemoveDaFila: Remove um valor da fila
NO *RemoveDaFila(FILA &fila)
{
	// Vari�veis locais
	NO *p;

	// Verificando Underflow
	if (fila.p == fila.u) return NULL;

	// Retirando o elemento
	p = fila.ponte[fila.u];
	fila.u = Sucessor(fila, fila.u);
	return p;

} // Fim de RemoveDaFila

//----------------------------------------------------------------------------//
// Fun��o SalvaArvores: Salva um arquivo de �ndices em disco
bool SalvaArvores(NO *rAcervo, NO *rAssuntos, NO *rAutores, long int endAcervo, long int endAssuntos, long int endAutores, unsigned short int indice)
{
	// Defini��o de vari�veis
   FILA	fila;
   NO		*r, *t, *ult;
   FINAL	*ultimo;
   FILE	*arq;
   char	i, j;

	// Inicializa��o das vari�veis
	fila.base = fila.p = fila.u = 0;
	fila.limite = MAX-1;

	// Abre o arquivo de �ndices
	arq = fopen("INDICE.DAT", "a+");
	if (arq == NULL) return 0;

	// Salva os ponteiros de endere�o de arquivo
	fwrite(&endAcervo, sizeof(long int), 1, arq);
	fwrite(&endAssuntos, sizeof(long int), 1, arq);
	fwrite(&endAutores, sizeof(long int), 1, arq);
	fwrite(&indice, sizeof(unsigned short int), 1, arq);

	// Salva o Acervo
   for (j = 0; j < 3; j++)
   {
   	switch (j)
      {
      	case 0 : InsereNaFila(fila, rAcervo);
         			break;
      	case 1 : InsereNaFila(fila, rAssuntos);
         			break;
      	case 2 : InsereNaFila(fila, rAutores);
         			break;
   	} // Fim de switch

	   while (fila.p != fila.u)
	   {
	   	r = RemoveDaFila(fila);
			fwrite(r, sizeof(NO), 1, arq);
			ultimo = r->folha;
	      while (ultimo != NULL)
	      {
	      	fwrite(ultimo, sizeof(FINAL), 1, arq);
	         ultimo = ultimo->prox;
	      } // Fim de while
	      for (i = 0; i < ALFABETO; i++)
	      {
	      	t = BuscaSubArvore(r, i);
	         if (t) InsereNaFila(fila, t);
	      } // Fim de for
	   } // Fim de while

		// N� de divis�o entre arquivos de �ndice
		ult = Aloca();
	   ult->letra = '&';
	   fwrite(ult, sizeof(NO), 1, arq);
   } // Fim de for

   // Fecha o arquivo de �ndices
	fclose(arq);

   return 1;

} // Fim de SalvaArvore

//----------------------------------------------------------------------------//
// Fun��o Sucessor: Circula a fila
unsigned short int Sucessor(FILA fila, unsigned short int px)
{
	if (px >= fila.limite) return fila.base;
	return ++px;
} // Fim de Sucessor

