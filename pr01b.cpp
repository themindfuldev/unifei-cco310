#include <conio.h>
#include <stdio.h>

#define MAX 8

void main()
{
	int vetor[MAX], inicio, troca1, troca2;

   for (int pos=0; pos<MAX; pos++)
   	scanf("%d", &vetor[pos]);

   for (int seg=1; seg<MAX; seg++)
   {
   	inicio = seg;

      while ((vetor[seg]<vetor[inicio-1])&&(inicio>0))
			inicio--;

		if (inicio<seg)
      {
	    	troca1 = vetor[inicio];
	      vetor[inicio] = vetor[seg];
	      inicio++;
         
	      while (inicio<seg)
	      {
	      	troca2 = vetor[inicio];
	         vetor[inicio] = troca1;
	         troca1 = troca2;
	         inicio++;
	      }

	      vetor[inicio] = troca1;
      }
   }

   for (int pos=0; pos<MAX; pos++)
   	printf("%d ", vetor[pos]);

   getch();
}