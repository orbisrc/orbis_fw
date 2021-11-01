/*
 * chsettings.c
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
#include "gui/stapp.h"
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include "core/buffer.h"
#include <core/rccurve.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>

#define CHSETTINGS_FIELD_WIDTH 70
#define CHSETTINGS_FIELD_HEIGHT 25
#define CHSETTINGS_FIELD_NAME_X 1
#define CHSETTINGS_FIELD_NAME_Y 50
#define CHSETTINGS_FIELD_NAME_STEP 30
#define CHSETTINGS_FIELD_NAME_NUM 3

#define CHSETTINGS_FIELD_INPUT_WIDTH 50
#define CHSETTINGS_FIELD_INPUT_HEIGHT 25
#define CHSETTINGS_FIELD_INPUT_X CHSETTINGS_FIELD_WIDTH + 5
#define CHSETTINGS_NAME_Y 20
#define CHSETTINGS_ENDPOINT_Y 50
#define CHSETTINGS_INVERT_Y 80
#define CHSETTINGS_TRIM_Y 110
#define CHSETTINGS_CURVES_Y 140

STScreenStyleTypeDef CHsettingsStyle = {0};
STScreenTypeDef CHsettingsST = {0};
STColorStyleTypeDef CHsettingsText = {0};
STColorStyleTypeDef CHsettingsPLotColor = {0};
STColorStyleTypeDef CHsettingsButtonColor = {0};
STColorStyleTypeDef CHsettingsCheckBoxColor = {0};

STinputBoxTypeDef CHsettingsLowRate = {0};
STinputBoxTypeDef CHsettingsHightRate = {0};
STinputBoxTypeDef CHsettingsTrim = {0};
STColorStyleTypeDef CHsettingsInputBoxColor = {0};
STinputBoxTypeDef CHsettingsexpoRate = {0};
STlistBoxTypeDef CHsettingsCurveTypeList = {0};
STlistBoxTypeDef CHsettingsCurentModelList = {0};
STgraphPlotTypeDef CHsettingsPLot = {0};
STbuttonTypeDef BackToModelMenuButtonCHSTT = {0};
STcheckBoxTypeDef CHsettingsinvertCheckBox = {0};

const char CHsettingsFieldNameLabel[CHSETTINGS_FIELD_NAME_NUM][16] = {"End point",
																	  "Invert",
																	  "Trim"};

STtextBoxTypeDef CHsettingsFieldName[CHSETTINGS_FIELD_NAME_NUM] = {0};

void _CHsettings()
{

	static uint16_t SelectedChannel = 0;
	uint16_t i = 0;

	if (STscreenShowNow(&CHsettingsST))
	{

		if (STbuttonPressed(&BackToModelMenuButtonCHSTT))
		{
			STappSetScreen(ModelMenu, &STApp);
		}

		if (STlistBoxValueChange(&CHsettingsCurentModelList))
		{
			SelectedChannel = STlistBoxGetValue(&CHsettingsCurentModelList);
			_CHsettingsLoadToItem(SelectedChannel);
		}

		if (STinputBoxValueChange(&CHsettingsLowRate))
		{
			RCChanelSetLowRate((uint16_t)STinputBoxGetValue(&CHsettingsLowRate), &RCChanel[SelectedChannel]);
		}

		if (STinputBoxValueChange(&CHsettingsHightRate))
		{
			RCChanelSetHightRate((uint16_t)STinputBoxGetValue(&CHsettingsHightRate), &RCChanel[SelectedChannel]);
		}

		if (STcheckBoxValueChange(&CHsettingsinvertCheckBox))
		{
			RCChanelSetInvertState(STcheckBoxGetState(&CHsettingsinvertCheckBox), &RCChanel[SelectedChannel]);
		}

		if (STinputBoxValueChange(&CHsettingsTrim))
		{
			RCChanelSetTrim(STinputBoxGetValue(&CHsettingsTrim), &RCChanel[SelectedChannel]);
		}

		if (STinputBoxValueChange(&CHsettingsexpoRate))
		{
			RCChanelSetExpoX((uint16_t)STinputBoxGetValue(&CHsettingsexpoRate), &RCChanel[SelectedChannel]);
			STcurveFill(SelectedChannel);
			STgraphPlotClear(&CHsettingsPLot);
		}

		if (STlistBoxValueChange(&CHsettingsCurveTypeList))
		{
			RCChanelSetCurveType(STlistBoxGetValue(&CHsettingsCurveTypeList), &RCChanel[SelectedChannel]);
			STcurveFill(SelectedChannel);
			STgraphPlotClear(&CHsettingsPLot);
		}

		for (i = 0; i < GUI_CH_MAX; i++)
		{
			STgraphPlotDrawDot(i, ___Curve[SelectedChannel][i], &CHsettingsPLot);
		}

		STgraphPlotSetValue(RCChanelGetValue(&RCChanel[SelectedChannel]), &CHsettingsPLot);
	}

	STscreenShow(&CHsettingsST);

	if (STscreenOnShow(&CHsettingsST))
	{
		_CHsettingsLoadToItem(SelectedChannel);
	}

	for (i = 0; i < CHSETTINGS_FIELD_NAME_NUM; i++)
	{
		STtextBoxShow(&CHsettingsFieldName[i]);
	}

	STlistBoxShow(&CHsettingsCurentModelList);
	STinputBoxShow(&CHsettingsHightRate);
	STinputBoxShow(&CHsettingsLowRate);
	STcheckBoxShow(&CHsettingsinvertCheckBox);
	STinputBoxShow(&CHsettingsTrim);
	STlistBoxShow(&CHsettingsCurveTypeList);
	STinputBoxShow(&CHsettingsexpoRate);
	STgraphPlotShow(&CHsettingsPLot);
	STbuttonShow(&BackToModelMenuButtonCHSTT);
}

void _CHsettingsLoadToItem(uint16_t SelectedChannel)
{
	STlistBoxSetValue(SelectedChannel, &CHsettingsCurentModelList);
	STlistBoxSetValue(RCChanelGetCurveType(&RCChanel[SelectedChannel]), &CHsettingsCurveTypeList);
	STinputBoxSetValue(RCChanelGetLowRate(&RCChanel[SelectedChannel]), &CHsettingsLowRate);
	STinputBoxSetValue(RCChanelGetHighRate(&RCChanel[SelectedChannel]), &CHsettingsHightRate);
	STinputBoxSetValue(RCChanelGetTrim(&RCChanel[SelectedChannel]), &CHsettingsTrim);
	STcheckBoxSetValue(RCChanelGetInvertState(&RCChanel[SelectedChannel]), &CHsettingsinvertCheckBox);
	STinputBoxSetValue(RCChanelGetExpoX(&RCChanel[SelectedChannel]), &CHsettingsexpoRate);
}

void _CHsettingsInit()
{
	uint16_t i = 0;

	CHsettingsStyle.HeaderFont = &verdana_8pt_bold;
	CHsettingsStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	CHsettingsStyle.HeaderColor = MAIN_HEADER_COLOR;
	CHsettingsStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	CHsettingsStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	CHsettingsST.App = &STApp;
	CHsettingsST.Style = &CHsettingsStyle;
	CHsettingsST.ScreenID = ChannelSettings;
	CHsettingsST.FullScreen = Yes;
	CHsettingsST.Width = GUI_MAX_WIDTH_PX;
	CHsettingsST.Height = GUI_MAX_HEIGHT_PX;
	CHsettingsST.Header = Yes;
	CHsettingsST.HeaderText = "CHANNEL SETTINGS";
	CHsettingsST.Footer = No;

	CHsettingsText.ParentScreen = &CHsettingsST;
	CHsettingsText.Font = &verdana_8pt_bold;
	CHsettingsText.Color = BASE_TEXT_COLOR;
	CHsettingsText.TextColor = BASE_TEXT_COLOR;
	CHsettingsText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	CHsettingsText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	CHsettingsText.BorderColor = BASE_TEXT_BORDER_COLOR;

	CHsettingsButtonColor.Font = &verdana_8pt_bold;
	CHsettingsButtonColor.ParentScreen = &CHsettingsST;
	CHsettingsButtonColor.Color = BASE_BUTTON_COLOR;
	CHsettingsButtonColor.TextColor = BASE_BUTTON_COLOR;
	CHsettingsButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	CHsettingsButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	CHsettingsButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	CHsettingsButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	CHsettingsButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	CHsettingsButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	CHsettingsButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	CHsettingsPLotColor.Color = BASE_GRAPHPLOT_COLOR;
	CHsettingsPLotColor.BackgroundColor = BASE_GRAPHPLOT_BACK_COLOR;
	CHsettingsPLotColor.BorderThickness = BASE_GRAPHPLOT_BORDER_THICKNESS;
	CHsettingsPLotColor.BorderColor = BASE_GRAPHPLOT_BORDER_COLOR;

	CHsettingsInputBoxColor.ParentScreen = &CHsettingsST;
	CHsettingsInputBoxColor.Font = &verdana_8pt_bold;
	CHsettingsInputBoxColor.Color = BASE_INPUT_COLOR;
	CHsettingsInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	CHsettingsInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	CHsettingsInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	CHsettingsInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	CHsettingsInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	CHsettingsInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	CHsettingsInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	CHsettingsInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	CHsettingsInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	CHsettingsCheckBoxColor.ParentScreen = &CHsettingsST;
	CHsettingsCheckBoxColor.Font = &verdana_8pt_bold;
	CHsettingsCheckBoxColor.Color = BASE_CHECKBOX_OFF_COLOR;
	CHsettingsCheckBoxColor.Color2 = BASE_CHECKBOX_ON_COLOR;
	CHsettingsCheckBoxColor.TextColor = BASE_CHECKBOX_TEXT_COLOR;
	CHsettingsCheckBoxColor.BorderThickness = BASE_CHECKBOX_BORDER_THICKNESS;
	CHsettingsCheckBoxColor.BorderColor = BASE_CHECKBOX_BORDER_COLOR;
	CHsettingsCheckBoxColor.FocusBorderThickness = FOCUS_CHECKBOX_BORDER_THICKNESS;
	CHsettingsCheckBoxColor.FocusBorderColor = FOCUS_CHECKBOX_BORDER_COLOR;

	CHsettingsHightRate.Style = &CHsettingsInputBoxColor;
	CHsettingsHightRate.Width = INPUT_BASE_WIDTH;
	CHsettingsHightRate.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsHightRate.X = CHSETTINGS_FIELD_INPUT_X + 60;
	CHsettingsHightRate.Y = CHSETTINGS_ENDPOINT_Y;
	CHsettingsHightRate.HiLimit = BASE_RATE;
	CHsettingsHightRate.LoLimit = 0;
	CHsettingsHightRate.TextAlignment = Centre;
	CHsettingsHightRate.Border = Yes;
	CHsettingsHightRate.Dynamic = Yes;
	CHsettingsHightRate.Units = "%";
	CHsettingsHightRate.UnitsShow = Yes;

	CHsettingsLowRate.Style = &CHsettingsInputBoxColor;
	CHsettingsLowRate.Width = INPUT_BASE_WIDTH;
	CHsettingsLowRate.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsLowRate.X = CHSETTINGS_FIELD_INPUT_X;
	CHsettingsLowRate.Y = CHSETTINGS_ENDPOINT_Y;
	CHsettingsLowRate.HiLimit = BASE_RATE;
	CHsettingsLowRate.LoLimit = 0;
	CHsettingsLowRate.TextAlignment = Centre;
	CHsettingsLowRate.Border = Yes;
	CHsettingsLowRate.Dynamic = Yes;
	CHsettingsLowRate.Units = "%";
	CHsettingsLowRate.UnitsShow = Yes;

	CHsettingsPLot.ParentScreen = &CHsettingsST;
	CHsettingsPLot.Style = &CHsettingsPLotColor;
	CHsettingsPLot.Width = 174;
	CHsettingsPLot.Height = 174;
	CHsettingsPLot.X = 60;
	CHsettingsPLot.Y = CHSETTINGS_CURVES_Y;
	CHsettingsPLot.Crosshairs = Yes;
	CHsettingsPLot.Border = Yes;
	CHsettingsPLot.HiLimitX = GUI_CH_MAX;
	CHsettingsPLot.LoLimitX = GUI_CH_MIN;
	CHsettingsPLot.HiLimitY = GUI_CH_MAX;
	CHsettingsPLot.LoLimitY = GUI_CH_MIN;

	CHsettingsCurentModelList.Style = &CHsettingsInputBoxColor;
	CHsettingsCurentModelList.Width = 70;
	CHsettingsCurentModelList.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsCurentModelList.X = 1;
	CHsettingsCurentModelList.Y = CHSETTINGS_NAME_Y;
	CHsettingsCurentModelList.HiLimit = MAX_RC_CHANNEL - 1;
	CHsettingsCurentModelList.LoLimit = 0;
	CHsettingsCurentModelList.TextAlignment = Centre;
	CHsettingsCurentModelList.Border = Yes;
	CHsettingsCurentModelList.Dynamic = Yes;

	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		STlistBoxSetListItem(__CHLabel[i], i, &CHsettingsCurentModelList);
	}

	CHsettingsCurveTypeList.Style = &CHsettingsInputBoxColor;
	CHsettingsCurveTypeList.Width = 50;
	CHsettingsCurveTypeList.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsCurveTypeList.X = 1;
	CHsettingsCurveTypeList.Y = CHSETTINGS_CURVES_Y;
	CHsettingsCurveTypeList.HiLimit = 2;
	CHsettingsCurveTypeList.LoLimit = 0;
	CHsettingsCurveTypeList.TextAlignment = Centre;
	CHsettingsCurveTypeList.Border = Yes;
	CHsettingsCurveTypeList.Dynamic = Yes;

	STlistBoxSetListItem("LIN", Linear, &CHsettingsCurveTypeList);
	STlistBoxSetListItem("QEXP", QuickExponential, &CHsettingsCurveTypeList);
	//	STlistBoxSetListItem("EXP", Exponential, &CHsettingsCurveTypeList);
	//	STlistBoxSetListItem("VTR", VTR, &CHsettingsCurveTypeList);
	//	STlistBoxSetListItem("CRV", Curve, &CHsettingsCurveTypeList);

	CHsettingsexpoRate.Style = &CHsettingsInputBoxColor;
	CHsettingsexpoRate.Width = 50;
	CHsettingsexpoRate.Height = CHSETTINGS_FIELD_HEIGHT;
	CHsettingsexpoRate.X = 1;
	CHsettingsexpoRate.Y = 175;
	CHsettingsexpoRate.HiLimit = EXPO_RATE_MAX;
	CHsettingsexpoRate.LoLimit = EXPO_RATE_MIN;
	CHsettingsexpoRate.TextAlignment = Centre;
	CHsettingsexpoRate.Border = Yes;
	CHsettingsexpoRate.Dynamic = Yes;
	CHsettingsexpoRate.UnitsShow = Yes;
	CHsettingsexpoRate.Units = "%";

	CHsettingsinvertCheckBox.OnText = __buttonLabelCheckBoxOn;
	CHsettingsinvertCheckBox.OffText = __buttonLabelCheckBoxOff;
	CHsettingsinvertCheckBox.Style = &CHsettingsCheckBoxColor;
	CHsettingsinvertCheckBox.Width = CHSETTINGS_FIELD_INPUT_WIDTH;
	CHsettingsinvertCheckBox.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsinvertCheckBox.X = CHSETTINGS_FIELD_INPUT_X;
	CHsettingsinvertCheckBox.Y = CHSETTINGS_INVERT_Y;
	CHsettingsinvertCheckBox.Border = Yes;
	CHsettingsinvertCheckBox.Dynamic = Yes;

	CHsettingsTrim.Style = &CHsettingsInputBoxColor;
	CHsettingsTrim.Width = INPUT_BASE_WIDTH;
	CHsettingsTrim.Height = CHSETTINGS_FIELD_INPUT_HEIGHT;
	CHsettingsTrim.X = CHSETTINGS_FIELD_INPUT_X;
	CHsettingsTrim.Y = CHSETTINGS_TRIM_Y;
	CHsettingsTrim.HiLimit = TRIM_CH_MAX;
	CHsettingsTrim.LoLimit = TRIM_CH_MIN;
	CHsettingsTrim.TextAlignment = Centre;
	CHsettingsTrim.Border = Yes;
	CHsettingsTrim.Dynamic = Yes;
	CHsettingsTrim.Units = "%";
	CHsettingsTrim.UnitsShow = No;

	for (i = 0; i < CHSETTINGS_FIELD_NAME_NUM; i++)
	{
		CHsettingsFieldName[i].Text = CHsettingsFieldNameLabel[i];
		CHsettingsFieldName[i].Style = &CHsettingsText;
		CHsettingsFieldName[i].Width = CHSETTINGS_FIELD_WIDTH;
		CHsettingsFieldName[i].Height = CHSETTINGS_FIELD_HEIGHT;
		CHsettingsFieldName[i].X = CHSETTINGS_FIELD_NAME_X;
		CHsettingsFieldName[i].Y = CHSETTINGS_FIELD_NAME_Y + CHSETTINGS_FIELD_NAME_STEP * i;
		CHsettingsFieldName[i].Border = No;
		CHsettingsFieldName[i].TextAlignment = Right;
		CHsettingsFieldName[i].Dynamic = No;

		STtextBoxInit(&CHsettingsFieldName[i]);
	}

	BackToModelMenuButtonCHSTT.Text = __buttonLabelBack;
	BackToModelMenuButtonCHSTT.Style = &CHsettingsButtonColor;
	BackToModelMenuButtonCHSTT.Width = MAIN_BACK_BUTTON_WIDTH;
	BackToModelMenuButtonCHSTT.Height = MAIN_BACK_BUTTON_HEIGHT;
	BackToModelMenuButtonCHSTT.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonCHSTT.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonCHSTT.Border = Yes;

	STlistBoxInit(&CHsettingsCurentModelList);
	STinputBoxInit(&CHsettingsLowRate);
	STinputBoxInit(&CHsettingsHightRate);
	STcheckBoxInit(&CHsettingsinvertCheckBox);
	STinputBoxInit(&CHsettingsTrim);
	STlistBoxInit(&CHsettingsCurveTypeList);
	STinputBoxInit(&CHsettingsexpoRate);
	STgraphPlotInit(&CHsettingsPLot);
	STbuttonInit(&BackToModelMenuButtonCHSTT);
}
