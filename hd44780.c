/*
 Wyświetlacz alfanumeryczny ze sterownikiem HD44780
 Sterowanie w trybie 4-bitowym bez odczytu flagi zajętości z dowolnym
 przypisaniem sygnałów sterujących
 Plik : HD44780.c
 Mikrokontroler : Atmel AVR XMEGA
 Kompilator : avr-gcc
 Autor : Radosław Kwiecień
 Źródło : http://radzio.dxp.pl/hd44780/
 Data : 24.03.2007

 Dostosowanie do procesorów XMEGA
 Dominik Leon Bieczyński
 Leon Instruments
 http://leon-instruments.blogspot.com

 Data: 24.01.2018
 Dostowanie do płytki eXtrinoXL FULL [Atmel AVR XMEGA128A3U]
 Paweł 'felixd' Wojciechowski
 Outsourcing IT - Konopnickiej.Com
 http://www.konopnickiej.com
 */

#include "hd44780.h"
#include <avr/pgmspace.h>

void _lcd_OutNibble(unsigned char nibbleToWrite)
{
    if (nibbleToWrite & 0b00000001)
        LCD_PORT.OUTSET = LCD_D4_bm;
    else
        LCD_PORT.OUTCLR = LCD_D4_bm;

    if (nibbleToWrite & 0b00000010)
        LCD_PORT.OUTSET = LCD_D5_bm;
    else
        LCD_PORT.OUTCLR = LCD_D5_bm;

    if (nibbleToWrite & 0b00000100)
        LCD_PORT.OUTSET = LCD_D6_bm;
    else
        LCD_PORT.OUTCLR = LCD_D6_bm;

    if (nibbleToWrite & 0b00001000)
        LCD_PORT.OUTSET = LCD_D7_bm;
    else
        LCD_PORT.OUTCLR = LCD_D7_bm;
}

void _lcd_Write(unsigned char dataToWrite)
{
    LCD_PORT.OUTSET = LCD_E_bm;
    _lcd_OutNibble(dataToWrite >> 4);
    LCD_PORT.OUTCLR = LCD_E_bm;
    asm volatile("nop");
    LCD_PORT.OUTSET = LCD_E_bm;
    _lcd_OutNibble(dataToWrite);
    LCD_PORT.OUTCLR = LCD_E_bm;
    _delay_us(50);
}

void LcdCommand(unsigned char commandToWrite)
{
    LCD_RS_PORT.OUTCLR = LCD_RS_bm;
    _lcd_Write(commandToWrite);
}

void LcdData(unsigned char dataToWrite)
{
    LCD_RS_PORT.OUTSET = LCD_RS_bm;
    _lcd_Write(dataToWrite);
}

void LcdWrite(char *text)
{
    while (*text)
        LcdData(*text++);
}

void LcdWriteProgmem(const char *data)
{
    char bufor;
    while ((bufor = (char)pgm_read_byte(data++)))
        LcdData(bufor);
}

void LcdGoto(unsigned char x, unsigned char y)
{
    LcdCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}

void LcdClear(void)
{
    LcdCommand(HD44780_CLEAR);
    _delay_ms(2);
}

void LcdInit(void)
{
    LCD_PORT.DIRSET = LCD_D4_bm |
                      LCD_D5_bm |
                      LCD_D6_bm |
                      LCD_D7_bm |
                      LCD_E_bm;

    LCD_RS_PORT.DIRSET = LCD_RS_bm;

    _delay_ms(5); // oczekiwanie na ustalibizowanie się napiecia zasilającego
    LCD_RS_PORT.OUTCLR = LCD_RS_bm;
    LCD_PORT.OUTCLR = LCD_E_bm;

    for (uint8_t i = 0; i < 3; i++)
    { // trzykrotne powtórzenie bloku instrukcji
        LCD_PORT.OUTSET = LCD_E_bm;
        _lcd_OutNibble(0x03); // tryb 8-bitowy
        LCD_PORT.OUTCLR = LCD_E_bm;
        _delay_ms(5); // czekaj 5ms
    }

    LCD_PORT.OUTSET = LCD_E_bm;
    _lcd_OutNibble(0x02); // tryb 4-bitowy
    LCD_PORT.OUTCLR = LCD_E_bm;

    _delay_ms(1); // czekaj 1ms
    LcdCommand(
        HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
    LcdCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);                        // wyłączenie wyswietlacza
    LcdCommand(HD44780_CLEAR);                                                      // czyszczenie zawartości pamięci DDRAM
    _delay_ms(2);
    LcdCommand(
        HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT); // inkrementaja adresu i przesuwanie kursora
    LcdCommand(
        HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // włącz LCD, bez kursora i mrugania
}

void LcdDec(uint32_t liczba)
{
    if (liczba == 0)
    {
        LcdData('0');
        return;
    }

    uint8_t cyfra[10];
    int8_t i = 0;

    while (liczba)
    {
        cyfra[i] = (uint8_t)(liczba % 10);
        liczba = liczba / 10;
        ++i;
    }
    --i;

    for (; i >= 0; --i)
    {
        LcdData(cyfra[i] + 48);
    }
}

uint8_t LcdDecComma(int32_t liczba, uint8_t po_przecinku)
{
    if (liczba == 0)
    {
        LcdData('0');
        if (po_przecinku != 0)
            LcdData(',');
        while (po_przecinku)
        {
            LcdData('0');
            po_przecinku--;
        }
        return 0;
    }

    uint8_t cyfra[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t i = 0;

    if (liczba < 0)
    {
        LcdData('-');
        liczba = -liczba;
    }

    while (liczba)
    {
        cyfra[i] = (uint8_t)(liczba % 10);
        liczba = liczba / 10;
        ++i;
    }

    if (i > po_przecinku)
        --i;
    else
        i = po_przecinku;

    for (; i >= 0; --i)
    {
        if (i == po_przecinku - 1)
            LcdData(',');
        LcdData(cyfra[i] + 48);
    }

    return 0;
}

void LcdBin(uint8_t liczba)
{
    if (liczba == 0)
    {
        Lcd("00000000");
        return;
    }

    uint8_t cyfra[8];
    int8_t i;

    for (i = 7; i >= 0; --i)
    {
        cyfra[i] = liczba % 2;
        liczba = liczba / 2;
    }

    for (i = 0; i < 8; ++i)
    {
        LcdData(cyfra[i] + 48);
    }
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------
