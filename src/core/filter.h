/*
 * filter.h
 *
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

#ifndef INC_STCORE_FILTER_H_
#define INC_STCORE_FILTER_H_

#include "stconfig.h"
#include "main.h"

#if ADC_LOOP > 2000
/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 4000 Hz

* 0 Hz - 100 Hz
  gain = 1
  desired ripple = 0.5 dB
  actual ripple = 0.34785384059391683 dB

* 200 Hz - 2000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -40.579467270488394 dB

*/

#define __FILTER_TAP_NUM 73


#elif ADC_LOOP < 2000

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 1000 Hz

* 0 Hz - 40 Hz
  gain = 1
  desired ripple = 0.5 dB
  actual ripple = 0.3039822217392819 dB

* 80 Hz - 500 Hz
  gain = 0
  desired attenuation = -80 dB
  actual attenuation = -81.75016530919748 dB

*/

#define __FILTER_TAP_NUM 81

#else

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 2000 Hz

* 0 Hz - 40 Hz
  gain = 1
  desired ripple = 0.1 dB
  actual ripple = 0.06231903600372993 dB

* 120 Hz - 1000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -41.460733464868525 dB

*/

#define __FILTER_TAP_NUM 59

#endif

typedef struct {
  float history[__FILTER_TAP_NUM];
  unsigned int last_index;
} __Filter;

extern __Filter __FIRFilter[ADC1_CH_NUM+1];

extern uint16_t ADCbuff[ADC1_CH_NUM];

void __Filter_init(__Filter* f);
void __Filter_put(__Filter* f, float input);
float __Filter_get(__Filter* f);

#endif /* INC_STCORE_FILTER_H_ */
