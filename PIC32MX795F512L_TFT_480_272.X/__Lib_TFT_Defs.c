//#include <built_in.h>
// Externs
//extern atomic sfr sbit TFT_RST_Direction, TFT_RST, TFT_RS_Direction, TFT_RS, TFT_CS_Direction, TFT_CS, TFT_WR, TFT_RD, TFT_WR_Direction, TFT_RD_Direction;
//extern atomic sfr char TFT_DataPort, TFT_DataPort_Direction;

static char TFT_Rotated_180 = 0;
static unsigned char TFT_Disp_Rotation = 0;
const  char TFT_NOT_ROTATED = 0;
const  char TFT_ROTATED180  = 1;

#define TFT_Write_Strobe()  TFT_WR = 0;  asm nop; TFT_WR = 1;

/**
  * @brief  Reads weather display has been rotated 180 degrees or not.
  * @retval The values can be : 0 - Display not rotated (working in 0� or 90� orientation)
  *                             1 - Display rotated     (working in 180� or 270� orientation)
  */
char Is_TFT_Rotated_180() {
  return TFT_Rotated_180;
}

/**
  * @brief  Sets display 180� rotation or not.
  * @param  Arotate : The values can be : TFT_NOT_ROTATED - Display not rotated, working in 0� or 90� orientation (depending on TFT_Disp_Rotation)
  *                                       TFT_ROTATED180  - Display rotated, working in 180� or 270� orientation (depending on TFT_Disp_Rotation)
  */
void TFT_Rotate_180(char Arotate){
  TFT_Rotated_180 = Arotate;
}

/**
  * @brief  Sets controllers register address to be written in next cycle.
  * @param  index : The values can be in [0x00 - 0xFF] range
  */
void TFT_Set_Index(unsigned short index) {
  TFT_RS = 0;
  TFT_DataPort = index;
  TFT_Write_Strobe();
}

/**
  * @brief  Sets value of controller register (address must be previously defined with TFT_Set_Index)
  * @param  cmd : The values can be in [0x00 - 0xFF] range
  */
void TFT_Write_Command(unsigned short cmd) {
  TFT_RS = 1;
  TFT_DataPort = cmd;
  TFT_Write_Strobe();
}

/**
  * @brief  Sets address and value of controller register
  * @param  index : Address of controller register. Values can be in [0x00 - 0xFF] range
  * @param  value : Value of controller register. Values can be in [0x00 - 0xFF] range
  */
void TFT_Set_Reg(unsigned short index, unsigned short value) {
  TFT_CS = 0;
  TFT_Set_Index_Ptr(index);
  TFT_Write_Command_Ptr(value);
  TFT_CS = 1;
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_Device() {
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  // Driving ability setting
  TFT_Set_Reg(0xEA, 0x00); // PTBA[15:8]
  TFT_Set_Reg(0xEB, 0x20); // PTBA[7:0]
  TFT_Set_Reg(0xEC, 0x0C); // STBA[15:8]
  TFT_Set_Reg(0xED, 0xC4); // STBA[7:0]
  TFT_Set_Reg(0xE8, 0x40); // OPON[7:0]
  TFT_Set_Reg(0xE9, 0x38); // OPON1[7:0]
  TFT_Set_Reg(0xF1, 0x01); // OTPS1B
  TFT_Set_Reg(0xF2, 0x10); // GEN
  TFT_Set_Reg(0x27, 0xA3);

  // Gamma 2.8 setting
  TFT_Set_Reg(0x40, 0x00); //
  TFT_Set_Reg(0x41, 0x00); //
  TFT_Set_Reg(0x42, 0x01); //
  TFT_Set_Reg(0x43, 0x13); //
  TFT_Set_Reg(0x44, 0x10); //
  TFT_Set_Reg(0x45, 0x26); //
  TFT_Set_Reg(0x46, 0x08); //
  TFT_Set_Reg(0x47, 0x51); //
  TFT_Set_Reg(0x48, 0x02); //
  TFT_Set_Reg(0x49, 0x12); //
  TFT_Set_Reg(0x4A, 0x18); //
  TFT_Set_Reg(0x4B, 0x19); //
  TFT_Set_Reg(0x4C, 0x14); //

  TFT_Set_Reg(0x50, 0x19); //
  TFT_Set_Reg(0x51, 0x2F); //
  TFT_Set_Reg(0x52, 0x2C); //
  TFT_Set_Reg(0x53, 0x3E); //
  TFT_Set_Reg(0x54, 0x3F); //
  TFT_Set_Reg(0x55, 0x3F); //
  TFT_Set_Reg(0x56, 0x2E); //
  TFT_Set_Reg(0x57, 0x77); //
  TFT_Set_Reg(0x58, 0x0B); //
  TFT_Set_Reg(0x59, 0x06); //
  TFT_Set_Reg(0x5A, 0x07); //
  TFT_Set_Reg(0x5B, 0x0D); //
  TFT_Set_Reg(0x5C, 0x1D); //
  TFT_Set_Reg(0x5D, 0xCC); //

  // Display setting
  if (TFT_Disp_Rotation == 90) {
    TFT_Set_Reg(0x04, 0x00);
    TFT_Set_Reg(0x05, 0xEF);
    TFT_Set_Reg(0x08, 0x01);
    TFT_Set_Reg(0x09, 0x3F);
  } else {
    TFT_Set_Reg(0x04, 0x01);
    TFT_Set_Reg(0x05, 0x3F);
    TFT_Set_Reg(0x08, 0x00);
    TFT_Set_Reg(0x09, 0xEF);
  }

  if (TFT_Disp_Rotation == 90) {
    if (Is_TFT_Rotated_180())
      TFT_Set_Reg(0x16, 0xC8); // MY=1, MX=1, MV=0, BGR=1
    else
      TFT_Set_Reg(0x16, 0x08); // MY=0, MX=0, MV=0, BGR=1
  } else {
    if (Is_TFT_Rotated_180())
      TFT_Set_Reg(0x16, 0xA8); // MY=0, MX=1, MV=1, BGR=1
    else
      TFT_Set_Reg(0x16, 0x68); // MY=1, MX=0, MV=1, BGR=1
  }
  // Power Voltage Setting
  TFT_Set_Reg(0x1B, 0x1B); // VRH = 4.65
  TFT_Set_Reg(0x1A, 0x01); // BT
  TFT_Set_Reg(0x24, 0x2F); // VMH
  TFT_Set_Reg(0x25, 0x57); // VML

  // Vcom offset
  TFT_Set_Reg(0x23, 0x8D); // FLICKER ADJUST

  // Power ON Setting
  TFT_Set_Reg(0x18, 0x36);
  TFT_Set_Reg(0x19, 0x01);
  TFT_Set_Reg(0x01, 0x00);
  TFT_Set_Reg(0x1F, 0x88);
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0x80);
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0x90);
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0xD0);
  Delay_5ms();
  // 65K Color Selection
  TFT_Set_Reg(0x17, 0x05);
  // Set Panel
  TFT_Set_Reg(0x36, 0x00); // Panel characteristic control register
  // Display ON Setting
  TFT_Set_Reg(0x28, 0x38);
  Delay_10ms();
  Delay_10ms();
  Delay_10ms();
  Delay_10ms();
  TFT_Set_Reg(0x28, 0x3C);
}

/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x02);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Set_Index_Ptr(0x03);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x06);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Set_Index_Ptr(0x07);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x22);
}

/**
  * @brief  Write 16bit value on TFT Data bus. This is 8bit communication
  *         so it is done in two cycles. This should be used when working with
  *         TFT controller in 8bit mode.
  * @param  _data : Data to be transfered. Range values [0x00 - 0xFFFF]
  */
void TFT_Write_Data(unsigned int _data) {
  TFT_RS = 1;
  TFT_DataPort = Hi(_data);
  TFT_Write_Strobe();
  TFT_DataPort = Lo(_data);
  TFT_Write_Strobe();
}

/**
  * @brief  Write 16bit value on TFT Data bus. This is 16bit communication
  *         so it is done in a single cycles. This should be used when 
  *         working with TFT controller in 8bit mode.
  * @param  _data : Data to be transfered. Range values [0x00 - 0xFFFF]
  */
void TFT_16bit_Write_Data(unsigned int _data) {
  TFT_RS = 1;
  *((unsigned int*)&TFT_DataPort) = _data;
  TFT_Write_Strobe();
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8347D)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Set DataPort as Output
  TFT_DataPort_Direction = 0;
  // Reset device
  TFT_Reset_Device();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8347D)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_Device();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}

