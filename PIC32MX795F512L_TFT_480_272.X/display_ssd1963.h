/* 
 * File:   display_ssd1963.h
 * Author: Alleen Wang
 *
 * Created on 2014/12/23, PM 3:42
 */

#ifndef DISPLAY_SSD1963_H
#define	DISPLAY_SSD1963_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DISPLAY_TRIS_RS  TRISDbits.TRISD3
#define DISPLAY_TRIS_WR  TRISDbits.TRISD4
#define DISPLAY_TRIS_RD  TRISDbits.TRISD5
#define DISPLAY_TRIS_DB0 TRISEbits.TRISE0
#define DISPLAY_TRIS_DB1 TRISEbits.TRISE1
#define DISPLAY_TRIS_DB2 TRISEbits.TRISE2
#define DISPLAY_TRIS_DB3 TRISEbits.TRISE3
#define DISPLAY_TRIS_DB4 TRISEbits.TRISE4
#define DISPLAY_TRIS_DB5 TRISEbits.TRISE5
#define DISPLAY_TRIS_DB6 TRISEbits.TRISE6
#define DISPLAY_TRIS_DB7 TRISEbits.TRISE7
#define DISPLAY_TRIS_CS  TRISBbits.TRISB5
#define DISPLAY_TRIS_RST TRISBbits.TRISB4
#define DISPLAY_TRIS_BL  TRISDbits.TRISD2

#define DISPLAY_RS  LATDbits.LATD3
#define DISPLAY_WR  LATDbits.LATD4
#define DISPLAY_RD  LATDbits.LATD5

#define DISPLAY_DB0 LATEbits.LATE0
#define DISPLAY_DB1 LATEbits.LATE1
#define DISPLAY_DB2 LATEbits.LATE2
#define DISPLAY_DB3 LATEbits.LATE3
#define DISPLAY_DB4 LATEbits.LATE4
#define DISPLAY_DB5 LATEbits.LATE5
#define DISPLAY_DB6 LATEbits.LATE6
#define DISPLAY_DB7 LATEbits.LATE7

#define DISPLAY_CS  LATBbits.LATB5
#define DISPLAY_RST LATBbits.LATB4
#define DISPLAY_BL  LATDbits.LATD2

#define DISPLAY_RS_DTAT_MODE 1
#define DISPLAY_RS_CONTROL_MODE 0

#define DataPort(x) \
            DISPLAY_DB0 = ( x >> 0) & 0b00000001; \
            DISPLAY_DB1 = ( x >> 1) & 0b00000001; \
            DISPLAY_DB2 = ( x >> 2) & 0b00000001; \
            DISPLAY_DB3 = ( x >> 3) & 0b00000001; \
            DISPLAY_DB4 = ( x >> 4) & 0b00000001; \
            DISPLAY_DB5 = ( x >> 5) & 0b00000001; \
            DISPLAY_DB6 = ( x >> 6) & 0b00000001; \
            DISPLAY_DB7 = ( x >> 7) & 0b00000001;

#define bit_test(x,n) (x & (0x01<<n))

void init_display_1963();

void TFT_Init_SSD1963_8bit(short display_width, short display_height);
void TFT_Reset_SSD1963_8bit();
void TFT_SSD1963_8bit_Set_Index(short index);
void TFT_SSD1963YT_8bit_Write_Command(char command);
void TFT_Convert_Color565(short Color565);
void TFT_Set_Address_SSD1963II(short x1, short y1, short x2, short y2);
void TFT_Set_Address_SSD1963(short x, short y);
void TFT_SSD1963YT_16bit_Write_Command(short command);
//void RGB888 (int PaintColor);
void DataColor(char R, char G, char B);

char Is_TFT_Rotated_180();
////////////////////////////////////////////////////////////////////////////////
void TFT_FillRGB(unsigned char R, unsigned char G, unsigned char B);
void TFT_Fill(unsigned long Color);
void TFT_Pixel(short x, short y, unsigned long Color);
void TFT_Line(short x1, short y1, short x2, short y2, unsigned long Color);
void TFT_Circle(short x, short y, short radius, char Fill, unsigned long Color);
void TFT_Rect(short x1, short y1, short x2, short y2, char fill, unsigned long Color);
void TFT_Text(short x, short y, char* textptr, char size, unsigned long Color);
void TFT_Pixel565(short x, short y, short ColorPaint565);

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_SSD1963_H */

