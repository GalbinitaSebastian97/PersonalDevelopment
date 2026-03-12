/* Includes */
/*============================================================================*/
#include "mirrorConfiguration.hpp"
#include "InternalTypes.hpp"
#include "Control_Interface.hpp"
#include "mirrorParam.hpp"
#include "FramelessFoldControl.hpp"
#include "PersistencyManager.hpp"
#include <cstring>

using namespace emblex;

#if EMBL_FRAMELESS_MIRROR_SUPPORT

/* Macros */
#define MIRROR_X_FOLD_POSITION 100u /* TBD: set the actual fold position on X axis */
#define MIRROR_Y_FOLD_POSITION 100u /* TBD: set the actual fold position on Y axis */

/* Exported functions */
/*============================================================================*/

FramelessFoldController::FramelessFoldController()
    : _data{}
    , _persistentData(std::nullopt)
    , _persistencyMgr(emblex::persistency::PersistencyManager::getInstance())
{
}
/**********************************************************************/
/** \brief Gets the singleton instance of FramelessFoldController.
  * @return Reference to the static instance.
  * @note Patterns that ensures only one instance of FramelessFoldController 
  * exists and provides global access to it. The instance is created on first 
  * use and destroyed when the program ends.
  * @warning Copy and assignment are deleted.
***********************************************************************/
FramelessFoldController& FramelessFoldController::getInstance()
{
    static FramelessFoldController instance;
    return instance;
}
/*********************************************************************/
/*! \brief Initialize FramelessFoldControl functionality
 * Initializes the sframeless fold data persistent data from NvM, and prepares for operation.
 *********************************************************************/
void FramelessFoldController::Init()
{
    single().FramelessFoldCmd_e         = MIRRFLD_COMMAND_IDLE;
    single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
    single().FramelessAutoAdjCmd_e      = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
    single().FramelessFoldActiveCmd_e   = MIRRFLD_COMMAND_IDLE;

    // Attempt to read
    _persistentData = _persistencyMgr.readData(); 
}

/*************************************************************/
/*! @brief FramelessFoldControl Main cyclic task
 * This task reads commands from the application's implementation
 * and executes the mirror control logic accordingly.
 *************************************************************/
void FramelessFoldController::MainTask()
{
    mirrorControl();
}

/*************************************************************/
/*! @brief Service Method: Request Fold (called by application)
*   @return true if request accepted, false otherwise
*   TBD: combine with requestUnfold into a single method with command parameter
*************************************************************/
bool FramelessFoldController::requestFold()
{
    /*Check if there is an ongoing operation*/
    if (single().FramelessFoldCmd_e != MIRRFLD_COMMAND_IDLE) {
        return false;
    }
    
    /* Accept the fold request */
    single().FramelessFoldCmd_e = MIRRFLD_COMMAND_FOLD;
    return true;
}

/*************************************************************/
/*! @brief Service Method: Request Unfold (called by application)
 *  @return true if request accepted, false otherwise
 *  TBD: combine with requestFold into a single method with command parameter
*************************************************************/
bool FramelessFoldController::requestUnfold()
{
    /*Check if there is an ongoing operation*/
    if (single().FramelessFoldCmd_e != MIRRFLD_COMMAND_IDLE) {
        return false; // Busy, reject request
    }
    
    /* Accept the unfold request */
    single().FramelessFoldCmd_e = MIRRFLD_COMMAND_UNFOLD;
    return true;
}

/*************************************************************/
/*! @brief Service Method: Cancel Operation (called by application)
 *************************************************************/
bool FramelessFoldController::cancelOperation()
{
    if (single().FramelessFoldCmd_e != MIRRFLD_COMMAND_IDLE) {
        single().FramelessFoldCmd_e = MIRRFLD_COMMAND_IDLE;
        single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
        single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
        single().FramelessFoldAbortReason_e = ABORT_REASON_LOW_PRIO;
        return true;
    }
    return false;
}

/*********************************************************************/
/*! @brief Get auto adjustment command state
 *********************************************************************/
t_emblGlassAutoAdjustCmd FramelessFoldController::getAutoAdjCmd() const
{
    return single().FramelessAutoAdjCmd_e;
}

/*********************************************************************/
/*! @brief Get Y target position
 *********************************************************************/