/***************************** SST7715R *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_SST7715R(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_SST7715R(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  TFT_RST = 1;
  Delay_1ms();
  TFT_RST = 0;
  Delay_10ms();
  TFT_RST = 1;
  Delay_100ms(); Delay_10ms(); Delay_10ms();

  TFT_CS = 0;

  TFT_Set_Index_Ptr(0x11); // Exit Sleep
  Delay_100ms(); Delay_10ms(); Delay_10ms(); Delay_10ms();

  TFT_Set_Index_Ptr(0xB1);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x23);
  TFT_Write_Command_Ptr(0x22);

  TFT_Set_Index_Ptr(0xB2);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x23);
  TFT_Write_Command_Ptr(0x22);

  TFT_Set_Index_Ptr(0xB3);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x23);
  TFT_Write_Command_Ptr(0x22);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x23);
  TFT_Write_Command_Ptr(0x22);

  TFT_Set_Index_Ptr(0xB4);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xC0);
  TFT_Write_Command_Ptr(0xA3);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x84);

  TFT_Set_Index_Ptr(0xC1);
  TFT_Write_Command_Ptr(0xC5);

  TFT_Set_Index_Ptr(0xC2);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xC3);
  TFT_Write_Command_Ptr(0x8A);
  TFT_Write_Command_Ptr(0x2A);

  TFT_Set_Index_Ptr(0xC4);
  TFT_Write_Command_Ptr(0x8A);
  TFT_Write_Command_Ptr(0xEE);

  TFT_Set_Index_Ptr(0xC5);
  TFT_Write_Command_Ptr(0x07);

  TFT_Set_Index_Ptr(0x36);
  if (TFT_Disp_Rotation == 90) {
    if (Is_TFT_Rotated_180() == 1)
      TFT_Write_Command_Ptr(0xA0);
    else
      TFT_Write_Command_Ptr(0x60);
  } else {
    if (Is_TFT_Rotated_180() == 1)
      TFT_Write_Command_Ptr(0x00);
    else
      TFT_Write_Command_Ptr(0xC0);
  }

  TFT_Set_Index_Ptr(0x3A);
  TFT_Write_Command_Ptr(0x05);  // RGB 565

  TFT_Set_Index_Ptr(0xF0);
  TFT_Write_Command_Ptr(0x01);

  TFT_Set_Index_Ptr(0xF6);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xE0);
  TFT_Write_Command_Ptr(0x03);//p1
  TFT_Write_Command_Ptr(0x1B);//p2
  TFT_Write_Command_Ptr(0x09);//p3
  TFT_Write_Command_Ptr(0x0E);//p4
  TFT_Write_Command_Ptr(0x32);//p5
  TFT_Write_Command_Ptr(0x2D);//p6
  TFT_Write_Command_Ptr(0x28);//p7
  TFT_Write_Command_Ptr(0x2C);//p8
  TFT_Write_Command_Ptr(0x2B);//p9
  TFT_Write_Command_Ptr(0x29);//p10
  TFT_Write_Command_Ptr(0x30);//p11
  TFT_Write_Command_Ptr(0x3B);//p12
  TFT_Write_Command_Ptr(0x00);//p13
  TFT_Write_Command_Ptr(0x01);//p14
  TFT_Write_Command_Ptr(0x02);//p15
  TFT_Write_Command_Ptr(0x10);//p16

  TFT_Set_Index_Ptr(0xE1);
  TFT_Write_Command_Ptr(0x03);//p1
  TFT_Write_Command_Ptr(0x1B);//p2
  TFT_Write_Command_Ptr(0x09);//p3
  TFT_Write_Command_Ptr(0x0E);//p4
  TFT_Write_Command_Ptr(0x32);//p5
  TFT_Write_Command_Ptr(0x2E);//p6
  TFT_Write_Command_Ptr(0x28);//p7
  TFT_Write_Command_Ptr(0x2C);//p8
  TFT_Write_Command_Ptr(0x2B);//p9
  TFT_Write_Command_Ptr(0x28);//p10
  TFT_Write_Command_Ptr(0x31);//p11
  TFT_Write_Command_Ptr(0x3C);//p12
  TFT_Write_Command_Ptr(0x00);//p13
  TFT_Write_Command_Ptr(0x00);//p14
  TFT_Write_Command_Ptr(0x02);//p15
  TFT_Write_Command_Ptr(0x10);//p15

  TFT_Set_Index_Ptr(0x13); // Normal Display Mode On
  TFT_Set_Index_Ptr(0x20); // Display inversion OFF
  TFT_Set_Index_Ptr(0x29); // Display ON
  TFT_Set_Index_Ptr(0x38); // Idle Mode Off
  Delay_100ms();

  TFT_Set_Index_Ptr(0x2A);         // set_column_address
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr((TFT_DISP_WIDTH-1)>>8);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH-1);

  TFT_Set_Index_Ptr(0x2B);         // set_page_address:
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr((TFT_DISP_HEIGHT-1)>>8);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT-1);
  TFT_Set_Index_Ptr(0x2C);
  Delay_10ms();
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (SST7715R)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SST7715R(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_DataPort_Direction = 0;
  TFT_Reset_SST7715R();
  TFT_Set_Address_Ptr = TFT_Set_Address_SST7715R;
}

/**
  * @brief  Complete initializing procedure for TFT controller (SST7715R)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SST7715R_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_SST7715R();
  TFT_Set_Address_Ptr = TFT_Set_Address_SST7715R;
}
/*************************** eof SST7715R ***************************/

