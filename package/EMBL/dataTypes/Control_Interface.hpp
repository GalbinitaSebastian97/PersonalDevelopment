#ifndef EMBLCONTROL_INTERFACE_H
#define EMBLCONTROL_INTERFACE_H

#include <cstdint>
#include "InternalTypes.hpp"
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************/
/** \brief Run EMBL initialization Task
  * 
  * EMBL initialization function. This interface needs to be run in
  * scheduler
  *  */
extern void EMBLControl_Init(void);

/**********************************************************************/
/** \brief EMBL Control main cyclic runnable 
  * 
  * This interface needs to be run in scheduler, see period defined by
  * EMBL_RUNNABLE_CYCLE_TIME
  *  */
extern void EMBLControl_CyclicRunMain(void);

/**********************************************************************/
/** \brief Write embl_AbortReason
  * \param data Abort reasons for mirror requests.
  *  */
extern void EmblCtrl_MirrorFoldCmd_Write_embl_AbortReason(t_emblAbortReason data);

/**********************************************************************/
/** \brief Shall set control session state
  * \param state TRUE IO Control session active, FALSE Session inactive
  *  */
extern void EmblCtrl_setIOControlSessionState(bool state);

/**********************************************************************/
/** \brief Read embl_MirrorFoldCmd
  * \param[out] data Command to fold or unfold the mirror
  *  */
extern void EmblCtrl_Read_embl_MirrorFoldCmd(t_emblMirrorFoldCmd* data);

/**********************************************************************/
/** \brief Called to acquire control and give different activation requests to motor H bridge.
  * \param MirrorMotor mirror motor number
  * \param instanceId Software instance which want to acquire / release H-bridge control
  * \param motorcmd control command for motor
  * \param dutyCycle PWM duty cycle [100% / 2^15]
  * \return Synchronous feedback to given motor command. If response is OK, given command was accepted. Different responses provide reason for rejecting command.
  *  */
extern t_MotorResponse EmblCtrl_setMotor(uint8 const MirrorMotor, t_InstanceIdentification instanceId, t_MotorCmd const motorcmd, uint16 const dutyCycle);

/**********************************************************************/
/** \brief Read embl_MirrorGlassAdjPosValidStat
  * \param[out] data Indicates if the glass adjust position is valid
  *  */
extern void EmblCtrl_Read_embl_MirrorGlassAdjPosValidStat(t_emblMirrorGlassAdjPosValidStat* data);

/**********************************************************************/
/** \brief Read embl_GlassAutoAdjustCmd
  * \param[out] data Command for automatic glass adjustment to the provided target position
  *  */
extern void EmblCtrl_Read_embl_GlassAutoAdjustCmd(t_emblGlassAutoAdjustCmd* data);

/**********************************************************************/
/** \brief Read embl_GlassManualAdjustCmd
  * \param[out] data Command for manual glass adjustment
  *  */
extern void EmblCtrl_Read_embl_GlassManualAdjustCmd(t_emblGlassManualAdjustCmd* data);

/**********************************************************************/
/** \brief Read embl_BlockState
  * \param BlockDetMotor block detection motor number
  * \param[out] data Block state of the mirror motor
  *  */
extern void EmblCtrl_Read_embl_BlockState(uint8 const BlockDetMotor, t_emblMirrorBlockState* data);

/**********************************************************************/
/** \brief Read embl_MirrorInitRoutineCmd
  * \param[out] data Frameless mirror initialization routine command trigger
  *  */
extern void EmblCtrl_Read_embl_MirrorInitRoutineCmd(t_embl_MirrorInitRoutineCmd* data);

/**********************************************************************/
/** \brief Trigger a move request
  * \param Command Command to specify movement type: MOVE_IN_DIR_A = 0, MOVE_IN_DIR_B = 1, MOVE_TO_POS = 2.
  * \param Position Target position in 1/16 motor rotations (1 increment = 1/16 rotation).
  * \param MaxMoveTime Maximum activation time allowed for a 'move in direction' request. Range: 0 ... 102 s.
  * \param PwmControlMode PWM control mode: PWM_NONE = 0, CONSTANT_SPEED = 1, SPEED_PROFILE = 2, MODE_ASSIST = 3, MODE_VOLTAGE = 4, MODE_ACTIVEHOLD = 5, PWM_DUTYCYCLE = 6.
  * \param PwmOption Option associated with the selected PWM control mode.
  * 
  * Triggers one of the supported movement types: move in direction A or
  * B, or move to a target position.
  *  */
