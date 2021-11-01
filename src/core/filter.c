	/*
 * filter.c
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


	Filter from http://t-filter.engineerjs.com/

*/

#include <core/filter.h>

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


const static float filter_taps[__FILTER_TAP_NUM]   __attribute__((section("ccmram"))) =
{
 0.005884881073845589,
  0.002612123638473842,
  0.0029702829795608537,
  0.0031807946781945333,
  0.003194092233025482,
  0.002966860667208367,
  0.0024648497866630857,
  0.001669476207735804,
  0.0005804746148391294,
  -0.0007837403145703681,
  -0.0023819542003645686,
  -0.004150855381456517,
  -0.006004573353357982,
  -0.00783193323792155,
  -0.009508360577805025,
  -0.010898570997863769,
  -0.011865626124985453,
  -0.012272360146126125,
  -0.011983908551548635,
  -0.010893005483600223,
  -0.008930404986589487,
  -0.006024323504903731,
  -0.0021808124966767413,
  0.0025801552915842253,
  0.008181482505524737,
  0.014506411779815938,
  0.021398242742485345,
  0.028664636417100837,
  0.03608559446876856,
  0.04342294373960488,
  0.0504288096982045,
  0.05685695780280856,
  0.062479621042434694,
  0.0670863497919933,
  0.07050933697539497,
  0.07261577298371875,
  0.07332596267386328,
  0.07261577298371875,
  0.07050933697539497,
  0.0670863497919933,
  0.062479621042434694,
  0.05685695780280856,
  0.0504288096982045,
  0.04342294373960488,
  0.03608559446876856,
  0.028664636417100837,
  0.021398242742485345,
  0.014506411779815938,
  0.008181482505524737,
  0.0025801552915842253,
  -0.0021808124966767413,
  -0.006024323504903731,
  -0.008930404986589487,
  -0.010893005483600223,
  -0.011983908551548635,
  -0.012272360146126125,
  -0.011865626124985453,
  -0.010898570997863769,
  -0.009508360577805025,
  -0.00783193323792155,
  -0.006004573353357982,
  -0.004150855381456517,
  -0.0023819542003645686,
  -0.0007837403145703681,
  0.0005804746148391294,
  0.001669476207735804,
  0.0024648497866630857,
  0.002966860667208367,
  0.003194092233025482,
  0.0031807946781945333,
  0.0029702829795608537,
  0.002612123638473842,
  0.005884881073845589

};

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

const static float filter_taps[__FILTER_TAP_NUM]   __attribute__((section("ccmram"))) =
{
		  -0.00011326894360721481,
		  -0.00021974993255220038,
		  -0.0004018485566778979,
		  -0.0006454683201242051,
		  -0.0009383639632779012,
		  -0.0012520254295890893,
		  -0.001540465185504639,
		  -0.0017421122800981264,
		  -0.0017853037759441787,
		  -0.001597642458681616,
		  -0.0011185193982734636,
		  -0.00031346278866755284,
		  0.0008117536207469526,
		  0.002199689809318923,
		  0.0037353627012880414,
		  0.005247945148435199,
		  0.006522081921892097,
		  0.007318885985000908,
		  0.0074055601794696495,
		  0.006591215939317358,
		  0.004764401694722978,
		  0.0019277312237581253,
		  -0.0017765518657368554,
		  -0.006054816869302308,
		  -0.010468271991218631,
		  -0.014459043679256727,
		  -0.01739569587669286,
		  -0.018634940510232866,
		  -0.017593660449457895,
		  -0.01382332817740183,
		  -0.007077605601975829,
		  0.002635372499878474,
		  0.015024964640094287,
		  0.029518258752548845,
		  0.0452926671827812,
		  0.06133717894951031,
		  0.076538570854719,
		  0.08978330747561612,
		  0.10006378322396904,
		  0.10657765172194239,
		  0.1088083625483035,
		  0.10657765172194239,
		  0.10006378322396904,
		  0.08978330747561612,
		  0.076538570854719,
		  0.06133717894951031,
		  0.0452926671827812,
		  0.029518258752548845,
		  0.015024964640094287,
		  0.002635372499878474,
		  -0.007077605601975829,
		  -0.01382332817740183,
		  -0.017593660449457895,
		  -0.018634940510232866,
		  -0.01739569587669286,
		  -0.014459043679256727,
		  -0.010468271991218631,
		  -0.006054816869302308,
		  -0.0017765518657368554,
		  0.0019277312237581253,
		  0.004764401694722978,
		  0.006591215939317358,
		  0.0074055601794696495,
		  0.007318885985000908,
		  0.006522081921892097,
		  0.005247945148435199,
		  0.0037353627012880414,
		  0.002199689809318923,
		  0.0008117536207469526,
		  -0.00031346278866755284,
		  -0.0011185193982734636,
		  -0.001597642458681616,
		  -0.0017853037759441787,
		  -0.0017421122800981264,
		  -0.001540465185504639,
		  -0.0012520254295890893,
		  -0.0009383639632779012,
		  -0.0006454683201242051,
		  -0.0004018485566778979,
		  -0.00021974993255220038,
		  -0.00011326894360721481
};


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

