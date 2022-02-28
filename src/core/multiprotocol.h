/*
 *
 *	Copyright 2022 Denis Davydov
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

#ifndef __MULTIPROTOCOL__H__
#define __MULTIPROTOCOL__H__

#include "main.h"
#include "stconfig.h"

#define NONE 0
#define P_HIGH 1
#define P_LOW 0
#define AUTOBIND 1
#define NO_AUTOBIND 0
#define BIND_BIT_MASK 0x80
#define AUTO_BIND_BIT_MASK 0x40
#define RANGE_CHECK_MASK 0x20
#define MULTIPROTOCOL_FRAME_SIZE 27
#define MULTIPROTOCOL_HEADER 0x55

// https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/5afdff847716b4ace6d43d7f743b5008830fa398/Multiprotocol/Multiprotocol.h#L816
//****************************************
//*** MULTI protocol serial definition ***
//****************************************
/*
***************************
16 channels serial protocol
***************************
Serial: 100000 Baud 8e2      _ xxxx xxxx p --
  Total of 26 bytes for protocol V1, variable length 27..36 for protocol V2
  Stream[0]   = header
				0x55	sub_protocol values are 0..31	Stream contains channels
				0x54	sub_protocol values are 32..63	Stream contains channels
				0x57	sub_protocol values are 0..31	Stream contains failsafe
				0x56	sub_protocol values are 32..63	Stream contains failsafe
				Note: V2 adds the 2 top bits to extend the number of protocols to 256 in Stream[26]
  Stream[1]   = sub_protocol|BindBit|RangeCheckBit|AutoBindBit;
   sub_protocol is 0..31 (bits 0..4)
				Reserved	0
				Flysky		1
				Hubsan		2
				FrskyD		3
				Hisky		4
				V2x2		5
				DSM			6
				Devo		7
				YD717		8
				KN			9
				SymaX		10
				SLT			11
				CX10		12
				CG023		13
				Bayang		14
				FrskyX		15
				ESky		16
				MT99XX		17
				MJXQ		18
				SHENQI		19
				FY326		20
				Futaba		21
				J6PRO		22
				FQ777		23
				ASSAN		24
				FrskyV		25
				HONTAI		26
				OpenLRS		27
				AFHDS2A		28
				Q2X2		29
				WK2x01		30
				Q303		31
				GW008		32
				DM002		33
				CABELL		34
				ESKY150		35
				H8_3D		36
				CORONA		37
				CFlie		38
				Hitec		39
				WFLY		40
				BUGS		41
				BUGSMINI	42
				TRAXXAS		43
				NCC1701		44
				E01X		45
				V911S		46
				GD00X		47
				V761		48
				KF606		49
				REDPINE		50
				POTENSIC	51
				ZSX			52
				HEIGHT		53
				SCANNER		54
				FRSKY_RX	55
				AFHDS2A_RX	56
				HOTT		57
				FX816		58
				BAYANG_RX	59
				PELIKAN		60
				TIGER		61
				XK			62
				XN297DUMP	63
				FRSKYX2		64
				FRSKY_R9	65
				PROPEL		66
				FRSKYL		67
				SKYARTEC	68
				ESKY150V2	69
				DSM_RX		70
				JJRC345		71
				Q90C		72
				KYOSHO		73
				RLINK		74
				REALACC		76
				OMP			77
				MLINK		78
				WFLY2		79
				E016HV2		80
				E010R5		81
				LOLI		82
				E129		83
				JOYSWAY		84
				E016H		85
				XERALL		91
   BindBit=>		0x80	1=Bind/0=No
   AutoBindBit=>	0x40	1=Yes /0=No
   RangeCheck=>		0x20	1=Yes /0=No
  Stream[2]   = RxNum | Power | Type;
   RxNum value is 0..15 (bits 0..3)
   Type is 0..7 <<4     (bit 4..6)
		sub_protocol==Flysky
			Flysky		0
			V9x9		1
			V6x6		2
			V912		3
			CX20		4
		sub_protocol==Hubsan
			H107		0
			H301		1
			H501		2
		sub_protocol==Hisky
			Hisky		0
			HK310		1
		sub_protocol==DSM
			DSM2_1F 	0
			DSM2_2F 	1
			DSMX_1F 	2
			DSMX_2F 	3
			DSM_AUTO	4
		sub_protocol==YD717
			YD717		0
			SKYWLKR		1
			SYMAX4		2
			XINXUN		3
			NIHUI		4
		sub_protocol==KN
			WLTOYS		0
			FEILUN		1
		sub_protocol==SYMAX
			SYMAX		0
			SYMAX5C		1
		sub_protocol==CX10
			CX10_GREEN	0
			CX10_BLUE	1	// also compatible with CX10-A, CX12
			DM007		2
			---			3
			JC3015_1	4
			JC3015_2	5
			MK33041		6
		sub_protocol==Q2X2
			Q222		0
			Q242		1
			Q282		2
		sub_protocol==CG023
			CG023		0
			YD829		1
		sub_protocol==BAYANG
			BAYANG		0
			H8S3D		1
			X16_AH		2
			IRDRONE		3
			DHD_D4		4
		sub_protocol==MT99XX
			MT99		0
			H7			1
			YZ			2
			LS			3
			FY805		4
		sub_protocol==MJXQ
			WLH08		0
			X600		1
			X800		2
			H26D		3
			E010		4
			H26WH		5
			PHOENIX		6
		sub_protocol==FRSKYD
			FRSKYD		0
			DCLONE		1
		sub_protocol==FRSKYX
			CH_16		0
			CH_8		1
			EU_16		2
			EU_8		3
			XCLONE		4
		sub_protocol==FRSKYX2
			CH_16		0
			CH_8		1
			EU_16		2
			EU_8		3
			XCLONE		4
		sub_protocol==HONTAI
			HONTAI	0
			JJRCX1	1
			X5C1		2
			FQ777_951 3
		sub_protocol==AFHDS2A
			PWM_IBUS	0
			PPM_IBUS	1
			PWM_SBUS	2
			PPM_SBUS	3
			PWM_IB16	4
			PPM_IB16	5
		sub_protocol==V2X2
			V2X2		0
			JXD506		1
			V2X2_MR101 2
		sub_protocol==FY326
			FY326		0
			FY319		1
		sub_protocol==WK2x01
			WK2801		0
			WK2401		1
			W6_5_1		2
			W6_6_1		3
			W6_HEL		4
			W6_HEL_I	5
		sub_protocol==Q303
			Q303		0
			CX35		1
			CX10D		2
			CX10WD		3
		sub_protocol==CABELL
			CABELL_V3				0
			CABELL_V3_TELEMETRY		1
			CABELL_SET_FAIL_SAFE	6
			CABELL_UNBIND			7
		sub_protocol==H8_3D
			H8_3D		0
			H20H		1
			H20MINI		2
			H30MINI		3
		sub_protocol==CORONA
			COR_V1		0
			COR_V2		1
			FD_V3		2
		sub_protocol==HITEC
			OPT_FW		0
			OPT_HUB		1
			MINIMA		2
		sub_protocol==SLT
			SLT_V1		0
			SLT_V2		1
			Q100		2
			Q200		3
			MR100		4
		sub_protocol==E01X
			E012		0
			E015		1
		sub_protocol==GD00X
			GD_V1		0
			GD_V2		1
		sub_protocol==REDPINE
			RED_FAST	0
			RED_SLOW	1
		sub_protocol==TRAXXAS
			RX6519		0
		sub_protocol==ESKY150
			ESKY150_4CH	0
			ESKY150_7CH	1
		sub_protocol==V911S
			V911S_STD	0
			V911S_E119	1
		sub_protocol==XK
			X450		0
			X420		1
		sub_protocol==FRSKY_R9
			R9_915		0
			R9_868		1
			R9_915_8CH	2
			R9_868_8CH	3
			R9_FCC		4
			R9_EU		5
			R9_FCC_8CH	6
			R9_EU_8CH	7
		sub_protocol==ESKY
			ESKY_STD	0
			ESKY_ET4	1
		sub_protocol==FRSKY_RX
			FRSKY_RX	0
			FRSKY_CLONE	1
		sub_protocol==FRSKYL
			LR12		0
			LR12_6CH	1
		sub_protocol==HOTT
			HOTT_SYNC		0
			HOTT_NO_SYNC	1
		sub_protocol==PELIKAN
			PELIKAN_PRO		0
			PELIKAN_LITE	1
			PELIKAN_SCX24	2
		sub_protocol==V761
			V761_3CH	0
			V761_4CH	1
		sub_protocol==HEIGHT
			HEIGHT_5CH	0
			HEIGHT_8CH	1
		sub_protocol==JJRC345
			JJRC345		0
			SKYTMBLR	1
		sub_protocol==RLINK
			RLINK_SURFACE	0
			RLINK_AIR		1
			RLINK_DUMBORC	2

   Power value => 0x80	0=High/1=Low
  Stream[3]   = option_protocol;
   option_protocol value is -128..127
  Stream[4] to [25] = Channels or failsafe depending on Steam[0]
   16 Channels on 11 bits (0..2047)
	0		-125%
	204		-100%
	1024	   0%
	1843	+100%
	2047	+125%
   Values are concatenated to fit in 22 bytes like in SBUS protocol.
   Failsafe values have exactly the same range/values than normal channels except the extremes where
	  0=no pulse, 2047=hold. If failsafe is not set or RX then failsafe packets should not be sent.
  Stream[26]   = sub_protocol bits 6 & 7|RxNum bits 4 & 5|Telemetry_Invert 3|Future_Use 2|Disable_Telemetry 1|Disable_CH_Mapping 0
   sub_protocol is 0..255 (bits 0..5 + bits 6..7)
   RxNum value is 0..63 (bits 0..3 + bits 4..5)
   Telemetry_Invert		=> 0x08	0=normal, 1=invert
   Future_Use			=> 0x04	0=      , 1=
   Disable_Telemetry	=> 0x02	0=enable, 1=disable
   Disable_CH_Mapping	=> 0x01	0=enable, 1=disable
  Stream[27.. 35] = between 0 and 9 bytes for additional protocol data
	Protocol specific use:
	  FrSkyX and FrSkyX2: Stream[27] during bind Telem on=0x00,off=0x01 | CH1-8=0x00,CH9-16=0x02
	  FrSkyX and FrSkyX2: Stream[27..34] during normal operation unstuffed SPort data to be sent
	  HoTT: Stream[27] 1 byte for telemetry type
	  DSM: Stream[27..33] Forward Programming
*/

