/************************************************************
communication: Communication functions
************************************************************/
#include "Lunar_RBC.h"

char START_WIFI[] = "AT+CIPSTART=\"UDP\",\"0\",0,3105,2\r\n";


void USARTSend(char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        USART_SendData(USART2, *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
}

void WIFI_Start(void)
{
	USARTSend(START_WIFI,sizeof(START_WIFI));
}

int translate_char(unsigned char input)
{
	switch(input)
	{
		case '0':
		{
			return 0;
			break;
		}
		case '1':
		{
			return 1;
			break;
		}
		case '2':
		{
			return 2;
			break;
		}
		case '3':
		{
			return 3;
			break;
		}
		case '4':
		{
			return 4;
			break;
		}
		case '5':
		{
			return 5;
			break;
		}
		case '6':
		{
			return 6;
			break;
		}
		case '7':
		{
			return 7;
			break;
		}
		case '8':
		{
			return 8;
			break;
		}
		case '9':
		{
			return 9;
			break;
		}
		case 'a':
		{
			return 10;
			break;
		}
		case 'b':
		{
			return 11;
			break;
		}
		case 'c':
		{
			return 12;
			break;
		}
		case 'd':
		{
			return 13;
			break;
		}
		case 'e':
		{
			return 14;
			break;
		}
		case 'f':
		{
			return 15;
			break;
		}
		default:
		{
			return 0;
			break;
		}
	}
}



//END HERE
