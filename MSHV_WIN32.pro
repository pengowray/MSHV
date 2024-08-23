QT += widgets axcontainer network websockets
QMAKE_CXXFLAGS += -std=gnu++11 -pedantic-errors
CONFIG += warn_on release exceptions_off
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
DEFINES = _WIN32_
HEADERS = src/main_ms.h \
 src/config.h \
 src/config_msg_all.h \
 src/config_str_all.h \
 src/config_str_exc.h \
 src/config_str_con.h \
 src/config_band_all.h \
 src/config_str_sk.h \
 src/config_str_color.h \
 src/nhash.h \
 src/DisplayMs/display_ms.h \
 src/DisplayMs/HvCustomPalW/custompalw.h \
 src/HvStylePlastique/hvstyleplastique.h \
 src/HvStylePlastique/hvfontdialog.h \
 src/HvMsCore/mscore.h \
 src/SettingsMs/settings_ms.h \
 src/HvSMeter/hvsmeter_h.h \
 src/HvMsPlayer/msplayerhv.h \
 src/HvMsPlayer/libsound/config.h \
 src/HvMsPlayer/libsound/mpegsound.h \
 src/HvMsPlayer/libsound/mpegsound_locals.h \
 src/HvMsPlayer/libsound/genpom.h \
 src/HvMsPlayer/libsound/HvPackUnpackMsg/pack_unpack_msg.h \
 src/HvMsPlayer/libsound/HvPackUnpackMsg/pack_unpack_msg77.h \
 src/HvMsPlayer/libsound/HvGenMsk/config_rpt_msk40.h \
 src/HvMsPlayer/libsound/HvGenMsk/bpdecode_msk.h \
 src/HvMsPlayer/libsound/HvGenMsk/genmesage_msk.h \
 src/HvMsPlayer/libsound/HvGen65/gen65.h \
 src/HvMsPlayer/libsound/HvGenFt8/bpdecode_ft8_174_91.h \
 src/HvMsPlayer/libsound/HvGenFt8/gen_ft8.h \
 src/HvMsPlayer/libsound/HvGenFt4/gen_ft4.h \
 src/HvMsPlayer/libsound/HvGenQ65/gen_q65.h \
 src/HvMsPlayer/libsound/HvGenQ65/q65_subs.h \
 src/HvMsPlayer/libsound/HvRawFilter/hvrawfilter.h \
 src/HvTxW/HvAstroDataW/hvastrodataw.h \
 src/HvTxW/HvLogW/config_prop_all.h \
 src/HvTxW/HvLogW/hvlogw.h \
 src/HvTxW/HvMakros/hvmakros.h \
 src/HvTxW/HvRadioNetW/MessageClient.h \
 src/HvTxW/HvRadioNetW/NetworkMessage.h \
 src/HvTxW/HvRadioNetW/pimpl_h.h \
 src/HvTxW/HvRadioNetW/pimpl_impl.h \
 src/HvTxW/HvRadioNetW/pskreporterudptcp.h \
 src/HvTxW/HvRadioNetW/radionetw.h \
 src/HvTxW/HvRadioNetW/bcnlistw.h \
 src/HvTxW/config_rpt_all.h \
 src/HvTxW/hvtxw.h \
 src/HvTxW/hvqthloc.h \
 src/HvTxW/hvmsdb.h \
 src/HvTxW/hvinle.h \
 src/HvTxW/hvspinbox.h \
 src/HvTxW/hvcustomw.h \
 src/HvTxW/hvstditmmod.h \
 src/HvTxW/hvmultianswermodw.h \
 src/HvTextColor/hvtxtcolor.h \
 src/HvDecodeList/hvtooltip.h \
 src/HvDecodeList/hvcty.h \
 src/HvDecodeList/hvfilterdialog.h \
 src/HvDecodeList/decodelist.h \
 src/HvDecoderMs/decoderms.h \
 src/HvDecoderMs/decoderpom.h \
 src/HvDecoderMs/decoderq65.h \
 src/HvRigControl/hvrigcontrol.h \
 src/HvRigControl/qexsp_1_2rc/qextserialport.h \
 src/HvRigControl/qexsp_1_2rc/qextserialenumerator.h \
 src/HvRigControl/qexsp_1_2rc/qextserialport_global.h \
 src/HvRigControl/qexsp_1_2rc/qextserialport_p.h \
 src/HvRigControl/qexsp_1_2rc/qextserialenumerator_p.h \
 src/HvRigControl/HvRigCat/rigdef.h \
 src/HvRigControl/HvRigCat/hvutils.h \
 src/HvRigControl/HvRigCat/hvrigcatw.h \
 src/HvRigControl/HvRigCat/icom/icom.h \
 src/HvRigControl/HvRigCat/kenwood/kenwood.h \
 src/HvRigControl/HvRigCat/yaesu/yaesu.h \
 src/HvRigControl/HvRigCat/elecraft/elecraft.h \
 src/HvRigControl/HvRigCat/tentec/tentec.h \
 src/HvRigControl/HvRigCat/alinco/alinco.h \
 src/HvRigControl/HvRigCat/jrc/jrc.h \
 src/HvRigControl/HvRigCat/drake/drake.h \
 src/HvRigControl/HvRigCat/icmarine/icmarine.h \
 src/HvRigControl/HvRigCat/pcr/pcr.h \
 src/HvRigControl/HvRigCat/racal/racal.h \
 src/HvRigControl/HvRigCat/omnirig/omnirig.h \
 src/HvRigControl/HvRigCat/omnirig/omnirighv.h \
 src/HvRigControl/HvRigCat/sdrs/sdrs.h \
 src/HvRigControl/HvRigCat/network/network.h \
 src/HvRigControl/HvRigCat/mits/mits.h \
 src/LabWidget/labw.h \
 src/HvSlider_V_Identif/hvslider_v_identif.h \
 src/HvAllTxt/hvalltxt.h \
 src/HvHelpMs/hvhelpms.h \
 src/HvHelpSkMs/hvhelpskms.h \
 src/HvAboutMsHv/hvaboutmshv.h \
 src/CpuWidget/cpusensorhv.h \
 src/CpuWidget/cpuwudget.h \
 src/CpuWidget/HvProgBarSlowH/hvprogbarslowh.h \
 src/HvButtons/hvbutton_left2.h \
 src/HvButtons/hvbutton_left4.h \
 src/HvButtons/hvmodbtsw.h \
 src/HvMsProc/hvmsproc.h \
 src/HvSlider_H/hvslider_h.h \
 src/HvAggressiveW/aggressiv_d.h