enum PROTOCOLS
{
	PROTO_PROTOLIST = 0,	// NO RF
	PROTO_FLYSKY = 1,		// =>A7105
	PROTO_HUBSAN = 2,		// =>A7105
	PROTO_FRSKYD = 3,		// =>CC2500
	PROTO_HISKY = 4,		// =>NRF24L01
	PROTO_V2X2 = 5,			// =>NRF24L01
	PROTO_DSM = 6,			// =>CYRF6936
	PROTO_DEVO = 7,			// =>CYRF6936
	PROTO_YD717 = 8,		// =>NRF24L01
	PROTO_KN = 9,			// =>NRF24L01
	PROTO_SYMAX = 10,		// =>NRF24L01
	PROTO_SLT = 11,			// =>NRF24L01
	PROTO_CX10 = 12,		// =>NRF24L01
	PROTO_CG023 = 13,		// =>NRF24L01
	PROTO_BAYANG = 14,		// =>NRF24L01
	PROTO_FRSKYX = 15,		// =>CC2500
	PROTO_ESKY = 16,		// =>NRF24L01
	PROTO_MT99XX = 17,		// =>NRF24L01
	PROTO_MJXQ = 18,		// =>NRF24L01
	PROTO_SHENQI = 19,		// =>NRF24L01
	PROTO_FY326 = 20,		// =>NRF24L01
	PROTO_FUTABA = 21,		// =>CC2500
	PROTO_J6PRO = 22,		// =>CYRF6936
	PROTO_FQ777 = 23,		// =>NRF24L01
	PROTO_ASSAN = 24,		// =>NRF24L01
	PROTO_FRSKYV = 25,		// =>CC2500
	PROTO_HONTAI = 26,		// =>NRF24L01
	PROTO_OPENLRS = 27,		// =>OpenLRS hardware
	PROTO_AFHDS2A = 28,		// =>A7105
	PROTO_Q2X2 = 29,		// =>NRF24L01, extension of CX-10 protocol
	PROTO_WK2x01 = 30,		// =>CYRF6936
	PROTO_Q303 = 31,		// =>NRF24L01
	PROTO_GW008 = 32,		// =>NRF24L01
	PROTO_DM002 = 33,		// =>NRF24L01
	PROTO_CABELL = 34,		// =>NRF24L01
	PROTO_ESKY150 = 35,		// =>NRF24L01
	PROTO_H8_3D = 36,		// =>NRF24L01
	PROTO_CORONA = 37,		// =>CC2500
	PROTO_CFLIE = 38,		// =>NRF24L01
	PROTO_HITEC = 39,		// =>CC2500
	PROTO_WFLY = 40,		// =>CYRF6936
	PROTO_BUGS = 41,		// =>A7105
	PROTO_BUGSMINI = 42,	// =>NRF24L01
	PROTO_TRAXXAS = 43,		// =>CYRF6936
	PROTO_NCC1701 = 44,		// =>NRF24L01
	PROTO_E01X = 45,		// =>CYRF6936
	PROTO_V911S = 46,		// =>NRF24L01
	PROTO_GD00X = 47,		// =>NRF24L01
	PROTO_V761 = 48,		// =>NRF24L01
	PROTO_KF606 = 49,		// =>NRF24L01
	PROTO_REDPINE = 50,		// =>CC2500
	PROTO_POTENSIC = 51,	// =>NRF24L01
	PROTO_ZSX = 52,			// =>NRF24L01
	PROTO_HEIGHT = 53,		// =>A7105
	PROTO_SCANNER = 54,		// =>CC2500
	PROTO_FRSKY_RX = 55,	// =>CC2500
	PROTO_AFHDS2A_RX = 56,	// =>A7105
	PROTO_HOTT = 57,		// =>CC2500
	PROTO_FX816 = 58,		// =>NRF24L01
	PROTO_BAYANG_RX = 59,	// =>NRF24L01
	PROTO_PELIKAN = 60,		// =>A7105
	PROTO_TIGER = 61,		// =>NRF24L01
	PROTO_XK = 62,			// =>NRF24L01
	PROTO_XN297DUMP = 63,	// =>NRF24L01
	PROTO_FRSKYX2 = 64,		// =>CC2500
	PROTO_FRSKY_R9 = 65,	// =>SX1276
	PROTO_PROPEL = 66,		// =>NRF24L01
	PROTO_FRSKYL = 67,		// =>CC2500
	PROTO_SKYARTEC = 68,	// =>CC2500
	PROTO_ESKY150V2 = 69,	// =>CC2500+NRF24L01
	PROTO_DSM_RX = 70,		// =>CYRF6936
	PROTO_JJRC345 = 71,		// =>NRF24L01
	PROTO_Q90C = 72,		// =>NRF24L01 or CC2500
	PROTO_KYOSHO = 73,		// =>A7105
	PROTO_RLINK = 74,		// =>CC2500
	PROTO_REALACC = 76,		// =>NRF24L01
	PROTO_OMP = 77,			// =>CC2500 & NRF24L01
	PROTO_MLINK = 78,		// =>CYRF6936
	PROTO_WFLY2 = 79,		// =>A7105
	PROTO_E016HV2 = 80,		// =>CC2500 & NRF24L01
	PROTO_E010R5 = 81,		// =>CYRF6936
	PROTO_LOLI = 82,		// =>NRF24L01
	PROTO_E129 = 83,		// =>CYRF6936
	PROTO_JOYSWAY = 84,		// =>A7105
	PROTO_E016H = 85,		// =>NRF24L01
	PROTO_CONFIG = 86,		// Module config
	PROTO_IKEAANSLUTA = 87, // =>CC2500
	PROTO_WILLIFM = 88,		// 27/35ab/40/41/72 MHz module external project
	PROTO_LOSI = 89,		// =>CYRF6936
	PROTO_MOULDKG = 90,		// =>NRF24L01
	PROTO_XERALL = 91,		// =>NRF24L01

