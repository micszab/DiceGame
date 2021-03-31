#include "MKL05Z4.h"                    /* Device header */
#include "lcd1602.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool; //definicja bool

bool move_l = false;
bool move_r = false;
bool S4_ON = false;

uint32_t seed = 0;

uint8_t position = 6;
uint8_t cursor;
uint8_t k[5] = {0, 0, 0, 0, 0};

int game_status = 0;
int p_number = 2;

void S2(void)
{
		if (game_status == 0)
			move_l = true;
		if (game_status == 1)
		{
			position_move_l();
			cursor_value();
		}
}

void S3(void)
{
		if (game_status == 0)
			move_r = true;
		if (game_status == 1)
		{
			position_move_r();
			cursor_value();
		}	
}

void S4(void)
{
		S4_ON = true;	
}


//opoznienie
void delay_ms( int n) 
{
volatile int i;
volatile int j;
	for( i = 0 ; i < n; i++)
	for(j = 0; j < 3500; j++) {}
}

//pozycja w prawo
void position_move_r(void)
{
		position++;
		if (position == 7) { position = 1; }
		delay_ms(200);
}

//pozycja w lewo
void position_move_l(void)
{	
		position--;
		if (position == 0) { position = 6; }
		delay_ms(200);
}

//ustal wartosc kursora na podstawie pozycji
void cursor_value(void)
{
	switch(position)
	{
		case 1:
			cursor = 0;
			break;
		case 2:
			cursor = 2;
			break;
		case 3:
			cursor = 4;
			break;
		case 4:
			cursor = 6;
			break;
		case 5:
			cursor = 8;
			break;
		case 6:
			cursor = 11;
			break;
	}
}

//rzuc ponownie
void dice_reset()
{
	k[position - 1] = 0;
	S4_ON = false;
}

int Game_start(void)
{
	char p_number_d[1];
	
	LCD1602_SetCursor(8,1);
	LCD1602_Print("START S4");
	
	LCD1602_SetCursor(0,0);
	LCD1602_Print("Liczba graczy:");
	
	while(1)
	{
		if (move_l)
		{	
			p_number--;
			if (p_number == 1) {p_number = 2;}
			move_l = false;
		}
		if (move_r)
		{
			p_number++;
			if (p_number == 5) {p_number = 4;}
			move_r = false;
		}
		
		if (S4_ON)
		{
			srand(seed);
			S4_ON = false;
			return p_number;
		}
		
		sprintf(p_number_d, "%d", p_number);
		LCD1602_SetCursor(0,1);
		LCD1602_Print(p_number_d);
	}
}

void player_turn(void)
{
	char k0_d [1], k1_d [1], k2_d [1], k3_d [1], k4_d [1];	//zmienne do wyswietlania wylosowanych liczb
	uint8_t rzut = 0;
	
	while(1)
	{
		//rzuty
		if (position == 6 && S4_ON)
			{
			game_status = 1;
			S4_ON = false;
			rzut++;
				
			if (rzut == 4) 
			{	
				game_status = 2;
				return;
			}
				
			LCD1602_ClearAll();
			
			for (int i = 0; i < 5; i++)
			{
					if (k[i] == 0)
					k[i] = rand() % 6 + 1;
			}
			
			LCD1602_SetCursor(0,1);
			sprintf(k0_d, "%d", k[0]);
			LCD1602_Print(k0_d);
			
			LCD1602_SetCursor(2,1);
			sprintf(k1_d, "%d", k[1]);
			LCD1602_Print(k1_d);
			
			LCD1602_SetCursor(4,1);
			sprintf(k2_d, "%d", k[2]);
			LCD1602_Print(k2_d);
			
			LCD1602_SetCursor(6,1);
			sprintf(k3_d, "%d", k[3]);
			LCD1602_Print(k3_d);
			
			LCD1602_SetCursor(8,1);
			sprintf(k4_d, "%d", k[4]);
			LCD1602_Print(k4_d);
			
			LCD1602_SetCursor(11,1);
			LCD1602_Print("DALEJ");
			
			LCD1602_SetCursor(11,0);
			if (rzut == 1) {LCD1602_Print("RZUT1");}
			if (rzut == 2) {LCD1602_Print("RZUT2");}
			if (rzut == 3) 
				{
				LCD1602_Print("RZUT3");
				for (int j = 0; j < 5; j++)
					{
							k[j] = 0;
					}
				}
			
			delay_ms(100);
			}
		
		if (rzut > 0 && rzut <4)
		{
			LCD1602_SetCursor(cursor,1);
			LCD1602_Blink_On();
		}
		
		//wybor czym rzucic ponownie
		if (S4_ON && position > 0 && position < 6 && rzut != 3 && rzut != 0)
		{
			S4_ON = false;
			LCD1602_SetCursor(cursor,0);
			LCD1602_Print("X");
			dice_reset();
			delay_ms(100);
		}
	}
}
