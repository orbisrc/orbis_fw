/*
 * chmapping.c
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


#include <stdio.h>
#include <string.h>
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/gui.h"
#include "gui/stapp.h"
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include "core/buffer.h"
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>

#define CHMP_NAME_X				110
#define CHMP_LISTBOX_X			160

#define CHMP_LINE_Y				23
#define CHMP_LINE_STEP			18

STScreenStyleTypeDef CHmappingStyle = { 0 };
STScreenTypeDef CHmappingST = { 0 };
STColorStyleTypeDef CHmappingText = { 0 };
STColorStyleTypeDef CHmappingButtonColor = { 0 };
STColorStyleTypeDef CHmappingListColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonCHMP = { 0 };

STlistBoxTypeDef CHmappingList[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef CHNameCHmapping[MAX_RC_CHANNEL]  = { 0 };

void _CHmapping()
{
	uint16_t i = 0 ;

	if (STscreenShowNow(&CHmappingST))
	{


		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			if (STlistBoxValueChange(&CHmappingList[i]))
			{
				RCChanelBufferSetItem(STlistBoxGetValue(&CHmappingList[i]),&RCChanel[i]);
			}

			if(STlistBoxSelected(&CHmappingList[i]))
			{
				if(GetBufferActiveItemNumber() != NON_CHANGE_BUFFER_ITEM)
				{
					STlistBoxSetValue(GetBufferActiveItemNumber(),	&CHmappingList[i]);
					//STlistBoxSetValue(10,	&CHmappingList[i]);
				}

			}
		}

		if (STbuttonPressed(&BackToModelMenuButtonCHMP)) {
			STappSetScreen(ModelMenu, &STApp);
/*
 *
 *
 * Добавить сохранение настроек
 *
 *
 */
		}
	}

	STscreenShow(&CHmappingST);

	if (STscreenOnShow(&CHmappingST)) {

		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			STlistBoxSetValue(RCChanelBufferGetItem(&RCChanel[i]),	&CHmappingList[i]);
		}
	}

	for (i = 4 ; i < MAX_RC_CHANNEL;i++)
	{
		STlistBoxShow(&CHmappingList[i]);
		STtextBoxShow(&CHNameCHmapping[i]);
	}

	STbuttonShow(&BackToModelMenuButtonCHMP);
}
void _CHmappingInit() {

	uint16_t i = 0 ;
	uint16_t j = 0 ;

	CHmappingStyle.HeaderFont = &verdana_8pt_bold;
	CHmappingStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	CHmappingStyle.HeaderColor = MAIN_HEADER_COLOR;
	CHmappingStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	CHmappingStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	CHmappingST.App = &STApp;
	CHmappingST.Style = &CHmappingStyle;
	CHmappingST.ScreenID = ChannelMapping;
	CHmappingST.FullScreen = Yes;
	CHmappingST.Width = GUI_MAX_WIDTH_PX;
	CHmappingST.Height = GUI_MAX_HEIGHT_PX;
	CHmappingST.Header = Yes;
	CHmappingST.HeaderText = "CHANNEL MAPPING";
	CHmappingST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	CHmappingButtonColor.Font = &verdana_8pt_bold;
	CHmappingButtonColor.ParentScreen = &CHmappingST;
	CHmappingButtonColor.Color = BASE_BUTTON_COLOR;
	CHmappingButtonColor.TextColor = BASE_BUTTON_COLOR;
	CHmappingButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	CHmappingButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	CHmappingButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	CHmappingButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	CHmappingButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	CHmappingButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	CHmappingButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	CHmappingListColor.ParentScreen = &CHmappingST;
	CHmappingListColor.Font = &verdana_8pt_bold;
	CHmappingListColor.Color = BASE_LISTBOX_COLOR;
	CHmappingListColor.TextColor = BASE_LISTBOX_TEXT_COLOR;
	CHmappingListColor.BackgroundColor = BASE_LISTBOX_BACK_COLOR;
	CHmappingListColor.BorderThickness = BASE_LISTBOX_BORDER_THICKNESS;
	CHmappingListColor.BorderColor = BASE_LISTBOX_BORDER_COLOR;
	CHmappingListColor.FocusTextColor = FOCUS_LISTBOX_TEXT_COLOR;
	CHmappingListColor.FocusColor = FOCUS_LISTBOX_COLOR;
	CHmappingListColor.FocusBackgroundColor = FOCUS_LISTBOX_BACK_COLOR;
	CHmappingListColor.FocusBorderThickness = FOCUS_LISTBOX_BORDER_THICKNESS;
	CHmappingListColor.FocusBorderColor = FOCUS_LISTBOX_BORDER_COLOR;

	CHmappingText.ParentScreen = &CHmappingST;
	CHmappingText.Font = &verdana_8pt_bold;
	CHmappingText.Color = BASE_TEXT_COLOR;
	CHmappingText.TextColor = BASE_TEXT_COLOR;
	CHmappingText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	CHmappingText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	CHmappingText.BorderColor = BASE_TEXT_BORDER_COLOR;


	for (i = 4 ; i < MAX_RC_CHANNEL;i++)
	{
		CHmappingList[i].Style = &CHmappingListColor;
		CHmappingList[i].Width = 50;
		CHmappingList[i].Height = 17;
		CHmappingList[i].X = CHMP_LISTBOX_X;
		CHmappingList[i].Y = CHMP_LINE_Y + i * CHMP_LINE_STEP;;
		CHmappingList[i].HiLimit = CHANNEL_BUFFER_LENGTH;
		CHmappingList[i].LoLimit = 0;
		CHmappingList[i].TextAlignment = Centre;
		CHmappingList[i].Border = Yes;
		CHmappingList[i].Dynamic = Yes;


		CHNameCHmapping[i].Text = __CHLabel[i];
		CHNameCHmapping[i].Style = &CHmappingText;
		CHNameCHmapping[i].Width = 30;
		CHNameCHmapping[i].Height = BASE_CHECKBOX_HEIGHT;
		CHNameCHmapping[i].X = CHMP_NAME_X;
		CHNameCHmapping[i].Y = CHMP_LINE_Y + i * CHMP_LINE_STEP;
		CHNameCHmapping[i].Border = No;
		CHNameCHmapping[i].TextAlignment = Right;
		CHNameCHmapping[i].Dynamic = No;

		//
		for(j = 0; j < CHANNEL_BUFFER_LENGTH;j++ )
		{
			STlistBoxSetListItem(__CHBufferItemsLabel[j], j, &CHmappingList[i]);
		}

		STtextBoxInit(&CHNameCHmapping[i]);
		STlistBoxInit(&CHmappingList[i]);
	}


	BackToModelMenuButtonCHMP.Text = __buttonLabelBack;
	BackToModelMenuButtonCHMP.Style = &CHmappingButtonColor;
	BackToModelMenuButtonCHMP.Width = MAIN_BACK_BUTTON_WIDTH;
	BackToModelMenuButtonCHMP.Height = MAIN_BACK_BUTTON_HEIGHT;
	BackToModelMenuButtonCHMP.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonCHMP.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonCHMP.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonCHMP);
}