	PROTO_NANORF = 126, // =>NRF24L01
	PROTO_TEST = 127,	// =>CC2500
};

enum Flysky
{
	Flysky = 0,
	V9X9 = 1,
	V6X6 = 2,
	V912 = 3,
	CX20 = 4,
};
enum Height
{
	FZ410 = 0,
};
enum Hubsan
{
	H107 = 0,
	H301 = 1,
	H501 = 2,
};
enum AFHDS2A
{
	PWM_IBUS = 0,
	PPM_IBUS = 1,
	PWM_SBUS = 2,
	PPM_SBUS = 3,
	PWM_IB16 = 4,
	PPM_IB16 = 5,
	PWM_SB16 = 6,
	PPM_SB16 = 7,
};
enum Hisky
{
	Hisky = 0,
	HK310 = 1,
};
enum DSM
{
	DSM2_1F = 0,
	DSM2_2F = 1,
	DSMX_1F = 2,
	DSMX_2F = 3,
	DSM_AUTO = 4,
	DSMR = 5,
};
enum YD717
{
	YD717 = 0,
	SKYWLKR = 1,
	SYMAX4 = 2,
	XINXUN = 3,
	NIHUI = 4,
};
enum KN
{
	WLTOYS = 0,
	FEILUN = 1,
};
enum SYMAX
{
	SYMAX = 0,
	SYMAX5C = 1,
};
enum SLT
{
	SLT_V1 = 0,
	SLT_V2 = 1,
	Q100 = 2,
	Q200 = 3,
	MR100 = 4,
};
enum CX10
{
	CX10_GREEN = 0,
	CX10_BLUE = 1, // also compatible with CX10-A, CX12
	DM007 = 2,
	JC3015_1 = 4,
	JC3015_2 = 5,
	MK33041 = 6,
};
enum Q2X2
{
	Q222 = 0,
	Q242 = 1,
	Q282 = 2,
	F_Q222 = 8,
	F_Q242 = 9,
	F_Q282 = 10,
};
enum CG023
{
	CG023 = 0,
	YD829 = 1,
};
enum BAYANG
{
	BAYANG = 0,
	H8S3D = 1,
	X16_AH = 2,
	IRDRONE = 3,
	DHD_D4 = 4,
	QX100 = 5,
};
enum MT99XX
{
	MT99 = 0,
	H7 = 1,
	YZ = 2,
	LS = 3,
	FY805 = 4,
	A180 = 5,
	DRAGON = 6,
	F949G = 7,
};
enum MJXQ
{
	WLH08 = 0,
	X600 = 1,
	X800 = 2,
	H26D = 3,
	E010 = 4,
	H26WH = 5,
	PHOENIX = 6,
};
enum FRSKYD
{
	FRSKYD = 0,
	DCLONE = 1,
};
enum FRSKYX
{
	CH_16 = 0,
	CH_8 = 1,
	EU_16 = 2,
	EU_8 = 3,
	XCLONE_16 = 4,
	XCLONE_8 = 5,
};
enum HONTAI
{
	HONTAI = 0,
	JJRCX1 = 1,
	X5C1 = 2,
	FQ777_951 = 3,
};
enum V2X2
{
	V2X2 = 0,
	JXD506 = 1,
	V2X2_MR101 = 2,
};
enum FY326
{
	FY326 = 0,
	FY319 = 1,
};
enum WK2x01
{
	WK2801 = 0,
	WK2401 = 1,
	W6_5_1 = 2,
	W6_6_1 = 3,
	W6_HEL = 4,
	W6_HEL_I = 5,
};
enum Q303
{
	Q303 = 0,
	CX35 = 1,
	CX10D = 2,
	CX10WD = 3,
};
enum CABELL
{
	CABELL_V3 = 0,
	CABELL_V3_TELEMETRY = 1,
	CABELL_SET_FAIL_SAFE = 6,
	CABELL_UNBIND = 7,
};
enum H8_3D
{
	H8_3D = 0,
	H20H = 1,
	H20MINI = 2,
	H30MINI = 3,
};
enum CORONA
{
	COR_V1 = 0,
	COR_V2 = 1,
	FD_V3 = 2,
};
enum HITEC
{
	OPT_FW = 0,
	OPT_HUB = 1,
	MINIMA = 2,
};
enum E01X
{
	E012 = 0,
	E015 = 1,
	E016H = 2,
};
enum GD00X
{
	GD_V1 = 0,
	GD_V2 = 1,
};
enum BUGSMINI
{
	BUGSMINI = 0,
	BUGS3H = 1,
};
enum REDPINE
{
	RED_FAST = 0,
	RED_SLOW = 1,
};
enum TRAXXAS
{
	RX6519 = 0,
};
enum ESKY150
{
	ESKY150_4CH = 0,
	ESKY150_7CH = 1,
};
enum V911S
{
	V911S_STD = 0,
	V911S_E119 = 1,
};
enum XK
{
	X450 = 0,
	X420 = 1,
};
enum XN297DUMP
{
	XN297DUMP_250K = 0,
	XN297DUMP_1M = 1,
	XN297DUMP_2M = 2,
	XN297DUMP_AUTO = 3,
	XN297DUMP_NRF = 4,
	XN297DUMP_CC2500 = 5,
};
enum FRSKY_R9
{
	R9_915 = 0,
	R9_868 = 1,
	R9_915_8CH = 2,
	R9_868_8CH = 3,
	R9_FCC = 4,
	R9_EU = 5,
	R9_FCC_8CH = 6,
	R9_EU_8CH = 7,
};
enum ESKY
{
	ESKY_STD = 0,
	ESKY_ET4 = 1,
};