extern void EmblCtrl_GlassAdjXMotor_SetMoveRequest(t_bmcs_MoveType Command, uint16 Position, t_bmcs_MaxMoveTime MaxMoveTime, t_bmcs_PwmControlMode PwmControlMode, uint8 PwmOption);

/**********************************************************************/
/** \brief Trigger a move request
  * \param Command Command to specify movement type: MOVE_IN_DIR_A = 0, MOVE_IN_DIR_B = 1, MOVE_TO_POS = 2.
  * \param Position Target position in 1/16 motor rotations (1 increment = 1/16 rotation).
  * \param MaxMoveTime Maximum activation time allowed for a 'move in direction' request. Range: 0 ... 102 s.
  * \param PwmControlMode PWM control mode: PWM_NONE = 0, CONSTANT_SPEED = 1, SPEED_PROFILE = 2, MODE_ASSIST = 3, MODE_VOLTAGE = 4, MODE_ACTIVEHOLD = 5, PWM_DUTYCYCLE = 6.
  * \param PwmOption Option associated with the selected PWM control mode.
  * 
  * Triggers one of the supported movement types: move in direction A or
  * B, or move to a target position.
  *  */
extern void EmblCtrl_GlassAdjYMotor_SetMoveRequest(t_bmcs_MoveType Command, uint16 Position, t_bmcs_MaxMoveTime MaxMoveTime, t_bmcs_PwmControlMode PwmControlMode, uint8 PwmOption);

/**********************************************************************/
/** \brief Stops the ongoing movement.
  * \param StopType Stop type: STOP_BRAKE = 0, STOP_RAMPDOWN = 1.
  * 
  * If STOP_BRAKE is used, the H-Bridge is set to brake state immediately.
  * If STOP_RAMPDOWN is used, a soft stop is performed before switching
  * the H-Bridge to brake state.
  *  */
extern void EmblCtrl_GlassAdjXMotor_SetStopRequest(t_bmcs_StopType StopType);

/**********************************************************************/
/** \brief Authorizes movements by setting the client alive status.
  * \param ClientAlive TRUE authorizes movements, FALSE stops ongoing movements.
  * 
  * If 0 (FALSE), no movements can be started and ongoing movements
  * (except reversals) will be stopped. The default value will be assumed
  * when this field is not written for 100ms.
  *  */
extern void EmblCtrl_GlassAdjXMotor_SetClientAlive(bool ClientAlive);

/**********************************************************************/
/** \brief Read RequestResult
  * \param[out] data Result of the last motor control request (accepted or rejected).
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_RequestResult(t_bmcs_RequestResult* data);

/**********************************************************************/
/** \brief Read StopReason
  * \param[out] data Reason for completing the ongoing movement request (without errors).If movement was aborted due to errors, the information is available via MotorDriverError signal.
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_StopReason(t_bmcs_StopReason* data);

/**********************************************************************/
/** \brief Read RejectReason
  * \param[out] data Reason for rejecting the ongoing movement request (without errors).If movement was aborted due to errors, the information is available via MotorDriverError signal.
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_RejectReason(t_bmcs_RejectReason* data);

/**********************************************************************/
/** \brief Read MotorActivationStatus
  * \param[out] data If set to 1 (TRUE), motor activation is possible. Otherwise the feature is not available due to active errors.
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_MotorActivationStatus(bool* data);

/**********************************************************************/
/** \brief Read PositioningStatus
  * \param[out] data If set to 1 (TRUE), positioning is available. Otherwise the feature is not available due to active errors.
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_PositioningStatus(bool* data);

/**********************************************************************/
/** \brief Read MotorDriverError
  * \param[out] data Shows the error status of the motor driver
  *  */
