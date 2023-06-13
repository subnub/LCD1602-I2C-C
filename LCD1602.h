#ifndef LCD1602
#define LCD1602

int LCD1602Init(int i2cAddress);
void LCD1602WriteString(int lcd1602, char *string);
void LCD1602CursorReset(int lcd1602);
void LCD1602ClearScreen(int lcd1602);
void LCD1602RemoveCursor(int lcd1602);
void LCD1602ShowCursor(int lcd1602);
void LCD1602SetCursorPosition(int lcd1602, int x, int y);

#endif