#ifndef INC_Lcd_H_
#define INC_Lcd_H_
#include "std_types.h"

typedef enum {
	LCD_SHIFT_RIGHT,
	LCD_SHIFT_LEFT 
}LCD_ShiftDirectionType;

typedef enum {
	LCD_DISPLAY_OFF,
	LCD_DISPLAY_ON
}LCD_DisplayType;

typedef enum {
	LCD_CURSOR_OFF,
	LCD_CURSOR_ON_BLINK,
	LCD_CURSOR_ON_NOT_BLINK
}LCD_CursorType;

typedef enum {
	LCD_ONELINE,
	LCD_TWOLINES
}LCD_LinesType;

typedef enum {
	LCD_FONT_5x7,
	LCD_FONT_5x10
}LCD_FontType;

/*Display(on/off) ,Cursor(off/on_blink/on_Notblink),lines(one/two),Font(5x7/5x10)*/
typedef struct {
	LCD_DisplayType Display_Status;
	LCD_CursorType Cursor_Status;
	LCD_LinesType Lines_Status;
	LCD_FontType Font_Status;
}LCD_DisplayControlType;

/*Function definition:LCD initialization*/
/*input:void,output:void*/
void LCD_Init();

/*Function definition:LCD Send command Sends command to the LCD screen
	input:Command
	output:void*/
void LCD_SendCommand(u8 command); //done

/*Function definition:LCD display character to the screen
	input:Character_to_send
	output:void*/
void LCD_DisplayChar(u8 Char); //done

/*Function definition:LCD display string to the screen
	input:string_to_send
	output:void*/
void LCD_DisplayString(const u8* String); //done


/*Function definition:LCD display number to the screen
	input:Number_to_send
	output:void*/
void LCD_DisplayNumber(s32 data); //done

/*Function definition:LCD clear display
	input:void
	output:void*/
void LCD_ClearDisplay(); //done


/*Function definition:Move cursor position
	input:row(0..16) + column(1..2)
	output:void*/
void LCD_SetCursorPostion(u8 X, u8 Y);//done


/*Function definition:Clears the LCD Display
	input:Direction(LCD_SHIFT_RIGHT/LCD_SHIFT_LEFT) + Number_Of_Shifted_lines
	output:void*/
void LCD_ShiftDisplay(LCD_ShiftDirectionType Direction, u8 NumberOfShiftedLines);//NotWorking


/*Function definition:Returns Cursor to base postion
	input:svoid
	output:void*/
void LCD_ReturnHome();//done


/*Function definition:Controls diplay's Font + Lines + Cursor + On/Off
	input:LCD_Display_Controls
	output:void*/
void LCD_DisplayControl(LCD_DisplayControlType controls);//done


/*Function definition:Read the character at the current cursor location
	input:void
	output:void*/
//u8 LCD_ReadCharacter();
void LCD_SaveSpecialCharacter(u8* data, u8 location);
void LCD_Print(const u8* str, ...);

/*100=full contrast*/
void LCD_ContrastChange(u8 lvl);

void LCD_DisplayFloatNumber(double var_floatNum_f32);

#endif
