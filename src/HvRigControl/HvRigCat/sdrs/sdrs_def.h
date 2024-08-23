/* MSHV
 *
 * By Hrisimir Hristov - LZ2HV
 * May be used under the terms of the GNU General Public License (GPL)
 */
#ifndef SDRS_DEF_H
#define SDRS_DEF_H

#include "../rigdef.h"
#define SDRS_ID 12
#define SDRS_COUNT 2 
static RigSet rigs_sdrs[SDRS_COUNT] =
    {  
        {"Smart SDR",RIG_PTT_RIG,RIG_PORT_SERIAL,4800,9600,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},
        {"Power SDR",RIG_PTT_RIG,RIG_PORT_SERIAL,4800,9600,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},     		
    };
#endif
