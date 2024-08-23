/* MSHV Part from RigControl
 * Copyright 2015 Hrisimir Hristov, LZ2HV
 * May be used under the terms of the GNU General Public License (GPL)
 */

#include "yaesu.h"
#include "yaesu_def.h"
#include "../hvutils.h"
#include <stdio.h>
#include <QTimer>

//#include <QtGui>

#define YAESU_CMD_LENGTH 5
#define NEWCAT 201
#define USECMD 202
#define FI 0x3b  //end of fraze NEWCAT -> ;
//FT-450d
enum native_cmd_e {
    NATIVE_CAT_PTT_ON = 0,
    NATIVE_CAT_PTT_OFF,
    NATIVE_CAT_SET_FREQ,
    NATIVE_CAT_GET_FREQ_MODE_STATUS,
    NATIVE_CAT_SET_MODE_USB,
    NATIVE_CAT_GET_MODE,
    CAT_ON,
    CAT_OFF
};

struct yaesu_cmd_set
{
    unsigned char ncomp;		        /* 1 = complete, 0 = incomplete, needs extra info */
    unsigned char nseq[YAESU_CMD_LENGTH];	/* native cmd sequence */
};
typedef struct yaesu_cmd_set yaesu_cmd_set_t;
static const yaesu_cmd_set_t ncmd[YAESU_COUNT][8] =
    {
        //FT-100
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set freq */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x10 } }, /* get FREQ and MODE status 32 bites */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* mode set main USB */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x10 } }, /* get FREQ and MODE status 32 bites */
         { USECMD, { 0x00, 0x00, 0x00, 0x01, 0xfa } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-847
        {{ 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x88 } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 1, { 0x01, 0x00, 0x00, 0x00, 0x07 } }, /* mode set main USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* CAT = On */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x80 } },}, /* CAT = Off */
        //FT-990
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* Set Op Freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data return 32 bites */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* OP Mode Set USB */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data  return 32 bites */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-991
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-857
        {{ 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x88 } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 1, { 0x01, 0x00, 0x00, 0x00, 0x07 } }, /* mode set main USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-1000D
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* Set Op Freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Update Op Data  return 16 bites */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* OP Mode Set USB */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data  return 16 bites */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-1000MP
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set VFOA freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Update Op Data */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* vfo A mode set USB */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-2000
        //{{ NEWCAT, { 0x54, 0x58, 0x31, 0x3b, 0x00 } }, /*identif newcat NEWCAT TX1; ptt on */
        // { NEWCAT, { 0x54, 0x58, 0x30, 0x3b, 0x00 } },}, /*identif newcat NEWCAT TX0;  ptt off */
        //FT-2000
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-DX3000  //AI0;=off AI1;-on Auto information  VS;vefo select
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-DX5000  //AI0;=off AI1;-on Auto information  VS;vefo select
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FTDX-9000
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-950
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-920
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } },  /* set vfo A freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Status Update Data--Current operating data for VFO/Memory (28 bytes) */
         { 0, { 0x00, 0x00, 0x00, 0x01, 0x0c } },    /* mode set */ //#define MODE_SET_A_USB      0x01 P1=3bit
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },  /* Status Update Data--Current operating data for VFO/Memory (28 bytes) */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-900
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set display freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { 0, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* mode set *///#define MODE_SET_USB    0x01 P1=3bit
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-897
        {{ 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x88 } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 1, { 0x01, 0x00, 0x00, 0x00, 0x07 } }, /* mode set main USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-890
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set display freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { 0, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* mode set */  //    #define MODE_SET_USB    0x01   P1=3bit
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-840
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set display freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },  /* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { 0, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* mode set */ // #define MODE_SET_USB    0x01
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },  /* Status Update Data--Current operating data for VFO/Memory (19 bytes) */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-817
        {{ 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x88 } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 1, { 0x01, 0x00, 0x00, 0x00, 0x07 } }, /* mode set main USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x03 } }, /* get FREQ and MODE status */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-747GX
        //#define FT747_SUMO_DISPLAYED_FREQ             0x01 from_bcd_be
        //#define FT747_SUMO_VFO_A_FREQ                 0x09 from_bcd_be
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set freq */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x10 } },//2.46 get mode/frq =0x10 status #define FT747_SUMO_DISPLAYED_FREQ 0x01
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* mode set USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x10 } },//2.46 get mode/frq =0x10 status #define FT747_SUMO_DISPLAYED_MODE 0x18
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-450
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-450D
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-767GX      NONE HV ?????????  get func
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } },	//#define CMD_FREQ_SET	0x08
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } },	//#define STATUS_CURR_FREQ	1	/* Operating Frequency */ //#define STATUS_VFOA_FREQ	14 ????
         { 1, { 0x00, 0x00, 0x00, 0x11, 0x0a } },	 //set mode //#define SUBCMD_MODE_USB	0x11 /* 8 bytes returned */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } },//PROBLEM get mode   //#define STATUS_CURR_MODE	6      #define SUBCMD_MODE_USB	0x11 /* 8 bytes returned */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { USECMD, { 0x00, 0x00, 0x00, 0x01, 0x00 } },}, /* CAT = Off */
        //FT-757GX      NONE HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set freq */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x10 } }, //#define STATUS_CURR_FREQ    5   /* Operating Frequency *  /#define STATUS_VFOA_FREQ    10
         { 0, { 0x00, 0x00, 0x00, 0x01, 0x0c } },	//set mode
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x10} },	//get mode
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },},
        /* CAT = Off */
        //FRG-9600      NONE HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 1, { 0x0a, 0x00, 0x00, 0x00, 0x00 } }, /* set freq */ //-1  block
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* set mode */ //none
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get mode */ //none
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FRG-8800      NONE HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x80 } }, /* set mode*/
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FRG-100      NONE HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } }, /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* Set Op Freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* OP Mode Set USB */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //VR-5000      NONE HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } },   /* ptt off */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } },   /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x80 } },}, /* CAT = Off */
        //FT-736R   ?
        {{ 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x88 } },   /* ptt off */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x01 } },   /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get freq */ //none
         { 1, { 0x01, 0x00, 0x00, 0x00, 0x07 } },   /* set mode */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /* get mode */ //none
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */
         { USECMD, { 0x80, 0x80, 0x80, 0x80, 0x80 } },}, /* CAT = Off */
        //FT-980    TESTED HV
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } },   /* ptt off */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x08 } },   /* set freq */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* get freq */ //
         { 1, { 0x00, 0x00, 0x00, 0x11, 0x0a } },   /* set mode */	//#define MD_USB  0x11
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x01 } }, /* get mode */ //
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } }, /*  CAT = On */  //2.56
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */ //2.56
        //MARK-V FT-1000MP
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set VFOA freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Update Op Data */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* vfo A mode set USB */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //MARK-V Field FT-1000MP
        {{ 1, { 0x00, 0x00, 0x00, 0x01, 0x0f } },   /* ptt on */
         { 1, { 0x00, 0x00, 0x00, 0x00, 0x0f } }, /* ptt off */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x0a } }, /* set VFOA freq */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } }, /* Update Op Data */
         { 1, { 0x00, 0x00, 0x00, 0x01, 0x0c } }, /* vfo A mode set USB */
         { 1, { 0x00, 0x00, 0x00, 0x02, 0x10 } },/* Update Op Data */
         { USECMD, { 0x00, 0x00, 0x00, 0x00, 0x0e } }, /*  CAT = On */
         { 0, { 0x00, 0x00, 0x00, 0x00, 0x00 } },}, /* CAT = Off */
        //FT-710	
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */         	
        //FT-891
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-DX10
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-DX101
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
        //FT-DX1200
        {{ NEWCAT, { "TX1;" } }, /*identif newcat NEWCAT TX1; ptt on */
         { NEWCAT, { "TX0;" } }, /*identif newcat NEWCAT TX0;  ptt off */
         { NEWCAT, { "FA" } },   /*identif newcat NEWCAT FA;  set freq */
         { NEWCAT, { "FA;" } }, /*identif newcat NEWCAT FA;  get freq */
         { NEWCAT, { "MD0" } } , /*identif newcat NEWCAT FA;  set mode USB ili C data-usb */
         { NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  get mode */
         { USECMD, { "AI0;" } }, /* ??? 0=no use    CAT = On */
         { 0, { "AI1;" } },}, /* ??? 0=no use  CAT = Off */
    };