/***************************** HX8352A *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_HX8352A(unsigned int x, unsigned int y) {
  if (TFT_Disp_Rotation == 90) {
    TFT_Set_Index_Ptr(0x02);
    TFT_Write_Command_Ptr(x>>8);
    TFT_Set_Index_Ptr(0x03);
    TFT_Write_Command_Ptr(x);
    TFT_Set_Index_Ptr(0x06);
    TFT_Write_Command_Ptr(y>>8);
    TFT_Set_Index_Ptr(0x07);
    TFT_Write_Command_Ptr(y);
  }
  else {
    TFT_Set_Index_Ptr(0x02);
    TFT_Write_Command_Ptr(y>>8);
    TFT_Set_Index_Ptr(0x03);
    TFT_Write_Command_Ptr(y);
    TFT_Set_Index_Ptr(0x06);
    TFT_Write_Command_Ptr(x>>8);
    TFT_Set_Index_Ptr(0x07);
    TFT_Write_Command_Ptr(x);
  }
  TFT_Set_Index_Ptr(0x22);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_HX8352A(){
char _counter;
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_CS = 0;
  //  Start initial squence
  TFT_Set_Index_Ptr(0x83); TFT_Write_Command_Ptr(0x02);
  TFT_Set_Index_Ptr(0x85); TFT_Write_Command_Ptr(0x03);
  TFT_Set_Index_Ptr(0x8B); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x8C); TFT_Write_Command_Ptr(0x13);
  TFT_Set_Index_Ptr(0x91); TFT_Write_Command_Ptr(0x01);
  TFT_Set_Index_Ptr(0x83); TFT_Write_Command_Ptr(0x00);
  Delay_10ms();

  // Gamma Setting
  TFT_Set_Index_Ptr(0x3E); TFT_Write_Command_Ptr(0xC4);
  TFT_Set_Index_Ptr(0x3F); TFT_Write_Command_Ptr(0x44);
  TFT_Set_Index_Ptr(0x40); TFT_Write_Command_Ptr(0x22);
  TFT_Set_Index_Ptr(0x41); TFT_Write_Command_Ptr(0x57);
  TFT_Set_Index_Ptr(0x42); TFT_Write_Command_Ptr(0x03);
  TFT_Set_Index_Ptr(0x43); TFT_Write_Command_Ptr(0x47);
  TFT_Set_Index_Ptr(0x44); TFT_Write_Command_Ptr(0x02);
  TFT_Set_Index_Ptr(0x45); TFT_Write_Command_Ptr(0x55);
  TFT_Set_Index_Ptr(0x46); TFT_Write_Command_Ptr(0x06);
  TFT_Set_Index_Ptr(0x47); TFT_Write_Command_Ptr(0x4C);
  TFT_Set_Index_Ptr(0x48); TFT_Write_Command_Ptr(0x06);
  TFT_Set_Index_Ptr(0x49); TFT_Write_Command_Ptr(0x8C);
  // Power Supply Setting
  TFT_Set_Index_Ptr(0x17); TFT_Write_Command_Ptr(0x91);
  TFT_Set_Index_Ptr(0x23); TFT_Write_Command_Ptr(0x01);
  TFT_Set_Index_Ptr(0x2B); TFT_Write_Command_Ptr(0xF9);
  Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x18); TFT_Write_Command_Ptr(0x3A);
  TFT_Set_Index_Ptr(0x1B); TFT_Write_Command_Ptr(0x11);

  TFT_Set_Index_Ptr(0x83); TFT_Write_Command_Ptr(0x02);
  TFT_Set_Index_Ptr(0x8A); TFT_Write_Command_Ptr(0x06);
  TFT_Set_Index_Ptr(0x83); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x1A); TFT_Write_Command_Ptr(0x11);
  TFT_Set_Index_Ptr(0x1C); TFT_Write_Command_Ptr(0x0A);
  TFT_Set_Index_Ptr(0x1F); TFT_Write_Command_Ptr(0x58);
  Delay_10ms(); Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x19); TFT_Write_Command_Ptr(0x0A);
  TFT_Set_Index_Ptr(0x19); TFT_Write_Command_Ptr(0x1A);
  Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x19); TFT_Write_Command_Ptr(0x12);
  Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x1E); TFT_Write_Command_Ptr(0x2E);
  Delay_100ms();
  // DUG FUNCTION ON
  TFT_Set_Index_Ptr(0x5A); TFT_Write_Command_Ptr(0x01);
  // RGB LUT
  TFT_Set_Index_Ptr(0x5C);
  for(_counter=0; _counter < 3; _counter++) {
    TFT_Write_Command_Ptr(0x00);//---------0
    TFT_Write_Command_Ptr(0x03);
    TFT_Write_Command_Ptr(0x0A);
    TFT_Write_Command_Ptr(0x0F);
    TFT_Write_Command_Ptr(0x13);
    TFT_Write_Command_Ptr(0x16);//---------5
    TFT_Write_Command_Ptr(0x19);
    TFT_Write_Command_Ptr(0x1C);
    TFT_Write_Command_Ptr(0x1E);
    TFT_Write_Command_Ptr(0x1F);
    TFT_Write_Command_Ptr(0x25);//---------10
    TFT_Write_Command_Ptr(0x2A);
    TFT_Write_Command_Ptr(0x30);
    TFT_Write_Command_Ptr(0x35);
    TFT_Write_Command_Ptr(0x39);
    TFT_Write_Command_Ptr(0x3D);//---------15
    TFT_Write_Command_Ptr(0x41);
    TFT_Write_Command_Ptr(0x45);
    TFT_Write_Command_Ptr(0x48);
    TFT_Write_Command_Ptr(0x4C);
    TFT_Write_Command_Ptr(0x4F);//---------20
    TFT_Write_Command_Ptr(0x53);
    TFT_Write_Command_Ptr(0x58);
    TFT_Write_Command_Ptr(0x5D);
    TFT_Write_Command_Ptr(0x61);
    TFT_Write_Command_Ptr(0x66);//---------25
    TFT_Write_Command_Ptr(0x6A);
    TFT_Write_Command_Ptr(0x6E);
    TFT_Write_Command_Ptr(0x72);
    TFT_Write_Command_Ptr(0x76);
    TFT_Write_Command_Ptr(0x7A);//---------30
    TFT_Write_Command_Ptr(0x7E);
    TFT_Write_Command_Ptr(0x82);
    TFT_Write_Command_Ptr(0x85);
    TFT_Write_Command_Ptr(0x89);
    TFT_Write_Command_Ptr(0x8D);//---------35
    TFT_Write_Command_Ptr(0x90);
    TFT_Write_Command_Ptr(0x94);
    TFT_Write_Command_Ptr(0x96);
    TFT_Write_Command_Ptr(0x9A);
    TFT_Write_Command_Ptr(0x9D);//---------40
    TFT_Write_Command_Ptr(0xA1);
    TFT_Write_Command_Ptr(0xA4);
    TFT_Write_Command_Ptr(0xA8);
    TFT_Write_Command_Ptr(0xAB);
    TFT_Write_Command_Ptr(0xAF);//---------45
    TFT_Write_Command_Ptr(0xB3);
    TFT_Write_Command_Ptr(0xB7);
    TFT_Write_Command_Ptr(0xBB);
    TFT_Write_Command_Ptr(0xBF);
    TFT_Write_Command_Ptr(0xC3);//---------50
    TFT_Write_Command_Ptr(0xC8);
    TFT_Write_Command_Ptr(0xCC);
    TFT_Write_Command_Ptr(0xD1);
    TFT_Write_Command_Ptr(0xD6);
    TFT_Write_Command_Ptr(0xDB);//---------55
    TFT_Write_Command_Ptr(0xDE);
    TFT_Write_Command_Ptr(0xE1);
    TFT_Write_Command_Ptr(0xE5);
    TFT_Write_Command_Ptr(0xE7);
    TFT_Write_Command_Ptr(0xEC);//---------60
    TFT_Write_Command_Ptr(0xEF);
    TFT_Write_Command_Ptr(0xF4);
    TFT_Write_Command_Ptr(0xFF);//---------63
  }
  //------------- Display ON Setting --------------------------
  TFT_Set_Index_Ptr(0x3C); TFT_Write_Command_Ptr(0xC0);
  TFT_Set_Index_Ptr(0x3D); TFT_Write_Command_Ptr(0x1C);
  TFT_Set_Index_Ptr(0x34); TFT_Write_Command_Ptr(0x38);
  TFT_Set_Index_Ptr(0x35); TFT_Write_Command_Ptr(0x38);
  TFT_Set_Index_Ptr(0x24); TFT_Write_Command_Ptr(0x38);
  Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x24); TFT_Write_Command_Ptr(0x3C);

  TFT_Set_Index_Ptr(0x16);
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr(0x08);
    else
      TFT_Write_Command_Ptr(0xC8);
  else
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0xD8);
    else
      TFT_Write_Command_Ptr (0x18);

  TFT_Set_Index_Ptr(0x01); TFT_Write_Command_Ptr(0x06);
  TFT_Set_Index_Ptr(0x55); TFT_Write_Command_Ptr(0x00);

  // End row and column values have to be fixed disregarding the display orientation
  // 0�, 90�, 180�, 270� change values here if display with different dimensions is used
  TFT_Set_Index_Ptr(0x02); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x03); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x04); TFT_Write_Command_Ptr((/*TFT_DISP_WIDTH*/240-1) >> 8);
  TFT_Set_Index_Ptr(0x05); TFT_Write_Command_Ptr(/*TFT_DISP_WIDTH*/240-1);
  TFT_Set_Index_Ptr(0x06); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x07); TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0x08); TFT_Write_Command_Ptr((/*TFT_DISP_HEIGHT*/400-1) >> 8);
  TFT_Set_Index_Ptr(0x09); TFT_Write_Command_Ptr(/*TFT_DISP_HEIGHT*/400-1);

  TFT_Set_Index_Ptr(0x22);
  Delay_10ms();
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8352A)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8352A(unsigned int display_width, unsigned int display_height) {
  TFT_Set_HX8352A();
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  // Reset device
  TFT_Reset_HX8352A();
  TFT_Set_Address_Ptr = TFT_Set_Address_HX8352A;
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8352A)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8352A_Custom(unsigned int display_width, unsigned int display_height) {
  TFT_Set_HX8352A();
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_HX8352A();
  TFT_Set_Address_Ptr = TFT_Set_Address_HX8352A;
}
/*************************** eof HX8352A ***************************/

