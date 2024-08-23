/* MSHV decoderpom
 * Copyright 2015 Hrisimir Hristov, LZ2HV
 * May be used under the terms of the GNU General Public License (GPL)
 */
#ifndef DECODERPOM_H
#define DECODERPOM_H

#include <complex.h>
#define complex		_Complex
#include "../Hv_Lib_fftw/fftw3.h"
#define NAMA_ 20
#define NPMAX 100
class HvThr
{
public:
    void four2a_c2c(double complex *a,double complex *a1,fftw_plan *pc,int &cpc,int nfft,int isign,int iform);
    void four2a_d2c(double complex *a,double complex *a1,double *d,double *d1,fftw_plan *pd,int &cpd,
                    int nfft,int isign,int iform);
    void DestroyPlans(fftw_plan *pc,int &cpc,fftw_plan *pd,int &cpd,bool imid);
private:
    int nn_c2c[NPMAX+NAMA_];
    int ns_c2c[NPMAX+NAMA_];
    int nf_c2c[NPMAX+NAMA_];
    int nn_d2c[NPMAX+NAMA_];
    int ns_d2c[NPMAX+NAMA_];
    int nf_d2c[NPMAX+NAMA_];
};
class F2a
{
public:
    void four2a_c2c(double complex *a,int nfft,int isign,int iform,int thr = 0);
    void four2a_d2c(double complex *a,double *d,int nfft,int isign,int iform,int thr = 0);
    void DestroyPlansAll(bool imid);
private:
    HvThr HvThr0;
    HvThr HvThr1;
    HvThr HvThr2;
    HvThr HvThr3;
    HvThr HvThr4;
    HvThr HvThr5;    
};

#include <QString>//2.61
class PomAll
{
public:
    void initPomAll();
    double peakup(double ym,double y0,double yp);
    double maxval_da_beg_to_end(double*a,int a_beg,int a_end);
    int maxloc_da_beg_to_end(double*a,int a_beg,int a_end);
    int maxloc_da_end_to_beg(double*a,int a_beg,int a_end);
    double db(double x);
    void polyfit(double*x,double*y,double*sigmay,int npts,int nterms,int mode,double*a,double &chisqr);
    void zero_double_beg_end(double*,int begin,int end);
    double pctile_shell(double *yellow,int nblks,int npct);    
    void zero_double_comp_beg_end(double complex*,int begin,int end);
    double ps_hv(double complex z);
    void cshift1(double complex *a,int cou_a,int ish);
    double complex sum_dca_mplay_conj_dca(double complex *a,int a_beg,int a_end,double complex *b);
    void indexx_msk(double *arr,int n,int *indx);
    bool isStandardCall(QString);//2.61  same as  MultiAnswerModW
    //bool isStandardCall(char*,int);
private:
    double pctile_shell_tmp[141122];//141072+50
    void shell(int n,double*a);
    double determ(double array_[10][10],int norder);
    //bool is_digit(char c);
    //bool is_letter(char c);
};

class PomFt
{
public:
    void initPomFt();
    void nuttal_window(double *win,int n);
    void normalizebmet(double *bmet,int n);
    void twkfreq1(double complex *ca,int npts,double fsample,double *a,double complex *cb);

    void decode174_91(double *llr,int maxosd,int norder,bool *apmask,bool *message91,bool *cw,int &nharderror,double &dmin);//ntype,//int Keff,

    //void bpdecode174_91(double *llr,bool *apmask,int maxiterations,bool *decoded77,bool *cw,int &nharderror);
    //void osd174_91(double *llr,bool *apmask,int ndeep,bool *message77,bool *cw,int &nhardmin,double &dmin);
private:
    PomAll pomAll;
    double twopi;
    double pi;
    //short crc14(unsigned char const * data, int length);
    void platanh(double x, double &y);
    //void chkcrc14a(bool *decoded,int &nbadcrc);
    int indexes_ft8_2_[2][5020];//5000+20
    int fp_ft8_2[525020];//525000+20
    int np_ft8_2[5020];//5000+20
    void boxit91(bool &reset,bool *e2,int ntau,int npindex,int i1,int i2);
    int lastpat_ft8_2;
    int inext_ft8_2;
    void fetchit91(bool &reset,bool *e2,int ntau,int &i1,int &i2);
    bool any_ca_iand_ca_eq1_91(bool *a,bool *b,int count);
    void nextpat_step1_91(bool *mi,int k,int iorder,int &iflag);
    void mrbencode91(bool *me,bool *codeword,bool g2_[91][174],int N,int K);

    void bshift1(bool *a,int cou_a,int ish);
    void get_crc14(bool *mc,int len,int &ncrc);
    bool first_osd174_91;
    //N=174, K=91, M=N-K=83
    char gen_osd174_91_[180][97];//integer*1 gen(K,N)   out from array +3
    bool first_enc174_91_nocrc;
    char gen_osd174_91_nocrc[100][95];//gen(M,K)   [100][95];//91 83
    void encode174_91_nocrc(bool *message910,bool *codeword);
    void osd174_91_1(double *llr,/*int Keff=91*/bool *apmask,int ndeep,bool *message91,bool *cw,int &nhardmin,double &dmin);
};
#endif