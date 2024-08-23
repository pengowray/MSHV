/* MSHV HvRigCat
 * Copyright 2015 Hrisimir Hristov, LZ2HV
 * May be used under the terms of the GNU General Public License (GPL)
 */
#include "hvrigcatw.h"
//#include <QtGui>

#include "yaesu/yaesu_def.h"
#include "yaesu/yaesu.h"
#include "icom/icom_def.h"
#include "icom/icom.h"
#include "kenwood/kenwood_def.h"
#include "kenwood/kenwood.h"
#include "elecraft/elecraft_def.h"
#include "elecraft/elecraft.h"
#include "tentec/tentec_def.h"
#include "tentec/tentec.h"
#include "alinco/alinco_def.h"
#include "alinco/alinco.h"
#include "jrc/jrc_def.h"
#include "jrc/jrc.h"
#include "drake/drake_def.h"
#include "drake/drake.h"
#include "icmarine/icmarine_def.h"
#include "icmarine/icmarine.h"
#include "pcr/pcr_def.h"
#include "pcr/pcr.h"
#include "racal/racal_def.h"
#include "racal/racal.h"
#include "sdrs/sdrs_def.h"
#include "sdrs/sdrs.h"
#include "network/network_def.h"
#include "network/network.h"
#include "mits/mits_def.h"
#include "mits/mits.h"
////// omnirig /////////////
#if defined _WIN32_
#include "omnirig/omnirighv_def.h"
#include "omnirig/omnirighv.h"
#endif
////// omnirig end /////////

