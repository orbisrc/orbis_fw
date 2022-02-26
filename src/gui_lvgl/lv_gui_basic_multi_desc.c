#include "lv_gui_basic_multi_desc.h"
#include "core/multiprotocol.h"

const char *multi_protocols_opts =
    "PROTOLIST\n"
    "FLYSKY\n"
    "HUBSAN\n"
    "FRSKYD\n"
    "HISKY\n"
    "V2X2\n"
    "DSM\n"
    "DEVO\n"
    "YD717\n"
    "KN\n"
    "SYMAX\n"
    "SLT\n"
    "CX10\n"
    "CG023\n"
    "BAYANG\n"
    "FRSKYX\n"
    "ESKY\n"
    "MT99XX\n"
    "MJXQ\n"
    "SHENQI\n"
    "FY326\n"
    "FUTABA\n"
    "J6PRO\n"
    "FQ777\n"
    "ASSAN\n"
    "FRSKYV\n"
    "HONTAI\n"
    "OPENLRS\n"
    "AFHDS2A\n"
    "Q2X2\n"
    "WK2x01\n"
    "Q303\n"
    "GW008\n"
    "DM002\n"
    "CABELL\n"
    "ESKY150\n"
    "H8_3D\n"
    "CORONA\n"
    "CFLIE\n"
    "HITEC\n"
    "WFLY\n"
    "BUGS\n"
    "BUGSMINI\n"
    "TRAXXAS\n"
    "NCC1701\n"
    "E01X\n"
    "V911S\n"
    "GD00X\n"
    "V761\n"
    "KF606\n"
    "REDPINE\n"
    "POTENSIC\n"
    "ZSX\n"
    "HEIGHT\n"
    "SCANNER\n"
    "FRSKY_RX\n"
    "AFHDS2A_RX\n"
    "HOTT\n"
    "FX816\n"
    "BAYANG_RX\n"
    "PELIKAN\n"
    "TIGER\n"
    "XK\n"
    "XN297DUMP\n"
    "FRSKYX2\n"
    "FRSKY_R9\n"
    "PROPEL\n"
    "FRSKYL\n"
    "SKYARTEC\n"
    "ESKY150V2\n"
    "DSM_RX\n"
    "JJRC345\n"
    "Q90C\n"
    "KYOSHO\n"
    "RLINK\n"
    "REALACC\n"
    "OMP\n"
    "MLINK\n"
    "WFLY2\n"
    "E016HV2\n"
    "E010R5\n"
    "LOLI\n"
    "E129\n"
    "JOYSWAY\n"
    "E016H\n"
    "CONFIG\n"
    "IKEAANSLUTA\n"
    "WILLIFM\n"
    "LOSI\n"
    "MOULDKG\n"
    "XERALL";

const char *sub_protocol_flysky =
    "Flysky\n"
    "V9X9\n"
    "V6X6\n"
    "V912\n"
    "CX20";

const char *sub_protocol_height =
    "FZ410\n";

const char *sub_protocol_hubsan =
    "H107\n"
    "H301\n"
    "H501";

const char *sub_protocol_afhds2a =
    "PWM_IBUS\n"
    "PPM_IBUS\n"
    "PWM_SBUS\n"
    "PPM_SBUS\n"
    "PWM_IB16\n"
    "PPM_IB16\n"
    "PWM_SB16\n"
    "PPM_SB16";

const char *sub_protocol_hisky =
    "Hisky\n"
    "HK310";

const char *sub_protocol_dsm =
    "DSM2_1F\n"
    "DSM2_2F\n"
    "DSMX_1F\n"
    "DSMX_2F\n"
    "DSM_AUTO\n"
    "DSMR";

const char *sub_protocol_yd717 =
    "YD717\n"
    "SKYWLKR\n"
    "SYMAX4\n"
    "XINXUN\n"
    "NIHUI";

const char *sub_protocol_kn =
    "WLTOYS\n"
    "FEILUN";

const char *sub_protocol_symax =
    "SYMAX\n"
    "SYMAX5C";

const char *sub_protocol_slt =
    "SLT_V1\n"
    "SLT_V2\n"
    "Q100\n"
    "Q200\n"
    "MR100";

