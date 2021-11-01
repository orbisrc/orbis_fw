/*
 * stgui.h
 *
 *	Copyright 2019 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#ifndef STGUI_STGUI_H_
#define STGUI_STGUI_H_

#include "gui/fonts/font.h"
#include "stm32f4xx_hal.h"
#include "gui/stdispdriver.h"
#include "gui/guiconfig.h"
#include "stconfig.h"

/***********************************************
 *  Defines
 **********************************************/

#define MAX_GUI_DEBUG_BUF				255

/***********************************************
 *  Enum
 **********************************************/

typedef enum {
	Left = 1,
	Centre,
	Right,
	TopLeft,
	TopCentre,
	TopRight,
	CntrLeft,
	CntrCentre,
	CntrRight,
	BottomLeft,
	BottomCentre,
	BottomRight
} STalignmentTypeDef;

typedef enum {
	No, Yes
} STconfigStatusTypeDef;

typedef enum {
	Horizontal,
	Vertical
} STorientationTypeDef;

typedef enum {
	Hide, Drawing, Redrawing, Update, Idle
} STstate;

/***********************************************
 *  TypeDef
 **********************************************/

/*
 * STApplicationTypeDef
 */
typedef struct {
	uint16_t SetScreen;
	uint16_t CurrentScreen;
	uint16_t PrevScreen;
	uint16_t ScreenCounter;
	uint16_t WidthPx;
	uint16_t HeightPx;
	uint16_t BuffBusy;
	uint16_t BuffFull;
	uint16_t ButtonLeft;
	uint16_t ButtonRight;
	uint16_t ButtonUp;
	uint16_t ButtonDown;
	uint16_t ButtonEnter;
	uint16_t ButtonBack;
	uint16_t ButtonLeftLong;
	uint16_t ButtonRightLong;
	uint16_t ButtonUpLong;
	uint16_t ButtonDownLong;
	uint16_t ButtonEnterLong;
	uint16_t ButtonBackLong;
	uint16_t EncoderPeriod;
	uint16_t EncoderCounter;
	uint16_t EncoderDivider;
	uint16_t preEncoder;
	uint16_t Beep;
	uint16_t LongBeep;
	char InputBuff[TEXT_BUFF_SIZE];
	uint16_t Acknowledgment;
	uint16_t BeeperEnable;
} STApplicationTypeDef;

/*
 * STScreenStyleTypeDef
 */
typedef struct {
	uint16_t BackgroundColor;
	uint16_t HeaderColor;
	uint16_t HeaderTextColor;
	uint16_t HeaderHeight;
	uint16_t BorderThickness;
	uint16_t BorderColor;
	const FONT_INFO *HeaderFont;
} STScreenStyleTypeDef;

/*
 * STScreenTypeDef
 */
typedef struct {
	STApplicationTypeDef *App;
	const STScreenStyleTypeDef *Style;
	const char *HeaderText;
	const char *FooterText;
	uint16_t ScreenID;
	uint16_t FullScreen;					// 1 - Enable, 0 - Disable
	uint16_t Width;
	uint16_t Height;
	uint16_t Position;
	uint8_t Border;
	uint16_t ItemCounter;
	uint16_t ItemDynamicCounter;
	uint16_t EncoderBusy;
	uint16_t preEncoderBusy;
	uint8_t State;
	uint8_t OnShow;
	uint8_t Header;							// 1 - Enable, 0 - Disable
	uint8_t Footer;							// 1 - Enable, 0 - Disable
	uint8_t  ItemSelected;					// 1 - Selected
} STScreenTypeDef;

/*
 * STkeyboard
 */
typedef struct {
	STApplicationTypeDef *App;
	const STScreenStyleTypeDef *Style;
	uint16_t ScreenID;
	uint16_t Width;
	uint16_t Height;
	uint16_t ItemCounter;
	uint8_t ItemSelected;
	uint8_t State;
	uint8_t OnShow;
	uint8_t Border;
} STkeyboardTypeDef;

/*
 * STColorStyleTypeDef
 */
