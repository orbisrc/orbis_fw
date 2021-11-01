/*
 * font.c
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

#include "gui/fonts/font.h"

uint16_t GetTextLenghtPx(const char* Text, const FONT_INFO *Font) {

	volatile const FONT_CHAR_INFO *Char_Info;
	volatile uint16_t Char_offset;
	volatile uint16_t TextLenghtCounter = 0;

	while (*Text >= Font->FirstChar && *Text <= Font->LastChar) {
		Char_Info = Font->FontTable;

		Char_offset = (uint16_t) *Text - (uint16_t) Font->FirstChar; //32

		Char_Info = Char_Info + Char_offset;

		TextLenghtCounter += (Char_Info->width + LETTER_SPACING);

		Text++;
	}

	return TextLenghtCounter;
}

uint16_t GetCharLenghtPx(char Key, const FONT_INFO *Font) {
	const FONT_CHAR_INFO *Char_Info;
	uint16_t Char_offset;

	Char_Info = Font->FontTable;

	Char_offset = (uint16_t) Key - 32;

	Char_Info = Char_Info + Char_offset;

	return Char_Info->width;
}
