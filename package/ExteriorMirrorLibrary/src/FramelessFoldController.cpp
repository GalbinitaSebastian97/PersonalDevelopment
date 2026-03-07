
/* Includes */
/*============================================================================*/
#include "Control_Interface.hpp"// In development
#include "mirrorConfiguration.hpp"
#include "InternalTypes.hpp"
#include "Param.hpp"  // In development
#include "FramelessFoldController.hpp"

using namespace emblex;

#if EMBL_FRAMELESS_MIRROR_SUPPORT
// The C-style macros, global state, and free-standing helpers have been
// replaced by the `FramelessFoldController` class.  All state lives in
// `_data` and `_nvMem` members; member functions implement the old logic.

// No module-level variables or prototypes are required any more.

/* Exported functions */
/*============================================================================*/

/*********************************************************************/
/*! \brief This functions is used to init FramelessFoldControl functionality
 *********************************************************************/
void FramelessFoldController::Init()
{
    single().FramelessFoldCmd_e     = MIRRFLD_COMMAND_IDLE;
    single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
    single().FramelessAutoAdjCmd_e  = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;

    _nvMem = emblControl_getNvMRamImageAddress();
}
/*************************************************************/
/*! @brief FramelessFoldControl Main cyclic task.
 *************************************************************/
void FramelessFoldController::MainTask()
{
    /*Read mirror frameless command*/
    EmblCtrl_Read_embl_MirrorFoldCmd(&single().FramelessFoldCmd_e);

    mirrorControl();

    /*Write abort reason*/
    EmblCtrl_MirrorFoldCmd_Write_embl_AbortReason(single().FramelessFoldAbortReason_e);
}

/*********************************************************************/
/*! @brief Sets the auto adjustment command state.
 *
 * This function shall be used in emblGlassAdjustment to set the auto
 * command.
 *
 * @return TRUE: Auto adj is enabled; FALSE: Auto Adj is disabled.
 *********************************************************************/
t_emblGlassAutoAdjustCmd FramelessFoldController::getAutoAdjCmd() const
{
    return single().FramelessAutoAdjCmd_e;
}

/*********************************************************************/
/*! @brief Sets the Y target position for the frameless fold control.
 *
 * This function shall be used in emblGlassAdjustment to set the target
 * position on the vertical axis.
 * 
 * @return The value of the target position that was set.
 *********************************************************************/
uint16 FramelessFoldController::getYTargetPos() const
{
    return axis(VERTICAL_AXIS).FramelessTargetPos_u16;
}

/*********************************************************************/
/*! @brief Sets the X target position for the frameless fold control.
 *
 * This function shall be used in emblGlassAdjustment to set the target
 * position on the horizontal axis.
 * 
 * @return The value of the target position that was set.
 *********************************************************************/
uint16 FramelessFoldController::getXTargetPos() const
{
    return axis(HORIZONTAL_AXIS).FramelessTargetPos_u16;
}

/*********************************************************************/
/*! @brief Retrieves the current fold command of frameless mirror control.
 *
 * @return The current command of mirror fold.
 *********************************************************************/
t_emblMirrorFoldCmd FramelessFoldController::getFoldCommand() const
{
    return single().FramelessFoldActiveCmd_e;
}

/* Private functions */
/*============================================================================*/
/*********************************************************************/
/*! @brief Triggers the movement of the frameless fold mechanism.
 *
 * This function sets the target positions for the fold and vertical axes,
 * and enables the automatic adjustment command for the frameless control system.
 *********************************************************************/
void FramelessFoldController::triggerFoldMovement()
{
    axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 = _nvMem->LowPositionFold_u16 + EMBL_MM_PARAM(embl_ctrl_FramelessFoldPositionOffSet);
    axis(VERTICAL_AXIS).FramelessTargetPos_u16 =(uint16)((_nvMem->HighPositionY_u16 + _nvMem->LowPositionY_u16) >> 1u);
    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_ON;
    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_FOLD;
}
/*********************************************************************/
/*! @brief Trigger the unfold movement for frameless mirror. Use the positions
 * stored in NVM or default values if NVM read operation was not successful
 * otherwise use param values.
 *********************************************************************/