typedef struct {
	const FONT_INFO *Font;
	STScreenTypeDef *ParentScreen;
	uint16_t Color;
	uint16_t Color2;
	uint16_t TextColor;
	uint16_t BackgroundColor;
	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t FocusColor;
	uint16_t FocusColor2;
	uint16_t FocusTextColor;
	uint16_t FocusBackgroundColor;
	uint16_t FocusBorderColor;
	uint16_t FocusBorderThickness;
} STColorStyleTypeDef;

typedef struct {
	const FONT_INFO *Font;
	STScreenTypeDef *ParentScreen;
	uint16_t Width;
	uint16_t Height;
	uint16_t Color;
	uint16_t Color2;
	uint16_t TextColor;
	uint16_t BackgroundColor;
	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t FocusColor;
	uint16_t FocusColor2;
	uint16_t FocusTextColor;
	uint16_t FocusBackgroundColor;
	uint16_t FocusBorderColor;
	uint16_t FocusBorderThickness;
} STkeyCharColorStyleTypeDef;

/*
 * STkeyCharTypeDef
 */
typedef struct {
	const STkeyCharColorStyleTypeDef *Style;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint8_t InFocus;
	uint8_t State;
	char Char;
} STkeyCharTypeDef;

/*
 * STprogressBarTypeDef
 */
typedef struct {
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t State;
	uint16_t HiLimit;
	uint16_t LoLimit;
	uint16_t Value;
	uint16_t preValue;
} STprogressBarTypeDef;

/*
 * STchannelBarTypeDef
 */
typedef struct {
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
//	uint16_t					ScaleMarksNumber;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t HiLimit;
	uint16_t LowLimit;
	uint16_t Value;
	uint16_t preValue;
	uint8_t State;
} STchannelBarTypeDef;

/*
 * STtrimBarTypeDef
 */
typedef struct {
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	int16_t HiLimit;
	int16_t LoLimit;
	int16_t Value;
	int16_t preValue;
	uint16_t ItemID;
	uint8_t ValueChange;
	uint8_t Dynamic;
	uint8_t InFocus;
	uint8_t State;
	uint8_t Orientation;						/*
												*	0 - Horizontal
												*	1 - Vertical
												*/
} STtrimBarTypeDef;

/*
 * STtextBoxTypeDef
 */
typedef struct {
//	const 						FONT_INFO	*Font;
	char TextBuff[TEXT_BUFF_SIZE];
	const char *Text;
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint8_t Border;
	uint8_t TextAlignment;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t preState;
} STtextBoxTypeDef;

/*
 * STlistBoxTypeDef
 */
typedef struct {
	char List[LISTBOX_ITEMS][LISTBOX_BUFF_SIZE];
	const STColorStyleTypeDef *Style;
	uint32_t preTick;
	int16_t Value;
	int16_t preValue;
	int16_t HiLimit;
	int16_t LoLimit;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint8_t Border;
	uint8_t TextAlignment;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t preState;
	uint8_t InFocus;
	uint8_t BlinkState;
	uint8_t ValueChange;
	uint8_t Selected;
} STlistBoxTypeDef;

/*
 * STlistBoxTypeDef
 */
typedef struct {
//	const 						FONT_INFO	*Font;
	const char *Text;
	const char *Units;
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint32_t preTick;
	int16_t Value;
	int16_t preValue;
	int16_t HiLimit;
	int16_t LoLimit;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint16_t DotPosition;
	uint8_t Border;
	uint8_t BlinkState;
	uint8_t ValueChange;
	uint8_t Selected;
	uint8_t InFocus;
	uint8_t UnitsShow;
	uint8_t AddDot;
	uint8_t TextAlignment;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t preState;
	char TextBuff[INPUT_BUFF_SIZE];
} STinputBoxTypeDef;

/*
 * STbuttonTypeDef
 */
typedef struct {
//	const FONT_INFO				*Font;
	const char *Text;
//	char						ButtonLabel[32];
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint8_t Border;
	uint8_t InFocus;
	uint8_t State;
} STbuttonTypeDef;

/*
 * STcheckBoxTypeDef
 */
