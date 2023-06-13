#include <pigpio.h>

int DELAY = 1000;

// Commands
int LCD_BACKLIGHT = 0x08;
int FOUR_BIT_MODE = 0x02;
int DISPLAY_ON_CURSOR_OFF = 0x0c;
int UNDERLINE_CURSOR = 0x0E;
int BLINK_CURSOR = 0x0F;
int CLEAR_SCREEN = 0x01;
int CURSOR_POSITION = 0x80;
int CURSOR_POSITION_SECOND_LINE = 0xC0;
int LINE_MODE = 0x28; // 2 lines, 5x8 dot matrix
int LDC_BACKLIGHT = 0x08;
int REMOVE_CURSOR = 0x0C;

void writeByte(int lcd1602, int data, int mode);
int LCD1602Init(int i2cAddress);
void LCD1602WriteString(int lcd1602, char *string);
void LCD1602CursorReset(int lcd1602);
void LCD1602ClearScreen(int lcd1602);
void LCD1602ShowCursor(int lcd1602);
void LCD1602SetCursorPosition(int lcd1602, int x, int y);

int LCD1602Init(int i2cAddress)
{
    int lcd1602 = i2cOpen(1, i2cAddress, 0);
    writeByte(lcd1602, FOUR_BIT_MODE, 0);
    writeByte(lcd1602, LINE_MODE, 0);
    writeByte(lcd1602, BLINK_CURSOR, 0);
    LCD1602ClearScreen(lcd1602);

    return lcd1602;
}

// Mode 0 means command
// Mode 1 means we are sending data such as text
// We have to set the EN pins first to high, then low,
// For each nibble.
void writeByte(int lcd1602, int data, int mode)
{
    // Set EN pin high (1)
    i2cWriteByte(lcd1602, mode | (data & 0xF0) | LDC_BACKLIGHT | 0x04);
    gpioDelay(DELAY);

    // Set EN pin low (0)
    i2cWriteByte(lcd1602, mode | (data & 0xF0) | LDC_BACKLIGHT);
    gpioDelay(DELAY);

    // Set EN pin high (1)
    i2cWriteByte(lcd1602, mode | ((data << 4) & 0xF0) | LDC_BACKLIGHT | 0x04);
    gpioDelay(DELAY);

    // Set EN pin low (0)
    i2cWriteByte(lcd1602, mode | ((data << 4) & 0xF0) | LDC_BACKLIGHT);
    gpioDelay(DELAY);
}

void LCD1602WriteString(int lcd1602, char *string)
{
    while (*string != '\0')
    {
        writeByte(lcd1602, *string, 1);
        gpioDelay(DELAY);
        string++;
    }
}

void LCD1602CursorReset(int lcd1602)
{
    writeByte(lcd1602, CURSOR_POSITION, 0);
}

void LCD1602RemoveCursor(int lcd1602)
{
    writeByte(lcd1602, REMOVE_CURSOR, 0);
}

void LCD1602ShowCursor(int lcd1602)
{
    writeByte(lcd1602, BLINK_CURSOR, 0);
}

void LCD1602SetCursorPosition(int lcd1602, int x, int y)
{
    if (y == 0)
    {
        writeByte(lcd1602, CURSOR_POSITION + x, 0);
    }
    else
    {
        writeByte(lcd1602, CURSOR_POSITION_SECOND_LINE + x, 0);
    }
}

void LCD1602ClearScreen(int lcd1602)
{
    writeByte(lcd1602, CLEAR_SCREEN, 0);
    LCD1602CursorReset(lcd1602);
}