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

#include <stdio.h>
#include <string.h>
#include "multiprotocol.h"
#include "core/rcchannel.h"
#include "stconfig.h"
#include "multiprotocol.h"

#define ARRAYLEN(x) (sizeof(x) / sizeof((x)[0]))
#define OPTIONS_BUFF_SIZE 1024

static char protocols_opts[OPTIONS_BUFF_SIZE] = {0};
static char sub_protocols_opts[OPTIONS_BUFF_SIZE] = {0};

#define CLI_PROTOCOL_DEF(name, code, subProtocols) \
    {                                              \
        name,                                      \
            code,                                  \
            subProtocols                           \
    }

#define CLI_SUB_PROTOCOL_DEF(name, chNumber, code) \
    {                                              \
        name,                                      \
            chNumber,                              \
            code                                   \
    }

const MULTITX_SUB_PROTO_ItemTypedef __sub_protocol_flysky[] = {
    CLI_SUB_PROTOCOL_DEF("Flysky", 16, Flysky),
    CLI_SUB_PROTOCOL_DEF("V9X9", 16, V9X9),
    CLI_SUB_PROTOCOL_DEF("V6X6", 16, V6X6),
    CLI_SUB_PROTOCOL_DEF("V912", 16, V912),
    CLI_SUB_PROTOCOL_DEF("PWM_IBUS", 16, CX20),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_height[] = {
    CLI_SUB_PROTOCOL_DEF("FZ410", 16, FZ410),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_hubsan[] = {
    CLI_SUB_PROTOCOL_DEF("H107", 16, H107),
    CLI_SUB_PROTOCOL_DEF("H301", 16, H301),
    CLI_SUB_PROTOCOL_DEF("H501", 16, H501),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_hisky[] = {
    CLI_SUB_PROTOCOL_DEF("Hisky", 8, Hisky),
    CLI_SUB_PROTOCOL_DEF("HK310", 5, HK310),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_dsm[] = {
    CLI_SUB_PROTOCOL_DEF("DSM2_1F", 16, DSM2_1F),
    CLI_SUB_PROTOCOL_DEF("DSM2_2F", 16, DSM2_2F),
    CLI_SUB_PROTOCOL_DEF("DSMX_1F", 16, DSMX_1F),
    CLI_SUB_PROTOCOL_DEF("DSMX_2F", 16, DSM_AUTO),
    CLI_SUB_PROTOCOL_DEF("DSM_AUTO", 16, DSMR),
    CLI_SUB_PROTOCOL_DEF("DSMR", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_yd717[] = {
    CLI_SUB_PROTOCOL_DEF("YD717", 16, YD717),
    CLI_SUB_PROTOCOL_DEF("SKYWLKR", 16, SKYWLKR),
    CLI_SUB_PROTOCOL_DEF("SYMAX4", 16, SYMAX4),
    CLI_SUB_PROTOCOL_DEF("XINXUN", 16, XINXUN),
    CLI_SUB_PROTOCOL_DEF("NIHUI", 16, NIHUI),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_kn[] = {
    CLI_SUB_PROTOCOL_DEF("WLTOYS", 16, WLTOYS),
    CLI_SUB_PROTOCOL_DEF("FEILUN", 16, FEILUN),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_symax[] = {
    CLI_SUB_PROTOCOL_DEF("SYMAX", 16, SYMAX),
    CLI_SUB_PROTOCOL_DEF("SYMAX5C", 16, SYMAX5C),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_slt[] = {
    CLI_SUB_PROTOCOL_DEF("SLT_V1", 16, SLT_V1),
    CLI_SUB_PROTOCOL_DEF("SLT_V2", 16, SLT_V2),
    CLI_SUB_PROTOCOL_DEF("Q100", 16, Q100),
    CLI_SUB_PROTOCOL_DEF("Q200", 16, Q200),
    CLI_SUB_PROTOCOL_DEF("MR100", 16, MR100),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_cx10[] = {
    CLI_SUB_PROTOCOL_DEF("CX10_GREEN", 16, CX10_GREEN),
    CLI_SUB_PROTOCOL_DEF("CX10_BLUE", 16, CX10_BLUE),
    CLI_SUB_PROTOCOL_DEF("DM007", 16, DM007),
    CLI_SUB_PROTOCOL_DEF("JC3015_1", 16, JC3015_1),
    CLI_SUB_PROTOCOL_DEF("JC3015_2", 16, JC3015_2),
    CLI_SUB_PROTOCOL_DEF("MK33041", 16, MK33041),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_q2x2[] = {
    CLI_SUB_PROTOCOL_DEF("Q222", 16, Q222),
    CLI_SUB_PROTOCOL_DEF("Q242", 16, Q242),
    CLI_SUB_PROTOCOL_DEF("Q282", 16, Q282),
    CLI_SUB_PROTOCOL_DEF("F_Q222", 16, F_Q222),
    CLI_SUB_PROTOCOL_DEF("F_Q242", 16, F_Q242),
    CLI_SUB_PROTOCOL_DEF("F_Q282", 16, F_Q282),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_cg023[] = {
    CLI_SUB_PROTOCOL_DEF("CG023", 16, CG023),
    CLI_SUB_PROTOCOL_DEF("YD829", 16, YD829),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_bayang[] = {
    CLI_SUB_PROTOCOL_DEF("BAYANG", 15, BAYANG),
    CLI_SUB_PROTOCOL_DEF("H8S3D", 15, H8S3D),
    CLI_SUB_PROTOCOL_DEF("X16_AH", 15, X16_AH),
    CLI_SUB_PROTOCOL_DEF("IRDRONE", 15, IRDRONE),
    CLI_SUB_PROTOCOL_DEF("DHD_D4", 15, DHD_D4),
    CLI_SUB_PROTOCOL_DEF("QX100", 15, QX100),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_mt99xx[] = {
    CLI_SUB_PROTOCOL_DEF("MT99", 16, MT99),
    CLI_SUB_PROTOCOL_DEF("H7", 16, H7),
    CLI_SUB_PROTOCOL_DEF("YZ", 16, YZ),
    CLI_SUB_PROTOCOL_DEF("LS", 16, LS),
    CLI_SUB_PROTOCOL_DEF("FY805", 16, FY805),
    CLI_SUB_PROTOCOL_DEF("A180", 16, A180),
    CLI_SUB_PROTOCOL_DEF("DRAGON", 16, DRAGON),
    CLI_SUB_PROTOCOL_DEF("F949G", 16, F949G),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_mjxq[] = {
    CLI_SUB_PROTOCOL_DEF("WLH08", 16, WLH08),
    CLI_SUB_PROTOCOL_DEF("X600", 16, X600),
    CLI_SUB_PROTOCOL_DEF("X800", 16, X800),
    CLI_SUB_PROTOCOL_DEF("H26D", 16, H26D),
    CLI_SUB_PROTOCOL_DEF("E010", 16, E010),
    CLI_SUB_PROTOCOL_DEF("H26WH", 16, H26WH),
    CLI_SUB_PROTOCOL_DEF("PHOENIX", 16, PHOENIX),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_frskyd[] = {
    CLI_SUB_PROTOCOL_DEF("FRSKYD", 16, FRSKYD),
    CLI_SUB_PROTOCOL_DEF("DCLONE", 16, DCLONE),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_frskyx[] = {
    CLI_SUB_PROTOCOL_DEF("CH_16", 16, CH_16),
    CLI_SUB_PROTOCOL_DEF("CH_8", 16, CH_8),
    CLI_SUB_PROTOCOL_DEF("EU_16", 16, EU_16),
    CLI_SUB_PROTOCOL_DEF("EU_8", 16, EU_8),
    CLI_SUB_PROTOCOL_DEF("XCLONE_16", 16, XCLONE_16),
    CLI_SUB_PROTOCOL_DEF("XCLONE_8", 16, XCLONE_8),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_hontai[] = {
    CLI_SUB_PROTOCOL_DEF("HONTAI", 16, HONTAI),
    CLI_SUB_PROTOCOL_DEF("JJRCX1", 16, JJRCX1),
    CLI_SUB_PROTOCOL_DEF("X5C1", 16, X5C1),
    CLI_SUB_PROTOCOL_DEF("FQ777_951", 16, FQ777_951),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_v2x2[] = {
    CLI_SUB_PROTOCOL_DEF("V2X2", 16, V2X2),
    CLI_SUB_PROTOCOL_DEF("JXD506", 16, JXD506),
    CLI_SUB_PROTOCOL_DEF("V2X2_MR101", 16, V2X2_MR101),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_fy326[] = {
    CLI_SUB_PROTOCOL_DEF("FY326", 16, FY326),
    CLI_SUB_PROTOCOL_DEF("FY319", 16, FY319),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_wk2x01[] = {
    CLI_SUB_PROTOCOL_DEF("WK2801", 16, WK2801),
    CLI_SUB_PROTOCOL_DEF("WK2401", 16, WK2401),
    CLI_SUB_PROTOCOL_DEF("W6_5_1", 16, W6_5_1),
    CLI_SUB_PROTOCOL_DEF("W6_6_1", 16, W6_6_1),
    CLI_SUB_PROTOCOL_DEF("W6_HEL", 16, W6_HEL),
    CLI_SUB_PROTOCOL_DEF("W6_HEL_I", 16, W6_HEL_I),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_q303[] = {
    CLI_SUB_PROTOCOL_DEF("Q303", 16, Q303),
    CLI_SUB_PROTOCOL_DEF("CX35", 16, CX35),
    CLI_SUB_PROTOCOL_DEF("CX10D", 16, CX10D),
    CLI_SUB_PROTOCOL_DEF("CX10WD", 16, CX10WD),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_cabell[] = {
    CLI_SUB_PROTOCOL_DEF("CABELL_V3", 16, CABELL_V3),
    CLI_SUB_PROTOCOL_DEF("CABELL_V3_TELEMETRY", 16, CABELL_V3_TELEMETRY),
    CLI_SUB_PROTOCOL_DEF("CABELL_SET_FAIL_SAFE", 16, CABELL_SET_FAIL_SAFE),
    CLI_SUB_PROTOCOL_DEF("CABELL_UNBIND", 16, CABELL_UNBIND),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_h8_3d[] = {
    CLI_SUB_PROTOCOL_DEF("H8_3D", 16, H8_3D),
    CLI_SUB_PROTOCOL_DEF("H20H", 16, H20H),
    CLI_SUB_PROTOCOL_DEF("H20MINI", 16, H20MINI),
    CLI_SUB_PROTOCOL_DEF("H30MINI", 16, H30MINI),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_corona[] = {
    CLI_SUB_PROTOCOL_DEF("COR_V1", 16, COR_V1),
    CLI_SUB_PROTOCOL_DEF("COR_V2", 16, COR_V2),
    CLI_SUB_PROTOCOL_DEF("FD_V3", 16, FD_V3),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_hitec[] = {
    CLI_SUB_PROTOCOL_DEF("OPT_FW", 16, OPT_FW),
    CLI_SUB_PROTOCOL_DEF("OPT_HUB", 16, OPT_HUB),
    CLI_SUB_PROTOCOL_DEF("MINIMA", 16, MINIMA),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_e01x[] = {
    CLI_SUB_PROTOCOL_DEF("E012", 16, E012),
    CLI_SUB_PROTOCOL_DEF("E015", 16, E015),
    CLI_SUB_PROTOCOL_DEF("E016H", 16, E016H),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_gd00x[] = {
    CLI_SUB_PROTOCOL_DEF("GD_V1", 16, GD_V1),
    CLI_SUB_PROTOCOL_DEF("GD_V2", 16, GD_V2),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_bugsmini[] = {
    CLI_SUB_PROTOCOL_DEF("BUGSMINI", 16, BUGSMINI),
    CLI_SUB_PROTOCOL_DEF("BUGS3H", 16, BUGS3H),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_redpine[] = {
    CLI_SUB_PROTOCOL_DEF("RED_FAST", 16, RED_FAST),
    CLI_SUB_PROTOCOL_DEF("RED_SLOW", 16, RED_SLOW),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_traxxas[] = {
    CLI_SUB_PROTOCOL_DEF("RX6519", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_esky150[] = {
    CLI_SUB_PROTOCOL_DEF("ESKY150_4CH", 16, ESKY150_4CH),
    CLI_SUB_PROTOCOL_DEF("ESKY150_7CH", 16, ESKY150_7CH),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_v911s[] = {
    CLI_SUB_PROTOCOL_DEF("V911S_STD", 16, V911S_STD),
    CLI_SUB_PROTOCOL_DEF("V911S_E119", 16, V911S_E119),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_xk[] = {
    CLI_SUB_PROTOCOL_DEF("X450", 16, X450),
    CLI_SUB_PROTOCOL_DEF("X420", 16, X420),
    CLI_SUB_PROTOCOL_DEF("Cars", 16, XK_CARS),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_xn297dump[] = {
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_250K", 16, XN297DUMP_250K),
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_1M", 16, XN297DUMP_1M),
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_2M", 16, XN297DUMP_2M),
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_AUTO", 16, XN297DUMP_AUTO),
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_NRF", 16, XN297DUMP_NRF),
    CLI_SUB_PROTOCOL_DEF("XN297DUMP_CC2500", 16, XN297DUMP_CC2500),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_frsky_r9[] = {
    CLI_SUB_PROTOCOL_DEF("R9_915", 16, R9_915),
    CLI_SUB_PROTOCOL_DEF("R9_868", 16, R9_868),
    CLI_SUB_PROTOCOL_DEF("R9_915_8CH", 16, R9_915_8CH),
    CLI_SUB_PROTOCOL_DEF("R9_868_8CH", 16, R9_868_8CH),
    CLI_SUB_PROTOCOL_DEF("R9_FCC", 16, R9_FCC),
    CLI_SUB_PROTOCOL_DEF("R9_EU", 16, R9_EU),
    CLI_SUB_PROTOCOL_DEF("R9_FCC_8CH", 16, R9_FCC_8CH),
    CLI_SUB_PROTOCOL_DEF("R9_EU_8CH", 16, R9_EU_8CH),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_esky[] = {
    CLI_SUB_PROTOCOL_DEF("ESKY_STD", 16, ESKY_STD),
    CLI_SUB_PROTOCOL_DEF("ESKY_ET4", 16, ESKY_ET4),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_frsky_rx[] = {
    CLI_SUB_PROTOCOL_DEF("FRSKY_RX", 16, FRSKY_RX),
    CLI_SUB_PROTOCOL_DEF("FRSKY_CLONE", 16, FRSKY_CLONE),
    CLI_SUB_PROTOCOL_DEF("FRSKY_ERASE", 16, FRSKY_ERASE),
    CLI_SUB_PROTOCOL_DEF("FRSKY_CPPM", 16, FRSKY_CPPM),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_frskyl[] = {
    CLI_SUB_PROTOCOL_DEF("LR12", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF("LR12_6CH", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_hott[] = {
    CLI_SUB_PROTOCOL_DEF("HOTT_SYNC", 16, HOTT_SYNC),
    CLI_SUB_PROTOCOL_DEF("HOTT_NO_SYNC", 16, HOTT_NO_SYNC),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_pelikan[] = {
    CLI_SUB_PROTOCOL_DEF("PELIKAN_PRO", 16, PELIKAN_PRO),
    CLI_SUB_PROTOCOL_DEF("PELIKAN_LITE", 16, PELIKAN_LITE),
    CLI_SUB_PROTOCOL_DEF("PELIKAN_SCX24", 16, PELIKAN_SCX24),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_v761[] = {
    CLI_SUB_PROTOCOL_DEF("V761_3CH", 16, V761_3CH),
    CLI_SUB_PROTOCOL_DEF("V761_4CH", 16, V761_4CH),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_kyosho[] = {
    CLI_SUB_PROTOCOL_DEF("KYOSHO_FHSS", 16, KYOSHO_FHSS),
    CLI_SUB_PROTOCOL_DEF("KYOSHO_HYPE", 16, KYOSHO_HYPE),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

// const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_kyosho2[] = {
//     CLI_SUB_PROTOCOL_DEF("KT-17", 16, KT_17)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_JJRC345[] = {
    CLI_SUB_PROTOCOL_DEF("PWM_IBUS", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF("PWM_IBUS", 16, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_MOULDKG[] = {
    CLI_SUB_PROTOCOL_DEF("MOULDKG_ANALOG", 16, MOULDKG_ANALOG),
    CLI_SUB_PROTOCOL_DEF("MOULDKG_DIGIT", 16, MOULDKG_DIGIT),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef sub_protocol_KF606[] = {
    CLI_SUB_PROTOCOL_DEF("KF606_KF606", 16, KF606_KF606),
    CLI_SUB_PROTOCOL_DEF("KF606_MIG320", 16, KF606_MIG320),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef __sub_protocol_afhds2a[] = {
    CLI_SUB_PROTOCOL_DEF("PWM_IBUS", 14, PWM_IBUS),
    CLI_SUB_PROTOCOL_DEF("PPM_IBUS", 14, PPM_IBUS),
    CLI_SUB_PROTOCOL_DEF("PWM_SBUS", 14, PWM_SBUS),
    CLI_SUB_PROTOCOL_DEF("PPM_SBUS", 14, PPM_SBUS),
    CLI_SUB_PROTOCOL_DEF("PWM_IB16", 14, PWM_IB16),
    CLI_SUB_PROTOCOL_DEF("PPM_IB16", 14, PPM_IB16),
    CLI_SUB_PROTOCOL_DEF("PWM_SB16", 14, PWM_SB16),
    CLI_SUB_PROTOCOL_DEF("PPM_SB16", 14, PPM_SB16),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_SUB_PROTO_ItemTypedef __sub_protocol_RLINK[] = {
    CLI_SUB_PROTOCOL_DEF("Surface", 8, 0),
    CLI_SUB_PROTOCOL_DEF("Air", 8, 1),
    CLI_SUB_PROTOCOL_DEF("DumboRC", 8, 2),
    CLI_SUB_PROTOCOL_DEF(NULL, 0, 0)};

const MULTITX_PROTO_ItemTypedef tx_protocols[] = {
    CLI_PROTOCOL_DEF("PPM", PROTO_PROTOLIST, NULL),
    CLI_PROTOCOL_DEF("Assan", PROTO_ASSAN, NULL),
    CLI_PROTOCOL_DEF("Bayang", PROTO_BAYANG, sub_protocol_bayang),
    CLI_PROTOCOL_DEF("Bayang RX", PROTO_BAYANG_RX, NULL),
    CLI_PROTOCOL_DEF("Bug", PROTO_BUGS, NULL),
    CLI_PROTOCOL_DEF("BugsMini", PROTO_BUGSMINI, sub_protocol_bugsmini),
    CLI_PROTOCOL_DEF("Cabell", PROTO_CABELL, sub_protocol_cabell),
    CLI_PROTOCOL_DEF("CFlie", PROTO_CFLIE, NULL),
    CLI_PROTOCOL_DEF("CG023", PROTO_CG023, sub_protocol_cg023),
    CLI_PROTOCOL_DEF("Corona", PROTO_CORONA, sub_protocol_corona),
    CLI_PROTOCOL_DEF("CX10", PROTO_CX10, sub_protocol_cx10),
    CLI_PROTOCOL_DEF("Devo", PROTO_DEVO, NULL),
    CLI_PROTOCOL_DEF("DM002", PROTO_DM002, NULL),
    CLI_PROTOCOL_DEF("DSM", PROTO_DSM, sub_protocol_dsm),
    CLI_PROTOCOL_DEF("DSM_RX", PROTO_DSM_RX, NULL),
    CLI_PROTOCOL_DEF("E010R5", PROTO_E010R5, NULL),
    CLI_PROTOCOL_DEF("E016H", PROTO_E016H, NULL),
    CLI_PROTOCOL_DEF("E016HV2", PROTO_E016HV2, NULL),
    CLI_PROTOCOL_DEF("E01X", PROTO_E01X, sub_protocol_e01x),
    CLI_PROTOCOL_DEF("E129", PROTO_E129, NULL),
    CLI_PROTOCOL_DEF("ESky", PROTO_ESKY, sub_protocol_esky),
    CLI_PROTOCOL_DEF("ESky150", PROTO_ESKY150, sub_protocol_esky150),
    CLI_PROTOCOL_DEF("ESky150V2", PROTO_ESKY150V2, NULL),
    CLI_PROTOCOL_DEF("Flysky", PROTO_FLYSKY, __sub_protocol_flysky),
    CLI_PROTOCOL_DEF("FQ777", PROTO_FQ777, NULL),
    CLI_PROTOCOL_DEF("FrskyD", PROTO_FRSKYD, sub_protocol_frskyd),
    CLI_PROTOCOL_DEF("FrskyL", PROTO_FRSKYL, sub_protocol_frskyl),
    CLI_PROTOCOL_DEF("FrskyR9", PROTO_FRSKY_R9, sub_protocol_frsky_r9),
    CLI_PROTOCOL_DEF("FrskyV", PROTO_FRSKYV, NULL),
    CLI_PROTOCOL_DEF("FrskyX", PROTO_FRSKYX, sub_protocol_frskyx),
    CLI_PROTOCOL_DEF("FrskyX2", PROTO_FRSKYX2, NULL),
    CLI_PROTOCOL_DEF("Frsky_RX", PROTO_FRSKY_RX, sub_protocol_frsky_rx),
    CLI_PROTOCOL_DEF("Futaba/SFHSS", PROTO_FUTABA, NULL),
    CLI_PROTOCOL_DEF("AFHDS2A", PROTO_AFHDS2A, __sub_protocol_afhds2a),
    CLI_PROTOCOL_DEF("FY326", PROTO_FY326, sub_protocol_fy326),
    CLI_PROTOCOL_DEF("GD00X", PROTO_GD00X, sub_protocol_gd00x),
    CLI_PROTOCOL_DEF("GW008", PROTO_GW008, NULL),
    CLI_PROTOCOL_DEF("H8_3D", PROTO_H8_3D, sub_protocol_h8_3d),
    CLI_PROTOCOL_DEF("Height", PROTO_HEIGHT, sub_protocol_height),
    CLI_PROTOCOL_DEF("Hisky", PROTO_HISKY, sub_protocol_hisky),
    CLI_PROTOCOL_DEF("Hitec", PROTO_HITEC, sub_protocol_hitec),
    CLI_PROTOCOL_DEF("Hontai", PROTO_HONTAI, sub_protocol_hontai),
    CLI_PROTOCOL_DEF("HoTT", PROTO_HOTT, sub_protocol_hott),
    CLI_PROTOCOL_DEF("Hubsan", PROTO_HUBSAN, sub_protocol_hubsan),
    CLI_PROTOCOL_DEF("J6Pro", PROTO_J6PRO, NULL),
    CLI_PROTOCOL_DEF("JJRC345", PROTO_JJRC345, sub_protocol_JJRC345),
    CLI_PROTOCOL_DEF("JOYSWAY", PROTO_JOYSWAY, NULL),
    CLI_PROTOCOL_DEF("KF606", PROTO_KF606, NULL),
    CLI_PROTOCOL_DEF("KN", PROTO_KN, sub_protocol_kn),
    CLI_PROTOCOL_DEF("Kyosho", PROTO_KYOSHO, sub_protocol_kyosho),
    CLI_PROTOCOL_DEF("Kyosho2", PROTO_KYOSHO2, NULL),
    CLI_PROTOCOL_DEF("LOLI", PROTO_LOLI, NULL),
    CLI_PROTOCOL_DEF("Losi", PROTO_LOSI, NULL),
    CLI_PROTOCOL_DEF("MJXq", PROTO_MJXQ, sub_protocol_mjxq),
    CLI_PROTOCOL_DEF("MLINK", PROTO_MLINK, NULL),
    CLI_PROTOCOL_DEF("MouldKg", PROTO_MOULDKG, sub_protocol_MOULDKG),
    CLI_PROTOCOL_DEF("MT99xx", PROTO_MT99XX, sub_protocol_mt99xx),
    CLI_PROTOCOL_DEF("MT99xx2", PROTO_MT99XX2, NULL),
    CLI_PROTOCOL_DEF("NCC1701", PROTO_NCC1701, NULL),
    CLI_PROTOCOL_DEF("OMP", PROTO_OMP, NULL),
    CLI_PROTOCOL_DEF("OpenLRS", PROTO_OPENLRS, NULL),
    CLI_PROTOCOL_DEF("Pelikan", PROTO_PELIKAN, sub_protocol_pelikan),
    CLI_PROTOCOL_DEF("Potensic", PROTO_POTENSIC, NULL),
    CLI_PROTOCOL_DEF("PROPEL", PROTO_PROPEL, NULL),
    CLI_PROTOCOL_DEF("Q2X2", PROTO_Q2X2, sub_protocol_q2x2),
    CLI_PROTOCOL_DEF("Q303", PROTO_Q303, sub_protocol_q303),
    CLI_PROTOCOL_DEF("Q90C", PROTO_Q90C, NULL),
    CLI_PROTOCOL_DEF("RadioLink", PROTO_RADIOLINK, __sub_protocol_RLINK),
    CLI_PROTOCOL_DEF("Realacc", PROTO_REALACC, NULL),
    CLI_PROTOCOL_DEF("Redpine", PROTO_REDPINE, sub_protocol_redpine),
    CLI_PROTOCOL_DEF("Scanner", PROTO_SCANNER, NULL),
    CLI_PROTOCOL_DEF("Shenqi", PROTO_SHENQI, NULL),
    CLI_PROTOCOL_DEF("Skyartec", PROTO_SKYARTEC, NULL),
    CLI_PROTOCOL_DEF("SLT", PROTO_SLT, sub_protocol_slt),
    CLI_PROTOCOL_DEF("SymaX", PROTO_SYMAX, sub_protocol_symax),
    CLI_PROTOCOL_DEF("Tiger", PROTO_TIGER, NULL),
    CLI_PROTOCOL_DEF("Traxxas", PROTO_TRAXXAS, sub_protocol_traxxas),
    CLI_PROTOCOL_DEF("V2x2", PROTO_V2X2, sub_protocol_v2x2),
    CLI_PROTOCOL_DEF("V761", PROTO_V761, sub_protocol_v761),
    CLI_PROTOCOL_DEF("V911S", PROTO_V911S, sub_protocol_v911s),
    CLI_PROTOCOL_DEF("WFLY", PROTO_WFLY, NULL),
    CLI_PROTOCOL_DEF("WFLY2", PROTO_WFLY2, NULL),
    CLI_PROTOCOL_DEF("WK2x01", PROTO_WK2x01, sub_protocol_wk2x01),
    CLI_PROTOCOL_DEF("XERALL", PROTO_XERALL, NULL),
    CLI_PROTOCOL_DEF("XK", PROTO_XK, sub_protocol_xk),
    CLI_PROTOCOL_DEF("YD717", PROTO_YD717, sub_protocol_yd717),
    CLI_PROTOCOL_DEF("ZSX", PROTO_ZSX, NULL)};

static void makeProtocolsOptions()
{
    uint16_t protocolsNumber = ARRAYLEN(tx_protocols);

    for (uint16_t i = 0; i < protocolsNumber; i++)
    {
        strcat(protocols_opts, tx_protocols[i].name);

        if (i < protocolsNumber - 1)
        {
            strcat(protocols_opts, "\n");
        }
    }
}

static char *makeSubProtocolsOptions(const MULTITX_PROTO_ItemTypedef *protocol)
{
    memset(sub_protocols_opts, 0, sizeof(sub_protocols_opts));
    MULTITX_SUB_PROTO_ItemTypedef *subProtocols = (MULTITX_SUB_PROTO_ItemTypedef *)protocol->subProtocols;

    while (subProtocols->name != NULL)
    {
        strcat(sub_protocols_opts, subProtocols->name);
        subProtocols++;

        if (subProtocols->name != NULL)
        {
            strcat(sub_protocols_opts, "\n");
        }
    }

    return sub_protocols_opts;
}

const MULTITX_PROTO_ItemTypedef *getProtocolByItemNumber(uint16_t number)
{
    return &tx_protocols[number];
}

const char *getProtocolsOptions()
{
    return protocols_opts;
}

const char *getSubProtocolsOptions(const MULTITX_PROTO_ItemTypedef *protocol)
{
    return makeSubProtocolsOptions(protocol);
}

uint16_t getProtocolItemNumberByCode(uint16_t code)
{
    uint16_t currentProtocol = 0;
    uint16_t protocolsNumbers = ARRAYLEN(tx_protocols);
    for (uint16_t i = 0; i < protocolsNumbers; i++)
    {
        if (tx_protocols[i].code == code)
        {
            currentProtocol = i;
        }
    }

    return currentProtocol;
}

uint16_t getSubProtocolItemNumberByCode(const MULTITX_PROTO_ItemTypedef *protocol, uint16_t code)
{
    uint16_t currentProtocol = 0;
    MULTITX_SUB_PROTO_ItemTypedef *subProtocols = (MULTITX_SUB_PROTO_ItemTypedef *)protocol->subProtocols;

    uint16_t i = 0;
    while (subProtocols->name != NULL || i > OPTIONS_BUFF_SIZE)
    {

        if (subProtocols->code == code)
        {
            currentProtocol = i;
            return currentProtocol;
        }
        subProtocols++;
        i++;
    }

    return currentProtocol;
}

void multiprotocolDescriptionInit()
{
    makeProtocolsOptions();
}