typedef struct {
//	const FONT_INFO				*Font;
	const char *OnText;
	const char *OffText;
//	STScreenTypeDef				*ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Value;
	uint16_t ValueChange;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint8_t Border;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t ItemID;
	uint8_t InFocus;
} STcheckBoxTypeDef;

/*
 * STgraphPlotTypeDef
 */
typedef struct {
	const FONT_INFO *Font;
	const STScreenTypeDef *ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Value;
	uint16_t preValue;
	uint16_t HiLimitX;
	uint16_t LoLimitX;
	uint16_t HiLimitY;
	uint16_t LoLimitY;
//	uint16_t					ValueChange;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint8_t Crosshairs;
	uint8_t Border;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t InFocus;
	uint8_t Clear;
} STgraphPlotTypeDef;

/*
 * STcontainerTypeDef
 */
typedef struct {
	const FONT_INFO *Font;
	const char *Text;
	const STScreenTypeDef *ParentScreen;
	const STColorStyleTypeDef *Style;
	uint16_t Width;
	uint16_t Height;
	uint16_t X;
	uint16_t Y;
	uint16_t ItemID;
	uint8_t Border;
	uint8_t Dynamic;
	uint8_t State;
	uint8_t InFocus;
} STcontainerTypeDef;
/******************************************************
 * 	Application
 *****************************************************/
uint16_t STappGetCurrentScreen(STApplicationTypeDef *STapp);

uint16_t STappGetPrevScreen(STApplicationTypeDef *STapp);

void STappSetScreen(uint16_t SetScreen, STApplicationTypeDef *STapp);

void STappShow(STApplicationTypeDef *STapp);

void STappFillInputBuff(char *Buff, STApplicationTypeDef *STapp);

char *STappGetInputTextBuff(STApplicationTypeDef *STapp);

void STappClearBuff(STApplicationTypeDef *STapp);

uint16_t STappGetTextBuffState(STApplicationTypeDef *STapp);

uint16_t STappGetAckState(STApplicationTypeDef *STapp);

void STappSetAckState(STApplicationTypeDef *STapp);

void STappEncoderSetDivider(uint16_t Divider);

void STappEncoderSetPeriod(uint16_t Period);

void STappEncoderSetCounter(uint16_t Counter);

uint16_t STappEncoderGetCounter();

uint16_t STappGetBeeperState(STApplicationTypeDef *STapp);

void STappSetBeeperState(uint16_t Value, STApplicationTypeDef *STapp);

/******************************************************
 *  Common
 ******************************************************/

uint32_t STgetTick();

/******************************************************
 *  Screen
 ******************************************************/
void STscreenInit(STScreenTypeDef *STScreen);

void STscreenShow(STScreenTypeDef *STScreen);

void STscreenDraw(STScreenTypeDef *STScreen);

void STscreenHeaderDraw(STScreenTypeDef *STScreen);

uint16_t STscreenShowNow(STScreenTypeDef *STScreen);

uint16_t STscreenOnShow(STScreenTypeDef *STScreen);

void STscreenNextItem(STScreenTypeDef *STScreen);

void STscreenPreItem(STScreenTypeDef *STScreen);

/******************************************************
 *  Keyboard
 ******************************************************/
void STkeyboardInit(STScreenTypeDef *STkeyboard);

void STkeyboardShow(STScreenTypeDef *STkeyboard);

void STkeyboardDraw(STScreenTypeDef *STkeyboard);

uint16_t STkeyboardShowNow(STScreenTypeDef *STkeyboard);

uint16_t STkeyboardOnShow(STScreenTypeDef *STkeyboard);

void STkeyboardNextItem(STScreenTypeDef *STkeyboard);

void STkeyboardPreItem(STScreenTypeDef *STkeyboard);

void STkeyboardNextLine(STScreenTypeDef *STkeyboard);

void STkeyboardPreLine(STScreenTypeDef *STkeyboard);

/******************************************************
 *  ����������� ��������
 ******************************************************/
/*
 * ProgressBar
 */
void STprogressBarInit(STprogressBarTypeDef *STprogressBar);

void STprogressBarShow(STprogressBarTypeDef *STprogressBar);

