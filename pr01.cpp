#include <conio.h>
#include <stdio.h>

#define MAX 8

void main()
{
	int inicio, pos, troca, vetor[MAX];

   for (pos=0; pos<MAX; pos++)
   	scanf("%d", &vetor[pos]);

   for (int seg=1; seg<MAX; seg++)
   {
   	inicio = seg;

      while ((vetor[seg]<vetor[inicio-1])&&(inicio>0))
			inicio--;

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