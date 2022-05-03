/*
 * stgui.c
 *
	Copyright 2019 Denis Davydov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
 */


#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "gui/stgui.h"
#include "gui/guiconfig.h"
#include "core/encoder.h"
#include "core/common.h"

/*
 */


/******************************************************
 * 	Application
 *****************************************************/

uint16_t STappGetCurrentScreen(STApplicationTypeDef *STapp)
{
	return STapp->CurrentScreen;
}

uint16_t STappGetPrevScreen(STApplicationTypeDef *STapp)
{
	return STapp->PrevScreen;
}

void STappSetScreen(uint16_t SetScreen, STApplicationTypeDef *STapp)
{
	STapp->SetScreen = SetScreen;
}

void STappShow(STApplicationTypeDef *STapp)
{
	STapp->Beep = No;


	if (	STapp->ButtonLeft 		||
			STapp->ButtonRight  	||
			STapp->ButtonUp  		||
			STapp->ButtonDown  		||
			STapp->ButtonEnter  	||
			STapp->ButtonBack)
	{
		STapp->Beep = Yes;
	}
	else
	{
		STapp->Beep = No;
	}

	if (	STapp->ButtonLeftLong 	||
			STapp->ButtonRightLong 	||
			STapp->ButtonUpLong 	||
			STapp->ButtonDownLong 	||
			STapp->ButtonEnterLong 	||
			STapp->ButtonBackLong
	)
	{
		STapp->LongBeep = Yes;
	}
	else
	{
		STapp->LongBeep = No;
	}


	if (STapp->preEncoder != STappEncoderGetCounter())
	{
		STapp->preEncoder = STappEncoderGetCounter();

		STapp->Beep = Yes;
	}
	else
	{
		STapp->Beep = No;
	}

}

void STappFillInputBuff(char *Buff, STApplicationTypeDef *STapp)
{
	if (Buff != NULL)
	{
		if (!STapp->BuffFull)
		{
			strlcpy(STapp->InputBuff, Buff, TEXT_BUFF_SIZE);
			STapp->BuffFull = Yes;
		}

	}
}

char *STappGetInputTextBuff(STApplicationTypeDef *STapp)
{
	return STapp->InputBuff;
}

void STappClearBuff(STApplicationTypeDef *STapp)
{
	STapp->BuffFull = No;
}

uint16_t STappGetTextBuffState(STApplicationTypeDef *STapp)
{
	return STapp->BuffFull;
}

uint16_t STappGetAckState(STApplicationTypeDef *STapp)
{
	uint16_t Value = 0;

	Value = STapp->Acknowledgment;

	if(STapp->Acknowledgment)
	{
		STapp->Acknowledgment = No;
	}
	return Value;
}

void STappSetAckState(STApplicationTypeDef *STapp)
{
	STapp->Acknowledgment = Yes;
}

void STappEncoderSetDivider(uint16_t Divider)
{
	EncoderSetDivider(Divider, &Encoder);
}

void STappEncoderSetPeriod(uint16_t Period)
{
	EncoderSetPeriod(Period, &Encoder);
}

void STappEncoderSetCounter(uint16_t Counter)
{
	EncoderSetCounter(Counter, &Encoder);
}

uint16_t STappEncoderGetCounter()
{
	return EncoderGetCounter(&Encoder);
}

uint16_t STappGetBeeperState(STApplicationTypeDef *STapp)
{
	return STapp->BeeperEnable;
}

void STappSetBeeperState(uint16_t Value, STApplicationTypeDef *STapp)
{
	STapp->BeeperEnable = Value;
}

/******************************************************
 *  Common
 ******************************************************/

uint32_t STgetTick()
{
	return HAL_GetTick();
}

/******************************************************
 * 	Screen
 *****************************************************/

void STscreenInit(STScreenTypeDef *STScreen)
{


#if DEBUG_UART_MESSAGE_GUI_GUI == 1

	char Message[MAX_GUI_DEBUG_BUF] = "";

	snprintf(Message, MAX_GUI_DEBUG_BUF, "%s -  screen init completed", STScreen->HeaderText);
	STprintf(Message);

#endif


}

void STscreenShow(STScreenTypeDef *STScreen)
{

#if DEBUG_UART_MESSAGE_GUI == 1

	char Message[MAX_GUI_DEBUG_BUF] = "";

#endif


	if(STScreen->App->CurrentScreen != STScreen->ScreenID)
	{
		if (STScreen->App->SetScreen != STScreen->ScreenID)
		{
			STScreen->State = Hide;
		}
		else
		{
			STScreen->State = Drawing;
			STScreen->App->PrevScreen = STScreen->App->CurrentScreen;
			STScreen->App->CurrentScreen = STScreen->App->SetScreen;
			STScreen->OnShow = Yes;
			STScreen->ItemSelected = 0;

		#if CONTROL_METHOD == 1

			STappEncoderSetDivider(ENCODER_MENU_STEP_PER_ITEM * ENCODER_MENU_STEP_PER_ITEM);
			STappEncoderSetPeriod(STScreen->ItemCounter + 1);
			STappEncoderSetCounter(1);

		#endif

		}
	}
	else
	{
		STScreen->State = Idle;
		STScreen->OnShow = No;

		#if CONTROL_METHOD == 1

			if (STScreen->EncoderBusy == 0)
			{
				if (STScreen->ItemSelected != STappEncoderGetCounter())
				{
					STScreen->ItemSelected = STappEncoderGetCounter();
				}

			}

		#elif CONTROL_METHOD == 2

			STscreenNextItem(STScreen);
			STscreenPreItem(STScreen);

		#endif

	}

	switch (STScreen->State)
	{

	case Hide:

#if DEBUG_UART_MESSAGE_GUI == 1

	snprintf(Message, MAX_GUI_DEBUG_BUF, "%s -  screen HIDE", STScreen->HeaderText);
	STprintf(Message);

#endif

		break;

	case Drawing:

#if DEBUG_UART_MESSAGE_GUI == 1

	snprintf(Message, MAX_GUI_DEBUG_BUF, "%s -  screen DRAWING", STScreen->HeaderText);
	STprintf(Message);

#endif
		STscreenDraw(STScreen);
		break;

	case Redrawing:

#if DEBUG_UART_MESSAGE_GUI == 1

	snprintf(Message, MAX_GUI_DEBUG_BUF, "%s -  screen REDRAWING", STScreen->HeaderText);
	STprintf(Message);

#endif

		break;

	case Update:

#if DEBUG_UART_MESSAGE_GUI == 1

	snprintf(Message, MAX_GUI_DEBUG_BUF, "%s -  screen UPDATE", STScreen->HeaderText);
	STprintf(Message);

#endif

		break;

	case Idle:



		break;

	default:
		;

	}
}

void STscreenDraw(STScreenTypeDef *STScreen)
{
	if(STScreen->FullScreen == Yes)
	{
		STFillScreen(STScreen->Style->BackgroundColor);
	}
	else
	{
		STRectangle((GUI_MAX_WIDTH_PX - STScreen->Width) / 2,
				(GUI_MAX_HEIGHT_PX - STScreen->Height) / 2,
				STScreen->Width,
				STScreen->Height,
				STScreen->Style->BorderThickness,
				STScreen->Style->BorderColor,
				STScreen->Style->BackgroundColor);
	}

	STscreenHeaderDraw(STScreen);

	STScreen->State = Idle;
}

void STscreenHeaderDraw(STScreenTypeDef *STScreen)
{
	uint16_t X;
	uint16_t Y;
	uint16_t Width;
	uint16_t Height;
	uint16_t TextLenght;

	if (STScreen->Header == Yes)
	{
		if (STScreen->FullScreen == Yes)
		{
			X = 0;
			Y = 0;
			Width = GUI_MAX_WIDTH_PX;
			Height = GUI_SCREEN_HEADER_HEIGHT_PX;
		}
		else
		{
			X = (GUI_MAX_WIDTH_PX - STScreen->Width) / 2 + STScreen->Style->BorderThickness;
			Y = (GUI_MAX_HEIGHT_PX - STScreen->Height) / 2 + STScreen->Style->BorderThickness;
			Width = STScreen->Width - 2 * STScreen->Style->BorderThickness;
			Height = GUI_SCREEN_HEADER_HEIGHT_PX;
		}

		STRectangle(X,
				Y,
				Width,
				Height,
				0,
				STScreen->Style->BorderColor,
				STScreen->Style->HeaderColor);

		TextLenght = GetTextLenghtPx(STScreen->HeaderText, STScreen->Style->HeaderFont);
		X = (uint16_t)(GUI_MAX_WIDTH_PX - TextLenght)/2;

		if (STScreen->FullScreen == Yes)
		{
			Y = (uint16_t)(GUI_SCREEN_HEADER_HEIGHT_PX - STScreen->Style->HeaderFont->HeightPx)/2;
		}
		else
		{
			Y = (GUI_MAX_HEIGHT_PX - STScreen->Height) / 2 + STScreen->Style->BorderThickness + (uint16_t)(GUI_SCREEN_HEADER_HEIGHT_PX - STScreen->Style->HeaderFont->HeightPx)/2;
		}

		STPutText(STScreen->HeaderText,
				STScreen->Style->HeaderFont,
				X,
				Y,
				STScreen->Style->HeaderTextColor,
				STScreen->Style->HeaderTextColor);
	}

}

uint16_t STscreenShowNow(STScreenTypeDef *STScreen)
{
	uint16_t ScreenState;

	if (STScreen->App->CurrentScreen == STScreen->ScreenID)
	{
		ScreenState = Yes;
	}
	else
	{
		ScreenState = No;
	}

	return ScreenState;
}

uint16_t STscreenOnShow(STScreenTypeDef *STScreen)
{
	return STScreen->OnShow;
}

void STscreenNextItem(STScreenTypeDef *STScreen)
{
	if(STScreen->App->ButtonDown)
	{
		STScreen->ItemSelected++;
	}

	if (STScreen->ItemSelected > STScreen->ItemCounter)
	{
		STScreen->ItemSelected = 1;
	}
}