void STprogressBarDraw(STprogressBarTypeDef *STprogressBar);

void STBarValueDraw(STprogressBarTypeDef *STprogressBar);

void STBarValueUpdate(STprogressBarTypeDef *STprogressBar);

uint16_t STprogressBarGetValueLength(uint16_t Value,
		STprogressBarTypeDef *STprogressBar);

void STprogressBarSetValue(uint16_t Value, STprogressBarTypeDef *STprogressBar);

uint16_t STprogressBarGetValue(STprogressBarTypeDef *STprogressBar);

/*
 * ChannelBar
 */
void STchannelBarInit(STchannelBarTypeDef *STchannelBar);

void STchannelBarShow(STchannelBarTypeDef *STchannelBar);

void STchannelBarDraw(STchannelBarTypeDef *STchannelBar);

void STchannelBarValueDraw(STchannelBarTypeDef *STchannelBar);

void STchannelBarValueUpdate(STchannelBarTypeDef *STchannelBar);

void STchannelBarSetValue(uint16_t Value, STchannelBarTypeDef *STchannelBar);

uint16_t STchannelBarGetValue(STchannelBarTypeDef *STchannelBar);

uint16_t STchannelBarGetValueLength(uint16_t Value,
		STchannelBarTypeDef *STchannelBar);

/*
 * TrimmBar
 */
void STtrimBarInit(STtrimBarTypeDef *STtrimBar);

void STtrimBarShow(STtrimBarTypeDef *STtrimBar);

void STtrimBarDraw(STtrimBarTypeDef *STchannelBar);

void STtrimBarValueDraw(STtrimBarTypeDef *STtrimBar);

void STtrimBarValueUpdate(STtrimBarTypeDef *STtrimBar);

void STtrimBarSetValue(int16_t Value, STtrimBarTypeDef *STtrimBar);

int16_t STtrimBarGetValue(STtrimBarTypeDef *STtrimBar);

uint16_t STtrimBarGetValuePosition(int16_t Value, STtrimBarTypeDef *STtrimBar);

uint16_t STtrimBarValueChange(STtrimBarTypeDef *STtrimBar);

void STtrimBarInc(STtrimBarTypeDef *STtrimBar);

void STtrimBarDec(STtrimBarTypeDef *STtrimBar);

void STtrimBarEncoder(STtrimBarTypeDef *STtrimBar);

void STtrimBarValueCheck(STtrimBarTypeDef *STtrimBar);


/*
 * TextBox
 */
void STtextBoxInit(STtextBoxTypeDef *STtextBox);

void STtextBoxShow(STtextBoxTypeDef *STtextBox);

void STtextBoxDraw(STtextBoxTypeDef *STtextBox);

void STtextBoxUpdate(STtextBoxTypeDef *STtextBox);

void STtextBoxSetValue(char *Text, STtextBoxTypeDef *STtextBox);

char* STtextBoxGetValue(STtextBoxTypeDef *STtextBox);

/*
 * InputBox
 */
void STinputBoxInit(STinputBoxTypeDef *STinputBox);

void STinputBoxShow(STinputBoxTypeDef *STinputBox);

void STinputBoxDraw(STinputBoxTypeDef *STinputBox);

void STinputBoxUpdate(STinputBoxTypeDef *STinputBox);

void STinputBoxInc(STinputBoxTypeDef *STinputBox);

void STinputBoxDec(STinputBoxTypeDef *STinputBox);

void STinputBoxEncoder(STinputBoxTypeDef *STinputBox);

void STinputBoxValueCheck(STinputBoxTypeDef *STinputBox);

void STinputBoxSetValue(int16_t Value, STinputBoxTypeDef *STinputBox);

void STinputBoxSetLoLimit(int16_t Value, STinputBoxTypeDef *STinputBox);

void STinputBoxSetHiLimit(int16_t Value, STinputBoxTypeDef *STinputBox);

int16_t STinputBoxGetLoLimit(STinputBoxTypeDef *STinputBox);

int16_t STinputBoxGetHiLimit(STinputBoxTypeDef *STinputBox);