uint16 FramelessFoldController::getYTargetPos() const
{
    return axis(VERTICAL_AXIS).FramelessTargetPos_u16;
}

/*********************************************************************/
/*! @brief Get X target position
 *********************************************************************/
uint16 FramelessFoldController::getXTargetPos() const
{
    return axis(HORIZONTAL_AXIS).FramelessTargetPos_u16;
}

/*********************************************************************/
/*! @brief Get current fold command
 *********************************************************************/
t_emblMirrorFoldCmd FramelessFoldController::getCurrentCommand() const
{
    return single().FramelessFoldActiveCmd_e;
}

/*********************************************************************/
/*! @brief Get current abort reason
 *********************************************************************/
t_emblAbortReason FramelessFoldController::getAbortReason() const
{
    return single().FramelessFoldAbortReason_e;
}

/* Private functions */
/*============================================================================*/

/*********************************************************************/
/*! @brief Triggers the fold movement.
 * The target position is determined based on saved positions in persistent storage.
 * If valid saved positions are not available, default parameters are used.
 *********************************************************************/
void FramelessFoldController::triggerFoldMovement()
{
    /*Check if persistent data is available*/
    if (!_persistentData.has_value()) {return;}
    
    /*Deduce the type automatically(mirrorCtrlNvMData), do not create a copy of the instance, take the real data
    in case that data changes I want the real values to be changed as well*/
    auto& data = _persistentData.value();
    
    /*Go to the soft stop position on X axis*/
    axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 = data.LowPositionFold_u16 + EMBL_MM_PARAM(embl_ctrl_FramelessFoldPositionOffSet);
    /*Go to the midpoint position on Y axis*/
    axis(VERTICAL_AXIS).FramelessTargetPos_u16 = static_cast<uint16>((data.HighPositionY_u16 + data.LowPositionY_u16) >> 1u);
    
    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_ON;
    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_FOLD;
}

/*********************************************************************/
/*! @brief Triggers the unfold movement.
 * The target position is determined based on saved positions in persistent storage. 
 * If valid saved positions are not available, default parameters are used.
 *********************************************************************/
void FramelessFoldController::triggerUnfoldMovement()
{
    /*Check if persistent data is available*/
    if (!_persistentData.has_value()) {return;}
    
    mirrorCtrlNvMData& data = _persistentData.value();
    
    /* If saved positions are valid, use them. Otherwise, use default parameters */
    if ((data.FoldDrivePosY_u16 != 0u) && (data.FoldDrivePosX_u16 != 0u))
    {
        // Use saved positions from persistent storage
        axis(VERTICAL_AXIS).FramelessTargetPos_u16 = data.FoldDrivePosY_u16;
        axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 = data.FoldDrivePosX_u16;
    }
    else
    {
        // Use default positions from parameters
        axis(VERTICAL_AXIS).FramelessTargetPos_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosY);
        axis(HORIZONTAL_AXIS).FramelessTargetPos_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosX);
    }
    
    // Initiate the unfold sequence with auto-adjustment
    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_UNFOLD;
    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_ON;
}

/*********************************************************************/
/*! @brief Check abort conditions.
 * Evaluates various conditions that could lead to aborting the fold/unfold operation, such as:
 * - Position errors
 * - Manual adjustment requests
 * - Block detection (if enabled)
 * - TBD: During glass adjustment, check the abort conditions from glassAdjustment(not implemented)
 * The function returns the appropriate abort reason based on the evaluated conditions.
 *********************************************************************/
t_emblAbortReason FramelessFoldController::checkAbortConditions(bool requestOngoing)
{
    t_emblMirrorGlassAdjPosValidStat posAxisXAvailable;
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
#endif

    EmblCtrl_Read_embl_MirrorGlassAdjPosValidStat(&posAxisXAvailable);

    if ((posAxisXAvailable == true))
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
#endif

    if (requestOngoing == true)
    {
        /*TBD:*/
        return ABORT_REASON_NONE;
    }

    return ABORT_REASON_NONE;
}

