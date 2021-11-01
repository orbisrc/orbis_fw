/*
 * gui.h
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

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

#define BUTTON_LABEL_SIZE			16//MAX_RC_CHANNEL

extern const char * __buttonLabelYes;
extern const char * __buttonLabelNo;
extern const char * __buttonLabelBack;
extern const char * __buttonLabelNext;
extern const char * __buttonLabelMenu;
extern const char * __buttonLabelCheckBoxOn;
extern const char * __buttonLabelCheckBoxOff;

extern const char * __CH1label;
extern const char * __CH2label;
extern const char * __CH3label;
extern const char * __CH4label;
extern const char * __CH5label;
extern const char * __CH6label;
extern const char * __CH7label;
extern const char * __CH8label;

extern const char * __CHLabel[];
extern const char *  __CHLabelShort[];

extern const  char  * __USBmodeLaberl[];

void GUI_Init(void);
void GUI_DFU(void);
void GUI(void);

#endif /* GUI_GUI_H_ */
