/*
 * stcurve.c
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
#include <core/rccurve.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>

STScreenStyleTypeDef STcurveStyle = { 0 };
STScreenTypeDef STcurveST = { 0 };

STlistBoxTypeDef STCurveTypeList = { 0 };

STColorStyleTypeDef STcurvePlotColor = { 0 };
STColorStyleTypeDef STcurveButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonSTC = { 0 };

STColorStyleTypeDef STexpoRatePx1Color = { 0 };
STinputBoxTypeDef STexpoRatePx1 = { 0 };
STinputBoxTypeDef STexpoRatePy1 = { 0 };

STColorStyleTypeDef STexpoRatePx1NameColor = { 0 };
STtextBoxTypeDef STexpoRatePx1Name = { 0 };

STgraphPlotTypeDef STCurvePLot = { 0 };

void _STcurve() {
	uint16_t i = 0;

	if (STscreenShowNow(&STcurveST)) {
		if (STbuttonPressed(&BackToModelMenuButtonSTC)) {
			STappSetScreen(ModelMenu, &STApp);
//			STsaveSettings(&ModelSettings[0]);
//			STcopySettings(&ModelSettings[ModelSettings[0].ID],
//					&ModelSettings[0]);
		}

		/*
		 * ��������� ���� ������ ������
		 */
		if (STlistBoxValueChange(&STCurveTypeList)) {
//			RCChanelSetCurveType(STlistBoxGetValue(&STCurveTypeList), &RCChanel_0);
//			STcurveFill();
//			STgraphPlotClear(&STCurvePLot);
		}

		/*
		 * ��������� �������� �������� ����������
		 */
//		if (STinputBoxValueChange(&STexpoRatePy1)) {
//			RCChanelSetExpoY((uint16_t) STinputBoxGetValue(&STexpoRatePy1),	&RCChanel_0);
//			STcurveFill();
//			STgraphPlotClear(&STCurvePLot);
//		}

		if (STinputBoxValueChange(&STexpoRatePx1)) {
//			RCChanelSetExpoX((uint16_t) STinputBoxGetValue(&STexpoRatePx1), &RCChanel_0);
//			STcurveFill();
//			STgraphPlotClear(&STCurvePLot);
		}

		/*
		 * ��������� ���������� �� �������
		 */
		for (i = 0; i < GUI_CH_MAX; i++) {
//			STgraphPlotDrawDot(i, ___STcurve[i], &STCurvePLot);
		}

	//	STgraphPlotSetValue(RCChanelGetValue(&RCChanel_0), &STCurvePLot);
	}

	STscreenShow(&STcurveST);

	if (STscreenOnShow(&STcurveST)) {
//		STinputBoxSetValue(RCChanelGetExpoX(&RCChanel_0), &STexpoRatePx1);
//		STinputBoxSetValue(RCChanelGetExpoY(&RCChanel_0), &STexpoRatePy1);
//		STlistBoxSetValue(RCChanelGetCurveType(&RCChanel_0), &STCurveTypeList);
	}

	STgraphPlotShow(&STCurvePLot);
	STinputBoxShow(&STexpoRatePx1);