enum FRSKY_RX
{
	FRSKY_RX = 0,
	FRSKY_CLONE = 1,
	FRSKY_ERASE = 2,
	FRSKY_CPPM = 3,
};

enum FRSKYL
{
	LR12 = 0,
	LR12_6CH = 1,
};

enum HOTT
{
	HOTT_SYNC = 0,
	HOTT_NO_SYNC = 1,
};

enum PELIKAN
{
	PELIKAN_PRO = 0,
	PELIKAN_LITE = 1,
	PELIKAN_SCX24 = 2,
};

enum V761
{
	V761_3CH = 0,
	V761_4CH = 1,
};

enum HEIGHT
{
	HEIGHT_5CH = 0,
	HEIGHT_8CH = 1,
};

enum KYOSHO
{
	KYOSHO_FHSS = 0,
	KYOSHO_HYPE = 1,
};

enum JJRC345
{
	JJRC345 = 0,
	SKYTMBLR = 1,
};

enum RLINK
{
	RLINK_SURFACE = 0,
	RLINK_AIR = 1,
	RLINK_DUMBORC = 2,
};

enum MOULDKG
{
	MOULDKG_ANALOG = 0,
	MOULDKG_DIGIT = 1,
};

enum KF606
{
	KF606_KF606 = 0,
	KF606_MIG320 = 1,
};