typedef struct
{
    int cat_read_ar_size;
    int pos_frq;
    int bcd_size;//bcd size    in NEWCAT identif word size 8 or 9
    int method_frq; //s_method_frq->  0=from_bcd,  1=from_bcd_be,  2=3int,  3=4int,  4=ft100
    double multypl;
    int pos_mod;
    unsigned char id_mod_usb;
    unsigned char id_mod_digu;
}
YaesuReadParms;
static YaesuReadParms read_parms[YAESU_COUNT] = //{0,     0,     0,        0,      1.0,    0,       0x00},//NEWCAT
    {
        //size  p frq  bcd_size  method  multyp  pos_mod  mod_usb  mod_digu->0x7e=no suport=int->126 NEWCAT=0x00
        {32,    1,     4,        4,      1.25,   5,       0x01,    0x05},//FT-100
        {5,     0,     4,        1,      10.0,   4,       0x01,    0x7e},//FT-847
        {19,    1,     3,        2,      10.0,   7,       0x01,    0x05},//FT-990  0x05=rtty
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-991 NEWCAT
        {5,     0,     4,        1,      10.0,   4,       0x01,    0x0a},//FT-857
        {16,    1,     3,        2,      10.0,   7,       0x01,    0x05},//FT-1000D  0x05=rtty
        {16,    1,     4,        3,      0.625,  7,       0x01,    0x05},//FT-1000MP" 0.625 0x05=rtty
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-2000 NEWCAT  bcd_size special!!!
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-DX3000 NEWCAT  bcd_size special!!!
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-DX5000 NEWCAT  bcd_size special!!!
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FTDX-9000 NEWCAT  bcd_size special!!!
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-950 NEWCAT  bcd_size special!!!
        {28,    1,     4,        3,      1.0,    7,       0x40,    0x05},//FT-920
        {19,    2,     3,        2,      10.0,   7,       0x01,    0x7e},//FT-900
        {5,     0,     4,        1,      10.0,   4,       0x01,    0x0a},//FT-897
        {19,    2,     3,        2,      10.0,   7,       0x01,    0x7e},//FT-890
        {19,    2,     3,        2,      10.0,   7,       0x01,    0x7e},//FT-840
        {5,     0,     4,        1,      10.0,   4,       0x01,    0x0a},//FT-817
        {344,   1,     5,        1,      1.0,    24,      0x08,    0x7e},//FT-747GX   //2.46 = pos_mode=24
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-450 NEWCAT  bcd_size special!!!
        {0,     0,     8,        0,      1.0,    0,       0x00,    0x00},//FT-450D NEWCAT  bcd_size special!!!
        {86,    1,     4,        1,      10.0,   6,       0x10,    0x7e},//FT-767GX
        {75,    5,     4,        0,      10.0,   9,       0x01,    0x7e},//FT-757GX
        {0,     0,     0,        0,      1.0,    0,       0x00,    0x7e},//FRG-9600 NO
        {0,     0,     0,        0,      1.0,    0,       0x00,    0x7e},//FRG-8800 NO
        {0,     0,     0,        0,      1.0,    0,       0x00,    0x7e},//FRG-100 NO
        {0,     0,     0,        0,      1.0,    0,       0x00,    0x7e},//VR-5000 NO
        {0,     0,     0,        0,      1.0,    0,       0x00,    0x7e},//FT-736R NO READ
        {148,   143,   4,        0,      10.0,   142,     0x01,    0x7e},//FT-980 pos EXT CTRL=121
        {16,    1,     4,        3,      0.625,  7,       0x01,    0x05},//MARK-V FT-1000MP
        {16,    1,     4,        3,      0.625,  7,       0x01,    0x05},//MARK-V Field FT-1000MP
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-710 NEWCAT
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-891 NEWCAT
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-DX10 NEWCAT
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-DX101 NEWCAT
        {0,     0,     9,        0,      1.0,    0,       0x00,    0x00},//FT-DX1200 NEWCAT
    };