void STscreenPreItem(STScreenTypeDef *STScreen)
{
	if (STScreen->App->ButtonUp)
	{
		STScreen->ItemSelected--;
	}

	if (STScreen->ItemSelected == 0)
	{
		STScreen->ItemSelected = STScreen->ItemCounter;
	}
}

/******************************************************
 * 	Keyboard
 *****************************************************/

void STkeyboardInit(STScreenTypeDef *STkeyboard)
{

}

void STkeyboardShow(STScreenTypeDef *STkeyboard)
{
	if(STkeyboard->App->CurrentScreen != STkeyboard->ScreenID)
	{
		if (STkeyboard->App->SetScreen != STkeyboard->ScreenID)
		{
			STkeyboard->State = Hide;
		}
		else
		{
			STkeyboard->State = Drawing;
			STkeyboard->App->PrevScreen = STkeyboard->App->CurrentScreen;
			STkeyboard->App->CurrentScreen = STkeyboard->App->SetScreen;
			STkeyboard->OnShow = Yes;



		#if CONTROL_METHOD == 1

			STappEncoderSetDivider(ENCODER_MENU_STEP_PER_ITEM * ENCODER_MENU_STEP_PER_ITEM);
			STappEncoderSetPeriod(STkeyboard->ItemCounter + 1);
			STappEncoderSetCounter(1);

		#endif

		}
	}
	else
	{
		STkeyboard->State = Idle;
		STkeyboard->OnShow = No;



		#if CONTROL_METHOD == 1

				STkeyboard->ItemSelected = STappEncoderGetCounter();

		#elif CONTROL_METHOD == 1

				STkeyboardNextItem(STkeyboard);
				STkeyboardPreItem(STkeyboard);
				STkeyboardNextLine(STkeyboard);
				STkeyboardPreLine(STkeyboard);

		#endif
	}

	switch (STkeyboard->State)
	{

	case Hide:

		break;

	case Drawing:
		STkeyboardDraw(STkeyboard);
		break;

	case Redrawing:

		break;

	case Update:

		break;

	case Idle:

		break;

	default:
		;

	}
}

void STkeyboardDraw(STScreenTypeDef *STkeyboard)
{

	STRectangle((GUI_MAX_WIDTH_PX - STkeyboard->Width) / 2,
			(GUI_MAX_HEIGHT_PX - STkeyboard->Height) / 2,
			STkeyboard->Width,
			STkeyboard->Height,
			STkeyboard->Style->BorderThickness,
			STkeyboard->Style->BorderColor,
			STkeyboard->Style->BackgroundColor);

	STkeyboard->State = Idle;
}

uint16_t STkeyboardShowNow(STScreenTypeDef *STkeyboard)
{
	uint16_t ScreenState;

	if (STkeyboard->App->CurrentScreen == STkeyboard->ScreenID)
	{
		ScreenState = Yes;
	}
	else
	{
		ScreenState = No;
	}

	return ScreenState;
}

uint16_t STkeyboardOnShow(STScreenTypeDef *STkeyboard)
{
	return STkeyboard->OnShow;
}

void STkeyboardNextItem(STScreenTypeDef *STkeyboard)
{
	if(STkeyboard->App->ButtonRight)
	{
		STkeyboard->ItemSelected++;
	}

	if (STkeyboard->ItemSelected > STkeyboard->ItemCounter)
	{
		STkeyboard->ItemSelected = 1;
	}
}

void STkeyboardPreItem(STScreenTypeDef *STkeyboard)
{
	if (STkeyboard->App->ButtonLeft)
	{
		STkeyboard->ItemSelected--;
	}

	if (STkeyboard->ItemSelected == 0)
	{
		STkeyboard->ItemSelected = STkeyboard->ItemCounter;
	}
}

void STkeyboardNextLine(STScreenTypeDef *STkeyboard)
{
	if(STkeyboard->App->ButtonDown)
	{
		STkeyboard->ItemSelected += 10;
	}

	if (STkeyboard->ItemSelected > STkeyboard->ItemCounter)
	{
		STkeyboard->ItemSelected = STkeyboard->ItemCounter;
	}
}

void STkeyboardPreLine(STScreenTypeDef *STkeyboard)
{
	if(STkeyboard->App->ButtonUp)
	{
		if (STkeyboard->ItemSelected > 10)
		{
			STkeyboard->ItemSelected -= 10;
		}
		else
		{
			STkeyboard->ItemSelected = 1;
		}
	}

}

/******************************************************
 * ProgressBar
 ******************************************************/

void STprogressBarInit(STprogressBarTypeDef *STprogressBar)
{
	STprogressBar->Value = STprogressBar->LoLimit;
}

void STprogressBarShow(STprogressBarTypeDef *STprogressBar)
{
	if (STprogressBar->Style->ParentScreen->State != Hide)
	{

		if(STprogressBar->State == Hide)
		{
			STprogressBar->State = Drawing;
		}

		switch (STprogressBar->State)
		{
			case Hide:

				break;

			case Drawing:
				STprogressBarDraw(STprogressBar);
				STprogressBar->State = Idle;
				break;

			case Redrawing:

				break;

			case Update:
				STBarValueUpdate(STprogressBar);
				STprogressBar->State = Idle;
				break;

			case Idle:

				break;

			default:
			;
		}
	}

	else
	{
		STprogressBar->State = Hide;
	}
}

void STprogressBarDraw(STprogressBarTypeDef *STprogressBar)
{
	STRectangle(STprogressBar->X,
			STprogressBar->Y,
			STprogressBar->Width,
			STprogressBar->Height,
			STprogressBar->Style->BorderThickness,
			STprogressBar->Style->BorderColor,
			STprogressBar->Style->BackgroundColor);

	STBarValueDraw(STprogressBar);

	STprogressBar->State = Idle;
}

void STBarValueDraw(STprogressBarTypeDef *STprogressBar)
{
	STRectangle(STprogressBar->X + STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
			STprogressBar->Y + STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
			STprogressBarGetValueLength(STprogressBar->Value, STprogressBar),
			STprogressBar->Height - 2 * (STprogressBar->Style->BorderThickness + BASE_BAR_GAP),
			0,
			STprogressBar->Style->BorderColor,
			STprogressBar->Style->Color);

}