const static float filter_taps[__FILTER_TAP_NUM]   __attribute__((section("ccmram"))) =
{

		0.005202494059852252,
		  0.002028200244120176,
		  0.0020611295285771435,
		  0.0018023780144089529,
		  0.0011814581358518168,
		  0.00018671761541537945,
		  -0.0011718312034978643,
		  -0.0028501122909573577,
		  -0.0047449462784198764,
		  -0.006707149945126277,
		  -0.00855492678391512,
		  -0.010073013594108126,
		  -0.011024963997754282,
		  -0.011174675211829369,
		  -0.01030293682588995,
		  -0.0082269217201487,
		  -0.004821850638676238,
		  -0.00003470590363136943,
		  0.006106040082150835,
		  0.013481675901110015,
		  0.021882643342490522,
		  0.03101663090362838,
		  0.04052169186993611,
		  0.04998611462479662,
		  0.05897405644393578,
		  0.06705206024470295,
		  0.0738159591169944,
		  0.07891827701571968,
		  0.08209216265004783,
		  0.08316933250646918,
		  0.08209216265004783,
		  0.07891827701571968,
		  0.0738159591169944,
		  0.06705206024470295,
		  0.05897405644393578,
		  0.04998611462479662,
		  0.04052169186993611,
		  0.03101663090362838,
		  0.021882643342490522,
		  0.013481675901110015,
		  0.006106040082150835,
		  -0.00003470590363136943,
		  -0.004821850638676238,
		  -0.0082269217201487,
		  -0.01030293682588995,
		  -0.011174675211829369,
		  -0.011024963997754282,
		  -0.010073013594108126,
		  -0.00855492678391512,
		  -0.006707149945126277,
		  -0.0047449462784198764,
		  -0.0028501122909573577,
		  -0.0011718312034978643,
		  0.00018671761541537945,
		  0.0011814581358518168,
		  0.0018023780144089529,
		  0.0020611295285771435,
		  0.002028200244120176,
		  0.005202494059852252
};

#endif

__Filter __FIRFilter[ADC1_CH_NUM+1] = {0};

uint16_t ADCbuff[ADC1_CH_NUM];

void __Filter_init(__Filter* f) {
  int i;
  for(i = 0; i < __FILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void __Filter_put(__Filter* f, float input) {
  f->history[f->last_index++] = input;
  if(f->last_index >= __FILTER_TAP_NUM)
    f->last_index = 0;
}

float __Filter_get(__Filter* f) {
  float acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < __FILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : __FILTER_TAP_NUM-1;
    acc += f->history[index] * filter_taps[i];
  };
  return acc;
}