//	STinputBoxShow(&STexpoRatePy1);
//	STtextBoxShow(&STexpoRatePx1Name);
	STlistBoxShow(&STCurveTypeList);
	STbuttonShow(&BackToModelMenuButtonSTC);
}
void _STcurveInit() {
	/*
	 *  ������������� �������� ������
	 */
	STcurveStyle.HeaderFont = &verdana_8pt_bold;
	STcurveStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	STcurveStyle.HeaderColor = MAIN_HEADER_COLOR;
	STcurveStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	STcurveStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	STcurveST.App = &STApp;
	STcurveST.Style = &STcurveStyle;
	STcurveST.ScreenID = STcurve;
	STcurveST.FullScreen = Yes;
	STcurveST.Width = GUI_MAX_WIDTH_PX;
	STcurveST.Height = GUI_MAX_HEIGHT_PX;
	STcurveST.Header = Yes;
	STcurveST.HeaderText = "ST CURVE";
	STcurveST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	STcurveButtonColor.Font = &verdana_8pt_bold;
	STcurveButtonColor.ParentScreen = &STcurveST;
	STcurveButtonColor.Color = BASE_BUTTON_COLOR;
	STcurveButtonColor.TextColor = BASE_BUTTON_COLOR;
	STcurveButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	STcurveButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	STcurveButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	STcurveButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	STcurveButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	STcurveButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	STcurveButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	STcurvePlotColor.Color = BASE_GRAPHPLOT_COLOR;
	STcurvePlotColor.BackgroundColor = BASE_GRAPHPLOT_BACK_COLOR;
	STcurvePlotColor.BorderThickness = BASE_GRAPHPLOT_BORDER_THICKNESS;
	STcurvePlotColor.BorderColor = BASE_GRAPHPLOT_BORDER_COLOR;

	STexpoRatePx1NameColor.ParentScreen = &STcurveST;
	STexpoRatePx1NameColor.Font = &verdana_8pt_bold;
	STexpoRatePx1NameColor.Color = BASE_TEXT_COLOR;
	STexpoRatePx1NameColor.TextColor = BASE_TEXT_COLOR;
	STexpoRatePx1NameColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	STexpoRatePx1NameColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	STexpoRatePx1NameColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	STexpoRatePx1Color.ParentScreen = &STcurveST;
	STexpoRatePx1Color.Font = &verdana_8pt_bold;
	STexpoRatePx1Color.Color = BASE_INPUT_COLOR;
	STexpoRatePx1Color.TextColor = BASE_INPUT_TEXT_COLOR;
	STexpoRatePx1Color.BackgroundColor = BASE_INPUT_BACK_COLOR;
	STexpoRatePx1Color.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	STexpoRatePx1Color.BorderColor = BASE_INPUT_BORDER_COLOR;
	STexpoRatePx1Color.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	STexpoRatePx1Color.FocusColor = FOCUS_INPUT_COLOR;
	STexpoRatePx1Color.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	STexpoRatePx1Color.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	STexpoRatePx1Color.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	STCurvePLot.ParentScreen = &STcurveST;
	STCurvePLot.Style = &STcurvePlotColor;
	STCurvePLot.Width = 174;
	STCurvePLot.Height = 174;
	STCurvePLot.X = 60;
	STCurvePLot.Y = 25;
	STCurvePLot.Crosshairs = Yes;
	STCurvePLot.Border = Yes;
	STCurvePLot.HiLimitX = GUI_CH_MAX;
	STCurvePLot.LoLimitX = GUI_CH_MIN;
	STCurvePLot.HiLimitY = GUI_CH_MAX;
	STCurvePLot.LoLimitY = GUI_CH_MIN;

	STexpoRatePx1.Style = &STexpoRatePx1Color;
	STexpoRatePx1.Width = 50;
	STexpoRatePx1.Height = INPUT_BASE_HEIGHT;
	STexpoRatePx1.X = 1;
	STexpoRatePx1.Y = EXP_RATE_Y;
	STexpoRatePx1.HiLimit = EXPO_RATE_MAX;
	STexpoRatePx1.LoLimit = EXPO_RATE_MIN;
	STexpoRatePx1.TextAlignment = Centre;
	STexpoRatePx1.Border = Yes;
	STexpoRatePx1.Dynamic = Yes;
	STexpoRatePx1.UnitsShow = Yes;
	STexpoRatePx1.Units = "%";

	STexpoRatePy1.Style = &STexpoRatePx1Color;
	STexpoRatePy1.Width = 50;
	STexpoRatePy1.Height = INPUT_BASE_HEIGHT;
	STexpoRatePy1.X = 1;
	STexpoRatePy1.Y = 90;
	STexpoRatePy1.HiLimit = EXPO_RATE_MAX;
	STexpoRatePy1.LoLimit = EXPO_RATE_MIN;
	STexpoRatePy1.TextAlignment = Centre;
	STexpoRatePy1.Border = Yes;
	STexpoRatePy1.Dynamic = Yes;
	STexpoRatePy1.UnitsShow = Yes;
	STexpoRatePy1.Units = "%";

	STCurveTypeList.Style = &STexpoRatePx1Color;
	STCurveTypeList.Width = 50;
	STCurveTypeList.Height = 30;
	STCurveTypeList.X = 1;
	STCurveTypeList.Y = 25;
	STCurveTypeList.HiLimit = 4;
	STCurveTypeList.LoLimit = 0;
	STCurveTypeList.TextAlignment = Centre;
	STCurveTypeList.Border = Yes;
	STCurveTypeList.Dynamic = Yes;

	STlistBoxSetListItem("LIN", Linear, &STCurveTypeList);
	STlistBoxSetListItem("QEXP", QuickExponential, &STCurveTypeList);
	STlistBoxSetListItem("EXP", Exponential, &STCurveTypeList);
	STlistBoxSetListItem("VTR", VTR, &STCurveTypeList);
	STlistBoxSetListItem("CRV", Curve, &STCurveTypeList);

	BackToModelMenuButtonSTC.Text = __buttonLabelBack;
	BackToModelMenuButtonSTC.Style = &STcurveButtonColor;
	BackToModelMenuButtonSTC.Width = 60;
	BackToModelMenuButtonSTC.Height = 26;
	BackToModelMenuButtonSTC.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonSTC.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonSTC.Border = Yes;

	STlistBoxInit(&STCurveTypeList);
//	STtextBoxInit(&STexpoRatePx1Name);
	STinputBoxInit(&STexpoRatePx1);
//	STinputBoxInit(&STexpoRatePy1);

	STbuttonInit(&BackToModelMenuButtonSTC);
}