SOURCES = src/main.cpp \
 src/main_ms.cpp \
 src/nhash.cpp \
 src/DisplayMs/display_ms.cpp \
 src/DisplayMs/HvCustomPalW/custompalw.cpp \
 src/HvStylePlastique/hvstyleplastique.cpp \
 src/HvStylePlastique/hvfontdialog.cpp \
 src/HvMsCore/mscore.cpp \
 src/HvMsCore/winsound_in.cpp \
 src/SettingsMs/settings_ms.cpp \
 src/HvSMeter/hvsmeter_h.cpp \
 src/HvMsPlayer/msplayerhv.cpp \
 src/HvMsPlayer/libsound/fileinput.cpp \
 src/HvMsPlayer/libsound/rawplayer.cpp \
 src/HvMsPlayer/libsound/soundinputstream.cpp \
 src/HvMsPlayer/libsound/soundplayer.cpp \
 src/HvMsPlayer/libsound/wavetoraw.cpp \
 src/HvMsPlayer/libsound/winsound_out.cpp \
 src/HvMsPlayer/libsound/rawtodata.cpp \
 src/HvMsPlayer/libsound/soundplayer_wr.cpp \
 src/HvMsPlayer/libsound/rawtofile_wr.cpp \
 src/HvMsPlayer/libsound/genmesage.cpp \
 src/HvMsPlayer/libsound/genmesage_ms.cpp \
 src/HvMsPlayer/libsound/genpom.cpp \
 src/HvMsPlayer/libsound/HvPackUnpackMsg/pack_msg.cpp \
 src/HvMsPlayer/libsound/HvPackUnpackMsg/unpack_msg.cpp \
 src/HvMsPlayer/libsound/HvPackUnpackMsg/pack_unpack_msg77.cpp \
 src/HvMsPlayer/libsound/HvGenMsk/genmesage_msk.cpp \
 src/HvMsPlayer/libsound/HvGen65/gen65.cpp \
 src/HvMsPlayer/libsound/HvGenFt8/gen_ft8.cpp \
 src/HvMsPlayer/libsound/HvGenFt4/gen_ft4.cpp \
 src/HvMsPlayer/libsound/HvGenQ65/gen_q65.cpp \
 src/HvMsPlayer/libsound/HvGenQ65/q65_subs.cpp \
 src/HvMsPlayer/libsound/HvRawFilter/hvrawfilter.cpp \
 src/HvTxW/HvAstroDataW/hvastrodataw.cpp \
 src/HvTxW/HvLogW/hvlogw.cpp \
 src/HvTxW/HvMakros/hvmakros.cpp \
 src/HvTxW/HvRadioNetW/MessageClient.cpp \
 src/HvTxW/HvRadioNetW/NetworkMessage.cpp \
 src/HvTxW/HvRadioNetW/pskreporterudptcp.cpp \
 src/HvTxW/HvRadioNetW/radionetw.cpp \
 src/HvTxW/HvRadioNetW/bcnlistw.cpp \
 src/HvTxW/hvtxw.cpp \
 src/HvTxW/hvqthloc.cpp \
 src/HvTxW/hvmsdb.cpp \
 src/HvTxW/hvinle.cpp \
 src/HvTxW/hvspinbox.cpp \
 src/HvTxW/hvcustomw.cpp \
 src/HvTxW/hvstditmmod.cpp \
 src/HvTxW/hvmultianswermodw.cpp \
 src/HvTextColor/hvtxtcolor.cpp \
 src/HvDecodeList/hvtooltip.cpp \
 src/HvDecodeList/hvcty.cpp \
 src/HvDecodeList/hvfilterdialog.cpp \
 src/HvDecodeList/decodelist.cpp \
 src/HvDecoderMs/decoderms.cpp \
 src/HvDecoderMs/decoderpom.cpp \
 src/HvDecoderMs/decodermsk144.cpp \
 src/HvDecoderMs/decodermsk40.cpp \
 src/HvDecoderMs/decoderjtms.cpp \
 src/HvDecoderMs/decoderfsk441.cpp \
 src/HvDecoderMs/decoder6m.cpp \
 src/HvDecoderMs/decoderiscat.cpp \
 src/HvDecoderMs/decoderjt65.cpp \
 src/HvDecoderMs/decoderpi4.cpp \
 src/HvDecoderMs/decoderft8.cpp \
 src/HvDecoderMs/decoderft4.cpp \
 src/HvDecoderMs/decoderq65.cpp \
 src/HvRigControl/hvrigcontrol.cpp \
 src/HvRigControl/qexsp_1_2rc/qextserialport.cpp \
 src/HvRigControl/qexsp_1_2rc/qextserialenumerator.cpp \
 src/HvRigControl/qexsp_1_2rc/qextserialport_win.cpp \
 src/HvRigControl/qexsp_1_2rc/qextserialenumerator_win.cpp \
 src/HvRigControl/HvRigCat/hvutils.cpp \
 src/HvRigControl/HvRigCat/hvrigcatw.cpp \
 src/HvRigControl/HvRigCat/icom/icom.cpp \
 src/HvRigControl/HvRigCat/kenwood/kenwood.cpp \
 src/HvRigControl/HvRigCat/yaesu/yaesu.cpp \
 src/HvRigControl/HvRigCat/elecraft/elecraft.cpp \
 src/HvRigControl/HvRigCat/tentec/tentec.cpp \
 src/HvRigControl/HvRigCat/alinco/alinco.cpp \
 src/HvRigControl/HvRigCat/jrc/jrc.cpp \
 src/HvRigControl/HvRigCat/drake/drake.cpp \
 src/HvRigControl/HvRigCat/icmarine/icmarine.cpp \
 src/HvRigControl/HvRigCat/pcr/pcr.cpp \
 src/HvRigControl/HvRigCat/racal/racal.cpp \
 src/HvRigControl/HvRigCat/omnirig/omnirig.cpp \
 src/HvRigControl/HvRigCat/omnirig/omnirighv.cpp \
 src/HvRigControl/HvRigCat/sdrs/sdrs.cpp \
 src/HvRigControl/HvRigCat/network/network.cpp \
 src/HvRigControl/HvRigCat/mits/mits.cpp \
 src/LabWidget/labw.cpp \
 src/HvSlider_V_Identif/hvslider_v_identif.cpp \
 src/HvAllTxt/hvalltxt.cpp \
 src/HvHelpMs/hvhelpms.cpp \
 src/HvHelpSkMs/hvhelpskms.cpp \
 src/HvAboutMsHv/hvaboutmshv.cpp \
 src/CpuWidget/cpusensorhv.cpp \
 src/CpuWidget/cpuwudget.cpp \
 src/CpuWidget/HvProgBarSlowH/hvprogbarslowh.cpp \
 src/HvButtons/hvbutton_left2.cpp \
 src/HvButtons/hvbutton_left4.cpp \
 src/HvButtons/hvmodbtsw.cpp \
 src/HvMsProc/hvmsproc.cpp \
 src/HvSlider_H/hvslider_h.cpp \
 src/HvAggressiveW/aggressiv_d.cpp
