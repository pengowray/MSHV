#ifndef __CONFIG_STR_CON_H__
#define __CONFIG_STR_CON_H__
  
#define COUNT_CONTEST 17 
#if defined _CONT_NAME_
static const QString s_cont_name[COUNT_CONTEST] =
    {"None","EU RSQ And Serial Number","NA VHF Contest","EU VHF Contest","ARRL Field Day",
     "ARRL Inter. Digital Contest","WW Digi DX Contest",
     "FT4 DX Contest","FT8 DX Contest","FT Roundup Contest","Bucuresti Digital Contest",
     "FT4 SPRINT Fast Training","PRO DIGI Contest","CQ WW VHF Contest","Pileup","NCCC Sprint",
     "ARRL Inter. EME Contest"//,"XX"
    };
#endif
#if defined _CONT_NAMEID_      
static const QString s_cont_name_id[COUNT_CONTEST] =
    {"NONE","EU-RSQ-AND-SN","NA-VHF-CONTEST","EU-VHF-CONTEST","ARRL-FIELD-DAY","ARRL-DIGI","WW-DIGI",
     "FT4-DX","FT8-DX","FT8-RU","BUCURESTI-DIGITAL","FT4-SPRINT","FT4-PDC","CQ-VHF","PILEUP","NCCC-SPRINT",
     "ARRL-EME"//,"XX-XXX"
    }; 
#endif         
#endif // __CONFIG_STR_CON_H__