#include <unistd.h>   // uslepp Linux and windows
//#include <windows.h>  // Slepp windows
/*#if defined _LINUX_
#include <unistd.h>
#endif

#if defined _WIN32_
#include <windows.h>
#endif
*/
typedef struct
{
    QString name;
    int facid;
    int model;
}
rigstruct;
static QList<rigstruct>lmodel;
bool hv_sort_nameorder(const rigstruct &d1,const rigstruct &d2)
{
    return d1.name.toLower() < d2.name.toLower();
}
HvRigCat::HvRigCat( QWidget *parent )
        : QWidget(parent)
{
    //s_mode = 11;//tci
    tci_select = 0;
    c_rig_cat_active_and_read = 200;//reset
    s_active_model_id = -100;
    s_active_fact_id = -100;
    have_read_data_rts_on = 0;
    c_poll_comm = -1;
    s_port_type = RIG_PORT_NONE;
    s_port_poen = false;

    f_rig_activ = false;
    QLabel *l_rb = new QLabel("RIG:");
    RigsBoxs = new QComboBox();
    RigsBoxs->setFixedWidth(215);

    rb_ptt = new QRadioButton("PTT");
    rb_ptt_mic = new QRadioButton("PTT MIC");
    rb_ptt_data = new QRadioButton("PTT DATA");  
    sb_pollint = new QSpinBox();
    sb_pollint->setPrefix("Poll, int  ");
    sb_pollint->setSuffix("  ms");
    sb_pollint->setRange(200,1000);
    sb_pollint->setSingleStep(100);
    sb_pollint->setValue(1000);
    sb_pollint->findChild<QLineEdit*>()->setReadOnly(true);
    //sb_pollint->setStyleSheet("QSpinBox {selection-color: black; selection-background-color: white;}");

    connect(rb_ptt, SIGNAL(toggled(bool)), this, SLOT(PttChanget(bool)));
    connect(rb_ptt_mic, SIGNAL(toggled(bool)), this, SLOT(PttChanget(bool)));
    connect(rb_ptt_data, SIGNAL(toggled(bool)), this, SLOT(PttChanget(bool)));
    connect(sb_pollint, SIGNAL(valueChanged(int)), this, SLOT(SBPollIntChanged(int)));

    SetPttCaps(false);
    s_ptt_on_type = RIG_PTT_ON;

    QHBoxLayout *H_L = new QHBoxLayout();
    H_L->setContentsMargins(1,1,1,1);
    H_L->setSpacing(4);
    H_L->addWidget(l_rb);
    H_L->addWidget(RigsBoxs);
    H_L->addWidget(rb_ptt);
    H_L->addWidget(rb_ptt_mic);
    H_L->addWidget(rb_ptt_data);
    H_L->addWidget(sb_pollint);

    //H_L->removeWidget(H_L->itemAt(0)->widget());
    this->setLayout(H_L);

    //QStringList ls;
    rigstruct rstruc;
    for (int i = 0; i<YAESU_COUNT; i++)
    {
        rstruc.name = "Yaesu "+rigs_yeasu[i].name;
        rstruc.facid = YAESU_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<ICOM_COUNT; i++)
    {
    	QString rn = rigs_icom[i].name;
    	if (!rn.startsWith("Xiegu ")) rn = "Icom "+rigs_icom[i].name;  
        rstruc.name = rn;
        rstruc.facid = ICOM_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<KENWOOD_COUNT; i++)
    {
        rstruc.name = "Kenwood "+rigs_kenwood[i].name;
        rstruc.facid = KENWOOD_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<ELECRAFT_COUNT; i++)
    {
        rstruc.name = "Elecraft "+rigs_elecraft[i].name;
        rstruc.facid = ELECRAFT_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<TENTEC_COUNT; i++)
    {
        rstruc.name = "Ten-Tec "+rigs_tentec[i].name;
        rstruc.facid = TENTEC_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<ALINCO_COUNT; i++)
    {
        rstruc.name = "Alinco "+rigs_alinco[i].name;
        rstruc.facid = ALINCO_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<JRC_COUNT; i++)
    {
        rstruc.name = "JRC "+rigs_jrc[i].name;
        rstruc.facid = JRC_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<DRAKE_COUNT; i++)
    {
        rstruc.name = "Drake "+rigs_drake[i].name;
        rstruc.facid = DRAKE_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<ICMARINE_COUNT; i++)
    {
        rstruc.name = "Icom "+rigs_icmarine[i].name;
        rstruc.facid = ICMARINE_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<PCR_COUNT; i++)
    {
        rstruc.name = "Icom "+rigs_pcr[i].name;
        rstruc.facid = PCR_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<RACAL_COUNT; i++)
    {
        rstruc.name = "Racal "+rigs_racal[i].name;
        rstruc.facid = RACAL_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
////// omnirig /////////////
#if defined _WIN32_
    for (int i = 0; i<OMNIRIGHV_COUNT; i++)
    {
        rstruc.name = "OmniRig "+rigs_omnirighv[i].name;
        rstruc.facid = OMNIRIGHV_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
#endif
////// omnirig end /////////
    for (int i = 0; i<NETWORK_COUNT; i++)
    {
        rstruc.name = rigs_network[i].name;
        rstruc.facid = NETWORK_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<SDRS_COUNT; i++)
    {
        rstruc.name = rigs_sdrs[i].name;
        rstruc.facid = SDRS_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }
    for (int i = 0; i<MITS_COUNT; i++)
    {
        rstruc.name = rigs_mits[i].name;
        rstruc.facid = MITS_ID;
        rstruc.model = i;
        lmodel.append(rstruc);
    }

    /*model.sort(0);
    ls.clear();
    ls << "None" << "-1" << "-1";
    InsertItemModel(ls);*/
    std::sort(lmodel.begin(),lmodel.end(),hv_sort_nameorder);
    rstruc.name = "None";
    rstruc.facid = -1;
    rstruc.model = -1;
    lmodel.prepend(rstruc);

    QStringList lst_outb;
    for (int i = 0; i<lmodel.count(); i++)
    {
        //lst_outb << model.data(model.index(i,0,QModelIndex()),Qt::DisplayRole).toString();
        lst_outb << lmodel.at(i).name; //qDebug()<<i<<lmodel.at(i).name;
    }    
    RigsBoxs->addItems(lst_outb);
    connect(RigsBoxs, SIGNAL(activated(int)), this, SLOT(SetRig(int)));

    polling_timer = new QTimer();
    connect(polling_timer, SIGNAL(timeout()), this, SLOT(PollingTimerReadRig()));
    s_f_ptt = false;
    tx_rx_timer = new QTimer();
    connect(tx_rx_timer, SIGNAL(timeout()), this, SLOT(StartPttTimer()));

////// omnirig /////////////
#if defined _WIN32_
    omnirig_reinit_timer = new QTimer();
    connect(omnirig_reinit_timer, SIGNAL(timeout()), this, SLOT(SetOmniRigReinit()));
#endif
////// omnirig end /////////////
}
HvRigCat::~HvRigCat()
{}
/*
void HvRigCat::CatStopPttIfClose()
{
	if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen && s_f_ptt)
	{
		emit SetCmd(SET_PTT,RIG_PTT_OFF,"");
		//qDebug()<<"CLOSE DestroyPort()"<<s_port_poen<<s_f_ptt;
	}
}
*/
/*void HvRigCat::SetMode(int m)//tci for samplerate
{
	s_mode = m;
	emit EmitMode(s_mode);
}*/
/*bool HvRigCat::hv_sort_nameorder(const rigstruct &d1,const rigstruct &d2)
{
    return d1.name.toLower() < d2.name.toLower();
}*/
/*void HvRigCat::SetFont(QFont fo)
{
    sb_pollint->setFont(fo);
}*/
void HvRigCat::SBPollIntChanged(int i)
{	//qDebug()<<i;
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen) polling_timer->start(i);	
}
void HvRigCat::SetTciSelect(int i)
{
	tci_select = i;
	emit EmitTciSelect(tci_select);
}
void HvRigCat::DestroyRig()
{
    if (f_rig_activ)
    {
        //qDebug()<<"DestroyRig()";
        emit DesleteRig();
    }
    f_rig_activ = false;
}
////// omnirig /////////////
#if defined _WIN32_
void HvRigCat::SetOmniRigChanged()//from omni rig reinit
{
    omnirig_reinit_timer->start(752);//hi hi
}
void HvRigCat::SetOmniRigReinit()
{
    omnirig_reinit_timer->stop();
    int index = RigsBoxs->currentIndex();
    if (index >= 0)
    {
        RigsBoxs->setCurrentIndex(index);
        SetRig(index);//tova opravia i SetPttCaps();
    }
}
#endif
////// omnirig end /////////////
void HvRigCat::SetRig(int index)
{
    StartStopPollingTimer(false);
    //if (index!=0)//not up 0 object
    //{
    //	RigSet sett = 0;

    rigstruct rstruc;
    rstruc.facid = lmodel.at(index).facid;
    rstruc.model = lmodel.at(index).model;
    rstruc.name = lmodel.at(index).name;
    //qDebug()<<"STOP TIMER SetRig="<<index<<rstruc.facid<<rstruc.model;

    DestroyRig(); 
    s_active_model_id = -100;
    s_active_fact_id = -100;

    have_read_data_rts_on = 0;

    if (rstruc.facid == YAESU_ID)
    {
        Yaesu *TYaesu = new Yaesu(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TYaesu, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TYaesu, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TYaesu, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TYaesu, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TYaesu, SLOT(SetReadyRead(QByteArray,int)));
        connect(TYaesu, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TYaesu, SLOT(SetOnOffCatCommand(bool,int,int)));
        have_read_data_rts_on = 3; //2.56 need to be here
        s_active_model_id = rstruc.model;
        s_active_fact_id = YAESU_ID;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TYaesu, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == ICOM_ID)
    {
        Icom *TIcom = new Icom(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TIcom, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TIcom, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TIcom, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TIcom, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TIcom, SLOT(SetReadyRead(QByteArray,int)));
        connect(TIcom, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        //connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TIcom, SLOT(SetOnOffCatCommand(bool,int,int)));
        have_read_data_rts_on = 3; //2.56 need to be here
        //s_active_model_id = lst_outb.at(2).toInt();
        //s_active_fact_id = ICOM_ID;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TIcom, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == KENWOOD_ID)
    {
        Kenwood *TKenwood = new Kenwood(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TKenwood, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TKenwood, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TKenwood, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TKenwood, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TKenwood, SLOT(SetReadyRead(QByteArray,int)));
        connect(TKenwood, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TKenwood, SLOT(SetOnOffCatCommand(bool,int,int)));        
        have_read_data_rts_on = 3;//2.56 need to be here
		/*if 	(rstruc.name=="Kenwood TS-940S")   have_read_data_rts_on = 2;        
        else if (rstruc.name=="Kenwood TS-950SDX") have_read_data_rts_on = 1;
        else if (rstruc.name=="Kenwood TS-990s")   have_read_data_rts_on = 0;
        else have_read_data_rts_on = 3;*/
        s_active_model_id = rstruc.model;
        s_active_fact_id = KENWOOD_ID;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(true);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TKenwood, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == ELECRAFT_ID)
    {
        Elecraft *TElecraft = new Elecraft(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TElecraft, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TElecraft, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TElecraft, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TElecraft, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TElecraft, SLOT(SetReadyRead(QByteArray,int)));
        connect(TElecraft, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TElecraft, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == TENTEC_ID)
    {
        Tentec *TTentec = new Tentec(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TTentec, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TTentec, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TTentec, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TTentec, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TTentec, SLOT(SetReadyRead(QByteArray,int)));
        connect(TTentec, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TTentec, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == ALINCO_ID)
    {
        Alinco *TAlinco = new Alinco(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TAlinco, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TAlinco, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TAlinco, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TAlinco, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TAlinco, SLOT(SetReadyRead(QByteArray,int)));
        connect(TAlinco, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TAlinco, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == JRC_ID)
    {
        Jrc *TJrc = new Jrc(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TJrc, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TJrc, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TJrc, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TJrc, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TJrc, SLOT(SetReadyRead(QByteArray,int)));
        connect(TJrc, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TJrc, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == DRAKE_ID)
    {
        Drake *TDrake = new Drake(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TDrake, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TDrake, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TDrake, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TDrake, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TDrake, SLOT(SetReadyRead(QByteArray,int)));
        connect(TDrake, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TDrake, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == ICMARINE_ID)
    {
        Icmarine *TIcmarine = new Icmarine(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TIcmarine, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TIcmarine, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TIcmarine, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TIcmarine, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TIcmarine, SLOT(SetReadyRead(QByteArray,int)));
        connect(TIcmarine, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TIcmarine, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == PCR_ID)
    {
        Pcr *TPcr = new Pcr(rstruc.model); 
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TPcr, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TPcr, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        //connect(TPcr, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TPcr, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        //connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TPcr, SLOT(SetReadyRead(QByteArray,int)));
        //connect(TPcr, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TPcr, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == RACAL_ID)
    {
        Racal *TRacal = new Racal(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TRacal, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TRacal, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TRacal, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TRacal, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TRacal, SLOT(SetReadyRead(QByteArray,int)));
        connect(TRacal, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TRacal, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
////// omnirig /////////////
#if defined _WIN32_
    else if (rstruc.facid == OMNIRIGHV_ID)
    {
        OmniRigHV *TOmniRigHV = new OmniRigHV(rstruc.model);

        connect(TOmniRigHV, SIGNAL(EmitOmniRigActive(bool,QString)), this, SIGNAL(EmitOmniRigActive(bool,QString)));
        connect(this, SIGNAL(EmitOmniRigPttMethod(bool*)), TOmniRigHV, SLOT(SetOmniRigPttMethod(bool*)));
        connect(TOmniRigHV, SIGNAL(EmitOmniRigChanged()), this, SLOT(SetOmniRigChanged()));

        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TOmniRigHV, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TOmniRigHV, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TOmniRigHV, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TOmniRigHV, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        //connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TOmniRigHV, SLOT(SetReadyRead(QByteArray,int)));
        connect(TOmniRigHV, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        have_read_data_rts_on = 3;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TOmniRigHV, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = TOmniRigHV->InitOmniRigHV();
        if (!f_rig_activ)
        {
            //qDebug()<<"OmniExcept DeleteLater"<<f1;
            TOmniRigHV->deleteLater();
        }
    }
#endif
////// omnirig end /////////
    else if (rstruc.facid == NETWORK_ID)
    {        
        Network *TNetwork = new Network(rstruc.model);

        connect(this, SIGNAL(EmitNetSP(QString)), TNetwork, SLOT(ConnectNet(QString)));
        connect(TNetwork, SIGNAL(EmitNetConnInfo(QString,bool,bool)), this, SIGNAL(EmitNetConnInfo(QString,bool,bool)));

        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TNetwork, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TNetwork, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TNetwork, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TNetwork, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        //connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TNetwork, SLOT(SetReadyRead(QByteArray,int)));
        connect(TNetwork, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        //tci

        connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TNetwork, SLOT(SetOnOffCatCommand(bool,int,int)));
        have_read_data_rts_on = 3; //2.56 need to be here
        s_active_model_id = rstruc.model;
        s_active_fact_id = NETWORK_ID;
        connect(this, SIGNAL(EmitTciSelect(int)), TNetwork, SLOT(SetTciSelect(int)));//2.59 tci
        SetTciSelect(tci_select);//2.59 tci
        //connect(this, SIGNAL(EmitMode(int)), TNetwork, SLOT(SetMode(int)));//tci
        //SetMode(s_mode);//tci
        //end tci
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(true);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TNetwork, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else if (rstruc.facid == SDRS_ID)
    {
        SDRs *TSDRs = new SDRs(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TSDRs, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TSDRs, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TSDRs, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TSDRs, SLOT(deleteLater()));

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TSDRs, SLOT(SetReadyRead(QByteArray,int)));
        connect(TSDRs, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TSDRs, SLOT(SetOnOffCatCommand(bool,int,int)));
        have_read_data_rts_on = 3; //2.56 need to be here
        s_active_model_id = rstruc.model;
        s_active_fact_id = SDRS_ID;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(true);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TSDRs, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;

    }
    else if (rstruc.facid == MITS_ID)
    {
        Mits *TMits = new Mits(rstruc.model);
        connect(this, SIGNAL(SetCmd(CmdID,ptt_t,QString)), TMits, SLOT(SetCmd(CmdID,ptt_t,QString)));
        connect(TMits, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        connect(TMits, SIGNAL(EmitWriteCmd(char*,int)), this, SLOT(SetWriteCmd(char*,int)));
        connect(this, SIGNAL(DesleteRig()), TMits, SLOT(deleteLater()));//deleteLater()

        ////////////////////////////////////////////////////////new read com
        connect(this, SIGNAL(EmitReadyRead(QByteArray,int)), TMits, SLOT(SetReadyRead(QByteArray,int)));
        connect(TMits, SIGNAL(EmitReadedInfo(CmdID,QString)), this, SLOT(SetReadedInfo(CmdID,QString)));
        connect(this, SIGNAL(EmitOnOffCatCommand(bool,int,int)), TMits, SLOT(SetOnOffCatCommand(bool,int,int)));
        connect(TMits, SIGNAL(EmitPttDtr(bool)), this, SIGNAL(EmitPttDtr(bool)));// sea-235
        if (rstruc.name=="SEA-235") have_read_data_rts_on = 1; //2.73 need to be here
        else have_read_data_rts_on = 3;	
        s_active_model_id = rstruc.model;
        s_active_fact_id = MITS_ID;
        ////////////////////////////////////////////////////////end new read com

        //SetPttCaps(false);
        emit SetCmd(GET_SETT,RIG_PTT_OFF,"");
        disconnect(TMits, SIGNAL(EmitRigSet(RigSet)), this, SLOT(SetRigSet(RigSet)));
        f_rig_activ = true;
    }
    else
    {
        SetPttCaps(false);
        //Reset if None
        RigSet sett;
        sett.ptt_type = RIG_PTT_NONE;
        sett.port_type = RIG_PORT_NONE;
        sett.serial_rate_min = 4800;
        sett.serial_rate_max = 4800;
        sett.serial_data_bits = 8;
        sett.serial_stop_bits = 2;
        sett.serial_parity = PAR_NONE;
        sett.serial_flow_type = FLOW_OFF;
        sett.write_delay = 0;
        sett.post_write_delay = 100;
        sett.timeout = 100;
        sett.retry = 0;
        SetRigSet(sett);
    }
}
void HvRigCat::SetRigSet(RigSet sett)
{
    //if(f_rig_activ)
    //if (sett.ptt_type == RIG_PTT_NONE || sett.ptt_type == RIG_PTT_RIG)
    //SetPttCaps(false);
    if (sett.ptt_type == RIG_PTT_RIG_MICDATA)
        SetPttCaps(true);
    else
        SetPttCaps(false);

    s_port_type = sett.port_type;//imporetent to be here


    emit EmitSetRigSet(sett,have_read_data_rts_on);

    //qDebug()<<"RIG_SETT"<<sett.name;
}
void HvRigCat::SetPttCaps(bool f)
{
    //qDebug()<<"RIG_ComboBox"<<f;
    if (f)
    {
        rb_ptt_mic->setEnabled(true);
        rb_ptt_data->setEnabled(true);
    }
    else
    {
        rb_ptt->setChecked(true);
        rb_ptt_mic->setEnabled(false);
        rb_ptt_data->setEnabled(false);
    }
}
void HvRigCat::PttChanget(bool)
{
    if (rb_ptt->isChecked()) s_ptt_on_type = RIG_PTT_ON;
    if (rb_ptt_mic->isChecked()) s_ptt_on_type = RIG_PTT_ON_MIC;
    if (rb_ptt_data->isChecked()) s_ptt_on_type = RIG_PTT_ON_DATA;
}
/*void HvRigCat::InsertItemModel(QStringList list)
{
    if (!list.isEmpty())
    {
        QList<QStandardItem *> qlsi;
        for (QStringList::iterator it =  list.begin(); it != list.end(); ++it)
        {
            QStandardItem *item = new QStandardItem(QString(*it));
            //item->setEditable(false);
            qlsi.append(item);
        }
        //model.appendRow(qlsi);
        model.insertRow(0, qlsi);// za tozi slu4ei e taka HV
        //this->setCurrentIndex(model.index(model.rowCount()-1,0));
    }
    //qDebug()<<list.count();
}*/
void HvRigCat::SetRigName(QString s)//from read settings
{
    int index = RigsBoxs->findText(s, Qt::MatchCaseSensitive);
    if (index >= 0)
    {
        RigsBoxs->setCurrentIndex(index);
        SetRig(index);//tova opravia i SetPttCaps();
    }
}
void HvRigCat::SetPttCatType(QString s)
{
	QStringList ls=s.split("#");
	ls<<"1000"<<"0";
    if (ls.at(0)=="0") rb_ptt->setChecked(true);
    if (ls.at(0)=="1") rb_ptt_mic->setChecked(true);
    if (ls.at(0)=="2") rb_ptt_data->setChecked(true);
    int ip = ls.at(1).toInt();
    if (ip>199 && ip<1001) sb_pollint->setValue(ip);
}
void HvRigCat::SetWriteCmd(char*data,int size)
{
    //QByteArray array(data, size);
    //qDebug()<<"WriteCmd="<<(QString(array.toHex()))<<size;
    polling_timer->stop();//block timer
    emit EmitWriteCmd(data,size);
    //if (s_port_type == RIG_PORT_SERIAL && s_port_poen) // && s_cb_read_data_rts_on
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen)
    {
        polling_timer->start(sb_pollint->value());
    }
}
void HvRigCat::StartPttTimer()
{
    //qDebug()<<"set_ptt_timer"<<QTime::currentTime().toString("ss:zzz")<<s_f_ptt;
    if (s_f_ptt)
        emit SetCmd(SET_PTT,s_ptt_on_type,"");
    else
        emit SetCmd(SET_PTT,RIG_PTT_OFF,"");
    tx_rx_timer->stop();
}
void HvRigCat::set_ptt(bool f, bool imd)//2.38  immediately
{
    //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen)
    {
        polling_timer->stop();
        s_f_ptt = f;
        if (imd) //2.38 immediately
            StartPttTimer();
        else
            tx_rx_timer->start(50);//2.17=50ms 1.66=100ms wait to stop poling timer tx start at 250ms in MsPlayerHV
    }
}
void HvRigCat::set_mode(QString str)
{
    //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen)
    {
        polling_timer->stop();
        usleep(50000);
        //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
        emit SetCmd(SET_MODE,RIG_PTT_OFF,str);//RIG_PTT_OFF fictive
    }
}
void HvRigCat::set_freq(QString sfreq)
{
    //qDebug()<<"set_freq"<<s_port_poen<<s_port_type<<sfreq;
    //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen)
    {
        polling_timer->stop();
        usleep(50000);
        //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
        emit SetCmd(SET_FREQ,RIG_PTT_OFF,sfreq);//RIG_PTT_OFF fictive
    }
}
void HvRigCat::SetReadyRead(QByteArray a,int i)
{
    emit EmitReadyRead(a,i);
}
void HvRigCat::SetReadedInfo(CmdID i,QString str)
{
    if (i==GET_FREQ)
    {
        if (c_rig_cat_active_and_read == 200)
            emit EmitCatAactiveAndRead(true,true);//2.53 true to TxWidget
        c_rig_cat_active_and_read = 0;

        emit EmitGetedFreq(str);
    }
    else if (i==GET_MODE)
        emit EmitGetedMode(str);
    //qDebug()<<"READ INFO="<<i<<str;
}
void HvRigCat::StartStopPollingTimer(bool popen)/*, bool read_data_rts_on*/
{
    s_port_poen = popen;
    //s_cb_read_data_rts_on = read_data_rts_on;
    //qDebug()<<"s_port_poen="<<s_port_poen<<s_port_type;

    //if (s_port_type == RIG_PORT_SERIAL && s_port_poen)
    if ((s_port_type == RIG_PORT_SERIAL || s_port_type == RIG_PORT_NETWORK) && s_port_poen)
    {
        //qDebug()<<"START TIMER StartStopPollingTimer="<<s_port_type<<popen;
        //if(f_rig_activ)
        emit EmitOnOffCatCommand(true,s_active_model_id,s_active_fact_id);
        polling_timer->start(sb_pollint->value());
    }
    else
    {
        //qDebug()<<"STOP TIMER StartStopPollingTimer="<<s_port_type<<popen;
        polling_timer->stop();
        emit EmitOnOffCatCommand(false,s_active_model_id,s_active_fact_id);
    }

    bool fl = false;
    if (!popen) fl = true;  //qDebug()<<"UPDATE="<<fl;
    emit EmitCatAactiveAndRead(false,fl);//2.53 false no to TxWidget
    c_rig_cat_active_and_read = 200;//reset 200
}
void HvRigCat::get_freq()
{
    emit SetCmd(GET_FREQ,RIG_PTT_OFF,"");//RIG_PTT_OFF fictive
}
void HvRigCat::get_mode()
{
    emit SetCmd(GET_MODE,RIG_PTT_OFF,"");//RIG_PTT_OFF fictive
}
//int t = 1;
void HvRigCat::PollingTimerReadRig()
{
    c_poll_comm++;
    if (c_poll_comm==0)
    {
        get_freq();
        if (c_rig_cat_active_and_read != 200)
        {
            //qDebug()<<"TIMER="<<t;  t++;
            //need 76s=1min and 16s  2000x38=76000ms
            //2.53 to 5min 2000x155=310000=5,16min    2000x65=130000=2,16min 2000*95=190000=3,16min
            if (c_rig_cat_active_and_read > 94)//2.55=94 2.53=154   old->37=1,2min
            {
                emit EmitCatAactiveAndRead(false,true);//2.53 true to TxWidget
                c_rig_cat_active_and_read = 200;
            }
            else
                c_rig_cat_active_and_read++;
        }
    }
    else if (c_poll_comm==1)
    {
        get_mode();
        c_poll_comm = -1;//reset polling commands
    }
    //qDebug()<<"TIMER WORKKKKKKKKKKKKKKKKKKK=";
}
////////////////////////////////////////////////////////end new read com

