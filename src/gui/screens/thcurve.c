/*
 * thcurve.c
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

#define CURVE_POINT_Y				205
#define CURVE_POINT_NUM_X			1
#define CURVE_POINT_POINT_X			20
#define CURVE_POINT_RATIO_X			65
#define CURVE_POINT_STEP_Y			30
#define CURVE_POINT_STEP_X			120

STScreenStyleTypeDef THcurveStyle = { 0 };
STScreenTypeDef THcurveST = { 0 };

STlistBoxTypeDef THCurveTypeList = { 0 };

STinputBoxTypeDef THexpoRatePx1 = {0};

STColorStyleTypeDef THcurvePlotColor = { 0 };
STColorStyleTypeDef THcurveButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonTHC = { 0 };

STColorStyleTypeDef THexpoRateColor = { 0 };
STinputBoxTypeDef THexpoRate = { 0 };


STColorStyleTypeDef THexpoRateNameColor = { 0 };
STtextBoxTypeDef THexpoRateName = { 0 };

STgraphPlotTypeDef THCurvePLot = { 0 };

void _THcurve() {
//	uint16_t i = 0;

	if (STscreenShowNow(&THcurveST)) {


		/*
		 * ������ ������ ����������
		 */
//		for (i = 500; i < GUI_CH_MAX; i++) {
//			STgraphPlotDrawDot(i, ___THcurve[i], &THCurvePLot);
//		}
//		STgraphPlotSetValue(RCChanelGetValue(&RCChanel_1), &THCurvePLot);


		if (STbuttonPressed(&BackToModelMenuButtonTHC)) {
			STappSetScreen(ModelMenu, &STApp);
//			STsaveSettings(&ModelSettings[0]);
//			STcopySettings(&ModelSettings[ModelSettings[0].ID],
//					&ModelSettings[0]);
		}

		/*
		 * ��������� ���� ������ ������
		 */
		if (STlistBoxValueChange(&THCurveTypeList)) {
//			RCChanelSetCurveType(STlistBoxGetValue(&THCurveTypeList), &RCChanel_1);
			THcurveFill();
			STgraphPlotClear(&THCurvePLot);
		}

		/*
		 * ��������� �������� �������� ����������
		 */
		if (STinputBoxValueChange(&THexpoRatePx1)) {
//			RCChanelSetExpoX((uint16_t) STinputBoxGetValue(&THexpoRatePx1),  &RCChanel_1);;
			THcurveFill();
			STgraphPlotClear(&THCurvePLot);
		}
	}

	STscreenShow(&THcurveST);

	if (STscreenOnShow(&THcurveST))
	{
//		STinputBoxSetValue(RCChanelGetExpoX(&RCChanel_1), &THexpoRatePx1);
//		STlistBoxSetValue(RCChanelGetCurveType(&RCChanel_1), &THCurveTypeList);
	}


	STgraphPlotShow(&THCurvePLot);
	STinputBoxShow(&THexpoRatePx1);
	STlistBoxShow(&THCurveTypeList);

	STbuttonShow(&BackToModelMenuButtonTHC);
}
void _THcurveInit() {

	/*
	 *  ������������� �������� ������
	 */
	THcurveStyle.HeaderFont = &verdana_8pt_bold;
	THcurveStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	THcurveStyle.HeaderColor = MAIN_HEADER_COLOR;
	THcurveStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	THcurveStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	THcurveST.App = &STApp;
	THcurveST.Style = &THcurveStyle;
	THcurveST.ScreenID = THcurve;
	THcurveST.FullScreen = Yes;
	THcurveST.Width = GUI_MAX_WIDTH_PX;
	THcurveST.Height = GUI_MAX_HEIGHT_PX;
	THcurveST.Header = Yes;
	THcurveST.HeaderText = "TH CURVE";
	THcurveST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	THcurveButtonColor.Font = &verdana_8pt_bold;
	THcurveButtonColor.ParentScreen = &THcurveST;
	THcurveButtonColor.Color = BASE_BUTTON_COLOR;
	THcurveButtonColor.TextColor = BASE_BUTTON_COLOR;
	THcurveButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	THcurveButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	THcurveButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	THcurveButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	THcurveButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	THcurveButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	THcurveButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	THcurvePlotColor.Color = BASE_GRAPHPLOT_COLOR;
	THcurvePlotColor.BackgroundColor = BASE_GRAPHPLOT_BACK_COLOR;
	THcurvePlotColor.BorderThickness = BASE_GRAPHPLOT_BORDER_THICKNESS;
	THcurvePlotColor.BorderColor = BASE_GRAPHPLOT_BORDER_COLOR;

	THexpoRateNameColor.ParentScreen = &THcurveST;
	THexpoRateNameColor.Font = &verdana_8pt_bold;
	THexpoRateNameColor.Color = BASE_TEXT_COLOR;
	THexpoRateNameColor.TextColor = BASE_TEXT_COLOR;
	THexpoRateNameColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	THexpoRateNameColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	THexpoRateNameColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	THexpoRateColor.ParentScreen = &THcurveST;
	THexpoRateColor.Font = &verdana_8pt_bold;
	THexpoRateColor.Color = BASE_INPUT_COLOR;
	THexpoRateColor.TextColor = BASE_INPUT_TEXT_COLOR;
	THexpoRateColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	THexpoRateColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	THexpoRateColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	THexpoRateColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	THexpoRateColor.FocusColor = FOCUS_INPUT_COLOR;
	THexpoRateColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	THexpoRateColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	THexpoRateColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	THCurvePLot.ParentScreen = &THcurveST;
	THCurvePLot.Style = &THcurvePlotColor;
	THCurvePLot.Width = 174;
	THCurvePLot.Height = 174;
	THCurvePLot.X = 60;
	THCurvePLot.Y = 25;
//	THCurvePLot.Crosshairs = Yes;
	THCurvePLot.Border = Yes;
	THCurvePLot.HiLimitX = GUI_CH_MAX;
	THCurvePLot.LoLimitX = 500;
	THCurvePLot.HiLimitY = GUI_CH_MAX;
	THCurvePLot.LoLimitY = 500;

	THexpoRateName.Text = "EXP Rate";
	THexpoRateName.Style = &THexpoRateNameColor;
	THexpoRateName.Width = 60;
	THexpoRateName.Height = 26;
	THexpoRateName.X = EXP_RATE_NAME_X;
	THexpoRateName.Y = EXP_RATE_Y;
	THexpoRateName.Border = No;
	THexpoRateName.TextAlignment = Right;
	THexpoRateName.Dynamic = No;

	THexpoRate.Style = &THexpoRateColor;
	THexpoRate.Width = 40;
	THexpoRate.Height = INPUT_BASE_HEIGHT;
	THexpoRate.X = EXP_RATE_INPUT;
	THexpoRate.Y = EXP_RATE_Y;
	THexpoRate.HiLimit = 500;
	THexpoRate.LoLimit = 0;
	THexpoRate.TextAlignment = Centre;
	THexpoRate.Border = Yes;
	THexpoRate.Dynamic = Yes;
	THexpoRate.Units = "%";

	THCurveTypeList.Style = &THexpoRateColor;
	THCurveTypeList.Width = 40;
	THCurveTypeList.Height = 30;
	THCurveTypeList.X = 1;
	THCurveTypeList.Y = 25;
	THCurveTypeList.HiLimit = 1;
	THCurveTypeList.LoLimit = 0;
	THCurveTypeList.TextAlignment = Centre;
	THCurveTypeList.Border = Yes;
	THCurveTypeList.Dynamic = Yes;

	THexpoRatePx1.Style = &THexpoRateColor;
	THexpoRatePx1.Width = 50;
	THexpoRatePx1.Height = INPUT_BASE_HEIGHT;
	THexpoRatePx1.X = 1;
	THexpoRatePx1.Y = EXP_RATE_Y;
	THexpoRatePx1.HiLimit = EXPO_RATE_MAX;
	THexpoRatePx1.LoLimit = EXPO_RATE_MIN;
	THexpoRatePx1.TextAlignment = Centre;
	THexpoRatePx1.Border = Yes;
	THexpoRatePx1.Dynamic = Yes;
	THexpoRatePx1.UnitsShow = Yes;
	THexpoRatePx1.Units = "%";

	STlistBoxSetListItem("LIN", Linear, &THCurveTypeList);
	STlistBoxSetListItem("QEXP", QuickExponential, &THCurveTypeList);
//	STlistBoxSetListItem("EXP", Exponential, &THCurveTypeList);
//	STlistBoxSetListItem("VTR", VTR, &THCurveTypeList);
//	STlistBoxSetListItem("CRV", Curve, &THCurveTypeList);

	BackToModelMenuButtonTHC.Text = __buttonLabelBack;
	BackToModelMenuButtonTHC.Style = &THcurveButtonColor;
	BackToModelMenuButtonTHC.Width = 60;
	BackToModelMenuButtonTHC.Height = 26;
	BackToModelMenuButtonTHC.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonTHC.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonTHC.Border = Yes;

	STlistBoxInit(&THCurveTypeList);
	STinputBoxInit(&THexpoRatePx1);
	STtextBoxInit(&THexpoRateName);

	STbuttonInit(&BackToModelMenuButtonTHC);
}