extern void EmblCtrl_GlassAdjXMotor_Read_MotorDriverError(t_bmcs_MotorDriverError* data);

/**********************************************************************/
/** \brief Stops the ongoing movement.
  * \param StopType Stop type: STOP_BRAKE = 0, STOP_RAMPDOWN = 1.
  * 
  * If STOP_BRAKE is used, the H-Bridge is set to brake state immediately.
  * If STOP_RAMPDOWN is used, a soft stop is performed before switching
  * the H-Bridge to brake state.
  *  */
extern void EmblCtrl_GlassAdjYMotor_SetStopRequest(t_bmcs_StopType StopType);

/**********************************************************************/
/** \brief Authorizes movements by setting the client alive status.
  * \param ClientAlive TRUE authorizes movements, FALSE stops ongoing movements.
  * 
  * If 0 (FALSE), no movements can be started and ongoing movements
  * (except reversals) will be stopped. The default value will be assumed
  * when this field is not written for 100ms.
  *  */
extern void EmblCtrl_GlassAdjYMotor_SetClientAlive(bool ClientAlive);

/**********************************************************************/
/** \brief Read RequestResult
  * \param[out] data Result of the last motor control request (accepted or rejected).
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_RequestResult(t_bmcs_RequestResult* data);

/**********************************************************************/
/** \brief Read StopReason
  * \param[out] data Reason for completing the ongoing movement request (without errors).If movement was aborted due to errors, the information is available via MotorDriverError signal.
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_StopReason(t_bmcs_StopReason* data);

/**********************************************************************/
/** \brief Read RejectReason
  * \param[out] data Reason for rejecting the ongoing movement request (without errors).If movement was aborted due to errors, the information is available via MotorDriverError signal.
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_RejectReason(t_bmcs_RejectReason* data);

/**********************************************************************/
/** \brief Read MotorActivationStatus
  * \param[out] data If set to 1 (TRUE), motor activation is possible. Otherwise the feature is not available due to active errors.
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_MotorActivationStatus(bool* data);

/**********************************************************************/
/** \brief Read PositioningStatus
  * \param[out] data If set to 1 (TRUE), positioning is available. Otherwise the feature is not available due to active errors.
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_PositioningStatus(bool* data);

/**********************************************************************/
/** \brief Read MotorDriverError
  * \param[out] data Shows the error status of the motor driver
  *  */
extern void EmblCtrl_GlassAdjYMotor_Read_MotorDriverError(t_bmcs_MotorDriverError* data);

/**********************************************************************/
/** \brief write block
 *  \param[in] data Pointer to the data block to be written to NvM
 *  \return true if write request was accepted, false if NvM is busy
 */
extern Std_ReturnType EmblCtrl_WriteBlock(NvM_SrcPtrType_cil data);

/**********************************************************************/
/** \brief read block
 *  \param[in] data Pointer to the data block to be filled with NvM data
 *  \return true if read request was accepted, false if NvM is busy
 */
extern Std_ReturnType EmblCtrl_ReadBlock(NvM_DstPtrType_cil data);
/** mirror control supported motors */
typedef enum {
  FOLD_MOTOR,                    
  GLASS_ADJ_X_MOTOR,             
  GLASS_ADJ_Y_MOTOR,             
  FRAMELESS_HORIZONTAL_ADJ_MOTOR,
  FRAMELESS_VERTICAL_ADJ_MOTOR,  
  NUMBER_OF_MIRROR_MOTORS        
} t_MirrorMotorType;

/** Block detection motor types. */
typedef enum {
  EMBLC_INST_FRAMELESS_HORIZONTAL_MOTOR,
  EMBLC_NUMBER_OF_FOLD_MOTORS           
} t_emblcBlockDetMotorInstance;

/** Glass adjustment axis types. */
typedef enum {
  GLASSADJ_AXIS_X,             
  GLASSADJ_AXIS_Y,             
  NUMBER_OF_GLASS_ADJ_CHANNELS 
} t_GlassAdjAxisType;

#ifdef __cplusplus
} // extern "C"
#endif

#endif