void STBarValueUpdate(STprogressBarTypeDef *STprogressBar)
{
	if (STprogressBar->Value > STprogressBar->preValue)
	{
		STRectangle(STprogressBar->X + STprogressBarGetValueLength(STprogressBar->preValue, STprogressBar) + STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
				STprogressBar->Y + STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
				STprogressBarGetValueLength(STprogressBar->Value, STprogressBar)  - STprogressBarGetValueLength(STprogressBar->preValue, STprogressBar),
				STprogressBar->Height - 2 * (STprogressBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STprogressBar->Style->BorderColor,
				STprogressBar->Style->Color);

	}
	else
	{
		STRectangle(STprogressBar->X + STprogressBarGetValueLength(STprogressBar->Value, STprogressBar) + STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
				STprogressBar->Y +STprogressBar->Style->BorderThickness + BASE_BAR_GAP,
				STprogressBarGetValueLength(STprogressBar->preValue, STprogressBar) - STprogressBarGetValueLength(STprogressBar->Value, STprogressBar),
				STprogressBar->Height - 2 * (STprogressBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STprogressBar->Style->BorderColor,
				STprogressBar->Style->BackgroundColor);
	}

	STprogressBar->State = Idle;
}

void STprogressBarSetValue(uint16_t Value, STprogressBarTypeDef *STprogressBar)
{
	if(Value != STprogressBar->Value)
	{
		STprogressBar->preValue = STprogressBar->Value;

		if(Value > STprogressBar->HiLimit)
		{
			STprogressBar->Value = STprogressBar->HiLimit;
		}
		else if(Value < STprogressBar->LoLimit)
		{
			STprogressBar->Value = STprogressBar->LoLimit;
		}
		else
		{
			STprogressBar->Value = Value;
		}

		STprogressBar->State = Update;
	}

}

uint16_t STprogressBarGetValueLength(uint16_t Value, STprogressBarTypeDef *STprogressBar)
{
	return (uint16_t)((Value - STprogressBar->LoLimit) * (STprogressBar->Width - 2 * (STprogressBar->Style->BorderThickness + BASE_BAR_GAP))/( STprogressBar->HiLimit - STprogressBar->LoLimit));
}

/******************************************************
 * Channel Bar
 ******************************************************/

void STchannelBarInit(STchannelBarTypeDef *STchannelBar)
{
	STchannelBar->Value = STchannelBar->LowLimit;
}

void STchannelBarShow(STchannelBarTypeDef *STchannelBar)
{
	if (STchannelBar->Style->ParentScreen->State != Hide)
	{
		if(STchannelBar->State == Hide)
		{
			STchannelBar->State = Drawing;
		}

		switch (STchannelBar->State)
		{
		case Hide:

			break;

		case Drawing:
			STchannelBarDraw(STchannelBar);
			STchannelBar->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STchannelBarValueUpdate(STchannelBar);
			STchannelBar->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}

	else
	{
		STchannelBar->State = Hide;
	}
}

void STchannelBarDraw(STchannelBarTypeDef *STchannelBar)
{
	STRectangle(STchannelBar->X,
			STchannelBar->Y,
			STchannelBar->Width,
			STchannelBar->Height,
			STchannelBar->Style->BorderThickness,
			STchannelBar->Style->BorderColor,
			STchannelBar->Style->BackgroundColor);

	STchannelBarValueDraw(STchannelBar);

	STRectangle(STchannelBar->X + STchannelBar->Width/ 2 - BASE_BAR_CENTER_WIDTH,
			STchannelBar->Y + STchannelBar->Style->BorderThickness,
			BASE_BAR_CENTER_WIDTH * 2,
			STchannelBar->Height - STchannelBar->Style->BorderThickness * 2,
			0,
			STchannelBar->Style->BorderColor,
			STchannelBar->Style->Color2);



}

void STchannelBarValueDraw(STchannelBarTypeDef *STchannelBar)
{
	if (STchannelBar->Value < (uint16_t)((STchannelBar->HiLimit + STchannelBar->LowLimit) / 2))
	{
		STRectangle(STchannelBar->X  + STchannelBarGetValueLength(STchannelBar->Value, STchannelBar) +  STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
				STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
				STchannelBarGetValueLength(((STchannelBar->HiLimit - STchannelBar->LowLimit) / 2 - STchannelBar->Value), STchannelBar),
				STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STchannelBar->Style->BorderColor,
				STchannelBar->Style->Color);
	}
	else
	{
		STRectangle(STchannelBar->X + STchannelBar->Width / 2 + BASE_BAR_CENTER_WIDTH,
				STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
				STchannelBarGetValueLength(STchannelBar->Value - (STchannelBar->HiLimit - STchannelBar->LowLimit) / 2, STchannelBar),
				STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STchannelBar->Style->BorderColor,
				STchannelBar->Style->Color);
	}
}

void STchannelBarValueUpdate(STchannelBarTypeDef *STchannelBar)
{
	if (STchannelBar->Value > (uint16_t)(STchannelBar->HiLimit - STchannelBar->LowLimit) / 2 &&
		STchannelBar->preValue < (uint16_t)(STchannelBar->HiLimit - STchannelBar->LowLimit) / 2)
	{
		STRectangle(STchannelBar->X + STchannelBar->Style->BorderThickness,
				STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
				(uint16_t)STchannelBar->Width/2 -  BASE_BAR_GAP - BASE_BAR_CENTER_WIDTH,
				STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STchannelBar->Style->BorderColor,
				STchannelBar->Style->BackgroundColor);
	}

	if (STchannelBar->Value < (uint16_t)(STchannelBar->HiLimit - STchannelBar->LowLimit) / 2 &&
		STchannelBar->preValue > (uint16_t)(STchannelBar->HiLimit - STchannelBar->LowLimit) / 2)
	{
		STRectangle(STchannelBar->X  + (uint16_t)STchannelBar->Width/2 + BASE_BAR_CENTER_WIDTH,
				STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
				(uint16_t)STchannelBar->Width/2 -  STchannelBar->Style->BorderThickness - BASE_BAR_GAP,
				STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
				0,
				STchannelBar->Style->BorderColor,
				STchannelBar->Style->BackgroundColor);
	}

	if (STchannelBar->Value < (uint16_t)((STchannelBar->HiLimit - STchannelBar->LowLimit) / 2))
	{
		if(STchannelBar->Value > STchannelBar->preValue)
		{
			STRectangle(STchannelBar->X  + STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar) +  STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBarGetValueLength(STchannelBar->Value, STchannelBar) - STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar),
					STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
					0,
					STchannelBar->Style->BorderColor,
					STchannelBar->Style->BackgroundColor);
		}
		if(STchannelBar->Value < STchannelBar->preValue)
		{
			STRectangle(STchannelBar->X  + STchannelBarGetValueLength(STchannelBar->Value, STchannelBar) +  STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar) - STchannelBarGetValueLength(STchannelBar->Value, STchannelBar),
					STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
					0,
					STchannelBar->Style->BorderColor,
					STchannelBar->Style->Color);
		}
	}


	if (STchannelBar->Value > (uint16_t)((STchannelBar->HiLimit - STchannelBar->LowLimit) / 2))
	{
		if(STchannelBar->Value > STchannelBar->preValue)
		{
			STRectangle(STchannelBar->X + STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar) + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBarGetValueLength(STchannelBar->Value, STchannelBar) - STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar),
					STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
					0,
					STchannelBar->Style->BorderColor,
					STchannelBar->Style->Color);
		}
		if(STchannelBar->Value < STchannelBar->preValue)
		{
			STRectangle(STchannelBar->X + STchannelBarGetValueLength(STchannelBar->Value, STchannelBar) + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBar->Y + STchannelBar->Style->BorderThickness + BASE_BAR_GAP,
					STchannelBarGetValueLength(STchannelBar->preValue, STchannelBar) - STchannelBarGetValueLength(STchannelBar->Value, STchannelBar),
					STchannelBar->Height - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP),
					0,
					STchannelBar->Style->BorderColor,
					STchannelBar->Style->BackgroundColor);
		}

	}

	STRectangle(STchannelBar->X + STchannelBar->Width/ 2 - BASE_BAR_CENTER_WIDTH,
			STchannelBar->Y + STchannelBar->Style->BorderThickness,
			BASE_BAR_CENTER_WIDTH * 2,
			STchannelBar->Height - STchannelBar->Style->BorderThickness * 2,
			0,
			STchannelBar->Style->BorderColor,
			STchannelBar->Style->Color2);

}

uint16_t STchannelBarGetValueLength(uint16_t Value, STchannelBarTypeDef *STchannelBar)
{
	return (uint16_t)(Value * (STchannelBar->Width - 2 * (STchannelBar->Style->BorderThickness + BASE_BAR_GAP))/(STchannelBar->HiLimit - STchannelBar->LowLimit));
}

void STchannelBarSetValue(uint16_t Value, STchannelBarTypeDef *STchannelBar)
{
	if(STchannelBar->Value != Value)
	{
		STchannelBar->preValue = STchannelBar->Value;

		if(Value > STchannelBar->HiLimit)
		{
			STchannelBar->Value = STchannelBar->HiLimit;
		}
		else if(Value < STchannelBar->LowLimit)
		{
			STchannelBar->Value = STchannelBar->LowLimit;
		}
		else
		{
			STchannelBar->Value = Value;
		}

		STchannelBar->State = Update;
	}
}

/******************************************************
 * Trimmer Bar
 ******************************************************/

void STtrimBarInit(STtrimBarTypeDef *STtrimBar)
{

	if (STtrimBar->Value > STtrimBar->HiLimit)
	{
		STtrimBar->Value = STtrimBar->HiLimit;
	}

	if(STtrimBar->Value < STtrimBar->LoLimit)
	{
		STtrimBar->Value = STtrimBar->LoLimit;
	}

	/*
	 * ������������ ����������� ItemID ��� ������� ������������ ��������
	 * ����� �������� �������� �� Dynamic
	 */
	if (STtrimBar->Dynamic == Yes)
	{
		STtrimBar->Style->ParentScreen->ItemCounter++;
		STtrimBar->ItemID = STtrimBar->Style->ParentScreen->ItemCounter;
	}

}

