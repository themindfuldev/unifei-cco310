#include <stdlib.h>


void BoyerMoore (char texto[C], char padrao[]){
	int i, j, M, ult[256];
   M = strlen(padrao); // M assume a quantidade de caracteres do padrao

   // pre-processamento a palavra padrao[]
   for (i=0; i<256; i++) ult[i]=0;
   for (i=1; i<=M; i++) ult[padrao[i]] = i;
   // busca da palavra padrao[] no texto
   i = M-1;
   j = M-1;
   do {
   	if (padrao[j] == texto[i]) {
      	if (j==0){
         	// padrao encontrado
            break;
         }
         else{
         	i--;
            j--;
         }
      }
      else {
      	i = i+M - min(j, 1+ult[texto[i]]);
         j = M-1;
      }
   }while (i<C-1);
}
//------------------------------------------------------------------------------