/***************************** R61526 *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_R61526(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_R61526(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  Delay_1ms();
  TFT_RST = 0;
  Delay_10ms();
  TFT_RST = 1;
  Delay_100ms(); Delay_10ms(); Delay_10ms();  Delay_10ms();

  TFT_CS = 0;

  TFT_Set_Index_Ptr(0xB0);//Manufacturer Command Access Protect
  TFT_Write_Command_Ptr(0x3F);
  TFT_Write_Command_Ptr(0x3F);
  Delay_100ms(); Delay_100ms(); Delay_10ms(); Delay_10ms();
  Delay_10ms(); Delay_10ms(); Delay_10ms(); Delay_10ms();

  TFT_Set_Index_Ptr(0xFE);//
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x21);
  TFT_Write_Command_Ptr(0xB4);

  TFT_Set_Index_Ptr(0xB3);//Frame Memory Access and Interface Setting
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x10);

  TFT_Set_Index_Ptr(0xE0);//NVM Access Control
  TFT_Write_Command_Ptr(0x00);//NVAE: NVM access enable register. NVM access is enabled when NVAE=1.
  TFT_Write_Command_Ptr(0x40);//FTT: NVM control bit.
  Delay_100ms(); Delay_100ms();

  TFT_Set_Index_Ptr(0xB3);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xFE);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x21);
  TFT_Write_Command_Ptr(0x30);

  TFT_Set_Index_Ptr(0xB0);
  TFT_Write_Command_Ptr(0x3F);
  TFT_Write_Command_Ptr(0x3F);

  TFT_Set_Index_Ptr(0xB3);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xB4);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xC0);
  TFT_Write_Command_Ptr(0x03);//GIP REV  SM GS BGR SS
  TFT_Write_Command_Ptr(0x4F);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0xA0);//BLV=0 LINE
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x01);
  TFT_Write_Command_Ptr(0x00);
  TFT_Set_Index_Ptr(0xC1);
  TFT_Write_Command_Ptr(0x01);//BC0   Frame inversion(0)/Line inversion(1)
  TFT_Write_Command_Ptr(0x02);//DIV0[1:0]  800kHz
  TFT_Write_Command_Ptr(0x20);//RTN0[5:0]   clocks    21
  TFT_Write_Command_Ptr(0x06);//BP0[7:0]   6 lines
  TFT_Write_Command_Ptr(0x06);//FP0[7:0]   6 lines
  Delay_100ms(); Delay_100ms();

  TFT_Set_Index_Ptr(0xC3);  //PARTIAL MODE
  TFT_Write_Command_Ptr(0x01);//BC2
  TFT_Write_Command_Ptr(0x00);//DIV2[1:0]
  TFT_Write_Command_Ptr(0x21);//RTN2[5:0]
  TFT_Write_Command_Ptr(0x08);//BP2[7:0]
  TFT_Write_Command_Ptr(0x08);//FP2[7:0]
  Delay_100ms(); Delay_100ms();

  TFT_Set_Index_Ptr(0xC4);
  TFT_Write_Command_Ptr(0x11);
  TFT_Write_Command_Ptr(0x01);
  TFT_Write_Command_Ptr(0x43);
  TFT_Write_Command_Ptr(0x04);

  TFT_Set_Index_Ptr(0x0c8);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x16);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x08);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x06);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x0E);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0x0c9);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x16);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x08);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x06);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x0E);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0x0ca);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x16);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x08);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x06);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x0E);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0x0cB);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x16);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x08);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x06);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x0E);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x0D);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xD0);
  TFT_Write_Command_Ptr(0x63);//BT[2:0]=110
  TFT_Write_Command_Ptr(0x53);
  TFT_Write_Command_Ptr(0x82);//VC2[2:0]=010,VCI2=5V
  TFT_Write_Command_Ptr(0x33);//33,VREG=4.7V

  TFT_Set_Index_Ptr(0xD1);
  TFT_Write_Command_Ptr(0x60);//VCOMH=VREG x 0.876      69
  TFT_Write_Command_Ptr(0x69);//VDV=VREG x 1.064

  TFT_Set_Index_Ptr(0xD2);//DC10[2:0], DC12[2:0]
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x24);

  TFT_Set_Index_Ptr(0xD4);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x24);

  TFT_Set_Index_Ptr(0xE2);//NVM Load Control
  TFT_Write_Command_Ptr(0x3F);

  TFT_Set_Index_Ptr(0x35);//set_tear_on
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0x36);
  if (TFT_Disp_Rotation == 90) {
    if (Is_TFT_Rotated_180()) {
      TFT_Write_Command_Ptr(0xC0);
    } else
      TFT_Write_Command_Ptr(0x00);
  } else {
    if (Is_TFT_Rotated_180()) {
      TFT_Write_Command_Ptr(0x70);
    } else
      TFT_Write_Command_Ptr(0x21);
  }

  TFT_Set_Index_Ptr(0x3A);        //set_pixel_format
  TFT_Write_Command_Ptr(0x55);

  TFT_Set_Index_Ptr(0x2A);          //set_column_address
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr((TFT_DISP_WIDTH-1)>>8);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH-1);

  TFT_Set_Index_Ptr(0x2B);          // set_page_address:
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr((TFT_DISP_HEIGHT-1)>>8);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT-1);

  TFT_Set_Index_Ptr(0x2C);          // write_memory_start:
  TFT_Set_Index_Ptr(0x11);          // exit_sleep_mode
  Delay_100ms(); Delay_100ms();
  TFT_Set_Index_Ptr(0x29);          // set_display_on
  Delay_100ms(); Delay_100ms();

  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (R61526)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_R61526(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_DataPort_Direction = 0;
  TFT_Reset_R61526();
  TFT_Set_Address_Ptr = TFT_Set_Address_R61526;
}

/**
  * @brief  Complete initializing procedure for TFT controller (R61526)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_R61526_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_R61526();
  TFT_Set_Address_Ptr = TFT_Set_Address_R61526;
}
/*************************** eof R61526 ***************************/

/***************************** HX8347G *****************************/
/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_HX8347G(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();
  // Driving ability setting
  TFT_Set_Reg(0xEA, 0x00); // PTBA[15:8]
  TFT_Set_Reg(0xEB, 0x20); // PTBA[7:0]
  TFT_Set_Reg(0xEC, 0x0C); // STBA[15:8]
  TFT_Set_Reg(0xED, 0xC4); // STBA[7:0]
  TFT_Set_Reg(0xE8, 0x40); // OPON[7:0]
  TFT_Set_Reg(0xE9, 0x38); // OPON1[7:0]
  TFT_Set_Reg(0xF1, 0x01); // OTPS1B
  TFT_Set_Reg(0xF2, 0x10); // GEN
  TFT_Set_Reg(0x27, 0xA3);

  // Gamma 2.8 setting
  TFT_Set_Reg(0x40, 0x00); //
  TFT_Set_Reg(0x41, 0x00); //
  TFT_Set_Reg(0x42, 0x01); //
  TFT_Set_Reg(0x43, 0x13); //
  TFT_Set_Reg(0x44, 0x10); //
  TFT_Set_Reg(0x45, 0x26); //
  TFT_Set_Reg(0x46, 0x08); //
  TFT_Set_Reg(0x47, 0x51); //
  TFT_Set_Reg(0x48, 0x02); //
  TFT_Set_Reg(0x49, 0x12); //
  TFT_Set_Reg(0x4A, 0x18); //
  TFT_Set_Reg(0x4B, 0x19); //
  TFT_Set_Reg(0x4C, 0x14); //

  TFT_Set_Reg(0x50, 0x19); //
  TFT_Set_Reg(0x51, 0x2F); //
  TFT_Set_Reg(0x52, 0x2C); //
  TFT_Set_Reg(0x53, 0x3E); //
  TFT_Set_Reg(0x54, 0x3F); //
  TFT_Set_Reg(0x55, 0x3F); //
  TFT_Set_Reg(0x56, 0x2E); //
  TFT_Set_Reg(0x57, 0x77); //
  TFT_Set_Reg(0x58, 0x0B); //
  TFT_Set_Reg(0x59, 0x06); //
  TFT_Set_Reg(0x5A, 0x07); //
  TFT_Set_Reg(0x5B, 0x0D); //
  TFT_Set_Reg(0x5C, 0x1D); //
  TFT_Set_Reg(0x5D, 0xCC); //

  // Window setting
  TFT_Set_Reg(0x04, (TFT_DISP_WIDTH-1) >> 8);
  TFT_Set_Reg(0x05, TFT_DISP_WIDTH-1);
  TFT_Set_Reg(0x08, (TFT_DISP_HEIGHT-1) >> 8);
  TFT_Set_Reg(0x09, TFT_DISP_HEIGHT-1);

  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Set_Reg(0x16, 0x88); // MY=1, MX=0, MV=0, BGR=1
    else
      TFT_Set_Reg(0x16, 0x48); // MY=0, MX=1, MV=0, BGR=1
  else
    if (Is_TFT_Rotated_180())
      TFT_Set_Reg(0x16, 0xE8); // MY=1, MX=1, MV=1, BGR=1
    else
      TFT_Set_Reg(0x16, 0x28); // MY=0, MX=0, MV=1, BGR=1

  // Power Voltage Setting
  TFT_Set_Reg(0x1B, 0x1B); // VRH = 4.65
  TFT_Set_Reg(0x1A, 0x01); // BT
  TFT_Set_Reg(0x24, 0x2F); // VMH
  TFT_Set_Reg(0x25, 0x57); // VML

  // Vcom offset
  TFT_Set_Reg(0x23, 0x8D); // FLICKER ADJUST

  // Power ON Setting
  TFT_Set_Reg(0x18, 0x36); //
  TFT_Set_Reg(0x19, 0x01); //
  TFT_Set_Reg(0x01, 0x00); //
  TFT_Set_Reg(0x1F, 0x88); //
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0x80); //
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0x90); //
  Delay_5ms();
  TFT_Set_Reg(0x1F, 0xD0); //
  Delay_5ms();
  // 65K Color Selection
  TFT_Set_Reg(0x17, 0x05); //
  // Set Panel
  TFT_Set_Reg(0x36, 0x00); // Panel characteristic control register
  // Display ON Setting
  TFT_Set_Reg(0x28, 0x38); //
  Delay_10ms();
  Delay_10ms();
  Delay_10ms();
  Delay_10ms();
  TFT_Set_Reg(0x28, 0x3C); //
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8347G)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8347G(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  TFT_DataPort_Direction = 0;
  // Reset device
  TFT_Reset_HX8347G();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}
/**
  * @brief  Complete initializing procedure for TFT controller (HX8347G)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8347G_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_HX8347G();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}
/*************************** eof HX8347G ***************************/