static char cmd_OK_ft[YAESU_CMD_LENGTH] =
    {
        0x00,0x00,0x00,0x00,0x0b
    };
static char echo_back_ft[YAESU_CMD_LENGTH];

Yaesu::Yaesu(int ModelID,QWidget *parent)
        : QWidget(parent)
{
	/*unsigned char fbcd[4];//65536  
    unsigned long long f =  99990000;//max
    f /= 10000;
    to_bcd_be_1(fbcd,f,4);
    //fbcd[0]=128;
    //fbcd[1]=128;
    //fbcd[2]=0x00;
    //fbcd[3]=0x00;
    qDebug()<<QString("%1").arg(fbcd[0],0,16)<<QString("%1").arg(fbcd[1],0,16)
    	    <<QString("%1").arg(fbcd[2],0,16)<<QString("%1").arg(fbcd[3],0,16);
	qDebug()<< from_bcd_be_(fbcd,4);*/

    s_ModelID = ModelID;
    s_rig_name = rigs_yeasu[s_ModelID].name;
    s_ncomp = ncmd[s_ModelID][NATIVE_CAT_PTT_ON].ncomp;

    if (s_rig_name=="FT-980" || s_rig_name=="FT-767GX") f_echo_back = true;
    else f_echo_back = false;
    i_ext_cntl = 0;

    oldcat_read_ar_size = read_parms[s_ModelID].cat_read_ar_size;
    s_pos_frq = read_parms[s_ModelID].pos_frq;
    s_bcd_size = read_parms[s_ModelID].bcd_size;
    s_method_frq = read_parms[s_ModelID].method_frq;
    s_multypl = read_parms[s_ModelID].multypl;
    s_pos_mod = read_parms[s_ModelID].pos_mod;
    s_id_mod_usb = read_parms[s_ModelID].id_mod_usb;
    s_id_mod_digu= read_parms[s_ModelID].id_mod_digu;

    s_CmdID = -1;
    s_read_array.clear();

    timer_cmd_ok = new QTimer();
    connect(timer_cmd_ok, SIGNAL(timeout()), this, SLOT(SetCmdOk()));
    timer_cmd_ok->setSingleShot(true);

    //f_rig_on = false;
    ////////////////////////////////////////////////////////end new read com
    //qDebug()<<"NEW RESET "<<s_rig_name<<s_ncomp;
    /*QByteArray array;
    array.append("9");
    array.append(0x7e);
    qDebug()<<"WriteCmd"<<(QString(array.toHex()))<<array[0]<<(int)array[1];*/
    //unsigned char kkk[4] = {0x00,0x73,0x35,0x00};//00747000  00733500
    //qDebug()<< from_bcd_(kkk, 4*2)*10;
    /*unsigned char TX    = 0xa0 & 0x01;
    unsigned char SPLIT = 0xa0 & 0x08;
    unsigned char VFO   = 0xa0 & 0x20;
    unsigned char CLAR  = 0xa0 & 0x40;
    qDebug()<< TX<<SPLIT<<VFO<<CLAR;*/
}
Yaesu::~Yaesu()
{
    //qDebug()<<"Delete"<<rigs_yeasu[s_ModelID].name;
}
#include <unistd.h>
void Yaesu::SetCmdOk()
{
    emit EmitWriteCmd(cmd_OK_ft,YAESU_CMD_LENGTH);
    timer_cmd_ok->stop();
}
void Yaesu::SetWriteCmd(char *cmdd,int lenn)
{
    if (f_echo_back)
    {
        for (int i = 0; i<YAESU_CMD_LENGTH; ++i) echo_back_ft[i]=cmdd[i];
    }
    emit EmitWriteCmd(cmdd,lenn);
}
void Yaesu::SetExtCntl()//2.57
{
    unsigned char cmd[8];
    for (int i = 0; i < YAESU_CMD_LENGTH; i++)
        cmd[i]=ncmd[s_ModelID][CAT_ON].nseq[i];
    SetWriteCmd((char*)cmd,YAESU_CMD_LENGTH);
}
void Yaesu::SetOnOffCatCommand(bool f, int model_id, int fact_id)
{
    if (model_id!=s_ModelID || fact_id!=YAESU_ID)
		return;

	i_ext_cntl = 0;
    if (f && ncmd[s_ModelID][CAT_ON].ncomp==USECMD)
    {
        //qDebug()<<"Yaesu CAT ON SetOnOffCatCommand="<<rigs_yeasu[model_id].name<<rigs_yeasu[s_ModelID].name<<f;
        unsigned char cmd[50];
        int len = YAESU_CMD_LENGTH;
        if (s_ncomp == NEWCAT)//1.78 ???
            len = strlen((char*)ncmd[s_ModelID][CAT_ON].nseq);
        for (int i = 0; i < len; i++)
            cmd[i]=ncmd[s_ModelID][CAT_ON].nseq[i];
        if (s_rig_name=="FT-980") //2.57 special ID for ft980
            i_ext_cntl = 2;
        else 
        	SetWriteCmd((char*)cmd,len); //2.57 else ,EmitWriteCmd((char*)cmd,len);
    }
    else if (!f && ncmd[s_ModelID][CAT_OFF].ncomp==USECMD)
    {
        //qDebug()<<"Yaesu CAT OFF SetOnOffCatCommand<========"<<rigs_yeasu[model_id].name<<rigs_yeasu[s_ModelID].name<<f;
        unsigned char cmd[50];
        int len = YAESU_CMD_LENGTH;
        if (s_ncomp == NEWCAT)//1.78 ???
            len = strlen((char*)ncmd[s_ModelID][CAT_OFF].nseq);
        for (int i = 0; i < len; i++)
            cmd[i]=ncmd[s_ModelID][CAT_OFF].nseq[i];
        emit EmitWriteCmd((char*)cmd,len);
        if (f_echo_back)// no from timer
        {
            //for (int i = 0; i<YAESU_CMD_LENGTH; ++i) echo_back_ft980[i]=cmd[i];
            usleep(80000);
            emit EmitWriteCmd(cmd_OK_ft,YAESU_CMD_LENGTH);
        }
    }
}
void Yaesu::SetCmd(CmdID i,ptt_t ptt,QString str)
{
    switch (i)
    {
    case GET_SETT:
        emit EmitRigSet(rigs_yeasu[s_ModelID]);
        break;
    case SET_PTT:
        set_ptt(ptt);
        break;
    case SET_FREQ:
        set_freq(str.toLongLong());
        break;
    case GET_FREQ:
        s_CmdID = GET_FREQ;
        get_freq();
        break;
    case SET_MODE:
        set_mode(str);
        break;
    case GET_MODE:
        s_CmdID = GET_MODE;
        get_mode();
        break;
    }
}
void Yaesu::set_ptt(ptt_t ptt)
{
    unsigned char *cmd;		/* points to sequence to send */

    if (ptt==RIG_PTT_ON)
        cmd = (unsigned char *) &ncmd[s_ModelID][NATIVE_CAT_PTT_ON].nseq; /* get native sequence */
    else
        cmd = (unsigned char *) &ncmd[s_ModelID][NATIVE_CAT_PTT_OFF].nseq;

    if (s_ncomp == NEWCAT)
    {
        int len = strlen((const char*)cmd);
        emit EmitWriteCmd((char *)cmd,len); /* get native sequence */
    }
    else
        SetWriteCmd((char *)cmd,YAESU_CMD_LENGTH);//EmitWriteCmd((char *)cmd,YAESU_CMD_LENGTH); /* get native sequence */
    //qDebug()<<"Delete1"<<(const char)cmd[3];
}
void Yaesu::set_freq(unsigned long long freq)
{
    //qDebug()<<"FREQ====FT1000MP=="<<((((((0x00<<8) + 0x15)<<8) + 0xbc)<<8) + 0x68)*10/1; // 1=16
    //qDebug()<<"FREQ====FT1000MP=="<<((((((0x68<<8) + 0xbc)<<8) + 0x15)<<8) + 0x00)*10/1; // 1=16
    //14250000 = 0015bc68
    if (s_ncomp == NEWCAT)
    {
        ////991,891  -> 14.250.000          FA;<-read ansver->FA014250000;
        //FT2000,5000,9000,950,450,450d /10 FA;<-read ansver->FA14250000;
        char cmdnc[40];//1.75 [30 to 40] out of baunds  O4
        QString frq = (char*)ncmd[s_ModelID][NATIVE_CAT_SET_FREQ].nseq;

        if (s_bcd_size==8)//FT2000,5000,9000,950,450,450d /10
            frq.append(QString("%1").arg(freq,8,10,QChar('0'))); //FA;<-read ansver->FA14250000;
        else
            frq.append(QString("%1").arg(freq,9,10,QChar('0'))); //FA;<-read ansver->FA014250000;


        frq.append(";");

        for (int i = 0; i < frq.count(); i++)
            cmdnc[i]=frq.at(i).toLatin1();
        emit EmitWriteCmd(cmdnc,frq.count()); /* get native sequence */
        //qDebug()<<"FREQ==========="<<frq;
    }
    else
    {
        unsigned char cmd[50];
        for (int i = 0; i < YAESU_CMD_LENGTH; i++)
            cmd[i]=ncmd[s_ModelID][NATIVE_CAT_SET_FREQ].nseq[i];

        //to_bcd_be(data, (freq + 5) / 10, 8);//847,857,897,817
        //to_bcd(p->p_cmd, (freq+12)/10, 8);//747
        //to_bcd_be(cmd+1, freq/10, 8); frg9600
        if (s_rig_name=="FT-847" || s_rig_name=="FT-857" || s_rig_name=="FT-897" || s_rig_name=="FT-817")
        {
            if (s_rig_name=="FT-847")
                freq = (int)(freq)/10;
            else
                freq = (int)(freq+5)/10;
            to_bcd_be_(cmd,freq,8);
        }
        else if (s_rig_name=="FT-747GX")
        {
            //freq = (int)(freq+12)/10;
            freq = (int)(freq+5)/10;//2.46
            to_bcd_(cmd,freq,8);//??? be triabva da e
        }
        else
        {
            freq = (int)(freq)/10;
            to_bcd_(cmd,freq,8);
        }

        //to_bcd_(cmd,freq,8);	/*store bcd format in in cmd*/
        SetWriteCmd((char*)cmd,YAESU_CMD_LENGTH);//EmitWriteCmd((char*)cmd,YAESU_CMD_LENGTH); /* get native sequence */
    }
}
void Yaesu::set_mode(QString str)
{
    //{ NEWCAT, { "MD0;" } }, /*identif newcat NEWCAT FA;  set mode USB = 2 moze i da e C -> data-usb*/
    //da se dobavi 2 ili C
    if (s_ncomp == NEWCAT)
    {
        ////MD0 2; <-USB
        unsigned char cmdnc[11];
        QString mod = (char*)ncmd[s_ModelID][NATIVE_CAT_SET_MODE_USB].nseq;
        if 		(str=="LSB" ) mod.append("1");
        else if (str=="USB" ) mod.append("2");        
        else if (str=="DIGU") mod.append("C");//DIGU
        else return;//mod.append("2");//usb
        mod.append(";");

        for (int i = 0; i < mod.count(); i++) cmdnc[i]=mod.at(i).toLatin1();
        emit EmitWriteCmd((char *)cmdnc,mod.count()); /* get native sequence */
        //qDebug()<<"MOD==========="<<mod;
    }
    else
    {
    	//if (str!="USB" ) return;
        unsigned char cmd[50];
        for (int i = 0; i < YAESU_CMD_LENGTH; i++) cmd[i]=ncmd[s_ModelID][NATIVE_CAT_SET_MODE_USB].nseq[i];
        SetWriteCmd((char*)cmd,YAESU_CMD_LENGTH);//EmitWriteCmd((char*)cmd,YAESU_CMD_LENGTH);
    }
}
bool Yaesu::WaitSetExtCntl() //2.57 ft980
{
    if (i_ext_cntl < 1) return false; 
    if (i_ext_cntl < 2)
    {
        s_CmdID = 200;//special id
        SetExtCntl();	
    }	
    i_ext_cntl--; 
    return true;
}
void Yaesu::get_freq()
{
    //osobenno -> FT-767GX FT747 from_bcd_be "FT-897" FT-817
    if (s_ncomp == NEWCAT)
    {
        //FA;<-read ansver->FA014250000;
        char *cmdnc;
        cmdnc =  (char *)ncmd[s_ModelID][NATIVE_CAT_GET_FREQ_MODE_STATUS].nseq;
        int len = strlen(cmdnc);
        emit EmitWriteCmd(cmdnc,len);
    }
    else
    {
        //qDebug()<<"FR COMMAND="<<i_ext_cntl;
        if (WaitSetExtCntl()) return; //2.57

        s_read_array.clear(); //for error corection no word end
        unsigned char cmd[10];
        for (int i = 0; i < YAESU_CMD_LENGTH; i++)
            cmd[i]=ncmd[s_ModelID][NATIVE_CAT_GET_FREQ_MODE_STATUS].nseq[i];
        SetWriteCmd((char *)cmd,YAESU_CMD_LENGTH);//EmitWriteCmd((char *)cmd,YAESU_CMD_LENGTH);
    }
}
void Yaesu::get_mode()
{
    //osobenno -> FT-767GX FT747 from_bcd_be "FT-897" FT-817
    if (s_ncomp == NEWCAT)
    {
        //MD0;<-read ansver->MD02;
        char *cmdnc;
        cmdnc =  (char *)ncmd[s_ModelID][NATIVE_CAT_GET_MODE].nseq;
        int len = strlen(cmdnc);
        emit EmitWriteCmd(cmdnc,len);
    }
    else
    {
        //qDebug()<<"MD COMMAND="<<i_ext_cntl;
        if (WaitSetExtCntl()) return; //2.57

        s_read_array.clear(); //for error corection no word end
        unsigned char cmd[10];
        for (int i = 0; i < YAESU_CMD_LENGTH; i++)
            cmd[i]=ncmd[s_ModelID][NATIVE_CAT_GET_MODE].nseq[i];
        SetWriteCmd((char *)cmd,YAESU_CMD_LENGTH);//EmitWriteCmd((char *)cmd,YAESU_CMD_LENGTH);
    }
}
void Yaesu::SetReadyRead(QByteArray ar,int size0)
{
    for (int i = 0; i < size0; i++)
    {
        if (s_ncomp == NEWCAT)
        {
            if ((unsigned char)ar[i]==FI)//;=hex value ??? end of word EOM
            {
                //qDebug()<<"YESU READ ALL COMMAND="<<(QString(s_read_array.toHex()))<<s_read_array.size();
                //s_read_array.append(ar[i]); // no ;=fi added
                int size = s_read_array.size();
                //rejekt small array
                if (size < 4)//to get mde ansver->MD02;  ansver->FA014250000;
                {                    
                    s_read_array.clear();//qDebug()<<"NO INAF SIZE";
                    return;
                }
                //F=0x46 A=0x41
                if (s_CmdID==GET_FREQ && s_read_array[0]==(char)0x46 && s_read_array[1]==(char)0x41)//my qestion for freq and freq filter vfoA
                {
                    if (size==11 || size==10)//bez fd=fi (size==11)
                    {
                        QByteArray tfreq;
                        if (s_bcd_size==8)//FT2000,5000,9000,950,450,450d /10
                            tfreq.append(s_read_array.mid(2,8)); //FA;<-read ansver->FA14250000   ;
                        else
                            tfreq.append(s_read_array.mid(2,9)); //FA;<-read ansver->FA014250000  ;
                        unsigned long long f = tfreq.toLongLong();
                        emit EmitReadedInfo(GET_FREQ,QString("%1").arg(f));
                        s_CmdID = -1;//I Find my answer no need more
                    }
                }
                //M=0x4d D=0x44 0=0x30
                if (s_CmdID==GET_MODE && s_read_array[0]==(char)0x4d && s_read_array[1]==(char)0x44 && s_read_array[2]==(char)0x30)//my qestion for mode and mode filter
                {
                    if (size==4)//no fd=fi 4
                    {
                        QString smode = "WRONG_MODE";
						if 		(s_read_array[3]==(char)0x31) smode = "LSB"; //0x31=LSB=1
                        else if (s_read_array[3]==(char)0x32) smode = "USB"; //0x32=USB=2                          
                        else if (s_read_array[3]==(char)0x33) smode = "CWU";//0x33=CW-U=3 
						else if (s_read_array[3]==(char)0x34) smode = "FM";  //0x34=FM=4 
						else if (s_read_array[3]==(char)0x35) smode = "AM";  //0x35=AM=5 
						else if (s_read_array[3]==(char)0x36) smode = "R-L"; //0x36=RTTY-LSB=6
						else if (s_read_array[3]==(char)0x37) smode = "CWL";//0x37=CW-L=7
						else if (s_read_array[3]==(char)0x38) smode = "DIGL";//0x38=DATA-LSB=8
						else if (s_read_array[3]==(char)0x39) smode = "R-U"; //0x39=RTTY-USB=9						
                        else if (s_read_array[3]==(char)0x41) smode = "FM-D"; //0x41=DATA-FM=A
						//else if (s_read_array[3]==(char)0x42) smode = "FM-N";//0x42=FM-N=B
						else if (s_read_array[3]==(char)0x43) smode = "DIGU";//0x43=DATA-USB=C
						//else if (s_read_array[3]==(char)0x44) smode = "AM-N";//0x44=AM-N=D 
						else if (s_read_array[3]==(char)0x45) smode = "C4FM";//0x44=AM-N=D					                            
                        emit EmitReadedInfo(GET_MODE,smode);
                        s_CmdID = -1;//I Find my answer no need more
                    }
                }
                s_read_array.clear();
            }
            else s_read_array.append(ar[i]);
        }
        else
        {
            s_read_array.append(ar[i]);
            if (f_echo_back)//2.56 ft980 ft767gx
            {
                if (s_read_array.size()==5 && size0==5)
                {
                    if (    s_read_array[0]==(char)echo_back_ft[0] && s_read_array[1]==(char)echo_back_ft[1] &&
                            s_read_array[2]==(char)echo_back_ft[2] && s_read_array[3]==(char)echo_back_ft[3] &&
                            s_read_array[4]==(char)echo_back_ft[4])
                    {
                        s_read_array.clear();
                        timer_cmd_ok->start(20);
                        continue;
                    }
                }
            }
            if (s_read_array.size()==oldcat_read_ar_size)
            {
                //qDebug()<<"YAESU READ ALL COMMAND="<<(QString(s_read_array.toHex()))<<s_read_array.size();
                if (s_CmdID==200) //2.57 ft980 special ID for ft980
                {
                    //qDebug()<<"COMMAND="<<s_CmdID<<(unsigned char)s_read_array[121];
                    unsigned char ext_cntl = (unsigned char)s_read_array[121];
                    if (ext_cntl==0x00) QTimer::singleShot(20, this, SLOT(SetExtCntl()));
                }
                if (s_CmdID==GET_FREQ || s_CmdID==GET_MODE)
                {
                    unsigned char *fbcd = new unsigned char[s_bcd_size+2];//fbcd[s_bcd_size+2];
                    for (int j=0; j < s_bcd_size; j++)//2.12
                        fbcd[j]=(unsigned char)s_read_array[j+s_pos_frq];

                    unsigned long long freq = 0;
                    if (s_method_frq==0)//s_method_frq-> 0=from_bcd,1=from_bcd_be,2=3int,3=4int,4=ft100
                        freq = from_bcd_(fbcd, s_bcd_size*2)*s_multypl;
                    else if (s_method_frq==1)
                        freq = from_bcd_be_(fbcd, s_bcd_size*2)*s_multypl;
                    else if (s_method_frq==2)
                        freq = ((((fbcd[0]<<8) + fbcd[1])<<8) + fbcd[2])*s_multypl;
                    else if (s_method_frq==3)
                        freq = (int)((double)((((((fbcd[0]<<8) + fbcd[1])<<8) + fbcd[2])<<8) + fbcd[3])*s_multypl);
                    //freq = (int)((double)(((((((unsigned char)s_read_array[1]<<8) + (unsigned char)s_read_array[2])<<8) + (unsigned char)s_read_array[3])<<8) + (unsigned char)s_read_array[4])*s_multypl);
                    else if (s_method_frq==4)
                    {
                        char freq_str[20];
                        sprintf(freq_str, "%02X%02X%02X%02X",fbcd[0],fbcd[1],fbcd[2],fbcd[3]);
                        unsigned long long d1=strtol(freq_str,NULL,16);
                        freq =(int)((double)d1*s_multypl);
                    }
                    emit EmitReadedInfo(GET_FREQ,QString("%1").arg(freq));

                    unsigned char mod = (unsigned char)s_read_array[s_pos_mod];
                    if (s_rig_name=="FT-747GX")//2.46
                    {
                        mod &= 0x9f;
                        mod = (mod & 0x1f);  //qDebug()<<"MODE="<<mod<<s_rig_name;
                    }
                    QString smode = "WRONG_MODE";
                    if (mod==s_id_mod_usb)//USB
                        smode = "USB";
                    else if (mod==s_id_mod_digu && s_id_mod_digu!=0x7e)//DATA-USB  no suported 0x7e
                        smode = "DIGU";

                    emit EmitReadedInfo(GET_MODE,smode);
                    delete [] fbcd;
                }
                s_CmdID = -1;
                s_read_array.clear();
            }
        }
    }
    //qDebug()<<s_read_array.size();
    if (s_read_array.size()>1024) s_read_array.clear(); //2.55 protection  max>344=FT-747GX=512  something is wrong
}




