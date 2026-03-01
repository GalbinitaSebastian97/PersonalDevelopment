
/* Includes */
/*============================================================================*/
#include "Control_Interface.h"// In development
#include "BasicMotorControlService_Types.h"// In development
#include "ExteriorMirror_Types.h" // In development
#include "mirrorConfiguration.h"
#include "InternalTypes.h"
#include "Param.h"  // In development



#if EMBL_FRAMELESS_MIRROR_SUPPORT
/* Constants and types  */
/*============================================================================*/
#define HORIZONTAL_AXIS                 0u
#define VERTICAL_AXIS                   1u
#define FRAMELESS_VAR_SINGLE(var)       mirrorFramelessFold_s.single.var
#define FRAMELESS_VAR_AXIS(num,var)     mirrorFramelessFold_s.axis[num].var

/* Variables */
/*============================================================================*/
static mirrorFramelessFoldData mirrorFramelessFold_s;
static mirrorCtrlNvMData* mirrorCtrlNvMemory_s;

/* Private functions prototypes */
/*============================================================================*/
static void emblFramelessFold_MirrorControl(void);
static void emblFramelessFold_TriggerFoldMovement(void);
static void emblFramelessFold_TriggerUnfoldMovement(void);
static t_emblAbortReason emblFramelessFold_CheckAbortConditions(boolean requestOngoing);
/* Exported functions */
/*============================================================================*/

/*********************************************************************/
/*! \brief This functions is used to init FramelessFoldControl functionality
 *********************************************************************/
void emblFramelessFold_Init(void)
{
    FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e) = MIRRFLD_COMMAND_IDLE;
    FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = ABORT_REASON_NONE;
    FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
    FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_IDLE;
    mirrorCtrlNvMemory_s = emblControl_getNvMRamImageAddress();
}
/*************************************************************/
/*! @brief FramelessFoldControl Main cyclic task.
 *************************************************************/
void emblFramelessFold_MainTask(void)
{
    emblFramelessFold_MirrorControl();
}

/*********************************************************************/
/*! @brief Sets the auto adjustment command state.
 *
 * This function shall be used in emblGlassAdjustment to set the auto
 * command.
 *
 * @return TRUE: Auto adj is enabled; FALSE: Auto Adj is disabled.
 *********************************************************************/
t_emblGlassAutoAdjustCmd emblFramelessFold_getAutoAdjCmd(void)
{
    return FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e);
}

/*********************************************************************/
/*! @brief Sets the Y target position for the frameless fold control.
 *
 * This function shall be used in emblGlassAdjustment to set the target
 * position on the vertical axis.
 * 
 * @return The value of the target position that was set.
 *********************************************************************/
uint16 emblFramelessFold_getYTargetPos(void)
{
    return FRAMELESS_VAR_AXIS(VERTICAL_AXIS,FramelessTargetPos_u16);
}

/*********************************************************************/
/*! @brief Sets the X target position for the frameless fold control.
 *
 * This function shall be used in emblGlassAdjustment to set the target
 * position on the horizontal axis.
 * 
 * @return The value of the target position that was set.
 *********************************************************************/
uint16 emblFramelessFold_getXTargetPos(void)
{
    return FRAMELESS_VAR_AXIS(HORIZONTAL_AXIS,FramelessTargetPos_u16);
}

/*********************************************************************/
/*! @brief Retrieves the current fold command of frameless mirror control.
 *
 * @return The current command of mirror fold.
 *********************************************************************/
t_emblMirrorFoldCmd emblFramelessFold_getFoldCommand(void)
{
    return FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e);
}

/* Private functions */
/*============================================================================*/
/*********************************************************************/
/*! @brief Triggers the movement of the frameless fold mechanism.
 *
 * This function sets the target positions for the fold and vertical axes,
 * and enables the automatic adjustment command for the frameless control system.
 *********************************************************************/
