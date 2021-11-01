/*
 * font.h
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

#ifndef FONTS_FONT_H_
#define FONTS_FONT_H_

#include "stm32f4xx_hal.h"

#define LETTER_SPACING 2

typedef struct {
	uint16_t width; /* Character width */
	uint16_t start; /* Character offset */
} FONT_CHAR_INFO;

typedef struct {
	const uint8_t Height; /*  Character height */
	const uint8_t HeightPx; /*	Character height dot */
	const uint8_t FirstChar; /*  Start character */
	const uint8_t LastChar; /*  End character */
	const FONT_CHAR_INFO *FontTable; /*  Character descriptor array */
	const uint8_t *FontBitmaps; /*  Character bitmap array */
} FONT_INFO;

uint16_t GetTextLenghtPx(const char* Text, const FONT_INFO *Font);
uint16_t GetCharLenghtPx(char Key, const FONT_INFO *Font);

#endif /* FONTS_FONT_H_ */