enum evenParity
{
	odd,
	even
};

typedef struct
{
	uint16_t protocol;
	uint16_t subProtocol;
	uint16_t bind;
	uint16_t lowPower;
	uint16_t rangeCheckBit;
	uint16_t autoBindBit;
	uint16_t rxNum;
	uint16_t outputBuffer[MAX_RC_CHANNEL];
	uint16_t inputBuffer[16];
	uint8_t outStream[MULTIPROTOCOL_FRAME_SIZE];
	uint8_t inputtStream[MULTIPROTOCOL_FRAME_SIZE];
} SBUS_HandlerTypedef;

extern SBUS_HandlerTypedef sbus;

void multiprotocolInit();

void multiprotocolHandler(SBUS_HandlerTypedef *sbus);

void multiprotocolBindEnable(SBUS_HandlerTypedef *sbus);

void multiprotocolBindDisable(SBUS_HandlerTypedef *sbus);

void multiprotocolSetProtocol(uint16_t protocol, SBUS_HandlerTypedef *sbus);

void multiprotocolSetSubProtocol(uint16_t subProtocol, SBUS_HandlerTypedef *sbus);

uint16_t multiprotocolGetProtocol(SBUS_HandlerTypedef *sbus);

uint16_t multiprotocolGetSubProtocol(SBUS_HandlerTypedef *sbus);

void multiprotocolAssignmentValues();

void multiprotocolSetChannel(SBUS_HandlerTypedef *sbus, uint16_t channelNumber, uint16_t value);

void makeOutputStream(SBUS_HandlerTypedef *sbus);

#endif /* __MULTIPROTOCOL__H__ */