#ifndef MOCKS_HPP
#define MOCKS_HPP

extern "C" void Read_GlassManualAdjustCmd(t_emblGlassManualAdjustCmd* cmd) {
    if(cmd) *cmd = GLASS_ADJUST_CMD_NO_REQUEST;
}

extern "C" void Read_GlassAutoAdjustCmd(t_emblGlassAutoAdjustCmd* cmd) {
    if(cmd) *cmd = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
}

extern "C" Std_ReturnType ReadBlock(NvM_DstPtrType_cil data) {
    (void)data;
    return true;
}

extern "C" void GlassAdjYMotor_Read_StopReason(t_bmcs_StopReason* reason) {
    if(reason) *reason = BMCS_STOP_REASON_NONE;
}

extern "C" void Read_MirrorGlassAdjPosValidStat(t_emblMirrorGlassAdjPosValidStat* stat) {
    if(stat) *stat = MIRR_GLASS_POS_VALID;
}

extern "C" void GlassAdjXMotor_Read_StopReason(t_bmcs_StopReason* reason) {
    if(reason) *reason = BMCS_STOP_REASON_NONE;
}

extern "C" void MirrorFoldCmd_Write_AbortReason(t_emblAbortReason data) {
    (void)data;
}

extern "C" Std_ReturnType WriteBlock(NvM_SrcPtrType_cil data) {
    (void)data;
    return true;
}

extern "C" void Read_MirrorFoldCmd(t_emblMirrorFoldCmd* cmd) {
    if(cmd) *cmd = MIRRFLD_COMMAND_IDLE;
}

// Add more mock functions as needed for testing
#endif // MOCKS_HPP