const char *sub_protocol_cx10 =
    "CX10_GREEN\n"
    "CX10_BLUE\n"
    "DM007\n"
    "JC3015_1\n"
    "JC3015_2\n"
    "MK33041";

const char *sub_protocol_q2x2 =
    "Q222\n"
    "Q242\n"
    "Q282\n"
    "F_Q222\n"
    "F_Q242\n"
    "F_Q282";

const char *sub_protocol_cg023 =
    "CG023\n"
    "YD829";

const char *sub_protocol_bayang =
    "BAYANG\n"
    "H8S3D\n"
    "X16_AH\n"
    "IRDRONE\n"
    "DHD_D4\n"
    "QX100";

const char *sub_protocol_mt99xx =
    "MT99\n"
    "H7\n"
    "YZ\n"
    "LS\n"
    "FY805\n"
    "A180\n"
    "DRAGON\n"
    "F949G";

const char *sub_protocol_mjxq =
    "WLH08\n"
    "X600\n"
    "X800\n"
    "H26D\n"
    "E010\n"
    "H26WH\n"
    "PHOENIX";

const char *sub_protocol_frskyd =
    "FRSKYD\n"
    "DCLONE";

const char *sub_protocol_frskyx =
    "CH_16\n"
    "CH_8\n"
    "EU_16\n"
    "EU_\n"
    "XCLONE_16\n"
    "XCLONE_8";

const char *sub_protocol_hontai =
    "HONTAI\n"
    "JJRCX1\n"
    "X5C1\n"
    "FQ777_951";

const char *sub_protocol_v2x2 =
    "V2X2\n"
    "JXD506\n"
    "V2X2_MR101";

const char *sub_protocol_fy326 =
    "FY326\n"
    "FY319";

const char *sub_protocol_wk2x01 =
    "WK2801\n"
    "WK2401\n"
    "W6_5_1\n"
    "W6_6_1\n"
    "W6_HEL\n"
    "W6_HEL_I";

const char *sub_protocol_q303 =
    "Q303\n"
    "CX35\n"
    "CX10D\n"
    "CX10WD";

const char *sub_protocol_cabell =
    "CABELL_V3\n"
    "CABELL_V3_TELEMETRY\n"
    "CABELL_SET_FAIL_SAFE\n"
    "CABELL_UNBIND";

const char *sub_protocol_h8_3d =
    "H8_3D\n"
    "H20H\n"
    "H20MINI\n"
    "H30MINI";

const char *sub_protocol_corona =
    "COR_V1\n"
    "COR_V2\n"
    "FD_V3";

const char *sub_protocol_hitec =
    "OPT_FW\n"
    "OPT_HUB\n"
    "MINIMA";

const char *sub_protocol_e01x =
    "E012\n"
    "E015\n"
    "E016H";

const char *sub_protocol_gd00x =
    "GD_V1\n"
    "GD_V2";

const char *sub_protocol_bugsmini =
    "BUGSMINI\n"
    "BUGS3H";

const char *sub_protocol_redpine =

    "RED_FAST\n"
    "RED_SLOW";

const char *sub_protocol_traxxas =
    "RX6519";

const char *sub_protocol_esky150 =
    "ESKY150_4CH\n"
    "ESKY150_7CH";

const char *sub_protocol_v911s =
    "V911S_STD\n"
    "V911S_E119";

const char *sub_protocol_xk =
    "X450\n"
    "X420";

const char *sub_protocol_xn297dump =
    "XN297DUMP_250K\n"
    "XN297DUMP_1M\n"
    "XN297DUMP_2M\n"
    "XN297DUMP_AUTO\n"
    "XN297DUMP_NRF\n"
    "XN297DUMP_CC2500";

const char *sub_protocol_frsky_r9 =
    "R9_915\n"
    "R9_868\n"
    "R9_915_8CH\n"
    "R9_868_8CH\n"
    "R9_FCC\n"
    "R9_EU\n"
    "R9_FCC_8CH\n"
    "R9_EU_8CH";

const char *sub_protocol_esky =
    "ESKY_STD\n"
    "ESKY_ET4";

const char *sub_protocol_frsky_rx =
    "FRSKY_RX\n"
    "FRSKY_CLONE\n"
    "FRSKY_ERASE\n"
    "FRSKY_CPPM";