/***************************** ILI9481 *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_ILI9481(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_ILI9481(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_CS = 0;

  TFT_Set_Index_Ptr(0x11);
  Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0xD0);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x41);
  TFT_Write_Command_Ptr(0x1D);

  TFT_Set_Index_Ptr(0xD1);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x2F);
  TFT_Write_Command_Ptr(0x1C);

  TFT_Set_Index_Ptr(0xD2);
  TFT_Write_Command_Ptr(0x01);
  TFT_Write_Command_Ptr(0x11);

  TFT_Set_Index_Ptr(0xC0);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x3B);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x02);
  TFT_Write_Command_Ptr(0x11);

  TFT_Set_Index_Ptr(0xC5);      // Frame rate
  TFT_Write_Command_Ptr(0x03);

  TFT_Set_Index_Ptr(0xC8);      // Gamma Setting
  TFT_Write_Command_Ptr(0x01);
  TFT_Write_Command_Ptr(0x47);
  TFT_Write_Command_Ptr(0x33);
  TFT_Write_Command_Ptr(0x23);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x0C);
  TFT_Write_Command_Ptr(0x44);
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x67);
  TFT_Write_Command_Ptr(0x32);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xE4);
  TFT_Write_Command_Ptr(0xA0);

  TFT_Set_Index_Ptr(0xF0);
  TFT_Write_Command_Ptr(0x08);

  TFT_Set_Index_Ptr(0xF3);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x1A);

  TFT_Set_Index_Ptr(0xF7);
  TFT_Write_Command_Ptr(0x80);

  TFT_Set_Index_Ptr(0x13); // Set Normal Mode

  TFT_Set_Index_Ptr(0x38); // Exit Idle Mode

  TFT_Set_Index_Ptr(0x36);     // memory access control
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0x09);
    else
      TFT_Write_Command_Ptr (0x0A);
  else
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0x2B);
    else
      TFT_Write_Command_Ptr (0x28);

  Delay_100ms(); Delay_10ms(); Delay_10ms();

  TFT_Set_Index_Ptr(0x29);

  // Column and page address setting
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr((TFT_DISP_WIDTH-1) >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH-1);

  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr((TFT_DISP_HEIGHT-1) >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT-1);

  TFT_Set_Index_Ptr(0x3A);  // pixel format set
  TFT_Write_Command_Ptr (0x55);
  TFT_Set_Index_Ptr(0x11);    // Set_address_mode
  Delay_100ms(); Delay_10ms(); Delay_10ms();
  Delay_10ms(); Delay_10ms(); Delay_10ms();
  TFT_Set_Index_Ptr(0x2C);
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9481)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9481(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  // Reset device
  TFT_Reset_ILI9481();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9481;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9481)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9481_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_ILI9481();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9481;
}
/*************************** eof ILI9481 ***************************/

/************************** ILI9481 (8bit) *************************/
/**
  * @brief  Complete initializing procedure for TFT controller (ILI9481)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9481_8bit(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  TFT_DataPort_Direction = 0;
  // Reset device
  TFT_Reset_ILI9481();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9481;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9481)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9481_8bit_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_ILI9481();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9481;
}
/************************ eof ILI9481 (8bit) ************************/


/***************************** ILI9342 *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_ILI9342(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_ILI9342(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_CS = 0;
  TFT_Set_Index_Ptr(0xB9);      // Set EXTC
  TFT_Write_Command_Ptr(0xFF);
  TFT_Write_Command_Ptr(0x93);
  TFT_Write_Command_Ptr(0x42);

  TFT_Set_Index_Ptr(0x36);     // memory access control
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0x60);
    else
      TFT_Write_Command_Ptr (0xA0);
  else
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0xC0);
    else
      TFT_Write_Command_Ptr (0x00);

  // Column and page address setting
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH);

  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT);

  TFT_Set_Index_Ptr(0x3A);     // pixel format set
  TFT_Write_Command_Ptr (0x55);

  TFT_Set_Index_Ptr(0xB1);     // Display Waveform Cycle 1
  TFT_Write_Command_Ptr (0x00);
  TFT_Write_Command_Ptr (0x10);

  TFT_Set_Index_Ptr(0xB6);     // Display Function Control
  TFT_Write_Command_Ptr (0x0A);
  TFT_Write_Command_Ptr (0xE2);
  TFT_Write_Command_Ptr (0x1D);

  TFT_Set_Index_Ptr(0xC0);     // Power Control 1
  TFT_Write_Command_Ptr (0x35); //28
  TFT_Write_Command_Ptr (0x0e); //0a

  TFT_Set_Index_Ptr(0xC1);     // Power Control 2
  TFT_Write_Command_Ptr (0x02);

  TFT_Set_Index_Ptr(0xC5);     // VCOM Control 1
  TFT_Write_Command_Ptr (0x31);
  TFT_Write_Command_Ptr (0x3C);

  TFT_Set_Index_Ptr(0xB8);    // Oscillator Control
  TFT_Write_Command_Ptr (0x0A);

  TFT_Set_Index_Ptr(0x26);    // Gamma Set
  TFT_Write_Command_Ptr (0x01);

  TFT_Set_Index_Ptr(0xC7);    // VCOM Control 2
  TFT_Write_Command_Ptr (0xBF);

  TFT_Set_Index_Ptr(0xE0);   // Positive Gamma Correction
  TFT_Write_Command_Ptr (0x08);
  TFT_Write_Command_Ptr (0x1C);
  TFT_Write_Command_Ptr (0x1B);
  TFT_Write_Command_Ptr (0x09);
  TFT_Write_Command_Ptr (0x0D);
  TFT_Write_Command_Ptr (0x08);
  TFT_Write_Command_Ptr (0x4B);
  TFT_Write_Command_Ptr (0xB8);
  TFT_Write_Command_Ptr (0x3B);
  TFT_Write_Command_Ptr (0x08);
  TFT_Write_Command_Ptr (0x10);
  TFT_Write_Command_Ptr (0x08);
  TFT_Write_Command_Ptr (0x20);
  TFT_Write_Command_Ptr (0x20);
  TFT_Write_Command_Ptr (0x08);

  TFT_Set_Index_Ptr(0xE1);  // Negative Gamma Correction
  TFT_Write_Command_Ptr (0x07);
  TFT_Write_Command_Ptr (0x23);
  TFT_Write_Command_Ptr (0x24);
  TFT_Write_Command_Ptr (0x06);
  TFT_Write_Command_Ptr (0x12);
  TFT_Write_Command_Ptr (0x07);
  TFT_Write_Command_Ptr (0x34);
  TFT_Write_Command_Ptr (0x47);
  TFT_Write_Command_Ptr (0x44);
  TFT_Write_Command_Ptr (0x07);
  TFT_Write_Command_Ptr (0x0F);
  TFT_Write_Command_Ptr (0x07);
  TFT_Write_Command_Ptr (0x1F);
  TFT_Write_Command_Ptr (0x1F);
  TFT_Write_Command_Ptr (0x07);

  TFT_Set_Index_Ptr(0x13); // Set Normal Mode
  TFT_Set_Index_Ptr(0x11); //Exit Sleep
  Delay_100ms();
  TFT_Set_Index_Ptr(0x11); //Exit Sleep
  Delay_100ms();
  TFT_Set_Index_Ptr(0x29);
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9342)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9342(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  // Reset device
  TFT_Reset_ILI9342();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9342;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9342)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9342_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_ILI9342();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9342;
}
/*************************** eof ILI9342 ***************************/

