//main
#include	<system_stm32f4xx.c>
#include "ks0108.c"

int main (void)
{
	RCC		->	AHB1ENR		|=	(1<<0);							//Enable PortA	
	RCC		->	AHB1ENR		|=	(1<<1);							//Enable PortB
	RCC		->	AHB1ENR		|=	(1<<4);
	init_ks0108();
	gotoxy_ks0108(10,10);
	unsigned char *ch;
	puts_ks0108(ch);
	while(1);
}
