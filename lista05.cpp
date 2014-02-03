/*******************************************************************************
*                                                                              *
*   CCO310 - Classificação de Pesquisa de Dados / Prof. Sdnei Brito            *
*   Lista 5 - Entrega: 30/05/2005                                              *
*                                                                              *
*   Nome: Nathalia Gonçalves Candido            Matrícula: 12744               *
*   Nome: Tiago Romero Garcia                   Matrícula: 12643               *
*   Ciência da Computação - Universidade Federal de Itajubá                    *
*                                                                              *
*******************************************************************************/

// BIBLIOTECAS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

# include <stdio.h>
# include <conio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>

// CONSTANTE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

# define MAX 4000
# define ALGS 3

// ESTRUTURA DE DADOS ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

typedef struct CADEIA{

	char Texto[MAX];
   char Trecho[20];

}CADEIA;

typedef struct RELATORIO{

	char  Nome[20];
	float Tempo;

}RELATORIO;

// PROTÓTIPOS DAS FUNÇÕES ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int Menu(void);
int SubMenu(void);
FILE *AbrirArquivo(void);
int LerTexto(FILE *PF, CADEIA &Cadeia);
int Backtracking(CADEIA Cadeia, int M, int N, int Inicio);
int KMP(CADEIA Cadeia, int M, int N, int Inicio);
int BoyerMoore(CADEIA Cadeia, int M, int N, int Inicio);
void Marca(CADEIA Cadeia, int subOpcao, int M, int N, float &Tempo);
void Relatorio(RELATORIO Rel[ALGS]);

// FUNÇÃO PRINCIPAL ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void main(void){

   //Definindo Variáveis Locais
	int Opcao, SubOpcao, Posicao, N, M, i;
   CADEIA Cadeia;
   FILE *PF=NULL;
   RELATORIO Rel[ALGS];

	if (!(LerTexto(PF, Cadeia)))
   {
    	printf("\n\n\a\t    >>> Impossivel abrir arquivo de texto! \n\t\tSaindo...\n");
      getch();
      Opcao = 4;
   } // Fim de if
   else
   {
	 	printf("\n\n\t    >>> Texto lido com sucesso!\n");
      getch();
      Opcao = 0;
   } // Fim de else

	// Inicialização
   strcpy(Rel[0].Nome, "Backtracking");
   strcpy(Rel[1].Nome, "Boyer-Moore");
   strcpy(Rel[2].Nome, "Knuth-Morris-Prat");
   for (i=0; i<ALGS; i++)
     	Rel[i].Tempo = 0;

   while (Opcao != 4){

      //Chamando Menu
   	Opcao = Menu();

 	   //Carregando Tamanhos
	   N = strlen(Cadeia.Texto);

   	switch (Opcao){

      	case 1:
                 clrscr();

                 printf ("\n\n\tO texto lido e:\n\n\t");
                 for (i=0; i<N; i++)
                 {
		              printf ("%c", Cadeia.Texto[i]);
	                 if ((i%1500 == 0)&&(i!=0))
                    {
                    		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                        getch();
                    }
                 }
                 printf ("\n\n\tO texto possui %d caracteres.\n", N);

                 getch();
         break;

         case 2:
                 do
                 {
	                 clrscr();
                    printf ("\n\n\tDigite o trecho que deseja procurar: ");
	                 (void)getchar();
   	              gets(Cadeia.Trecho);
                    if (strlen(Cadeia.Trecho)==0)
                    {
                       printf ("\n\n\t\aDigite uma cadeia nao-vazia!\n\t[Pressione qualquer tecla para continuar...]\n");
                       getch();
                    }
                 } while (strlen(Cadeia.Trecho)==0);

                 M = strlen(Cadeia.Trecho);

					  do {

					     clrscr();
                 	  gotoxy(20,2); printf("\tVoce esta pesquisando: %s", Cadeia.Trecho);
	                 SubOpcao = SubMenu();

                    switch (SubOpcao) {

	                    case 1:
                             Posicao = Backtracking(Cadeia, M, N, 0);

			  	                 if(!Posicao)
	         			        		printf ("\n\tO trecho procurado nao foi encontrado!\n");
                             else
                             {
                             		printf ("\n\tO trecho procurado foi encontrado!\n");
                                 getch();
				                 		Marca(Cadeia, SubOpcao, M, N, Rel[SubOpcao-1].Tempo);
                             } // Fim de else
				                 getch();
					         break;

					         case 2:
				                 Posicao = BoyerMoore(Cadeia, M, N, 0);

				                 if(!Posicao)
				                 		printf ("\n\tO trecho procurado nao foi encontrado!");
				                 else
                             {
                             		printf ("\n\tO trecho procurado foi encontrado!\n");
                                 getch();
				                 		Marca(Cadeia, SubOpcao, M, N, Rel[SubOpcao-1].Tempo);
                             } // Fim de else
				                 getch();
					         break;

					         case 3:
				                 Posicao = KMP(Cadeia, M, N, 0);

				                 if(!Posicao)
				                 		printf ("\n\tO trecho procurado nao foi encontrado!");
	         			        else
                             {
                             		printf ("\n\tO trecho procurado foi encontrado!\n");
                                 getch();
				                 		Marca(Cadeia, SubOpcao, M, N, Rel[SubOpcao-1].Tempo);
                             } // Fim de else
				                 getch();
					         break;

							   case 4:
                        break;

                        default:
				                 clrscr();
				         		  gotoxy (22,11);		printf ("\a>>> Opcao Invalida!\n");
				                 getch();
					         break;
	                 }// Fim do switch

             	  }while (SubOpcao != 4);

			break;

         case 3:
                 clrscr();
                 Relatorio(Rel);
                 printf("\n");
                 getch();
         break;

         case 4:
                 clrscr();
                 gotoxy (20,11);		printf (">>> Finalizando Programa...\n");
                 getch();
         break;

         default:
                 clrscr();
         		  gotoxy (22,11);		printf ("\a>>> Opcao Invalida!\n");
                 getch();
         break;

      }//Fim do switch

   } // Fim do while

}//Fim da Função Principal()