/***************************** ILI9340 *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_ILI9340(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(x>>8);
  TFT_Write_Command_Ptr(x);
  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(y>>8);
  TFT_Write_Command_Ptr(y);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_ILI9340(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_CS = 0;

  TFT_Set_Index_Ptr(0x11);//Exit Sleep
  Delay_10ms(); Delay_10ms(); Delay_10ms();
  Delay_10ms(); Delay_10ms(); Delay_10ms();

  TFT_Set_Index_Ptr(0xCB);TFT_Write_Command_Ptr(0x01);//AP[2:0]

  TFT_Set_Index_Ptr(0xC0);TFT_Write_Command_Ptr(0x26); //VRH[5:0]
                     TFT_Write_Command_Ptr(0x04);//VC[2:0]

  TFT_Set_Index_Ptr(0xC1);TFT_Write_Command_Ptr(0x04); //SAP[2:0];BT[3:0]


  TFT_Set_Index_Ptr(0xC5);TFT_Write_Command_Ptr(0x34); //VCM control
                     TFT_Write_Command_Ptr(0x40);
  TFT_Set_Index_Ptr(0x36);     // memory access control
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0xC8);
    else
      TFT_Write_Command_Ptr (0x08);
  else
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr (0xA8);
    else
      TFT_Write_Command_Ptr (0x68);

  TFT_Set_Index_Ptr(0xB1);TFT_Write_Command_Ptr(0x00); // Frame Rate Control
  TFT_Write_Command_Ptr(0x18);

  TFT_Set_Index_Ptr(0xB6);TFT_Write_Command_Ptr(0x0A); // Display Function Control
  TFT_Write_Command_Ptr(0xA2);

  TFT_Set_Index_Ptr(0xC7);TFT_Write_Command_Ptr(0xCD); //VCOM Control , VMF[6:0]

  TFT_Set_Index_Ptr(0xE8);
  TFT_Write_Command_Ptr(0x89);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x79);

  TFT_Set_Index_Ptr(0xF2);
  TFT_Write_Command_Ptr(0x00); // 3Gamma Function Disable

  TFT_Set_Index_Ptr(0xE0);
  TFT_Write_Command_Ptr(0x0F);
  TFT_Write_Command_Ptr(0x1D);
  TFT_Write_Command_Ptr(0x1A);
  TFT_Write_Command_Ptr(0x0B);
  TFT_Write_Command_Ptr(0x0F);
  TFT_Write_Command_Ptr(0x09);
  TFT_Write_Command_Ptr(0x47);
  TFT_Write_Command_Ptr(0xD6);
  TFT_Write_Command_Ptr(0x37);
  TFT_Write_Command_Ptr(0x0A);
  TFT_Write_Command_Ptr(0x0C);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x07);
  TFT_Write_Command_Ptr(0x05);
  TFT_Write_Command_Ptr(0x00);

  TFT_Set_Index_Ptr(0xE1);
  TFT_Write_Command_Ptr(0x00);
  TFT_Write_Command_Ptr(0x22);
  TFT_Write_Command_Ptr(0x25);
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0x10);
  TFT_Write_Command_Ptr(0x06);
  TFT_Write_Command_Ptr(0x38);
  TFT_Write_Command_Ptr(0x7F);
  TFT_Write_Command_Ptr(0x48);
  TFT_Write_Command_Ptr(0x05);
  TFT_Write_Command_Ptr(0x13);
  TFT_Write_Command_Ptr(0x38);
  TFT_Write_Command_Ptr(0x38);
  TFT_Write_Command_Ptr(0x3A);
  TFT_Write_Command_Ptr(0x0F);

  TFT_Set_Index_Ptr(0x29);
  TFT_Set_Index_Ptr(0x3A);
  TFT_Write_Command_Ptr(0x05);

  // End row and column values have to be fixed disregarding the display orientation
  // 0�, 90�, 180�, 270� change values here if display with different dimensions is used
  TFT_Set_Index_Ptr(0x2A);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr((TFT_DISP_WIDTH - 1) >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_WIDTH-1);

  TFT_Set_Index_Ptr(0x2B);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr(0);
  TFT_Write_Command_Ptr((TFT_DISP_HEIGHT - 1) >> 8);
  TFT_Write_Command_Ptr(TFT_DISP_HEIGHT-1);

  TFT_Set_Index_Ptr(0x2C);
  Delay_10ms();
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9340)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9340(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  // Reset device
  TFT_Reset_ILI9340();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9340;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9340)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9340_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_ILI9340();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9340;
}
/*************************** eof ILI9340 ***************************/

/***************************** ILI9340 (8bit) *****************************/
/**
  * @brief  Complete initializing procedure for TFT controller (ILI9340)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9340_8bit(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  TFT_DataPort_Direction = 0;
  // Reset device
  TFT_Reset_ILI9340();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9340;
}

/**
  * @brief  Complete initializing procedure for TFT controller (ILI9340)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_ILI9340_8bit_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_ILI9340();
  TFT_Set_Address_Ptr = TFT_Set_Address_ILI9340;
}
/*************************** eof ILI9340 ***************************/

/***************************** SSD1963 *****************************/
/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  * @param  x1 : X coordinate start. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y1 : Y coordinate start. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  * @param  x2 : X coordinate start. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y2 : Y coordinate start. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_SSD1963II(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
unsigned int s_col, e_col, s_page, e_page, _width, _height;
  // procedure optimized for two types of SSD1963 displays tested
  // 800x480 and 480x272
  // If different displays are used add checking here
  if ((TFT_DISP_WIDTH > 480) || (TFT_DISP_HEIGHT > 480)) {
    _width = 800;
    _height = 480;
  }
  else {
    _width = 480;
    _height = 272;
  }
  if (TFT_Disp_Rotation == 90) {
    if (Is_TFT_Rotated_180()) {
      s_col = (_width - 1) - y2;
      e_col = (_width - 1) - y1;
      s_page = x1;
      e_page = x2;
    } else {
      s_col = y1;
      e_col = y2;
      s_page = (_height - 1) - x2;
      e_page = (_height - 1) - x1;
    }
  } else {
    if (Is_TFT_Rotated_180()) {
      s_col = (_width - 1) - x2;
      e_col = (_width - 1) - x1;
      s_page = (_height - 1) - y2;
      e_page = (_height - 1) - y1;
    } else {
      s_col = x1;
      e_col = x2;
      s_page = y1;
      e_page = y2;
    }
  }
  TFT_Set_Index_Ptr(0x2a);             // SET column address
  TFT_Write_Command_Ptr((char)(s_col >> 8));
  TFT_Write_Command_Ptr(s_col);
  TFT_Write_Command_Ptr((char)(e_col >> 8));
  TFT_Write_Command_Ptr(e_col);

  TFT_Set_Index_Ptr(0x2b);             // SET page address
  TFT_Write_Command_Ptr((char)(s_page >> 8));
  TFT_Write_Command_Ptr(s_page);
  TFT_Write_Command_Ptr((char)(e_page >> 8));
  TFT_Write_Command_Ptr(e_page);
  TFT_Set_Index_Ptr(0x2C);
}

/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  *         Kept for backward compatibility. Use TFT_Set_Address_SSD1963II instead.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_SSD1963(unsigned int x, unsigned int y) {
  TFT_Set_Index_Ptr(0x2a);           // SET column address
  TFT_Write_Command_Ptr((x)>>8);     // SET start column address=0
  TFT_Write_Command_Ptr(x);
  if(TFT_Disp_Rotation == 90){
    TFT_Write_Command_Ptr(479>>8);   // SET end column address=271 (479)
    TFT_Write_Command_Ptr(479);
  }
  else{
    TFT_Write_Command_Ptr(799>>8);   // SET end column address=479 (799)
    TFT_Write_Command_Ptr(799);
  }
  TFT_Set_Index_Ptr(0x2b);           // SET page address
  TFT_Write_Command_Ptr((y)>>8);     // SET start page address=0
  TFT_Write_Command_Ptr(y);
  if(TFT_Disp_Rotation == 90){
    TFT_Write_Command_Ptr(799>>8);   // SET end page address=479
    TFT_Write_Command_Ptr(799);
  }
  else {
    TFT_Write_Command_Ptr(479>>8);   // SET end page address=271
    TFT_Write_Command_Ptr(479);
  }
  TFT_Set_Index_Ptr(0x2c);
}

/**
  * @brief  Inverts colors displayed on TFT (SSD1963).
  * @param  None.
  */
void TFT_Set_Invert_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x21);
  TFT_CS = 1;
}

/**
  * @brief  Set colors displayed on TFT to Normal (SSD1963)
  * @param  None.
  */
void TFT_Exit_Invert_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x20);
  TFT_CS = 1;
}

/**
  * @brief  Turn OFF display (SSD1963)
  * @param  None.
  */
void TFT_Set_Display_OFF_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x28);
  TFT_CS = 1;
}

/**
  * @brief  Turn ON display (SSD1963)
  * @param  None.
  */
void TFT_Set_Display_ON_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x29);
  TFT_CS = 1;
}

/**
  * @brief  Enter sleep mode (SSD1963)
  * @param  None.
  */
void TFT_Enter_Sleep_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x10);
  TFT_CS = 1;
  // The host processor must wait 5ms after sending this 
  // command before sending another command.
  Delay_5ms();
}

/**
  * @brief  Exit sleep mode (SSD1963)
  * @param  None.
  */
void TFT_Exit_Sleep_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x11);
  TFT_CS = 1;
}

/**
  * @brief  Enter normal mode (SSD1963)
  * @param  None.
  */
void TFT_Enter_Normal_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x13);
  TFT_CS = 1;
}

/**
  * @brief  Enter Idle mode (SSD1963)
  * @param  None.
  */
void TFT_Enter_Idle_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x39);
  TFT_CS = 1;
}

/**
  * @brief  Exit Idle mode (SSD1963)
  * @param  None.
  */
void TFT_Exit_Idle_Mode_SSD1963(){
  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x38);
  TFT_CS = 1;
}

