/* MSHV
 *
 * By Hrisimir Hristov - LZ2HV
 * May be used under the terms of the GNU General Public License (GPL)
 */
#ifndef NETWORK_DEF_H
#define NETWORK_DEF_H

#include "../rigdef.h"
#define NETWORK_ID 13
#define NETWORK_COUNT 6

static RigSet rigs_network[NETWORK_COUNT] =
    {
        {"Ham Radio Deluxe",RIG_PTT_RIG,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},
        {"FLRig",RIG_PTT_RIG,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},
        {"DX Lab Suite Commander",RIG_PTT_RIG,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},
        {"TCI Client RX1",RIG_PTT_RIG,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},//tci
        {"TCI Client RX2",RIG_PTT_RIG,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},//tci
        {"NET RigCtl",RIG_PTT_RIG_MICDATA,RIG_PORT_NETWORK,4800,4800,8,1,PAR_NONE,FLOW_OFF,0,0,200,3},
    };
#endif
