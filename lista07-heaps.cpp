// Constantes - A serem definidas no programa
#define ARQ 10
const int MAX = 2 * ARQ;

// Protótipos
void Ordenacao(int vet[MAX], FILE *arquivo[ARQ]);
void Multid(int raiz, int vet[MAX], FILE *arquivo[ARQ]);
void Heap(int raiz, int vet[MAX]);

//----------------------------------------------------------------------------//
// Função Ordenacao: Método da Ordenação Multidirecional

void Ordenacao(int vet[MAX], FILE *arquivo[ARQ]) {

	int i, pos;

	// Carrega os nós folhas
	for (i = ARQ; i < MAX; i++) {
		vet[i] = LeDado(arquivo[ARQ - i]);
	} // Fim de for

	// Forma a primeira árvore completa
   for (i = ARQ - 1; i > 0; i--) {
   	Multid(i, vet, arquivo);
      Heap(i, vet);
   } // Fim de for

   // Ordenação Multidirecional
	for (pos = 0; pos < MAX*10; pos++) {
   	final[pos] = vet[1];
      Multid(1, vet, arquivo);
      Heap(1, vet);
   } // Fim de for

} // Fim de Ordenacao

//----------------------------------------------------------------------------//
// Função Multid: Puxa os nós da árvore até ler um próximo dado de algum arquivo

void Multid(int raiz, int vet[MAX], FILE *arquivo[ARQ]) {

	// Se for folha
	if (raiz >= ARQ) {
   	vet[raiz] = LeDado(arquivo[ARQ - raiz]);
      return;
   } // Fim de if

   // Se não for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se não tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1) {
   	vet[raiz] = -1;
      return;
   } // Fim de if

   // Se só tiver o filho direito
   if (vet[fE] == -1) {
   	vet[raiz] = vet[fD];
      return;
   } // Fim de if

   // Se só tiver o filho esquerdo
   if (vet[fD] == -1) {
   	vet[raiz] = vet[fE];
      return;
   } // Fim de if

   // Se tiver ambos filhos
   if (vet[fE] < vet[fD]) {
   	vet[raiz] = vet[fE];
      Multid(fE, vet, arquivo);
   } // Fim de if
   else {
   	vet[raiz] = vet[fD];
      Multid(fD, vet, arquivo);
   } // Fim de else

} // Fim de Multid

//----------------------------------------------------------------------------//
// Função Heap: Faz um Heap decrescente na árvore

void Heap(int raiz, int vet[MAX]) {

	// Se for folha
	if (raiz >= ARQ) return;

   // Se não for folha
   int fE = raiz * 2;
   int fD = fE + 1;

	// Se não tiver mais filhos
   if (vet[fE] == -1 && vet[fD] == -1) return;

   int troca;

   // Se só tiver o filho direito
   if (vet[fE] == -1) {
		if (vet[raiz] > vet[fD]) {
      	troca = vet[raiz];
        	vet[raiz] = vet[fD];
         vet[fD] = troca;
      } // Fim de if
      return;
   } // Fim de if

   // Se só tiver o filho esquerdo
   if (vet[fD] == -1) {
		if (vet[raiz] > vet[fE]) {
      	troca = vet[raiz];
        	vet[raiz] = vet[fE];
         vet[fE] = troca;
      } // Fim de if
      return;
   } // Fim de if

   // Se tiver ambos filhos
   int menor = fE;

   if (vet[fE] > vet[fD]) menor++;

   if (vet[raiz] > vet[menor]) {
    	troca = vet[raiz];
     	vet[raiz] = vet[menor];
      vet[menor] = troca;
      Heap(menor, vet);
   } // Fim de if
   else
      Heap(menor, vet);

} // Fim de Heap