const char *sub_protocol_frskyl =
    "LR12\n"
    "LR12_6CH";

const char *sub_protocol_hott =
    "HOTT_SYNC\n"
    "HOTT_NO_SYNC";

const char *sub_protocol_pelikan =
    "PELIKAN_PRO\n"
    "PELIKAN_LITE\n"
    "PELIKAN_SCX24";

const char *sub_protocol_v761 =
    "V761_3CH\n"
    "V761_4CH";

// const char *sub_protocol_height =
//     "HEIGHT_5CH\n"
//     "HEIGHT_8CH";

const char *sub_protocol_kyosho =
    "KYOSHO_FHSS\n"
    "KYOSHO_HYPE";

const char *sub_protocol_JJRC345 =
    "JJRC345\n"
    "SKYTMBLR";

const char *sub_protocol_RLINK =
    "RLINK_SURFACE\n"
    "RLINK_AIR\n"
    "RLINK_DUMBORC";

const char *sub_protocol_MOULDKG =
    "MOULDKG_ANALOG\n"
    "MOULDKG_DIGIT";

const char *sub_protocol_KF606 =
    "KF606_KF606\n"
    "KF606_MIG320";

char *sub_protocols_opts[128] = {0};

void lv_multi_desc_init()
{
    sub_protocols_opts[PROTO_PROTOLIST] = NULL;
    sub_protocols_opts[PROTO_FLYSKY] = sub_protocol_flysky;
    sub_protocols_opts[PROTO_HUBSAN] = sub_protocol_hubsan;
    sub_protocols_opts[PROTO_FRSKYD] = sub_protocol_frskyd;
    sub_protocols_opts[PROTO_HISKY] = sub_protocol_hisky;
    sub_protocols_opts[PROTO_V2X2] = sub_protocol_v2x2;
    sub_protocols_opts[PROTO_DSM] = sub_protocol_dsm;
    sub_protocols_opts[PROTO_YD717] = sub_protocol_yd717;
    sub_protocols_opts[PROTO_KN] = sub_protocol_kn;
    sub_protocols_opts[PROTO_SYMAX] = sub_protocol_symax;
    sub_protocols_opts[PROTO_SLT] = sub_protocol_slt;
    sub_protocols_opts[PROTO_CX10] = sub_protocol_cx10;
    sub_protocols_opts[PROTO_CG023] = sub_protocol_cg023;
    sub_protocols_opts[PROTO_BAYANG] = sub_protocol_bayang;
    sub_protocols_opts[PROTO_FRSKYX] = sub_protocol_frskyx;
    sub_protocols_opts[PROTO_ESKY] = sub_protocol_esky;
    sub_protocols_opts[PROTO_MT99XX] = sub_protocol_mt99xx;
    sub_protocols_opts[PROTO_MJXQ] = sub_protocol_mjxq;
    sub_protocols_opts[PROTO_SHENQI] = NULL;
    sub_protocols_opts[PROTO_FY326] = sub_protocol_fy326;
    sub_protocols_opts[PROTO_FUTABA] = NULL;
    sub_protocols_opts[PROTO_J6PRO] = NULL;
    sub_protocols_opts[PROTO_FQ777] = NULL;
    sub_protocols_opts[PROTO_ASSAN] = NULL;
    sub_protocols_opts[PROTO_FRSKYV] = NULL;
    sub_protocols_opts[PROTO_HONTAI] = sub_protocol_hontai;
    sub_protocols_opts[PROTO_OPENLRS] = NULL;
    sub_protocols_opts[PROTO_AFHDS2A] = sub_protocol_afhds2a;
    sub_protocols_opts[PROTO_Q2X2] = sub_protocol_q2x2;
    sub_protocols_opts[PROTO_WK2x01] = sub_protocol_wk2x01;
    sub_protocols_opts[PROTO_Q303] = sub_protocol_q303;
    sub_protocols_opts[PROTO_GW008] = NULL;
    sub_protocols_opts[PROTO_DM002] = NULL;
    sub_protocols_opts[PROTO_CABELL] = sub_protocol_cabell;
    sub_protocols_opts[PROTO_ESKY150] = sub_protocol_esky150;
    sub_protocols_opts[PROTO_H8_3D] = sub_protocol_h8_3d;
    sub_protocols_opts[PROTO_CORONA] = sub_protocol_corona;
    sub_protocols_opts[PROTO_CFLIE] = NULL;
    sub_protocols_opts[PROTO_HITEC] = sub_protocol_hitec;
    sub_protocols_opts[PROTO_WFLY] = NULL;
    sub_protocols_opts[PROTO_BUGS] = NULL;
    sub_protocols_opts[PROTO_BUGSMINI] = sub_protocol_bugsmini;
    sub_protocols_opts[PROTO_TRAXXAS] = sub_protocol_traxxas;
    sub_protocols_opts[PROTO_NCC1701] = NULL;
    sub_protocols_opts[PROTO_E01X] = sub_protocol_e01x;
    sub_protocols_opts[PROTO_V911S] = sub_protocol_v911s;
    sub_protocols_opts[PROTO_GD00X] = sub_protocol_gd00x;
    sub_protocols_opts[PROTO_V761] = sub_protocol_v761;
    sub_protocols_opts[PROTO_KF606] = sub_protocol_KF606;
    sub_protocols_opts[PROTO_REDPINE] = sub_protocol_redpine;
    sub_protocols_opts[PROTO_POTENSIC] = NULL;
    sub_protocols_opts[PROTO_ZSX] = NULL;
    sub_protocols_opts[PROTO_HEIGHT] = sub_protocol_height;
    sub_protocols_opts[PROTO_SCANNER] = NULL;
    sub_protocols_opts[PROTO_FRSKY_RX] = sub_protocol_frsky_rx;
    sub_protocols_opts[PROTO_AFHDS2A_RX] = NULL;
    sub_protocols_opts[PROTO_HOTT] = sub_protocol_hott;
    sub_protocols_opts[PROTO_FX816] = NULL;
    sub_protocols_opts[PROTO_BAYANG_RX] = NULL;
    sub_protocols_opts[PROTO_PELIKAN] = sub_protocol_pelikan;
    sub_protocols_opts[PROTO_TIGER] = NULL;
    sub_protocols_opts[PROTO_XK] = sub_protocol_xk;
    sub_protocols_opts[PROTO_XN297DUMP] = sub_protocol_xn297dump;
    sub_protocols_opts[PROTO_FRSKYX2] = NULL;
    sub_protocols_opts[PROTO_FRSKY_R9] = sub_protocol_frsky_r9;
    sub_protocols_opts[PROTO_PROPEL] = NULL;
    sub_protocols_opts[PROTO_FRSKYL] = sub_protocol_frskyl;
    sub_protocols_opts[PROTO_SKYARTEC] = NULL;
    sub_protocols_opts[PROTO_ESKY150V2] = NULL;
    sub_protocols_opts[PROTO_DSM_RX] = NULL;
    sub_protocols_opts[PROTO_JJRC345] = sub_protocol_JJRC345;
    sub_protocols_opts[PROTO_Q90C] = NULL;
    sub_protocols_opts[PROTO_KYOSHO] = sub_protocol_kyosho;
    sub_protocols_opts[PROTO_RLINK] = sub_protocol_RLINK;
    sub_protocols_opts[PROTO_REALACC] = NULL;
    sub_protocols_opts[PROTO_OMP] = NULL;
    sub_protocols_opts[PROTO_MLINK] = NULL;
    sub_protocols_opts[PROTO_WFLY2] = NULL;
    sub_protocols_opts[PROTO_E016HV2] = NULL;
    sub_protocols_opts[PROTO_E010R5] = NULL;
    sub_protocols_opts[PROTO_LOLI] = NULL;
    sub_protocols_opts[PROTO_E129] = NULL;
    sub_protocols_opts[PROTO_JOYSWAY] = NULL;
    sub_protocols_opts[PROTO_E016H] = NULL;
    sub_protocols_opts[PROTO_CONFIG] = NULL;
    sub_protocols_opts[PROTO_IKEAANSLUTA] = NULL;
    sub_protocols_opts[PROTO_WILLIFM] = NULL;
    sub_protocols_opts[PROTO_LOSI] = NULL;
    sub_protocols_opts[PROTO_MOULDKG] = sub_protocol_MOULDKG;
    sub_protocols_opts[PROTO_XERALL] = NULL;
}