int16_t STinputBoxGetValue(STinputBoxTypeDef *STinputBox);

uint16_t STinputBoxValueChange(STinputBoxTypeDef *STinputBox);

uint16_t STinputBoxSelected(STinputBoxTypeDef *STinputBox);

/*
 * ListBox
 */
void STlistBoxInit(STlistBoxTypeDef *STlistBox);

void STlistBoxShow(STlistBoxTypeDef *STlistBox);

void STlistBoxDraw(STlistBoxTypeDef *STlistBox);

void STlistBoxUpdate(STlistBoxTypeDef *STlistBox);

void STlistBoxInc(STlistBoxTypeDef *STlistBox);

void STlistBoxDec(STlistBoxTypeDef *STlistBox);

void STlistBoxEncoder(STlistBoxTypeDef *STlistBox);

void STlistBoxValueCheck(STlistBoxTypeDef *STlistBox);

void STlistBoxSetValue(int16_t Value, STlistBoxTypeDef *STlistBox);

void STlistBoxSetListItem(const char *Text, int16_t Item,
		STlistBoxTypeDef *STlistBox);

int16_t STlistBoxGetValue(STlistBoxTypeDef *STlistBox);

uint16_t STlistBoxValueChange(STlistBoxTypeDef *STlistBox);

uint16_t STlistBoxSelected(STlistBoxTypeDef *STlistBox);

/*
 * Button
 */
void STbuttonInit(STbuttonTypeDef *STbutton);

void STbuttonShow(STbuttonTypeDef *STbutton);

void STbuttonDraw(STbuttonTypeDef *STbutton);

void STbuttonUpdate(STbuttonTypeDef *STbutton);

uint16_t STbuttonGetState(STbuttonTypeDef *STbutton);

uint16_t STbuttonPressed(STbuttonTypeDef *STbutton);

/*
 * CheckBox
 */
void STcheckBoxInit(STcheckBoxTypeDef *STcheckBox);

void STcheckBoxShow(STcheckBoxTypeDef *STcheckBox);

void STcheckBoxDraw(STcheckBoxTypeDef *STcheckBox);

void STcheckBoxUpdate(STcheckBoxTypeDef *STcheckBox);

uint16_t STcheckBoxGetState(STcheckBoxTypeDef *STcheckBox);

uint16_t STcheckBoxValueChange(STcheckBoxTypeDef *STcheckBox);

void STcheckBoxSetValue(uint16_t Value, STcheckBoxTypeDef *STcheckBox);

void STcheckBoxPressed(STcheckBoxTypeDef *STcheckBox);

/*
 * GraphPlot
 */
void STgraphPlotInit(STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotShow(STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotDraw(STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotUpdate(STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotVlineDraw(uint16_t Value, uint16_t Color,
		STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotSetValue(uint16_t Value, STgraphPlotTypeDef *STgraphPlot);

uint16_t STgraphGetLinePosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot);

uint16_t STgraphGetXPosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot);

uint16_t STgraphGetYPosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotDrawDot(uint16_t x, uint16_t y, STgraphPlotTypeDef *STgraphPlot);

void STgraphPlotClear(STgraphPlotTypeDef *STgraphPlot);

/*
 * Сontainer
 */
void STcontainerInit(STcontainerTypeDef *STcontainer);

void STcontainerShow(STcontainerTypeDef *STcontainer);

void STcontainerDraw(STcontainerTypeDef *STcontainer);

void STcontainerUpdate(STcontainerTypeDef *STcontainer);

/*
 * Keyboard char
 */
void STkeyCharInit(STkeyCharTypeDef *STkeyChar);

void STkeyCharShow(STkeyCharTypeDef *STkeyChar);

void STkeyCharDraw(STkeyCharTypeDef *STkeyChar);

uint16_t STkeyCharPressed(STkeyCharTypeDef *STkeyChar);

char STkeyCharGet(STkeyCharTypeDef *STkeyChar);

/*
 * Debug io stream
 */

uint8_t STprintf(const char * str);

#endif /* STGUI_STGUI_H_ */