// FUNCAO MENU ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int Menu(void){

	int Opc;

   clrscr();

   gotoxy(20,6); 	printf("************ MENU DE OPCOES ************");
   gotoxy(20,9); 	printf(" 1. Apresentar o texto lido. ");
   gotoxy(20,10); printf(" 2. Buscar um trecho no texto e marca-lo. ");
   gotoxy(20,11); printf(" 3. Relatorio. ");
   gotoxy(20,12); printf(" 4. Sair. ");
   gotoxy(20,15); printf (" Opcao: ");
   scanf("%d", &Opc);

   return Opc;

}//Fim da Função Menu()

// FUNCAO SUBMENU ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int SubMenu(void){

	int Opc;

   gotoxy(20,6); 	printf("************ SUBMENU ************");
   gotoxy(20,9); 	printf(" 1. Backtracking. ");
   gotoxy(20,10); printf(" 2. Boyer-Moore. ");
   gotoxy(20,11); printf(" 3. Knuth-Morris-Prat. ");
   gotoxy(20,12); printf(" 4. Voltar ao menu inicial. ");
   gotoxy(20,15); printf (" Opcao: ");
   scanf("%d", &Opc);

   return Opc;

}// Fim da Função SubMenu()

// FUNCAO ABRIR ARQUIVO ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

FILE *AbrirArquivo(){

   //Abrindo arquivo
   return fopen("Texto.txt","r");

}//Fim da Função AbrirArquivo()

// FUNÇÃO LER TEXTO ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int LerTexto(FILE *PF, CADEIA &Cadeia){

   PF = AbrirArquivo();

	if (PF == NULL)
   	return 0;

   fgets(Cadeia.Texto, MAX, PF);
   fclose(PF);

   return 1;

}//Fim da Função LerTexto()

// FUNÇÃO BACKTRAKING ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int Backtracking(CADEIA Cadeia, int M, int N, int Inicio){

	//Definindo Variáveis Local
   int i, j;

   //Inicializando Variável
   i = Inicio-1;

   do{

   	i++;
      j = 0;

      while((j<M)&&(Cadeia.Texto[i+j]==Cadeia.Trecho[j]))
      	j++;

   }while(!((j==M)||(i==N-M)));


   if(j == M)
   	return i;

   return 0;

}//Fim da Função Backtraking()

// FUNÇÃO BOYERMOORE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int BoyerMoore(CADEIA Cadeia, int M, int N, int Inicio){

	//Definindo variáveis Locais
   int i, j;
   int ult[256];

   //Pré-processamento do Trecho
   for (i=0; i < 256; i++)
   	ult[i] = 0;

   for (i=1; i <= M; i++)
   	ult[Cadeia.Trecho[i]] = i;

   //Busca do Trecho no Texto
   i = Inicio + M - 1;
   j = M-1;

   do{
     	if (Cadeia.Trecho[j] == Cadeia.Texto[i]){
      	if (j==0){ // Padrão encontrado
            break;
         } // Fim de if
         else{
         	i--;
            j--;
         } // Fim de else
      } // Fim de if
      else{
      	i = i + M - min(j, 1+ult[Cadeia.Texto[i]]);
         j = M-1;
      } // Fim de else

   } while (i < N-1);

	if (j==0) return i;
   return 0;

}//Fim da Função BoyerMoore

