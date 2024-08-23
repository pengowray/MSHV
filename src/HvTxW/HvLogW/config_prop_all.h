#ifndef __CONFIG_MS_PROP_H__
#define __CONFIG_MS_PROP_H__

#define COUNT_PROP 19
static const char *s_prop_mod[COUNT_PROP] =
    {"None","Aircraft Scatter","Aurora-E","Aurora","Back scatter","EchoLink"
     ,"Earth-Moon-Earth","Sporadic E","F2 Reflection","FAI"//->"Field Aligned Irregularities"
     ,"Internet-assisted","Ionoscatter","IRLP"
     ,"Meteor scatter","RPT"//->"Terrestrial or atmospheric repeater or transponder"
     ,"Rain scatter","Satellite","Trans-equatorial","Tropospheric ducting"
    };
static const char *s_id_prop_mod[COUNT_PROP] =
    {"","AS","AUE","AUR","BS","ECH"
     ,"EME","ES","F2","FAI"
     ,"INTERNET","ION","IRL"
     ,"MS","RPT"
     ,"RS","SAT","TEP","TR"
    };
#define COUNT_SATM 20
static const char *s_id_sat_mod[COUNT_SATM] =
    {"","A","B","BS","JA","JD","K","KA","KT","L","LS","LU","LX","S","SX","T","US","UV","VS","VU"
    };
//#if defined _S0SATIDN_    
#define COUNT_SATIDN 82
static const char *s0_sat_id_mod[COUNT_SATIDN][2] =
    {
        {"AISAT1","AISAT-1 AMSAT India APRS Digipeater"},
        {"AO-10","AMSAT-OSCAR 10"},
        {"AO-109","AMSAT-OSCAR 109"},
        {"AO-13","AMSAT-OSCAR 13"},
        {"AO-16","AMSAT-OSCAR 16"},
        {"AO-21","OSCAR 21/RS-14"},
        {"AO-27","AMRAD-OSCAR 27"},
        {"AO-3","AMSAT-OSCAR 3"},
        {"AO-4","AMSAT-OSCAR 4"},
        {"AO-40","AMSAT-OSCAR 40"},
        {"AO-51","AMSAT-OSCAR 51"},
        {"AO-6","AMSAT-OSCAR 6"},
        {"AO-7","AMSAT-OSCAR 7"},
        {"AO-73","AMSAT-OSCAR 73"},
        {"AO-8","AMSAT-OSCAR 8"},
        {"AO-85","AMSAT-OSCAR 85 (Fox-1A)"},
        {"AO-91","AMSAT-OSCAR 91 (RadFxSat / Fox-1B)"},
        {"AO-92","AMSAT-OSCAR 92 (Fox-1D)"},
        {"ARISS","ARISS"},
        {"Arsene","OSCAR 24"},
        {"BO-102","BIT Progress-OSCAR 102 (CAS-7B)"},
        {"BY70-1","Bayi Kepu Weixing 1"},
        {"CAS-3H","LilacSat-2"},
        {"CAS-4A","CAMSAT 4A (CAS-4A)"},
        {"CAS-4B","CAMSAT 4B (CAS-4B)"},
        {"DO-64","Delfi OSCAR-64"},
        {"EO-79","FUNcube-3"},
        {"EO-88","Emirates-OSCAR 88 (Nayif-1)"},
        {"FO-12","Fuji-OSCAR 12"},
        {"FO-20","Fuji-OSCAR 20"},
        {"FO-29","Fuji-OSCAR 29"},
        {"FO-99","Fuji-OSCAR 99 (NEXUS)"},
        {"FS-3","FalconSAT 3"},
        {"HO-107","HuskySat OSCAR 107"},
        {"HO-113","HO-113"},
        {"HO-68","Hope-Oscar 68"},
        {"IO-86","Indonesia-OSCAR 86 (LAPAN-ORARI)"},
        {"JO-97","Jordan-OSCAR 97(JY1Sat)"},
        {"KEDR","ARISSat-1"},
        {"LO-19","Lusat-OSCAR 19"},
        {"LO-78","LituanicaSAT-1"},
        {"LO-87","LUSEX-OSCAR 87"},
        {"LO-90","LilacSat-OSCAR 90 (LilacSat-1)"},
        {"MAYA-3","Cubesat"},
        {"MAYA-4","Cubesat"},
        {"MIREX","MIR Packet Digipeater"},
        {"NO-103","Navy-OSCAR 103 (BRICSAT 2)"},
        {"NO-104","Navy-OSCAR 104 (PSAT 2)"},
        {"NO-44","Navy-OSCAR 44"},
        {"NO-83","BRICsat"},
        {"NO-84","PSAT"},
        {"PO-101","Phillipines-OSCAR-101 (Diwata-2)"},
        {"QO-100","Qatar-OSCAR 100 (Es'hail-2/P4A)"},
        {"RS-1","Radio Sputnik 1"},
        {"RS-10","Radio Sputnik 10"},
        {"RS-11","Radio Sputnik 11"},
        {"RS-12","Radio Sputnik 12"},
        {"RS-13","Radio Sputnik 13"},
        {"RS-15","Radio Sputnik 15"},
        {"RS-2","Radio Sputnik 2"},
        {"RS-44","Radio Sputnik 44 (DOSAAF-85)"},
        {"RS-5","Radio Sputnik 5"},
        {"RS-6","Radio Sputnik 6"},
        {"RS-7","Radio Sputnik 7"},
        {"RS-8","Radio Sputnik 8"},
        {"SAREX","SAREX Digipeater"},
        {"SO-35","Sunsat-OSCAR 35"},
        {"SO-41","Saudi-OSCAR 41"},
        {"SO-50","Saudi-OSCAR 50"},
        {"SO-67","Sumbandila Oscar 67"},
        {"TAURUS","Taurus-1 (Jinniuzuo-1)"},
        {"TO-108","TQ-OSCAR 108 (CAS-6 / TQ-1)"},
        {"UKUBE1","UKube-1 (FUNcube-2)"},
        {"UO-14","UOSAT-OSCAR 14"},
        {"UVSQ","CubeSat"},
        {"VO-52","VUsat-OSCAR 52"},
        {"XW-2A","Hope 2A (CAS-3A)"},
        {"XW-2B","Hope 2B (CAS-3B)"},
        {"XW-2C","Hope 2C (CAS-3C)"},
        {"XW-2D","Hope 2D (CAS-3D)"},
        {"XW-2E","Hope 2E (CAS-3E)"},
        {"XW-2F","Hope 2F (CAS-2F)"}
    };
//#endif
#endif // __CONFIG_MSG_PROP_H__
