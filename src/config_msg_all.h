#ifndef __CONFIG_MSG_ALL_H__
#define __CONFIG_MSG_ALL_H__          

#if defined _JTMS_DH_
//////////  JTMS  /////////////////////////////////////////////////////////////////////////////////
static const char cc_JTMS_RX[64]=
    {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I',
     'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','.','/','?','-',
     ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_',' ',' ',' ',' ',' ','@'
    };
static const char cc_JTMS_TX[64]=
    {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I',
     'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','.','/','?','-',
     ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_',' ',' ',' ',' ',' ','@'
    };
#endif
//!                    1         2         3         4         5         6
//!          0123456789012345678901234567890123456789012345678901234567890123
//  data cc/'0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ./?-                 _     @'/
//////////  JTMS END  /////////////////////////////////////////////////////////////////////////////

#if defined _FSK441_DH_
//////////  FSK441  ///////////////////////////////////////////////////////////////////////////////
//#define NSPD441 25
//#define NSPD315 35 
//#define LTONE441 2
//#define LTONE315 3
//msk144=0 jtms=0,fsk441=25,fsk315=35,fsk735=15,fsk105=105,iscat-a,iscat-b,jt6m
//static const int NSPD_FOM_MODE[10] = 
//    {0,0,25,35,15,105};
//msk144=0 jtmsk=0 jtms=0,fsk441=2,fsk315=3,fsk735=1,fsk105=10,iscat-a,iscat-b,jt6m
//static const int LTONE_FOM_MODE[10] = 
//    {0,0,2 ,3 ,1 ,10};
inline static int NSPD_FOM_MODE(int i)//2.65
{
	int i0 = 25;		//fsk441
	if (i==3) i0=35;	//fsk315
    return i0;
} 
inline static int LTONE_FOM_MODE(int i)//2.65
{
	int i0 = 2;			//fsk441
	if (i==3) i0=3;		//fsk315
    return i0;
}
   	
/*
static const char c_FSK441_RX[48] =
    {' ','1','2','3','4','5','6','7','8','9','.',',','?','/',
     ' ',' ','$','A','B','C','D',' ','F','G','H','I','J','K','L','M','N','O',
     'P','Q','R','S','T','U','V','W','X','Y',' ','0','E','Z',' ',' '
    };
*/
    
static const char c_FSK441_RX[48] =
    {' ','1','2','3','4','5','6','7','8','9','.',',','?','/',
     '#',' ','$','A','B','C','D',' ','F','G','H','I','J','K','L','M','N','O',
     'P','Q','R','S','T','U','V','W','X','Y',' ','0','E','Z','*','!'
    };    
//data c/' 123456789.,?/# $ABCD FGHIJKLMNOPQRSTUVWXY 0EZ*!'/

static const int lookup_FSK441_TXRX[92] =
    {
        13, 15, 17, 46, 47, 45, 44, 12, 11, 14,
        1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
        16, 48, 18, 19, 20, 21, 22, 23, 24, 25,
        26, 27, 15, 47, 30, 14, 16, 42, 46, 35,
        36, 37, 21,  0, 11, 41, 10, 13, 43,  1,
        2,  3,  4,  5,  6,  7,  8,  9, 49, 56,
        52, 55, 54, 12, 63, 17, 18, 19, 20, 44,
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
        45, 63
    };
/*    
static const int lookup_FSK441_RX[92] =
    {
        13, 15, 17, 46, 47, 45, 44, 12, 11, 14,
        1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
        16, 48, 18, 19, 20, 21, 22, 23, 24, 25,
        26, 27, 15, 47, 30, 14, 16, 42, 46, 35,
        36, 37, 21,  0, 11, 41, 10, 13, 43,  1,
        2,  3,  4,  5,  6,  7,  8,  9, 49, 56,
        52, 55, 54, 12, 63, 17, 18, 19, 20, 44,
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
        45, 63
    };
*/    
//!                   1         2         3         4
//!          1234567890123456789012345678901234567890123
//  data cc/' 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.,?/#$'/
//  data lookup/13, 15, 17, 46, 47, 45, 44, 12, 11, 14, &
//               1,  2,  3,  4,  5,  6,  7,  8,  9, 10, &
//              16, 48, 18, 19, 20, 21, 22, 23, 24, 25, &
//              26, 27, 15, 47, 30, 14, 16, 42, 46, 35, &
//              36, 37, 21,  0, 11, 41, 10, 13, 43,  1, &
//               2,  3,  4,  5,  6,  7,  8,  9, 49, 56, &
//              52, 55, 54, 12, 63, 17, 18, 19, 20, 44, &
//              22, 23, 24, 25, 26, 27, 28, 29, 30, 31, &
//              32, 33, 34, 35, 36, 37, 38, 39, 40, 41, &
//              45, 63/
//////////  FSK441 END ////////////////////////////////////////////////////////////////////////////
#endif

#if defined _JT6M_DH_
//////////  JT6M  /////////////////////////////////////////////////////////////////////////////////
static const char pua_JT6M_RX[43]=
    {'0','1','2','3','4','5','6','7','8','9','.',',',' ','/','#','?','$',
     'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
     'P','Q','R','S','T','U','V','W','X','Y','Z'
    };
//data pua/'0123456789., /#?$ABCDEFGHIJKLMNOPQRSTUVWXYZ'/
//////////  JT6M END //////////////////////////////////////////////////////////////////////////////
#endif
#if defined _ISCAT_DH_
//////////  ISCAT-A ISCAT-B  //////////////////////////////////////////////////////////////////////
static const char c_ISCAT_RX[42]=
    {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H',
     'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
     ' ','/','.','?','@','-'
    };
static const char c_ISCAT_TX[42]=
    {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H',
     'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
     ' ','/','.','?','@','-'
    };
static const int nsync_rx = 4;
static const int nlen_rx = 2;
static const int ndat_rx = 18;    
static const int nsync_tx = 4;
static const int nlen_tx = 2;
static const int ndat_tx = 18;

//c/'0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ /.?@-'/
//////////  ISCAT-A ISCAT-B END ///////////////////////////////////////////////////////////////////
#endif
#endif // __CONFIG_MSG_ALL_H__