void STtrimBarShow(STtrimBarTypeDef *STtrimBar)
{
	if (STtrimBar->Style->ParentScreen->State != Hide)
	{

		if (STtrimBar->ItemID == STtrimBar->Style->ParentScreen->ItemSelected)
		{
			if (STtrimBar->InFocus != Yes)
			{
				STtrimBar->InFocus = Yes;
				STtrimBar->State = Drawing;
			}
		}
		else
		{
			if (STtrimBar->InFocus == Yes)
			{
				STtrimBar->State = Drawing;
				STtrimBar->InFocus = No;
			}

		}

		if(STtrimBar->State == Hide)
		{
			STtrimBar->State = Drawing;
		}

		#if CONTROL_METHOD == 1

			STtrimBarEncoder(STtrimBar);

		#elif CONTROL_METHOD == 2

			STtrimBarInc(STtrimBar);
			STtrimBarDec(STtrimBar);

		#endif

		STtrimBarValueCheck(STtrimBar);

		switch (STtrimBar->State)
		{
		case Hide:

			break;

		case Drawing:
			STtrimBarDraw(STtrimBar);
			STtrimBar->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STtrimBarDraw(STtrimBar);
			STtrimBar->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STtrimBar->State = Hide;
	}
}

void STtrimBarDraw(STtrimBarTypeDef *STtrimBar)
{
	uint16_t i;
	uint16_t lineStep = 0;
	uint16_t X = 0;

	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t BackgroundColor;
	//	uint16_t Color;

	if (STtrimBar->InFocus == Yes)
	{
		BorderThickness = STtrimBar->Style->FocusBorderThickness;
		BorderColor = STtrimBar->Style->FocusBorderColor;
		BackgroundColor = STtrimBar->Style->FocusBackgroundColor;
		//		Color = STtrimBar->Style->FocusColor;
	}
	else
	{
		BorderThickness = STtrimBar->Style->BorderThickness;
		BorderColor = STtrimBar->Style->BorderColor;
		BackgroundColor = STtrimBar->Style->BackgroundColor;
		//		Color = STtrimBar->Style->Color;
	}

	/*
	 * Main form
	 */

	STRectangle(STtrimBar->X,
			STtrimBar->Y,
			STtrimBar->Width,
			STtrimBar->Height,
			BorderThickness,
			BorderColor,
			BackgroundColor);


	switch(STtrimBar->Orientation)
	{
		case Vertical:

			lineStep = (uint16_t)(STtrimBar->Height + STtrimBar->Style->BorderThickness * 2) / 8 ;

		break;

		default:

			lineStep = (uint16_t)(STtrimBar->Width + STtrimBar->Style->BorderThickness * 2) / 8 ;

		break;
	}


	/*
	 * Scale line
	 */

	for (i = 0; i <= 6;i++)
	{
		X = X + lineStep;

		switch(STtrimBar->Orientation)
		{
			case Vertical:

				STRectangle(STtrimBar->X + STtrimBar->Style->BorderThickness , //
						STtrimBar->Y + 	X - STtrimBar->Style->BorderThickness - 1,
						STtrimBar->Width / 2 - STtrimBar->Style->BorderThickness,
						BASE_BAR_CENTER_WIDTH ,
						0,
						BorderColor,
						BorderColor);

			break;

			default:

				STRectangle(STtrimBar->X + X - STtrimBar->Style->BorderThickness -1 ,
						STtrimBar->Y + 	STtrimBar->Height / 2,
						BASE_BAR_CENTER_WIDTH,
						STtrimBar->Height / 2 - STtrimBar->Style->BorderThickness ,
						0,
						BorderColor,
						BorderColor);
			break;
		}
	}

	switch(STtrimBar->Orientation)
	{
		case Vertical:

			/*
			 * Central
			 */

			STRectangle(STtrimBar->X + STtrimBar->Style->BorderThickness,
					STtrimBar->Y + (uint16_t)(STtrimBar->Height)/2 - BASE_BAR_CENTER_WIDTH,
					STtrimBar->Width - STtrimBar->Style->BorderThickness * 2,
					BASE_BAR_CENTER_WIDTH * 2,
					0,
					BorderColor,
					STtrimBar->Style->Color2);
		break;

		default:

			/*
			 * Central
			 */

			STRectangle(STtrimBar->X + (uint16_t)(STtrimBar->Width)/2 - BASE_BAR_CENTER_WIDTH,
					STtrimBar->Y + STtrimBar->Style->BorderThickness,
					BASE_BAR_CENTER_WIDTH * 2,
					STtrimBar->Height - STtrimBar->Style->BorderThickness * 2,
					0,
					BorderColor,
					STtrimBar->Style->Color2);
		break;
	}

	/*
	 * Value line
	 */
	STtrimBarValueDraw(STtrimBar);
}

void STtrimBarValueDraw(STtrimBarTypeDef *STtrimBar)
{

	uint16_t Color;

	if (STtrimBar->InFocus == Yes)
	{
		Color = STtrimBar->Style->FocusColor;
	}
	else
	{

		Color = STtrimBar->Style->Color;
	}

	switch(STtrimBar->Orientation)
	{
		case Vertical:

			STRectangle(STtrimBar->X + STtrimBar->Style->BorderThickness + STtrimBar->Width / 2,
					STtrimBar->Y + STtrimBarGetValuePosition(STtrimBar->Value, STtrimBar) + STtrimBar->Style->BorderThickness,
					(uint16_t)(STtrimBar->Width - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					4,
					0,
					STtrimBar->Style->BorderColor,
					Color);

		break;

		default: /* Horizontal */

			STRectangle(STtrimBar->X + STtrimBarGetValuePosition(STtrimBar->Value, STtrimBar) + STtrimBar->Style->BorderThickness,
					STtrimBar->Y + STtrimBar->Style->BorderThickness,
					4,
					(uint16_t)(STtrimBar->Height - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					0,
					STtrimBar->Style->BorderColor,
					Color);
		break;
	}


}

void STtrimBarValueUpdate(STtrimBarTypeDef *STtrimBar)
{

	uint16_t Color;
	uint16_t BackgroundColor;

	if (STtrimBar->InFocus == Yes)
	{
		Color = STtrimBar->Style->FocusColor;
		BackgroundColor = STtrimBar->Style->FocusBackgroundColor;
	}
	else
	{

		Color = STtrimBar->Style->Color;
		BackgroundColor = STtrimBar->Style->BackgroundColor;
	}

	switch(STtrimBar->Orientation)
	{
		case Vertical:

			STRectangle(STtrimBar->X + STtrimBar->Style->BorderThickness	,
					STtrimBar->Y + STtrimBarGetValuePosition(STtrimBar->preValue, STtrimBar) + STtrimBar->Style->BorderThickness,
					(uint16_t)(STtrimBar->Width - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					4,
					0,
					STtrimBar->Style->BorderColor,
					BackgroundColor);

			STRectangle(STtrimBar->X + STtrimBar->Style->BorderThickness	,
					STtrimBar->Y + STtrimBarGetValuePosition(STtrimBar->Value, STtrimBar) + STtrimBar->Style->BorderThickness,
					(uint16_t)(STtrimBar->Width - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					4,
					0,
					STtrimBar->Style->BorderColor,
					Color);

		break;

		default: /* Horizontal */

			STRectangle(STtrimBar->X + STtrimBarGetValuePosition(STtrimBar->preValue, STtrimBar),
					STtrimBar->Y + STtrimBar->Style->BorderThickness,
					4,
					(uint16_t)(STtrimBar->Height - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					0,
					STtrimBar->Style->BorderColor,
					BackgroundColor);

			STRectangle(STtrimBar->X + STtrimBarGetValuePosition(STtrimBar->Value, STtrimBar),
					STtrimBar->Y + STtrimBar->Style->BorderThickness,
					4,
					(uint16_t)(STtrimBar->Height - 2 * (STtrimBar->Style->BorderThickness)) / 2,
					0,
					STtrimBar->Style->BorderColor,
					Color);
		break;
	}


}

void STtrimBarSetValue(int16_t Value, STtrimBarTypeDef *STtrimBar)
{
	if(STtrimBar->Value != Value)
	{
		STtrimBar->preValue = STtrimBar->Value;
		STtrimBar->Value = Value;
		STtrimBar->State = Update;
	}
}

uint16_t STtrimBarGetValuePosition(int16_t Value, STtrimBarTypeDef *STtrimBar)
{


	switch(STtrimBar->Orientation)
	{
		case Vertical:

			return (uint16_t)((Value -  STtrimBar->LoLimit)*(STtrimBar->Height - 2 * (STtrimBar->Style->BorderThickness + BASE_BAR_GAP))/( STtrimBar->HiLimit - STtrimBar->LoLimit));

		break;

		default:

			return (uint16_t)((Value -  STtrimBar->LoLimit)*(STtrimBar->Width - 2 * (STtrimBar->Style->BorderThickness + BASE_BAR_GAP))/( STtrimBar->HiLimit - STtrimBar->LoLimit));
		break;
	}

}

int16_t STtrimBarGetValue(STtrimBarTypeDef *STtrimBar)
{
	/*
	 * ��� ������� ����� �� ������������
	 */

	return STtrimBar->Value;
}

void STtrimBarInc(STtrimBarTypeDef *STtrimBar)
{
	if (STtrimBar->InFocus && STtrimBar->Style->ParentScreen->App->ButtonRight)
	{
		if(STtrimBar->Value >= STtrimBar->HiLimit)
		{
			STtrimBar->Value = STtrimBar->HiLimit;
		}
		else
		{
			STtrimBar->Value++;
		}
		STtrimBar->State = Update;
	}
}

void STtrimBarDec(STtrimBarTypeDef *STtrimBar)
{
	if (STtrimBar->InFocus && STtrimBar->Style->ParentScreen->App->ButtonLeft)
	{
		if(STtrimBar->Value <= STtrimBar->LoLimit)
		{
			STtrimBar->Value = STtrimBar->LoLimit;
		}
		else
		{
			STtrimBar->Value--;
		}
		STtrimBar->State = Update;
	}
}

void STtrimBarEncoder(STtrimBarTypeDef *STtrimBar)
{

	if (STtrimBar->InFocus)
	{
		if (STtrimBar->Style->ParentScreen->App->ButtonEnter)
		{
			if (STtrimBar->Style->ParentScreen->EncoderBusy == 0)
			{
				STtrimBar->Style->ParentScreen->EncoderBusy = STtrimBar->ItemID;

				STappEncoderSetPeriod(STtrimBar->HiLimit - STtrimBar->LoLimit);

				STappEncoderSetCounter(STtrimBar->Value - STtrimBar->LoLimit);

			} else

			{
				STtrimBar->Style->ParentScreen->ItemSelected = STtrimBar->Style->ParentScreen->EncoderBusy;

				STappEncoderSetPeriod(STtrimBar->Style->ParentScreen->ItemCounter + 1);

				STappEncoderSetCounter(STtrimBar->Style->ParentScreen->ItemSelected);

				STtrimBar->Style->ParentScreen->EncoderBusy = 0;
			}


		}

		if (STtrimBar->Style->ParentScreen->EncoderBusy != 0)
		{

			if (STappEncoderGetCounter() < STtrimBar->LoLimit)
			{
				STappEncoderSetCounter(STtrimBar->HiLimit);
			}

			STtrimBar->Value = STappEncoderGetCounter() + STtrimBar->LoLimit;
		}


	}
}

void STtrimBarValueCheck(STtrimBarTypeDef *STtrimBar)
{
	if (STtrimBar->Value != STtrimBar->preValue)
	{
		STtrimBar->ValueChange = Yes;
		STtrimBar->preValue = STtrimBar->Value;
		STtrimBar->State = Update;
	}
	else
	{
		STtrimBar->ValueChange = No;
//		STtrimBar->preValue = STtrimBar->Value;
	}
}

uint16_t STtrimBarValueChange(STtrimBarTypeDef *STtrimBar)
{
	return STtrimBar->ValueChange;
}

/******************************************************
 * TextBox
 *****************************************************/

void STtextBoxInit(STtextBoxTypeDef *STtextBox)
{
	if(strcmp(STtextBox->Text, STtextBox->TextBuff) != 0)
	{
		strlcpy(STtextBox->TextBuff, STtextBox->Text, TEXT_BUFF_SIZE);
	}
}

void STtextBoxShow(STtextBoxTypeDef *STtextBox)
{
	if (STtextBox->Style->ParentScreen->State != Hide)
	{

		if(STtextBox->State == Hide)
		{
			STtextBox->State = Drawing;
		}

		switch (STtextBox->State)
		{
		case Hide:

			break;

		case Drawing:
			STtextBoxDraw(STtextBox);
			STtextBox->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STtextBoxUpdate(STtextBox);
			STtextBox->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STtextBox->State = Hide;
	}
}

void STtextBoxDraw(STtextBoxTypeDef *STtextBox)
{
	uint16_t X = 0;
	uint16_t Y = 0;
	uint16_t TextLenght;

	switch (STtextBox->TextAlignment)
	{
	case Left:
		X = STtextBox->X + BASE_TEXT_GAP;
		break;
	case Centre:
		TextLenght = GetTextLenghtPx(STtextBox->TextBuff, STtextBox->Style->Font);
		X = STtextBox->X + (uint16_t)(STtextBox->Width - TextLenght)/2;
		break;
	case Right:
		TextLenght = GetTextLenghtPx(STtextBox->TextBuff, STtextBox->Style->Font);
		X = STtextBox->X + (uint16_t)(STtextBox->Width - TextLenght) - STtextBox->Style->BorderThickness;
		break;
	default:

		break;
	}
//
	Y = STtextBox->Y + (uint16_t)(STtextBox->Height - STtextBox->Style->Font->HeightPx)/2;

	STRectangle(STtextBox->X,
			STtextBox->Y,
			STtextBox->Width,
			STtextBox->Height,
			STtextBox->Style->BorderThickness,
			STtextBox->Style->BorderColor,
			STtextBox->Style->BackgroundColor);

	STPutText(STtextBox->TextBuff,
			STtextBox->Style->Font,
			X,
			Y,
			STtextBox->Style->Color,
			STtextBox->Style->BackgroundColor);
}

void STtextBoxUpdate(STtextBoxTypeDef *STtextBox)
{
	uint16_t TextLenght = 0;
	uint16_t TextLenghtDelta = 0;
	uint16_t X = 0;

	const char *Text;
	const char *NewText;

	Text = STtextBox->TextBuff;
	NewText = STtextBox->Text;

	switch (STtextBox->TextAlignment)
	{

	case Left:

		TextLenght = GetTextLenghtPx(Text, STtextBox->Style->Font);

		while (*Text >= STtextBox->Style->Font->FirstChar && *Text <= STtextBox->Style->Font->LastChar &&
				*NewText >= STtextBox->Style->Font->FirstChar && *NewText <= STtextBox->Style->Font->LastChar &&
				*Text == *NewText)
		{
			Text++;
			NewText++;
		}


		TextLenghtDelta = TextLenght - GetTextLenghtPx(Text, STtextBox->Style->Font);
		TextLenght = GetTextLenghtPx(Text, STtextBox->Style->Font);
		X = STtextBox->X + TextLenghtDelta + 2;

		/*
		 * ����������� ������ ������
		 */

		STRectangle(X,
				STtextBox->Y + STtextBox->Style->BorderThickness,
				TextLenght,
				STtextBox->Height - 2 * STtextBox->Style->BorderThickness,
				0,
				STtextBox->Style->BorderColor,
				STtextBox->Style->BackgroundColor);

		STPutText(NewText,
				STtextBox->Style->Font,
				X,
				STtextBox->Y + (uint16_t)(STtextBox->Height - STtextBox->Style->Font->HeightPx)/2,
				STtextBox->Style->Color,
				STtextBox->Style->BackgroundColor);

		strlcpy(STtextBox->TextBuff, STtextBox->Text, TEXT_BUFF_SIZE);
		STtextBox->Text = NULL;
		STtextBox->State = Idle;

		break;

	default:

		strlcpy(STtextBox->TextBuff, STtextBox->Text, TEXT_BUFF_SIZE);
		STtextBox->Text = NULL;
		STtextBoxDraw(STtextBox);
		break;
	}
}

void STtextBoxSetValue(char *Text, STtextBoxTypeDef *STtextBox)
{
	if (strcmp(Text, STtextBox->TextBuff) != 0)
	{
		STtextBox->Text = Text;
		STtextBox->State = Update;
	}
}

char* STtextBoxGetValue(STtextBoxTypeDef *STtextBox)
{
	return STtextBox->TextBuff;
}

/******************************************************
 * Button
 ******************************************************/

void STbuttonInit(STbuttonTypeDef *STbutton)
{

//	if (STbutton->Text != NULL)
//	{
//		strlcpy(STbutton->ButtonLabel, STbutton->Text, 32);
//	}
//	else
//	{
////		strlcpy(STbutton->ButtonLabel, "---", 32);
//	}

	STbutton->Style->ParentScreen->ItemCounter++;
	STbutton->ItemID = STbutton->Style->ParentScreen->ItemCounter;
}

void STbuttonShow(STbuttonTypeDef *STbutton)
{
	if (STbutton->Style->ParentScreen->State != Hide)
	{

		if (STbutton->ItemID == STbutton->Style->ParentScreen->ItemSelected)
		{
			if (STbutton->InFocus != Yes)
			{
				STbutton->InFocus = Yes;
				STbutton->State = Update;
			}
		}
		else
		{
			if (STbutton->InFocus == Yes)
			{
				STbutton->State = Update;
				STbutton->InFocus = No;
			}
		}

		if(STbutton->State == Hide)
		{
			STbutton->State = Drawing;
		}

		switch (STbutton->State)
		{
		case Hide:

			break;

		case Drawing:
			STbuttonDraw(STbutton);
			STbutton->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STbuttonDraw(STbutton);
			STbutton->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STbutton->State = Hide;
	}
}

void STbuttonDraw(STbuttonTypeDef *STbutton)
{
	/*
	 * ������ ��������� ������� ���������� ����
	 * ������� �������� ������ �� ������
	 * ������� �������� ������
	 */


	uint16_t X;
	uint16_t Y;
	uint16_t TextLenght;

	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t BackgroundColor;
	uint16_t Color;

	if (STbutton->InFocus == Yes)
	{
		BorderThickness = STbutton->Style->FocusBorderThickness;
		BorderColor = STbutton->Style->FocusBorderColor;
		BackgroundColor = STbutton->Style->FocusBackgroundColor;
		Color = STbutton->Style->FocusColor;
	}
	else
	{
		BorderThickness = STbutton->Style->BorderThickness;
		BorderColor = STbutton->Style->BorderColor;
		BackgroundColor = STbutton->Style->BackgroundColor;
		Color = STbutton->Style->Color;
	}

	STRectangle(STbutton->X,
			STbutton->Y,
			STbutton->Width,
			STbutton->Height,
			BorderThickness,
			BorderColor,
			BackgroundColor);

	TextLenght = GetTextLenghtPx(STbutton->Text, STbutton->Style->Font);				// STbutton->ButtonLabel
	X = STbutton->X + (uint16_t)(STbutton->Width - TextLenght)/2;
	Y = STbutton->Y + (uint16_t)(STbutton->Height - STbutton->Style->Font->HeightPx)/2;

	STPutText(STbutton->Text,
			STbutton->Style->Font,
			X,
			Y,
			Color,
			BackgroundColor);
}

uint16_t STbuttonPressed(STbuttonTypeDef *STbutton)
{
	if (STbutton->InFocus && STbutton->Style->ParentScreen->App->ButtonEnter)
	{
		STbutton->Style->ParentScreen->App->ButtonEnter = No;
		return Yes;
	}
	else
	{
		return No;
	}


}

/******************************************************
 * InputBox
 ******************************************************/

void STinputBoxInit(STinputBoxTypeDef *STinputBox)
{
	if(STinputBox->UnitsShow)
	{
		snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d %s",STinputBox->Value, STinputBox->Units);
	}
	else
	{
		snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d",STinputBox->Value);
	}


	if (STinputBox->Dynamic == Yes)
	{
		STinputBox->Style->ParentScreen->ItemCounter++;
		STinputBox->ItemID = STinputBox->Style->ParentScreen->ItemCounter;
	}
}

void STinputBoxShow(STinputBoxTypeDef *STinputBox)
{
	if (STinputBox->Style->ParentScreen->State != Hide)
	{
		if (STinputBox->ItemID == STinputBox->Style->ParentScreen->ItemSelected)
		{
			if (STinputBox->InFocus != Yes)
			{
				STinputBox->InFocus = Yes;
				STinputBox->State = Update;
			}
		}
		else
		{
			if (STinputBox->InFocus == Yes)
			{
				STinputBox->State = Update;
				STinputBox->InFocus = No;
			}

		}

		if (STinputBox->Selected)
		{
			switch(STinputBox->BlinkState)
			{
			case 0:

				if((STgetTick() - STinputBox->preTick) > LISTBOX_BLINK_PERIOD)
				{

					STinputBox->State = Update;

					STinputBox->BlinkState = 1;

					STinputBox->preTick = STgetTick();
				}


				break;

			case 1:

				if((STgetTick() - STinputBox->preTick) > LISTBOX_BLINK_PERIOD)
				{

					STinputBox->State = Update;

					STinputBox->BlinkState = 0;

					STinputBox->preTick = STgetTick();
				}

				break;

			default:

				break;
			}
		}

		if(STinputBox->State == Hide)
		{
			STinputBox->State = Drawing;
		}

		#if CONTROL_METHOD == 1

			STinputBoxEncoder(STinputBox);

		#elif CONTROL_METHOD == 2

				STinputBoxInc(STinputBox);
				STinputBoxDec(STinputBox);

		#endif

		STinputBoxValueCheck(STinputBox);

		switch (STinputBox->State)
		{
		case Hide:

			break;

		case Drawing:
			STinputBoxDraw(STinputBox);
			STinputBox->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STinputBoxUpdate(STinputBox);
			STinputBox->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STinputBox->State = Hide;
	}
}

void STinputBoxDraw(STinputBoxTypeDef *STinputBox)
{
	uint16_t X;
	uint16_t Y;
	uint16_t TextLenght;

	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t BackgroundColor;
	uint16_t Color;
	uint16_t TextColor;

	if (STinputBox->InFocus == Yes)
	{
		BorderThickness = STinputBox->Style->FocusBorderThickness;
		BorderColor = STinputBox->Style->FocusBorderColor;
		BackgroundColor = STinputBox->Style->FocusBackgroundColor;
		Color = STinputBox->Style->FocusColor;
		TextColor = STinputBox->Style->FocusTextColor;


		if(STinputBox->Selected)
		{
			switch(STinputBox->BlinkState)
			{
			case 0:

				BorderColor = STinputBox->Style->FocusBorderColor;

				break;

			case 1:

				BorderColor = STinputBox->Style->BorderColor;

				break;

			default:

				break;
			}
		}

		else
		{
			BorderColor = STinputBox->Style->FocusBorderColor;
		}
	}
	else
	{
		BorderThickness = STinputBox->Style->BorderThickness;
		BorderColor = STinputBox->Style->BorderColor;
		BackgroundColor = STinputBox->Style->BackgroundColor;
		Color = STinputBox->Style->Color;
		TextColor = STinputBox->Style->TextColor;
	}

	STRectangle(STinputBox->X,
			STinputBox->Y,
			STinputBox->Width,
			STinputBox->Height,
			BorderThickness,
			BorderColor,
			BackgroundColor);

	TextLenght = GetTextLenghtPx(STinputBox->TextBuff, STinputBox->Style->Font);
	X = STinputBox->X + (uint16_t)(STinputBox->Width - TextLenght)/2;
	Y = STinputBox->Y + (uint16_t)(STinputBox->Height - STinputBox->Style->Font->HeightPx)/2;

	STPutText(STinputBox->TextBuff,
			STinputBox->Style->Font,
			X,
			Y,
			TextColor,
			Color);
}

void STinputBoxUpdate(STinputBoxTypeDef *STinputBox)
{
	STinputBoxDraw(STinputBox);
}

void STinputBoxSetValue(int16_t Value, STinputBoxTypeDef *STinputBox)
{
	uint16_t i = 0;

	STinputBox->Value = Value;
	STinputBox->State = Update;

	if(STinputBox->UnitsShow)
	{
		snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d %s",STinputBox->Value, STinputBox->Units);
	}
	else
	{
		snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d",STinputBox->Value);
	}

	if(STinputBox->AddDot)
	{
		for(i = 0; i <  (INPUT_BUFF_SIZE - STinputBox->DotPosition); i++)
		{
			STinputBox->TextBuff[INPUT_BUFF_SIZE - i] = STinputBox->TextBuff[INPUT_BUFF_SIZE - i - 1];
		}

		STinputBox->TextBuff[STinputBox->DotPosition] = 46; // DOT
	}

#if CONTROL_METHOD == 1

	if(STinputBox->Selected)
	{
		STappEncoderSetCounter(STinputBox->Value - STinputBox->LoLimit);
	}

#elif CONTROL_METHOD == 2

#endif

}

void STinputBoxInc(STinputBoxTypeDef *STinputBox)
{
	if (STinputBox->InFocus && STinputBox->Style->ParentScreen->App->ButtonRight)
	{
		if(STinputBox->Value >= STinputBox->HiLimit)
		{
			STinputBox->Value = STinputBox->HiLimit;
		}
		else
		{
			STinputBox->Value++;
		}

		STinputBox->State = Update;


		if(STinputBox->UnitsShow)
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d %s",STinputBox->Value, STinputBox->Units);
		}
		else
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d",STinputBox->Value);
		}
	}
}

void STinputBoxDec(STinputBoxTypeDef *STinputBox)
{
	if (STinputBox->InFocus && STinputBox->Style->ParentScreen->App->ButtonLeft)
	{
		if(STinputBox->Value <= STinputBox->LoLimit)
		{
			STinputBox->Value = STinputBox->LoLimit;
		}
		else
		{
			STinputBox->Value--;
		}
		STinputBox->State = Update;
		if(STinputBox->UnitsShow)
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d %s",STinputBox->Value, STinputBox->Units);
		}
		else
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d",STinputBox->Value);
		}
	}
}

void STinputBoxEncoder(STinputBoxTypeDef *STinputBox)
{

	if (STinputBox->InFocus)
	{
		if (STinputBox->Style->ParentScreen->App->ButtonEnter)
		{
			if (STinputBox->Style->ParentScreen->EncoderBusy == 0)
			{
				STinputBox->Style->ParentScreen->EncoderBusy = STinputBox->ItemID;

				STappEncoderSetPeriod(STinputBox->HiLimit - STinputBox->LoLimit);

				STappEncoderSetCounter(STinputBox->Value - STinputBox->LoLimit);

				STinputBox->Selected = Yes;

			} else

			{
				STinputBox->Style->ParentScreen->ItemSelected = STinputBox->Style->ParentScreen->EncoderBusy;

				STappEncoderSetPeriod(STinputBox->Style->ParentScreen->ItemCounter + 1);

				STappEncoderSetCounter(STinputBox->Style->ParentScreen->ItemSelected);

				STinputBox->Style->ParentScreen->EncoderBusy = 0;

				STinputBox->Selected = No;
			}


		}

		if (STinputBox->Style->ParentScreen->EncoderBusy != 0)
		{
			STinputBox->Value = (int16_t)STappEncoderGetCounter() + STinputBox->LoLimit;
		}


	}
}

void STinputBoxValueCheck(STinputBoxTypeDef *STinputBox)
{
	uint16_t i = 0;

	if (STinputBox->Value != STinputBox->preValue)
	{
		STinputBox->preValue = STinputBox->Value;
		STinputBox->ValueChange = Yes;


		STinputBox->State = Update;
		if(STinputBox->UnitsShow)
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d %s",STinputBox->Value, STinputBox->Units);
		}
		else
		{
			snprintf (STinputBox->TextBuff, INPUT_BUFF_SIZE, "%d",STinputBox->Value);
		}

		if(STinputBox->AddDot)
		{
			for(i = 0; i <  (INPUT_BUFF_SIZE - STinputBox->DotPosition); i++)
			{
				STinputBox->TextBuff[INPUT_BUFF_SIZE - i] = STinputBox->TextBuff[INPUT_BUFF_SIZE - i - 1];
			}

			STinputBox->TextBuff[STinputBox->DotPosition] = 46; // DOT
		}

	}
	else
	{
		STinputBox->ValueChange = No;
		STinputBox->preValue = STinputBox->Value;
	}
}

int16_t STinputBoxGetValue(STinputBoxTypeDef *STinputBox)
{
	return STinputBox->Value;
}

uint16_t STinputBoxValueChange(STinputBoxTypeDef *STinputBox)
{
	if (STinputBox->ValueChange == Yes)
	{
		return Yes;
	}
	else
	{
		return No;
	}
}

void STinputBoxSetLoLimit(int16_t Value, STinputBoxTypeDef *STinputBox)
{
	STinputBox->LoLimit = Value;
}

void STinputBoxSetHiLimit(int16_t Value, STinputBoxTypeDef *STinputBox)
{
	STinputBox->HiLimit = Value;
}

int16_t  STinputBoxGetLoLimit(STinputBoxTypeDef *STinputBox)
{
	return STinputBox->LoLimit;
}

int16_t  STinputBoxGetHiLimit(STinputBoxTypeDef *STinputBox)
{
	return STinputBox->HiLimit;
}

uint16_t STinputBoxSelected(STinputBoxTypeDef *STinputBox)
{
	return STinputBox->Selected;
}

/******************************************************
 * CheckBox
 ******************************************************/

void STcheckBoxInit(STcheckBoxTypeDef *STcheckBox)
{
	if (STcheckBox->Dynamic == Yes)
	{
		STcheckBox->Style->ParentScreen->ItemCounter++;
		STcheckBox->ItemID = STcheckBox->Style->ParentScreen->ItemCounter;
	}
}

void STcheckBoxShow(STcheckBoxTypeDef *STcheckBox)
{
	if (STcheckBox->Style->ParentScreen->State != Hide)
	{

		if (STcheckBox->ItemID == STcheckBox->Style->ParentScreen->ItemSelected)
		{
			if (STcheckBox->InFocus != Yes)
			{
				STcheckBox->InFocus = Yes;
				STcheckBox->State = Update;
			}
		}
		else
		{
			if (STcheckBox->InFocus == Yes)
			{
				STcheckBox->State = Update;
				STcheckBox->InFocus = No;
			}

		}

		STcheckBoxPressed(STcheckBox);

		if(STcheckBox->State == Hide)
		{
			STcheckBox->State = Drawing;
		}

		switch (STcheckBox->State)
		{
		case Hide:

			break;

		case Drawing:
			STcheckBoxDraw(STcheckBox);
			STcheckBox->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STcheckBoxUpdate(STcheckBox);
			STcheckBox->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STcheckBox->State = Hide;
	}
}

void STcheckBoxDraw(STcheckBoxTypeDef *STcheckBox)
{


	uint16_t X = 0;
	uint16_t Y = 0;
	uint16_t TextLenght = 0;

	uint16_t BorderThickness = 0;
	uint16_t BorderColor = 0;
	uint16_t BackgroundColor = 0;
	uint16_t Color = 0;
	const char *Text;

	if (STcheckBox->InFocus == Yes)
	{
		BorderThickness = STcheckBox->Style->FocusBorderThickness;
		BorderColor = STcheckBox->Style->FocusBorderColor;
	//	BackgroundColor = STcheckBox->Style->FocusBackgroundColor;
	}
	else
	{
		BorderThickness = STcheckBox->Style->BorderThickness;
		BorderColor = STcheckBox->Style->BorderColor;
	//	BackgroundColor = STcheckBox->Style->BackgroundColor;

	}

	if (STcheckBox->Value == Yes)
	{
		Text = STcheckBox->OnText;
		Color = STcheckBox->Style->Color2;
	}
	else
	{
		Text = STcheckBox->OffText;
		Color = STcheckBox->Style->Color;
	}

	STRectangle(STcheckBox->X,
			STcheckBox->Y,
			STcheckBox->Width,
			STcheckBox->Height,
			BorderThickness,
			BorderColor,
			Color);

	TextLenght = GetTextLenghtPx(Text, STcheckBox->Style->Font);
	X = STcheckBox->X + (uint16_t)(STcheckBox->Width - TextLenght)/2;
	Y = STcheckBox->Y + (uint16_t)(STcheckBox->Height - STcheckBox->Style->Font->HeightPx)/2;

	STPutText(Text,
			STcheckBox->Style->Font,
			X,
			Y,
			STcheckBox->Style->TextColor,
			BackgroundColor);
}

void STcheckBoxUpdate(STcheckBoxTypeDef *STcheckBox)
{
	STcheckBoxDraw(STcheckBox);
}

uint16_t STcheckBoxGetState(STcheckBoxTypeDef *STcheckBox)
{
	return STcheckBox->Value;
}

uint16_t STcheckBoxValueChange(STcheckBoxTypeDef *STcheckBox)
{
	if (STcheckBox->ValueChange == Yes)
	{
		return Yes;
	}
	else
	{
		return No;
	}


}

void STcheckBoxSetValue(uint16_t Value, STcheckBoxTypeDef *STcheckBox)
{
	if (Value >= 1)// && STcheckBox->Value == No
	{
		STcheckBox->Value = Yes;
		STcheckBox->State = Update;
	}

	if (Value < 1) // && STcheckBox->Value == Yes
	{
		STcheckBox->Value = No;
		STcheckBox->State = Update;
	}
}

void STcheckBoxPressed(STcheckBoxTypeDef *STcheckBox)
{
	if (STcheckBox->InFocus && STcheckBox->Style->ParentScreen->App->ButtonEnter)
	{
		if (STcheckBox->Value >= Yes)
		{
			STcheckBox->Value = No;
		}
		else
		{
			STcheckBox->Value = Yes;
		}
		STcheckBox->ValueChange = Yes;
		STcheckBox->State = Update;
	}
	else
	{
		STcheckBox->ValueChange = No;
	}
}

/******************************************************
 * GraphPlot
 ******************************************************/

void STgraphPlotInit(STgraphPlotTypeDef *STgraphPlot)
{

}

void STgraphPlotShow(STgraphPlotTypeDef *STgraphPlot)
{
	if (STgraphPlot->ParentScreen->State != Hide)
	{
		if(STgraphPlot->State == Hide)
		{
			STgraphPlot->State = Drawing;
		}

		if(STgraphPlot->Clear)
		{
			STgraphPlot->Clear = No;
			STgraphPlot->State = Drawing;
		}

		switch (STgraphPlot->State)
		{
		case Hide:

			break;

		case Drawing:
			STgraphPlotDraw(STgraphPlot);
			STgraphPlot->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STgraphPlotUpdate(STgraphPlot);
			//STgraphPlotDraw(STgraphPlot);
			STgraphPlot->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STgraphPlot->State = Hide;
	}
}

void STgraphPlotDraw(STgraphPlotTypeDef *STgraphPlot)
{
	STRectangle(STgraphPlot->X,
			STgraphPlot->Y,
			STgraphPlot->Width,
			STgraphPlot->Height,
			STgraphPlot->Style->BorderThickness,
			STgraphPlot->Style->BorderColor,
			STgraphPlot->Style->BackgroundColor);

	if (STgraphPlot->Crosshairs != No)
	{
		/*
		 * ������������ ���
		 */
		STRectangle(STgraphPlot->X + (uint16_t)(STgraphPlot->Width - BASE_GRAPHPLOT_AXIS_THICKNESS)/2,
				STgraphPlot->Y,
				BASE_GRAPHPLOT_AXIS_THICKNESS,
				STgraphPlot->Height,
				0,
				STgraphPlot->Style->BorderColor,
				STgraphPlot->Style->BorderColor);

		/*
		 * �������������� ���
		 */
		STRectangle(STgraphPlot->X,
				STgraphPlot->Y + (uint16_t)(STgraphPlot->Height - BASE_GRAPHPLOT_AXIS_THICKNESS)/2,
				STgraphPlot->Width,
				BASE_GRAPHPLOT_AXIS_THICKNESS,
				0,
				STgraphPlot->Style->BorderColor,
				STgraphPlot->Style->BorderColor);
	}

	STgraphPlotVlineDraw(STgraphPlot->Value, STgraphPlot->Style->BorderColor, STgraphPlot);
}

void STgraphPlotUpdate(STgraphPlotTypeDef *STgraphPlot)
{
	STgraphPlotVlineDraw(STgraphPlot->preValue, STgraphPlot->Style->BackgroundColor, STgraphPlot);
	STgraphPlotVlineDraw(STgraphPlot->Value, STgraphPlot->Style->BorderColor, STgraphPlot);
}

void STgraphPlotVlineDraw(uint16_t Value, uint16_t  Color, STgraphPlotTypeDef *STgraphPlot)
{
	uint16_t	X = 0;

	X = STgraphGetLinePosition(Value, STgraphPlot);

	if(STgraphPlot->Crosshairs)
	{
		if (X + 4 < (uint16_t)STgraphPlot->Width/2)
		{
			STDrawVerticalLine(STgraphPlot->X + STgraphPlot->Style->BorderThickness + X,
								STgraphPlot->Y + STgraphPlot->Style->BorderThickness,
								STgraphPlot->Height - (uint16_t)STgraphPlot->Height/2 - BASE_GRAPHPLOT_AXIS_THICKNESS - STgraphPlot->Style->BorderThickness,
								Color);

			STDrawVerticalLine(STgraphPlot->X + STgraphPlot->Style->BorderThickness + X,
								STgraphPlot->Y + (uint16_t)STgraphPlot->Height/2 + BASE_GRAPHPLOT_AXIS_THICKNESS,
								STgraphPlot->Height - (uint16_t)STgraphPlot->Height/2 - BASE_GRAPHPLOT_AXIS_THICKNESS - STgraphPlot->Style->BorderThickness,
								Color);
		}

		if(X + BASE_GRAPHPLOT_AXIS_THICKNESS > (uint16_t)STgraphPlot->Width/2)
		{
			STDrawVerticalLine(STgraphPlot->X + STgraphPlot->Style->BorderThickness + X,
								STgraphPlot->Y + STgraphPlot->Style->BorderThickness,
								STgraphPlot->Height - (uint16_t)STgraphPlot->Height/2 - BASE_GRAPHPLOT_AXIS_THICKNESS - STgraphPlot->Style->BorderThickness,
								Color);

			STDrawVerticalLine(STgraphPlot->X + STgraphPlot->Style->BorderThickness + X,
								STgraphPlot->Y + (uint16_t)STgraphPlot->Height/2 + BASE_GRAPHPLOT_AXIS_THICKNESS,
								STgraphPlot->Height - (uint16_t)STgraphPlot->Height/2 - BASE_GRAPHPLOT_AXIS_THICKNESS - STgraphPlot->Style->BorderThickness,
								Color);
		}
	}
	else
	{
		STDrawVerticalLine(STgraphPlot->X + STgraphPlot->Style->BorderThickness + STgraphGetLinePosition(Value, STgraphPlot),
							STgraphPlot->Y + STgraphPlot->Style->BorderThickness,
							STgraphPlot->Height - 2*STgraphPlot->Style->BorderThickness,
							Color);
	}

}

uint16_t STgraphGetLinePosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot)
{
	return (uint16_t)((Value - STgraphPlot->LoLimitX) * (STgraphPlot->Width - 2*STgraphPlot->Style->BorderThickness)/( STgraphPlot->HiLimitX - STgraphPlot->LoLimitX));
}

void STgraphPlotSetValue(uint16_t Value, STgraphPlotTypeDef *STgraphPlot)
{
	STgraphPlot->preValue = STgraphPlot->Value;

	if(Value > STgraphPlot->HiLimitX)
	{
		STgraphPlot->Value = STgraphPlot->HiLimitX;
	}
	else if (Value < STgraphPlot->LoLimitX)
	{
		STgraphPlot->Value = STgraphPlot->LoLimitX;
	}
	else
	{
		STgraphPlot->Value = Value;
	}

	STgraphPlot->State = Update;

}

void STgraphPlotDrawDot(uint16_t x, uint16_t y, STgraphPlotTypeDef *STgraphPlot)
{
	if(x > STgraphPlot->HiLimitX)
	{
		x = STgraphPlot->HiLimitX;
	}
	else if(x < STgraphPlot->LoLimitX)
	{
		x = STgraphPlot->LoLimitX;
	}

	if(y > STgraphPlot->HiLimitY)
	{
		y = STgraphPlot->HiLimitY;
	}
	else if(y < STgraphPlot->LoLimitY)
	{
		y = STgraphPlot->LoLimitY;
	}

	STDrawPixel(STgraphPlot->X + STgraphGetXPosition(x, STgraphPlot) + STgraphPlot->Style->BorderThickness,
				STgraphPlot->Y + STgraphGetYPosition(y, STgraphPlot) ,//+ STgraphPlot->Style->BorderThickness
				STgraphPlot->Style->Color);
}

uint16_t STgraphGetXPosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot)
{
	return (uint16_t)((Value - STgraphPlot->LoLimitX)* (STgraphPlot->Width - 2*STgraphPlot->Style->BorderThickness)/( STgraphPlot->HiLimitX - STgraphPlot->LoLimitX));
}

uint16_t STgraphGetYPosition(uint16_t Value, STgraphPlotTypeDef *STgraphPlot)
{
	return (uint16_t)((STgraphPlot->Height - 2*STgraphPlot->Style->BorderThickness) - (Value - STgraphPlot->LoLimitX)* (STgraphPlot->Height-2*STgraphPlot->Style->BorderThickness)/
																																( STgraphPlot->HiLimitY - STgraphPlot->LoLimitY));
}

void STgraphPlotClear(STgraphPlotTypeDef *STgraphPlot)
{
	STgraphPlot->Clear = Yes;
}

/******************************************************
 * Container
 ******************************************************/

void STcontainerInit(STcontainerTypeDef *STcontainer)
{

}

void STcontainerShow(STcontainerTypeDef *STcontainer)
{

}

void STcontainerDraw(STcontainerTypeDef *STcontainer)
{

}

void STcontainerUpdate(STcontainerTypeDef *STcontainer)
{

}

/******************************************************
 * Keyboard Char
 ******************************************************/

void STkeyCharInit(STkeyCharTypeDef *STkeyChar)
{
	STkeyChar->Style->ParentScreen->ItemCounter++;
	STkeyChar->ItemID = STkeyChar->Style->ParentScreen->ItemCounter;
}

void STkeyCharShow(STkeyCharTypeDef *STkeyChar)
{
	if (STkeyChar->Style->ParentScreen->State != Hide)
	{

		if (STkeyChar->ItemID == STkeyChar->Style->ParentScreen->ItemSelected)
		{
			if (STkeyChar->InFocus != Yes)
			{
				STkeyChar->InFocus = Yes;
				STkeyChar->State = Update;
			}
		}
		else
		{
			if (STkeyChar->InFocus == Yes)
			{
				STkeyChar->State = Update;
				STkeyChar->InFocus = No;
			}

		}

		if(STkeyChar->State == Hide)
		{
			STkeyChar->State = Drawing;
		}

		switch (STkeyChar->State)
		{
		case Hide:

			break;

		case Drawing:
			STkeyCharDraw(STkeyChar);
			STkeyChar->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STkeyCharDraw(STkeyChar);
			STkeyChar->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STkeyChar->State = Hide;
	}
}

void STkeyCharDraw(STkeyCharTypeDef *STkeyChar)
{


	uint16_t X;
	uint16_t Y;
	uint16_t TextLenght;

	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t BackgroundColor;
	uint16_t Color;

	if (STkeyChar->InFocus == Yes)
	{
		BorderThickness = STkeyChar->Style->FocusBorderThickness;
		BorderColor = STkeyChar->Style->FocusBorderColor;
		BackgroundColor = STkeyChar->Style->FocusBackgroundColor;
		Color = STkeyChar->Style->FocusColor;
	}
	else
	{
		BorderThickness = STkeyChar->Style->BorderThickness;
		BorderColor = STkeyChar->Style->BorderColor;
		BackgroundColor = STkeyChar->Style->BackgroundColor;
		Color = STkeyChar->Style->Color;
	}

	STRectangle(STkeyChar->X,
			STkeyChar->Y,
			STkeyChar->Style->Width,
			STkeyChar->Style->Height,
			BorderThickness,
			BorderColor,
			BackgroundColor);

	TextLenght = GetCharLenghtPx(STkeyChar->Char, STkeyChar->Style->Font);
	X = STkeyChar->X + (uint16_t)(STkeyChar->Style->Width - TextLenght)/2;
	Y = STkeyChar->Y + (uint16_t)(STkeyChar->Style->Height - STkeyChar->Style->Font->HeightPx)/2;

	STPutText(&STkeyChar->Char,
			STkeyChar->Style->Font,
			X,
			Y,
			Color,
			BackgroundColor);
}

uint16_t STkeyCharPressed(STkeyCharTypeDef *STkeyChar)
{
	if (STkeyChar->InFocus && STkeyChar->Style->ParentScreen->App->ButtonEnter)
	{
		return Yes;
	}
	else
	{
		return No;
	}
}

char STkeyCharGet(STkeyCharTypeDef *STkeyChar)
{
	return STkeyChar->Char;
}

/******************************************************
 * ListBox
 ******************************************************/

void STlistBoxInit(STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->Dynamic == Yes)
	{
		STlistBox->Style->ParentScreen->ItemCounter++;
		STlistBox->ItemID = STlistBox->Style->ParentScreen->ItemCounter;
	}
}

void STlistBoxShow(STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->Style->ParentScreen->State != Hide)
	{
		if (STlistBox->ItemID == STlistBox->Style->ParentScreen->ItemSelected)
		{
			if (STlistBox->InFocus != Yes)
			{
				STlistBox->InFocus = Yes;
				STlistBox->State = Update;
			}
		}
		else
		{
			if (STlistBox->InFocus == Yes)
			{
				STlistBox->State = Update;
				STlistBox->InFocus = No;
			}

		}

		if(STlistBox->State == Hide)
		{
			STlistBox->State = Drawing;
		}

		if (STlistBox->Selected)
		{
			switch(STlistBox->BlinkState)
			{
			case 0:

				if((STgetTick() - STlistBox->preTick) > LISTBOX_BLINK_PERIOD)
				{

					STlistBox->State = Update;

					STlistBox->BlinkState = 1;

					STlistBox->preTick = STgetTick();
				}


				break;

			case 1:

				if((STgetTick() - STlistBox->preTick) > LISTBOX_BLINK_PERIOD)
				{

					STlistBox->State = Update;

					STlistBox->BlinkState = 0;

					STlistBox->preTick = STgetTick();
				}

				break;

			default:

				break;
			}
		}


		#if CONTROL_METHOD == 1

				STlistBoxEncoder(STlistBox);

		#elif CONTROL_METHOD == 2

				STlistBoxInc(STlistBox);
				STlistBoxDec(STlistBox);

		#endif

		STlistBoxValueCheck(STlistBox);

		switch (STlistBox->State)
		{
		case Hide:

			break;

		case Drawing:
			STlistBoxDraw(STlistBox);
			STlistBox->State = Idle;
			break;

		case Redrawing:

			break;

		case Update:
			STlistBoxUpdate(STlistBox);
			STlistBox->State = Idle;
			break;

		case Idle:

			break;

		default:
			;
		}
	}
	else
	{
		STlistBox->State = Hide;
	}
}

void STlistBoxDraw(STlistBoxTypeDef *STlistBox)
{
	uint16_t X;
	uint16_t Y;
	uint16_t TextLenght;

	uint16_t BorderThickness;
	uint16_t BorderColor;
	uint16_t BackgroundColor;
	uint16_t Color;
	uint16_t TextColor;

	if (STlistBox->InFocus == Yes)
	{
		BorderThickness = STlistBox->Style->FocusBorderThickness;
		BorderColor = STlistBox->Style->FocusBorderColor;
		BackgroundColor = STlistBox->Style->FocusBackgroundColor;
		Color = STlistBox->Style->FocusColor;
		TextColor = STlistBox->Style->FocusTextColor;


		if(STlistBox->Selected)
		{
			switch(STlistBox->BlinkState)
			{
			case 0:

				BorderColor = STlistBox->Style->FocusBorderColor;

				break;

			case 1:

				BorderColor = STlistBox->Style->BorderColor;

				break;

			default:

				break;
			}
		}

		else
		{
			BorderColor = STlistBox->Style->FocusBorderColor;
		}
	}
	else
	{
		BorderThickness = STlistBox->Style->BorderThickness;
		BorderColor = STlistBox->Style->BorderColor;
		BackgroundColor = STlistBox->Style->BackgroundColor;
		Color = STlistBox->Style->Color;
		TextColor = STlistBox->Style->TextColor;

	}

	STRectangle(STlistBox->X,
				STlistBox->Y,
				STlistBox->Width,
				STlistBox->Height,
				BorderThickness,
				BorderColor,
				BackgroundColor);

	TextLenght = GetTextLenghtPx(STlistBox->List[STlistBox->Value], STlistBox->Style->Font);
	X = STlistBox->X + (uint16_t)(STlistBox->Width - TextLenght)/2;
	Y = STlistBox->Y + (uint16_t)(STlistBox->Height - STlistBox->Style->Font->HeightPx)/2;

	STPutText(	STlistBox->List[STlistBox->Value],
				STlistBox->Style->Font,
				X,
				Y,
				TextColor,
				Color);
}

void STlistBoxUpdate(STlistBoxTypeDef *STlistBox)
{
	 STlistBoxDraw(STlistBox);
}

void STlistBoxInc(STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->InFocus && STlistBox->Style->ParentScreen->App->ButtonRight)
	{
		if(STlistBox->Value >= STlistBox->HiLimit)
		{
			STlistBox->Value = STlistBox->HiLimit;
		}
		else
		{
			STlistBox->Value++;
		}

		STlistBox->State = Update;
	}
}

void STlistBoxDec(STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->InFocus && STlistBox->Style->ParentScreen->App->ButtonLeft)
	{
		if(STlistBox->Value <= STlistBox->LoLimit)
		{
			STlistBox->Value = STlistBox->LoLimit;
		}
		else
		{
			STlistBox->Value--;
		}
		STlistBox->State = Update;
	}
}

void STlistBoxEncoder(STlistBoxTypeDef *STlistBox)
{

	if (STlistBox->InFocus)
	{
		if (STlistBox->Style->ParentScreen->App->ButtonEnter)
		{
			if (STlistBox->Style->ParentScreen->EncoderBusy == 0)
			{
				STlistBox->Style->ParentScreen->EncoderBusy = STlistBox->ItemID;

				STappEncoderSetPeriod(STlistBox->HiLimit - STlistBox->LoLimit);

				STappEncoderSetCounter(STlistBox->Value - STlistBox->LoLimit);

				STlistBox->Selected = Yes;


			} else

			{
				STlistBox->Style->ParentScreen->ItemSelected = STlistBox->Style->ParentScreen->EncoderBusy;

				STappEncoderSetPeriod(STlistBox->Style->ParentScreen->ItemCounter + 1);

				STappEncoderSetCounter(STlistBox->Style->ParentScreen->ItemSelected);

				STlistBox->Style->ParentScreen->EncoderBusy = 0;

				STlistBox->Selected = No;
			}

		}

		if (STlistBox->Style->ParentScreen->EncoderBusy != 0)
		{
			STlistBox->Value = STappEncoderGetCounter() + STlistBox->LoLimit;
		}
	}
}

void STlistBoxValueCheck(STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->Value != STlistBox->preValue)
	{
		STlistBox->preValue = STlistBox->Value;
		STlistBox->ValueChange = Yes;
		STlistBox->State = Update;
	}
	else
	{
		STlistBox->ValueChange = No;
		STlistBox->preValue = STlistBox->Value;
	}
}