RESOURCES += src/main_ms.qrc \
 src/HvSMeter/hvsmeter_h.qrc \
 src/DisplayMs/display_ms.qrc \
 src/HvSlider_V_Identif/hvslider_v_identif.qrc \
 src/HvTxW/HvLogW/hvlogw.qrc \
 src/CpuWidget/HvProgBarSlowH/hvprogbarslowh.qrc \
 src/HvButtons/hvbuttons.qrc \
 src/HvSlider_H/hvslider_h.qrc
LIBS = src/Hv_Lib_DirectX90c/win32/libdsound.a src/Hv_lib_fftw/win/libfftw3v3310_gcc730_32bit.a -lsetupapi
RC_FILE = src/ms_ico_win.rc
TEMPLATE = app
TRANSLATIONS += src/HvTranslations/mshv_bg.ts \
 src/HvTranslations/mshv_ru.ts \
 src/HvTranslations/mshv_zh.ts \
 src/HvTranslations/mshv_zhhk.ts \
 src/HvTranslations/mshv_eses.ts \
 src/HvTranslations/mshv_caes.ts \
 src/HvTranslations/mshv_ptpt.ts \
 src/HvTranslations/mshv_ptbr.ts \
 src/HvTranslations/mshv_roro.ts \
 src/HvTranslations/mshv_dadk.ts \
 src/HvTranslations/mshv_plpl.ts \
 src/HvTranslations/mshv_frfr.ts \
 src/HvTranslations/mshv_nbno.ts \
 src/HvTranslations/mshv_itit.ts \
 src/HvTranslations/mshv_cscz.ts
