#include <conio.h>
#include <stdio.h>

#define MAX 8

void main()
{
	int inicio, dir, esq, pos, troca, vetor[MAX];

   for (pos=0; pos<MAX; pos++)
   	scanf("%d", &vetor[pos]);

   for (int seg=1; seg<MAX; seg++)
   {
   	esq = 0;
      dir = seg;
		inicio = (esq+dir)/2;

      while (esq<dir)
      {
      	if (vetor[seg]<vetor[inicio])
         	dir = inicio;
			else
         	esq = inicio+1;
			inicio = (esq+dir)/2;
      }

		if (inicio<seg)
      {
      	pos = seg;
         do
         {
         	troca = vetor[pos];
            vetor[pos] = vetor[pos-1];
            vetor[pos-1] = troca;
            pos--;
         } while (pos>inicio);
      }
   }

   for (pos=0; pos<MAX; pos++)
   	printf("%d ", vetor[pos]);

   getch();
}