void STlistBoxSetValue(int16_t Value, STlistBoxTypeDef *STlistBox)
{
	if (STlistBox->Value != Value)
	{
		STlistBox->Value = Value;
		STlistBox->State = Update;



#if CONTROL_METHOD == 1

		if(STlistBox->Selected)
		{
			STappEncoderSetCounter(STlistBox->Value - STlistBox->LoLimit);
		}

#elif CONTROL_METHOD == 2

#endif

	}
}

int16_t STlistBoxGetValue(STlistBoxTypeDef *STlistBox)
{
	return STlistBox->Value;
}

uint16_t STlistBoxValueChange(STlistBoxTypeDef *STlistBox)
{
	return STlistBox->ValueChange;
}

void STlistBoxSetListItem(const char *Text, int16_t Item, STlistBoxTypeDef *STlistBox)
{
	if(strcmp(STlistBox->List[Item], Text) != 0)
	{
		strlcpy(STlistBox->List[Item], Text, LISTBOX_BUFF_SIZE);
	}
}

uint16_t STlistBoxSelected(STlistBoxTypeDef *STlistBox)
{
	return STlistBox->Selected;
}

/******************************************************
 * Debug io stream
 ******************************************************/

uint8_t STprintf(const char * str)
{
	uint8_t result = {0};

	result = common_printf(str);

	return result;
}