//void TFT_Set_DBC_SSD1963(char value) {
//  TFT_CS = 0;
//  TFT_Set_Index_Ptr(0xBE);                //set PWM for B/L
//  TFT_Write_Command_Ptr(0x06);
//  TFT_Write_Command_Ptr(value);
//  TFT_Write_Command_Ptr(0x01);
//  TFT_Write_Command_Ptr(0xf0);
//  TFT_Write_Command_Ptr(0x00);
//  TFT_Write_Command_Ptr(0x00);
//
////  TFT_Set_Index_Ptr(0xBE);
////  TFT_Write_Command_Ptr(value); // (set PWM frequency)
////  TFT_Write_Command_Ptr(0xFF); // (dummy value if DBC is used)
////  TFT_Write_Command_Ptr(0x09); // (enable PWM controlled by DBC)
////  TFT_Write_Command_Ptr(0xFF);
////  TFT_Write_Command_Ptr(0x00);
////  TFT_Write_Command_Ptr(0x00);
////WRITE COMMAND �0xD4� (Define the threshold value)
////WRITE DATA �..
//  TFT_Set_Index_Ptr(0xD4);
//  // Conservative mode
//  TFT_Write_Command_Ptr(0x00);
//  TFT_Write_Command_Ptr(0x09);
//  TFT_Write_Command_Ptr(0x90);
//  // Normal mode
//  TFT_Write_Command_Ptr(0x00);
//  TFT_Write_Command_Ptr(0x17);
//  TFT_Write_Command_Ptr(0xE8);
//  // Normal mode
//  TFT_Write_Command_Ptr(0x00);
//  TFT_Write_Command_Ptr(0x39);
//  TFT_Write_Command_Ptr(0x60);
//  TFT_Set_Index_Ptr(0xD0);
//  TFT_Write_Command_Ptr(0x4D); // (Enable DBC with Aggressive mode)
//  TFT_CS = 1;
//}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
// Begin Here!
static void TFT_Reset_SSD1963(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_CS = 0;
  TFT_Set_Index_Ptr(0x01);     //Software Reset
  //TFT_Set_Index_Ptr(0x01);
  //TFT_Set_Index_Ptr(0x01);
  delay_ms(10);

  TFT_Set_Index_Ptr(0xE2);
  TFT_Write_Command_Ptr(0x20);    // PLLclk = REFclk * 36 (360MHz)
  TFT_Write_Command_Ptr(0x02);     // SYSclk = PLLclk / 3  (120MHz)
  TFT_Write_Command_Ptr(0x54);  // validate M and N
  delay_us(10);
  // SSD1963 needed to switch to PLL output as system clock after PLL is locked. The following is the program sequence.
  // WRITE COMMAND �0xE0�
  // WRITE DATA �0x01�
  TFT_Set_Index_Ptr(0xe0);
  TFT_Write_Command_Ptr(0x01); // START PLL
  // Wait 100us to let the PLL stable
  Delay_50us(); Delay_50us();
  // WRITE DATA �0x03�
  TFT_Set_Index_Ptr(0xe0);
  TFT_Write_Command_Ptr(0x03); // LOCK PLL
  delay_us(10);
  
  TFT_Set_Index_Ptr(0xb0);          //SET LCD MODE  SET TFT 18Bits MODE
  TFT_Write_Command_Ptr(0x08);      //SET TFT MODE & hsync+Vsync+DEN MODE
  TFT_Write_Command_Ptr(0x00);      //SET TFT MODE & hsync+Vsync+DEN MODE
  TFT_Write_Command_Ptr(0x03);      //SET horizontal size=480-1 HightByte
  TFT_Write_Command_Ptr(0x1F);      //SET horizontal size=480-1 LowByte
  TFT_Write_Command_Ptr(0x01);      //SET vertical size=272-1 HightByte
  TFT_Write_Command_Ptr(0xDF);      //SET vertical size=272-1 LowByte
  TFT_Write_Command_Ptr(0x00);      //SET even/odd line RGB seq.=RGB

  TFT_Set_Index_Ptr(0xf0);
  TFT_Write_Command_Ptr(0x03);      //3 -16bit,0 -8bit        //SET pixel data I/F format=8bit
  //TFT_Set_Index_Ptr(0x3A);
  //TFT_Write_Command_Ptr(0x60);      // SET R G B format = 6 6 6
  // Set LSHIFT freq (DCLK from PLL)
  //  Typical DCLK for TM070RBH10 is 29.23MHz
  //  29.23 = 120 * (LCDC_FPR + 1) / 2^20
  //  LCD_FPR = 255415 (0x3E5B7)
  TFT_Set_Index_Ptr(0xe6);          //SET PCLK freq=10MHz  ; pixel clock frequency
  TFT_Write_Command_Ptr(0x04);
  TFT_Write_Command_Ptr(0xD7);
  TFT_Write_Command_Ptr(0xF9);

  TFT_Set_Index_Ptr(0xb4);          //SET HBP,
  TFT_Write_Command_Ptr(0x03);      //SET HSYNC Tatol 525
  TFT_Write_Command_Ptr(0x52);
  TFT_Write_Command_Ptr(0x00);      //SET HBP 43
  TFT_Write_Command_Ptr(0x31);
  TFT_Write_Command_Ptr(0x2E);      //SET VBP 41=40+1
  TFT_Write_Command_Ptr(0x00);      //SET Hsync pulse start position
  TFT_Write_Command_Ptr(0x03);
  TFT_Write_Command_Ptr(0x00);      //SET Hsync pulse subpixel start position

  TFT_Set_Index_Ptr(0xb6);          //SET VBP,
  TFT_Write_Command_Ptr(0x02);      //SET Vsync total 286=285+1
  TFT_Write_Command_Ptr(0x12);
  TFT_Write_Command_Ptr(0x00);      //SET VBP=12
  TFT_Write_Command_Ptr(0x33);
  TFT_Write_Command_Ptr(0x03);      //SET Vsync pulse 10=9+1
  TFT_Write_Command_Ptr(0x00);      //SET Vsync pulse start position
  TFT_Write_Command_Ptr(0x1C);

  TFT_Set_Index_Ptr(0x36);     // memory access control
  TFT_Write_Command_Ptr(0x01);
  /*
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr(0x60);
    else
      TFT_Write_Command_Ptr(0xA0);
  else
    if (Is_TFT_Rotated_180())
      TFT_Write_Command_Ptr(0xC0);
    else
      TFT_Write_Command_Ptr(0x00);
  */
  //Set_BackLight(0x80);
  TFT_Set_Index_Ptr(0x29);                //SET display on
  TFT_CS = 1;
}

/**
  * @brief  Complete initializing procedure for TFT controller (SSD1963)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SSD1963(unsigned int display_width, unsigned int display_height) {
  TFT_Set_SSD1963();
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  // Reset device
  TFT_Reset_SSD1963();
  TFT_SSD1963_Set_Address_Ptr = TFT_Set_Address_SSD1963II;
}

/**
  * @brief  Complete initializing procedure for TFT controller (SSD1963)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SSD1963_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_SSD1963();
  TFT_SSD1963_Set_Address_Ptr = TFT_Set_Address_SSD1963II;
}
/*************************** eof SSD1963 ***************************/

/***************************** SSD1963 8bit *****************************/
/**
  * @brief  Sets controllers register address to be written in next cycle.
  * @param  index : The values can be in [0x00 - 0xFF] range
  */
void TFT_SSD1963_8bit_Set_Index(unsigned short index) {
  Delay_1us(); Delay_1us();
  TFT_CS = 0;
  TFT_RD = 1;
  TFT_RS = 0;
  TFT_DataPort = index;
  TFT_WR = 0;
  TFT_WR = 1;
  TFT_CS = 1;
}

/**
  * @brief  Sets value of controller register (address must be previously defined with TFT_Set_Index)
  * @param  cmd : The values can be in [0x00 - 0xFF] range
  */
void TFT_SSD1963YT_8bit_Write_Command(unsigned char command) {
  TFT_CS = 0;
  TFT_RD = 1;
  TFT_RS = 1;
  TFT_DataPort = command;
  TFT_WR = 0;
  TFT_WR = 1;
  TFT_CS = 1;
  Delay_1us(); Delay_1us();
}

void TFT_SSD1963_8bit_Write_Data(unsigned int color) {
volatile unsigned short temp;
  temp = (color>>11);
  temp = (temp<<3);
  if ((temp>>7) == 1) {
    temp += 7;
  }
  TFT_SSD1963YT_8bit_Write_Command(temp);
  temp = (color>>5);
  temp = (temp<<2);
  if ((temp>>7) == 1) {
    temp += 3;
  }
  TFT_SSD1963YT_8bit_Write_Command(temp);
  temp = (color<<3);
  if ((temp>>7) == 1) {
    temp += 7;
  }
  TFT_SSD1963YT_8bit_Write_Command(temp);
}

/**
  * @brief  Sets address and value of controller register
  * @param  index : Address of controller register. Values can be in [0x00 - 0xFF] range
  * @param  value : Value of controller register. Values can be in [0x00 - 0xFF] range
  */
void TFT_SSD1963_8bit_Set_Reg(unsigned char index, unsigned char value) {
  TFT_SSD1963_8bit_Set_Index(index);
  TFT_SSD1963YT_8bit_Write_Command(value);
}

/**
  * @brief  Move cursor to designated coordinates, in preparation for drawing on display.
  *         Kept for backward compatibility. Use TFT_Set_Address_SSD1963II instead.
  * @param  x : X coordinate. Range shoud be [0 - TFT_DISP_WIDTH-1]
  * @param  y : Y coordinate. Range shoud be [0 - TFT_DISP_HEIGHT-1]
  */