static void emblFramelessFold_TriggerFoldMovement(void)
{
    FRAMELESS_VAR_AXIS(HORIZONTAL_AXIS,FramelessTargetPos_u16) = mirrorCtrlNvMemory_s->LowPositionFold_u16 + EMBL_MM_PARAM(embl_ctrl_FramelessFoldPositionOffSet);
    FRAMELESS_VAR_AXIS(VERTICAL_AXIS,FramelessTargetPos_u16) = (uint16)((mirrorCtrlNvMemory_s->HighPositionY_u16 + mirrorCtrlNvMemory_s->LowPositionY_u16) >> 1u);
    FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_ON;
    FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_FOLD;
}
/*********************************************************************/
/*! @brief Trigger the unfold movement for frameless mirror. Use the positions
 * stored in NVM or default values if NVM read operation was not successful
 * otherwise use param values.
 *********************************************************************/
static void emblFramelessFold_TriggerUnfoldMovement(void)
{
    if((mirrorCtrlNvMemory_s->FoldDrivePosY_u16 != 0u) && (mirrorCtrlNvMemory_s->FoldDrivePosX_u16 != 0u))
    {
        /*Read the target position from NVM*/
        FRAMELESS_VAR_AXIS(VERTICAL_AXIS,FramelessTargetPos_u16) = mirrorCtrlNvMemory_s->FoldDrivePosY_u16;
        FRAMELESS_VAR_AXIS(HORIZONTAL_AXIS,FramelessTargetPos_u16) = mirrorCtrlNvMemory_s->FoldDrivePosX_u16;
    }
    else{
        /*Set the default target position*/
        FRAMELESS_VAR_AXIS(VERTICAL_AXIS,FramelessTargetPos_u16) = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosY);
        FRAMELESS_VAR_AXIS(HORIZONTAL_AXIS,FramelessTargetPos_u16) = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosX);
    }
    FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_UNFOLD;
    FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_ON;
}
/*********************************************************************/
/*! @brief Checks for abort conditions during frameless fold operation.
 *
 * Evaluates all relevant conditions that may require aborting the frameless fold process:
 * - Unavailable position data
 * - Ongoing automatic glass adjustment with higher priority
 * - Block detected during horizontal movement
 * - Glass adjustment abort reason
 * @param requestOngoing For new requests, the abort reason from Glass Adjust
 *                       shall not be checked - wait 1 task to update
 * @retval Returns a specific abort reason
 *********************************************************************/
static t_emblAbortReason emblFramelessFold_CheckAbortConditions(boolean requestOngoing){    

    boolean posAxisXAvailable, posAxisYAvailable;
    t_bmcs_StopReason horizontalStopReason;
    EmblCtrl_GlassAdjXMotor_Read_StopReason(&horizontalStopReason);
#if EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE != 0u
    t_emblMirrorBlockState horizontalblockDetected;
    EmblCtrl_Read_embl_BlockState((uint8)EMBLC_INST_FRAMELESS_HORIZONTAL_MOTOR, &horizontalblockDetected);
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
        /* Position data unavailable: abort */
        return ABORT_REASON_POS_ERROR;
    }
    if (((manualAdjustCmd != GLASS_ADJUST_CMD_NO_REQUEST)   
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
           || (autoAdjustCmd != MIRR_GLASS_ADJ_AUTO_CMD_OFF)
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/
        ) && (EMBL_MM_PARAM(embl_ctrl_FramelessFoldGlassRequestPriority) == 0u) 
          && (FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) == MIRR_GLASS_ADJ_AUTO_CMD_OFF) 
          && (FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e) != MIRRFLD_COMMAND_IDLE))
    {
        /*If an automatic/manual movement is active with higher priority and a frameless fold command is triggered -> reject it*/
         return ABORT_REASON_LOW_PRIO;
    }
#if EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE != 0u
    if (horizontalStopReason == BMCS_STOP_REASON_BLOCK)
    {
        /* Block detected on horizontal movement: abort */
        return ABORT_REASON_POS_UNREACHABLE;
    }