void FramelessFoldController::triggerUnfoldMovement()
{
    if((_nvMem->FoldDrivePosY_u16 != 0u) && (_nvMem->FoldDrivePosX_u16 != 0u))
    {
        /* read the target position from NvM */
        axis(VERTICAL_AXIS).FramelessTargetPos_u16 = _nvMem->FoldDrivePosY_u16;
        axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 = _nvMem->FoldDrivePosX_u16;
    }
    else
    {
        /* set the default target position */
        axis(VERTICAL_AXIS).FramelessTargetPos_u16 =EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosY);
        axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 =EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosX);
    }
    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_UNFOLD;
    single().FramelessAutoAdjCmd_e   = MIRR_GLASS_ADJ_AUTO_CMD_ON;
}
/*********************************************************************/
/*! @brief Checks for abort conditions during frameless fold operation.
 *
 * Evaluates all relevant conditions that may require aborting the frameless
 * fold process.  See the earlier C implementation for full detail.
 * @param requestOngoing For new requests the glass-adjust abort reason is
 *        not considered; the flag indicates whether the current command is
 *        continuing.
 * @retval Returns a specific abort reason.
 *********************************************************************/
t_emblAbortReason FramelessFoldController::checkAbortConditions(bool requestOngoing)
{
    boolean posAxisXAvailable, posAxisYAvailable;
    t_bmcs_StopReason horizontalStopReason;
    EmblCtrl_GlassAdjXMotor_Read_StopReason(&horizontalStopReason);

#if EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE != 0u
    t_emblMirrorBlockState horizontalblockDetected;
    EmblCtrl_Read_embl_BlockState((uint8)EMBLC_INST_FRAMELESS_HORIZONTAL_MOTOR,&horizontalblockDetected);
#endif

    t_emblGlassManualAdjustCmd manualAdjustCmd;
    EmblCtrl_Read_embl_GlassManualAdjustCmd(&manualAdjustCmd);

#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
    t_emblGlassAutoAdjustCmd autoAdjustCmd;
    EmblCtrl_Read_embl_GlassAutoAdjustCmd(&autoAdjustCmd);
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/

    EmblCtrl_GlassAdjXMotor_Read_PositioningStatus(&posAxisXAvailable);
    EmblCtrl_GlassAdjYMotor_Read_PositioningStatus(&posAxisYAvailable);

#if EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE != 0u
    EmblCtrl_Read_embl_BlockState((uint8)GLASS_ADJ_X_MOTOR, &horizontalblockDetected);
#endif

    if ((posAxisXAvailable == (boolean)TRUE) || (posAxisYAvailable == (boolean)TRUE))
    {
        return ABORT_REASON_POS_ERROR;
    }

    if (((manualAdjustCmd != GLASS_ADJUST_CMD_NO_REQUEST)
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
         || (autoAdjustCmd != MIRR_GLASS_ADJ_AUTO_CMD_OFF)
#endif
        ) && (EMBL_MM_PARAM(embl_ctrl_FramelessFoldGlassRequestPriority) == 0u) &&
        (single().FramelessAutoAdjCmd_e == MIRR_GLASS_ADJ_AUTO_CMD_OFF) &&
        (single().FramelessFoldCmd_e != MIRRFLD_COMMAND_IDLE))
    {
        return ABORT_REASON_LOW_PRIO;
    }

#if EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE != 0u
    if (horizontalStopReason == BMCS_STOP_REASON_BLOCK)
    {
        return ABORT_REASON_POS_UNREACHABLE;
    }
#endif //EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE

    if (requestOngoing == true)
    {
        return emblGlassAdjust_getAutoAdjustAbortReason();
    }

    return ABORT_REASON_NONE;
}
/*********************************************************************/
/*! @brief Handles the state machine for frameless mirror fold and unfold operations. 
* Manages fold/unfold requests, saves positions, handles blocks, 
* and updates abort reasons. Also tracks manual/auto adjustments and 
* ensures safe operation using NvM data. 
*********************************************************************/
void FramelessFoldController::mirrorControl()
{
    static t_emblMirrorFoldCmd prevFramelessFoldCmd = MIRRFLD_COMMAND_IDLE;
    t_emblAbortReason foldAbortReason_e;
    t_emblGlassManualAdjustCmd manualAdjustCmd;
    EmblCtrl_Read_embl_GlassManualAdjustCmd(&manualAdjustCmd);
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
    t_emblGlassAutoAdjustCmd autoAdjustCmd;
    EmblCtrl_Read_embl_GlassAutoAdjustCmd(&autoAdjustCmd);
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/
    
    /* Check for abort conditions; at new request don't check glass preconditions, wait one task for request to be processed */
    const boolean requestOngoing_u8 = (prevFramelessFoldCmd != single().FramelessFoldCmd_e) ? FALSE : TRUE;
    foldAbortReason_e = checkAbortConditions(requestOngoing_u8);

    if(foldAbortReason_e == ABORT_REASON_NONE){ 
        /* Handle fold request */
        if ((single().FramelessFoldCmd_e == MIRRFLD_COMMAND_FOLD) && (prevFramelessFoldCmd != single().FramelessFoldCmd_e))
        {
            const boolean nvmBusy_u1 = emblControl_IsNvMBusy();
            if((nvmBusy_u1 == FALSE) && (_nvMem->FoldPosSaveAllowed_u1 == (boolean)TRUE))
            {
                /* Save current position before folding, reset abort reason */
                _nvMem->FoldDrivePosX_u16 = emblGlassAdjust_getXPosition();
                _nvMem->FoldDrivePosY_u16 = emblGlassAdjust_getYPosition();
                _nvMem->FoldPosSaveAllowed_u1 = FALSE;
                (void)emblControl_NvMWriteBlock();
            }
            triggerFoldMovement();
            single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
        }
        /* Handle unfold request */
        else if ((single().FramelessFoldCmd_e == MIRRFLD_COMMAND_UNFOLD) && (prevFramelessFoldCmd != single().FramelessFoldCmd_e))
        {
            triggerUnfoldMovement();
            single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
        }
        else
        {
            /*Ongoing fold/unfold movement active*/
            if(single().FramelessAutoAdjCmd_e == MIRR_GLASS_ADJ_AUTO_CMD_ON)
            {
                const t_emblMirrorGlassAutoAdjustStatus glassStatus_e = emblGlassAdjust_getAutoAdjStatus();
                /*Check if auto adjustment was finished or aborted to set the abort reason*/
                if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_FINISHED)
                {
                    single().FramelessFoldAbortReason_e = ABORT_REASON_POS_REACHED;
                    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
                }
                else if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_ABORTED)
                {
                    single().FramelessFoldAbortReason_e = ABORT_REASON_POS_UNREACHABLE;
                    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
                }
                else
                {
                    /*Auto adjustment ongoing - do nothing*/
                }
            }
        }
    }
    else
    {
        /*Abort reason active -> reset command*/
        single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
        single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
        single().FramelessFoldAbortReason_e = foldAbortReason_e;
    }
    /*If position in NvM is not allowed but user moved the mirror(manually or automatically) -> new position can be stored in NvM*/
    if ((_nvMem->FoldPosSaveAllowed_u1 == (boolean)FALSE) && ((manualAdjustCmd != GLASS_ADJUST_CMD_NO_REQUEST) 
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
     || (autoAdjustCmd != MIRR_GLASS_ADJ_AUTO_CMD_OFF)
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/
        ))
    {
        const boolean nvmStatus_u1 = emblControl_IsNvMBusy();
        if (nvmStatus_u1 == (boolean)FALSE)
        {
            _nvMem->FoldPosSaveAllowed_u1 = TRUE;
            (void)emblControl_NvMWriteBlock();
        }
    }

    prevFramelessFoldCmd = single().FramelessFoldCmd_e;
}
#endif //FRAMELESS_MIRROR_SUPPORT
