#ifndef MOCKS_HPP
#define MOCKS_HPP

#include "Control_Interface.hpp"

extern "C" void EmblCtrl_Read_embl_GlassManualAdjustCmd(t_emblGlassManualAdjustCmd* cmd) {
    if(cmd) *cmd = GLASS_ADJUST_CMD_NO_REQUEST;
}

extern "C" void EmblCtrl_Read_embl_GlassAutoAdjustCmd(t_emblGlassAutoAdjustCmd* cmd) {
    if(cmd) *cmd = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
}

extern "C" void EmblCtrl_Read_embl_MirrorGlassAdjPosValidStat(t_emblMirrorGlassAdjPosValidStat* stat) {
    if(stat) *stat = MIRR_GLASS_POS_VALID;
}

extern "C" void EmblCtrl_GlassAdjXMotor_Read_StopReason(t_bmcs_StopReason* reason) {
    if(reason) *reason = BMCS_STOP_REASON_NONE;
}

extern "C" void EmblCtrl_GlassAdjYMotor_Read_StopReason(t_bmcs_StopReason* reason) {
    if(reason) *reason = BMCS_STOP_REASON_NONE;
}

extern "C" Std_ReturnType EmblCtrl_WriteBlock(NvM_SrcPtrType_cil data) {
    (void)data;
    return true;
}

extern "C" Std_ReturnType EmblCtrl_ReadBlock(NvM_DstPtrType_cil data) {
    (void)data;
    return true;
}

extern "C" void EmblCtrl_Read_embl_MirrorFoldCmd(t_emblMirrorFoldCmd* cmd) {
    if(cmd) *cmd = MIRRFLD_COMMAND_IDLE;
}

extern "C" void EmblCtrl_MirrorFoldCmd_Write_embl_AbortReason(t_emblAbortReason data) {
    (void)data;
}
#endif // MOCKS_HPP