/*********************************************************************/
/*! @brief Mirror control state machine.
 * This function implements the main state machine for handling fold and unfold operations, including:
 * - Evaluating abort conditions
 * - Triggering fold/unfold movements
 * - Handling ongoing operations and auto-adjustment status
 * - Updating the state based on the evaluated conditions and operation status
 * The function is called cyclically in the MainTask to continuously monitor and control the fold operations.
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
#endif
    
    const bool requestOngoing_u8 = (prevFramelessFoldCmd != single().FramelessFoldCmd_e) ? false : true;
    foldAbortReason_e = checkAbortConditions(requestOngoing_u8);

    if (foldAbortReason_e == ABORT_REASON_NONE) { 
        /* Handle fold request */
        if ((single().FramelessFoldCmd_e == MIRRFLD_COMMAND_FOLD) && 
            (prevFramelessFoldCmd != single().FramelessFoldCmd_e))
        {
            // Check if persistency is busy
            const bool persistencyBusy = _persistencyMgr.isBusy();
            
            if (!persistencyBusy && _persistentData.has_value())
            {
                auto& data = _persistentData.value();
                
                if (data.FoldPosSaveAllowed_u1 == true)
                {
                    /* Save current position before folding */
                    /* TBD: Get actual current position from motors */
                    data.FoldDrivePosX_u16 = MIRROR_X_FOLD_POSITION; 
                    data.FoldDrivePosY_u16 = MIRROR_Y_FOLD_POSITION;
                    data.FoldPosSaveAllowed_u1 = false;
                    
                    // Write to persistency
                    _persistencyMgr.writeData(data);
                }
            }
            
            triggerFoldMovement();
            single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
        }
        /* Handle unfold request */
        else if ((single().FramelessFoldCmd_e == MIRRFLD_COMMAND_UNFOLD) && 
                 (prevFramelessFoldCmd != single().FramelessFoldCmd_e))
        {
            triggerUnfoldMovement();
            single().FramelessFoldAbortReason_e = ABORT_REASON_NONE;
        }
        else
        {
            /* Ongoing fold/unfold movement active */
            if (single().FramelessAutoAdjCmd_e == MIRR_GLASS_ADJ_AUTO_CMD_ON)
            {
                /* TBD: Get actual status from glass adjustment module */
                const t_emblMirrorGlassAutoAdjustStatus glassStatus_e = MIRR_AUTO_GLASS_ADJ_FINISHED;
                
                if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_FINISHED)
                {
                    single().FramelessFoldAbortReason_e = ABORT_REASON_POS_REACHED;
                    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
                    single().FramelessFoldCmd_e = MIRRFLD_COMMAND_IDLE; // Reset command
                }
                else if (glassStatus_e == MIRR_AUTO_GLASS_ADJ_ABORTED)
                {
                    single().FramelessFoldAbortReason_e = ABORT_REASON_POS_UNREACHABLE;
                    single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
                    single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
                    single().FramelessFoldCmd_e = MIRRFLD_COMMAND_IDLE; // Reset command
                }
            }
        }
    }
    else
    {
        /* Abort reason active -> reset command */
        single().FramelessAutoAdjCmd_e = MIRR_GLASS_ADJ_AUTO_CMD_OFF;
        single().FramelessFoldActiveCmd_e = MIRRFLD_COMMAND_IDLE;
        single().FramelessFoldAbortReason_e = foldAbortReason_e;
        single().FramelessFoldCmd_e = MIRRFLD_COMMAND_IDLE; // Reset command
    }
    
    /* If position save is not allowed but user moved the mirror -> allow saving new position */
    if (_persistentData.has_value())
    {
        mirrorCtrlNvMData& data = _persistentData.value();
        
        if ((data.FoldPosSaveAllowed_u1 == false) && ((manualAdjustCmd != GLASS_ADJUST_CMD_NO_REQUEST) 
#if EMBL_GLASS_AUTO_ADJUST_AVAILABLE
             || (autoAdjustCmd != MIRR_GLASS_ADJ_AUTO_CMD_OFF)
#endif
            ))
        {
            const bool persistencyBusy = _persistencyMgr.isBusy();
            if (!persistencyBusy)
            {
                data.FoldPosSaveAllowed_u1 = true;
                _persistencyMgr.writeData(data);
            }
        }
    }

    prevFramelessFoldCmd = single().FramelessFoldCmd_e;
}

#endif // FRAMELESS_MIRROR_SUPPORT