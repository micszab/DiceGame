/*-------------------------------------------------------------------------
autor: Michał Szablewski
----------------------------------------------------------------------------*/
	
#include "MKL05Z4.h"
#include "lcd1602.h"
#include "buttons.h"
#include "game.h"
#include <stdio.h>

extern uint32_t seed;

//przerwanie systick
void SysTick_Handler(void) { 							
	seed++;												
}

int main (void) 
{
	SysTick_Config(SystemCoreClock/1000); //konfiguracja systick
	
	LCD1602_Init();		 // Inicjalizacja LCD
	LCD1602_Backlight(TRUE);
	
	buttonsInitialize(); //Inicjalizacja przycisków
	
	char id[1]; //numer gracza
		
	//poczekaj na start
	int players = Game_start();

	//gra
  for (int j = 0; j < 13; j++)
	{
		for (int i = 1; i < players + 1; i++)
		{		
		LCD1602_Blink_Off();
		LCD1602_ClearAll();
			
		sprintf(id, "Gracz: %d", i);
		LCD1602_SetCursor(4,0);
		LCD1602_Print(id);
		
		player_turn();
		}
	}
}