// FUNÇÃO KMP ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int KMP(CADEIA Cadeia, int M, int N, int Inicio){

	//Definindo Variáveis Locais
   int j, k, i, d[30];

   //Determinando os Valores de d[]
   j = 0;
   k = -1;
   d[0] = -1;

   while (j<(M-1)){

      while ((k>=0)&&(Cadeia.Trecho[j]!=Cadeia.Trecho[k]))
      	k = d[k];

      j++;
      k++;

      if(Cadeia.Trecho[j]==Cadeia.Trecho[k])
      	d[j] = d[k];
      else
      	d[j] = k;

   }//Fim do while

   //Localizando Trecho em Texto
   i = Inicio;
   j = 0;
   while ((j<M)&&(i<N)){

   	while ((j>=0)&&(Cadeia.Texto[i]!=Cadeia.Trecho[j]))
			j = d[j];

      j++;
      i++;

   }//Fim do while

   if (j != M)
   	return 0;

   return i-M;

}//Fim da Função KMP()

// FUNÇÃO MARCA ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void Marca(CADEIA Cadeia, int SubOpcao, int M, int N, float &Tempo){

	//Definindo Variáveis Locais
	int Posicao, i = 0;
   clock_t Start, End;

   clrscr();
   textcolor(YELLOW);

   printf("\n\tAs ocorrencias estao em amarelo:\n\n\t");

	switch(SubOpcao) {
   	case 1:

         Start = clock();

      	Posicao = Backtracking(Cadeia, M, N, 0);

         while ((Posicao != 0)&&(i<=N))
         {
         	for (; (i<Posicao)&&(i<=N); i++)
            {
            	printf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

         	for (; (i<Posicao+M)&&(i<=N); i++)
            {
            	cprintf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

            if (i<=N)
		      	Posicao = Backtracking(Cadeia, M, N, i);

         } // Fim de while

         End = clock();

   		Tempo = (End - Start)/CLK_TCK;

        	for (; i<N; i++)
         {
           	printf("%c", Cadeia.Texto[i]);
            if ((i%1500 == 0)&&(i!=0))
            {
          		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
               getch();
            }
         }

         printf("\n");

         break;

   	case 2:

         Start = clock();

      	Posicao = BoyerMoore(Cadeia, M, N, 0);

         while ((Posicao != 0)&&(i<=N))
         {
         	for (; (i<Posicao)&&(i<=N); i++)
            {
            	printf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

         	for (; (i<Posicao+M)&&(i<=N); i++)
            {
            	cprintf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

            if (i<=N)
		      	Posicao = BoyerMoore(Cadeia, M, N, i);

            if ((Posicao == N-1)&&(Cadeia.Texto[i]!=Cadeia.Trecho[0])) Posicao = 0;
         } // Fim de while

         End = clock();

			Tempo = (End - Start)/CLK_TCK;

        	for (; i<N; i++)
         {
           	printf("%c", Cadeia.Texto[i]);
            if ((i%1500 == 0)&&(i!=0))
            {
          		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
               getch();
            }
         }


         printf("\n");

         break;

   	case 3:

         Start = clock();

      	Posicao = KMP(Cadeia, M, N, 0);

         while ((Posicao != 0)&&(i<=N))
         {
         	for (; (i<Posicao)&&(i<=N); i++)
            {
            	printf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

         	for (; (i<Posicao+M)&&(i<=N); i++)
            {
            	cprintf("%c", Cadeia.Texto[i]);
               if ((i%1500 == 0)&&(i!=0))
               {
              		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
                  getch();
               }
            }

            if (i<=N)
		      	Posicao = KMP(Cadeia, M, N, i);
         } // Fim de while

         End = clock();

   		Tempo = (End - Start)/CLK_TCK;

        	for (; i<N; i++)
         {
           	printf("%c", Cadeia.Texto[i]);
            if ((i%1500 == 0)&&(i!=0))
            {
          		printf("\n\n\t[Pressione qualquer tecla para ler a proxima pagina...]\n\n");
               getch();
            }
         }

         printf("\n");

         break;
   } // Fim do switch

} // Fim da Função Marca

// FUNÇÃO RELATÓRIO ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void Relatorio(RELATORIO Rel[ALGS]){

	//Definindo Variáveis Locais
	int i;

 	gotoxy(20,3);		printf ("RELATORIO");

   for (i=0; i<80; i++){
   	gotoxy(i,5);	printf ("*");
   }

   gotoxy(7,7);		printf ("FUNCAO");
   gotoxy(30,7);		printf ("TEMPO DE EXECUCAO");
   for (i=0; i<ALGS; i++)
   {
	   gotoxy(5,9+2*i);		printf ("%s", Rel[i].Nome);
	   gotoxy(33,9+2*i);		printf ("%f", Rel[i].Tempo);
   }

}//Fim da Função Relatório