static void TFT_Set_Address_SSD1963_8bit(unsigned int x, unsigned int y) {
  TFT_SSD1963_8bit_Set_Index(0x2A);           // SET column address
  TFT_SSD1963YT_8bit_Write_Command((x)>>8);   // SET start page address=0
  TFT_SSD1963YT_8bit_Write_Command(x);
  TFT_SSD1963YT_8bit_Write_Command(0x03);     // SET start column address=0
  TFT_SSD1963YT_8bit_Write_Command(0x1F);

  TFT_SSD1963_8bit_Set_Index(0x2B);           // SET page address
  TFT_SSD1963YT_8bit_Write_Command((y)>>8);   // SET start column address=0
  TFT_SSD1963YT_8bit_Write_Command(y);
  TFT_SSD1963YT_8bit_Write_Command(0x01);     // SET start page address=0
  TFT_SSD1963YT_8bit_Write_Command(0xDF);

  TFT_SSD1963_8bit_Set_Index(0x2C);
}

/**
  * @brief  Configures sets of TFT controllers register to work with parameters
  *         assigneg in TFT_Init. Also it resets the TFT controller.
  */
static void TFT_Reset_SSD1963_8bit(){
  // Hold in reset
  TFT_RST = 0;
  // Set reset pin as output
  TFT_RST_Direction = 0;

  // Enable data access
  TFT_RS = 1;
  // Set RS pin as output
  TFT_RS_Direction = 0;

  // Disable LCD
  TFT_CS = 1;
  // Set LCD CS pin as output
  TFT_CS_Direction = 0;

  TFT_RD_Direction = 0;
  TFT_WR_Direction = 0;
  TFT_RD = 1;
  TFT_WR = 1;

  // Release from reset
  Delay_100ms();
  TFT_RST = 1;
  Delay_100ms();
  Delay_100ms();

  TFT_SSD1963_8bit_Set_Index(0x01);     //Software Reset
  TFT_SSD1963_8bit_Set_Index(0x01);
  TFT_SSD1963_8bit_Set_Index(0x01);
  TFT_SSD1963_8bit_Set_Index(0xe0); //START PLL
  TFT_SSD1963YT_8bit_Write_Command(0x01);
  // Wait 100us to let the PLL stable
  Delay_50us(); Delay_50us();
  TFT_SSD1963_8bit_Set_Index(0xe0);//LOCK PLL
  TFT_SSD1963YT_8bit_Write_Command(0x03);
  TFT_SSD1963_8bit_Set_Index(0xb0); //SET LCD MODE  SET TFT 18Bits MODE
  TFT_SSD1963YT_8bit_Write_Command(0x08); //SET TFT MODE & hsync+Vsync+DEN
  //MODE
  TFT_SSD1963YT_8bit_Write_Command(0x80); //SET TFT MODE & hsync+Vsync+DEN
  //MODE
  TFT_SSD1963YT_8bit_Write_Command(0x03); //SET horizontal size
  //HightByte
  TFT_SSD1963YT_8bit_Write_Command(0x1f);        //SET horizontal size
  //LowByte
  TFT_SSD1963YT_8bit_Write_Command(0x01); //SET vertical size
  //HightByte
  TFT_SSD1963YT_8bit_Write_Command(0xdf); //SET vertical size LowByte
  TFT_SSD1963YT_8bit_Write_Command(0x2d); //SET even/odd line RGB seq.=RGB
  TFT_SSD1963_8bit_Set_Index(0xF0);
  TFT_SSD1963YT_8bit_Write_Command(0x00);
  //SET pixel data I/F format=8bit
  TFT_SSD1963_8bit_Set_Index(0x3A);
  TFT_SSD1963YT_8bit_Write_Command(0x60);

  TFT_SSD1963_8bit_Set_Index(0xe6);    //SET PCLK freq=33.26MHz
  TFT_SSD1963YT_8bit_Write_Command(0x02);
  TFT_SSD1963YT_8bit_Write_Command(0xef);
  TFT_SSD1963YT_8bit_Write_Command(0xff);

  TFT_SSD1963_8bit_Set_Index(0xb4); //SET HBP, HFP
  TFT_SSD1963YT_8bit_Write_Command(0x20); //SET HSYNC
  TFT_SSD1963YT_8bit_Write_Command(0xaf);
  TFT_SSD1963YT_8bit_Write_Command(0x00); //SET HBP
  TFT_SSD1963YT_8bit_Write_Command(0xa3);
  TFT_SSD1963YT_8bit_Write_Command(0x07); //SET VBP
  TFT_SSD1963YT_8bit_Write_Command(0x00); //SET Hsync pulse start position
  TFT_SSD1963YT_8bit_Write_Command(0x00);
  TFT_SSD1963YT_8bit_Write_Command(0x00); //SET Hsync pulse subpixel start
  //position
  TFT_SSD1963_8bit_Set_Index(0xb6);   //SET VBP, VFP
  TFT_SSD1963YT_8bit_Write_Command(0x01); //SET Vsync
  TFT_SSD1963YT_8bit_Write_Command(0xef);
  TFT_SSD1963YT_8bit_Write_Command(0x00); //SET VBP
  TFT_SSD1963YT_8bit_Write_Command(0x04);
  TFT_SSD1963YT_8bit_Write_Command(0x01); //SET Vsync pulse
  TFT_SSD1963YT_8bit_Write_Command(0x00); //SET Vsync pulse start position
  TFT_SSD1963YT_8bit_Write_Command(0x00);

  TFT_SSD1963_8bit_Set_Index(0x36);     // memory access control
  if (TFT_Disp_Rotation == 90)
    if (Is_TFT_Rotated_180())
      TFT_SSD1963YT_8bit_Write_Command(0x60);
    else
      TFT_SSD1963YT_8bit_Write_Command(0xA0);
  else
    if (Is_TFT_Rotated_180())
      TFT_SSD1963YT_8bit_Write_Command(0xC0);
    else
      TFT_SSD1963YT_8bit_Write_Command(0x00);

  TFT_SSD1963_8bit_Set_Index(0x29); //SET display on
}

/**
  * @brief  Complete initializing procedure for TFT controller (SSD1963 8bit)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SSD1963_8bit(unsigned int display_width, unsigned int display_height) {
  TFT_Set_SSD1963();
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_SSD1963_8bit_Set_Index;
    TFT_Write_Command_Ptr = TFT_SSD1963YT_8bit_Write_Command;
    TFT_Write_Data_Ptr = TFT_SSD1963_8bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  TFT_DataPort_Direction = 0;
  // Reset device
  TFT_Reset_SSD1963_8bit();
  TFT_SSD1963_Set_Address_Ptr = TFT_Set_Address_SSD1963II;
}

/**
  * @brief  Complete initializing procedure for TFT controller (SSD1963 8bit)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_SSD1963_8bit_Custom(unsigned int display_width, unsigned int display_height) {
  TFT_Set_SSD1963();
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_SSD1963_8bit_Set_Index;
    TFT_Write_Command_Ptr = TFT_SSD1963YT_8bit_Write_Command;
    TFT_Write_Data_Ptr = TFT_SSD1963_8bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_SSD1963_8bit();
  TFT_SSD1963_Set_Address_Ptr = TFT_Set_Address_SSD1963II;
}
/***************************** eof SSD1963 8bit *****************************/


/****************************** HX8347G 16bit ******************************/
/**
  * @brief  Complete initializing procedure for TFT controller (HX8347G 16bit)
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8347G_16bit(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  *((unsigned int*)&TFT_DataPort_Direction) = 0;
  TFT_Reset_HX8347G();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}

/**
  * @brief  Complete initializing procedure for TFT controller (HX8347G 16bit)
  * without setting TFT_DataPort direction.
  * This procedure should be used when calls to custom TFT routines are used.
  * Must be used with TFT_Set_Active.
  * @param  display_width  : Width of display (in pixels).
  * @param  display_height : Height of display (in pixels).
  */
void TFT_Init_HX8347G_16bit_Custom(unsigned int display_width, unsigned int display_height) {
  if (Is_TFT_Set() != 1) {
    TFT_Set_Index_Ptr = TFT_Set_Index;
    TFT_Write_Command_Ptr = TFT_Write_Command;
    TFT_Write_Data_Ptr = TFT_16bit_Write_Data;
  }

  TFT_DISP_WIDTH = display_width;
  TFT_DISP_HEIGHT = display_height;
  if (display_width >= display_height)
    TFT_Disp_Rotation = 0;
  else
    TFT_Disp_Rotation = 90;

  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Set_Brush(0, 0, 0, 0, 0, 0);

  TFT_Move_Cursor(0, 0);
  ExternalFontSet = 0;
  // Reset device
  TFT_Reset_HX8347G();
  TFT_Set_Address_Ptr = TFT_Set_Address;
}
/****************************** eof HX8347G 16bit ******************************/