#endif //EMBL_FRAMELESS_MIRROR_BLOCK_DET_TYPE

    if(requestOngoing == (boolean)TRUE)
    {
        /* Return the abort reason from Glass Adjustment if request was already started */
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
static void emblFramelessFold_MirrorControl(void){
    static t_emblMirrorFoldCmd prevFramelessFoldCmd = MIRRFLD_COMMAND_IDLE;
    t_emblAbortReason foldAbortReason_e;
    t_emblGlassManualAdjustCmd manualAdjustCmd;
    EmblCtrl_Read_embl_GlassManualAdjustCmd(&manualAdjustCmd);
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
    t_emblGlassAutoAdjustCmd autoAdjustCmd;
    EmblCtrl_Read_embl_GlassAutoAdjustCmd(&autoAdjustCmd);
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/
    mblCtrl_Read_embl_MirrorFoldCmd(&FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e));    
    /* Check for abort conditions; at new request don't check glass preconditions, wait one task for request to be processed */
    const boolean requestOngoing_u8 = (prevFramelessFoldCmd != FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e)) ? FALSE : TRUE;
    foldAbortReason_e = emblFramelessFold_CheckAbortConditions(requestOngoing_u8);

    if(foldAbortReason_e == ABORT_REASON_NONE){ 
        /* Handle fold request */
        if ((FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e) == MIRRFLD_COMMAND_FOLD) && (prevFramelessFoldCmd != FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e)))
        {
            const boolean nvmBusy_u1 = emblControl_IsNvMBusy();
            if((nvmBusy_u1 == FALSE) && (mirrorCtrlNvMemory_s->FoldPosSaveAllowed_u1 == (boolean)TRUE))
            {
                /* Save current position before folding, reset abort reason */
                mirrorCtrlNvMemory_s->FoldDrivePosX_u16 = emblGlassAdjust_getXPosition();
                mirrorCtrlNvMemory_s->FoldDrivePosY_u16 = emblGlassAdjust_getYPosition();
                mirrorCtrlNvMemory_s->FoldPosSaveAllowed_u1 = FALSE;
                (void)emblControl_NvMWriteBlock();
            }
            emblFramelessFold_TriggerFoldMovement();
            FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = ABORT_REASON_NONE;
        }
        /* Handle unfold request */
        else if ((FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e) == MIRRFLD_COMMAND_UNFOLD) && (prevFramelessFoldCmd != FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e)))
        {
            emblFramelessFold_TriggerUnfoldMovement();
            FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = ABORT_REASON_NONE;
        }
        else
        {
            /*Ongoing fold/unfold movement active*/
            if(FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) == MIRR_GLASS_ADJ_AUTO_CMD_ON)
            {
                const t_emblMirrorGlassAutoAdjustStatus glassStatus_e = emblGlassAdjust_getAutoAdjStatus();
                /*Check if auto adjustment was finished or aborted to set the abort reason*/
                if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_FINISHED)
                {
                    FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = ABORT_REASON_POS_REACHED;
                    FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_IDLE;
                }
                else if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_ABORTED)
                {
                    FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = ABORT_REASON_POS_UNREACHABLE;
                    FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_IDLE;
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
        FRAMELESS_VAR_SINGLE(FramelessAutoAdjCmd_e) = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
        FRAMELESS_VAR_SINGLE(FramelessFoldActiveCmd_e) = MIRRFLD_COMMAND_IDLE;
        FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e) = foldAbortReason_e;
    }
    /*If position in NvM is not allowed but user moved the mirror(manually or automatically) -> new position can be stored in NvM*/
    if ((mirrorCtrlNvMemory_s->FoldPosSaveAllowed_u1 == (boolean)FALSE) && ((manualAdjustCmd != GLASS_ADJUST_CMD_NO_REQUEST) 
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
     || (autoAdjustCmd != MIRR_GLASS_ADJ_AUTO_CMD_OFF)
#endif /*EMBL_GLASS_AUTO_ADJUST_AVAILABLE*/
        ))
    {
        const boolean nvmStatus_u1 = emblControl_IsNvMBusy();
        if (nvmStatus_u1 == (boolean)FALSE)
        {
            mirrorCtrlNvMemory_s->FoldPosSaveAllowed_u1 = TRUE;
            (void)emblControl_NvMWriteBlock();
        }
    }
    EmblCtrl_MirrorFoldCmd_Write_embl_AbortReason(FRAMELESS_VAR_SINGLE(FramelessFoldAbortReason_e));
    prevFramelessFoldCmd = FRAMELESS_VAR_SINGLE(FramelessFoldCmd_e);
}
#endif //FRAMELESS_MIRROR_SUPPORT
