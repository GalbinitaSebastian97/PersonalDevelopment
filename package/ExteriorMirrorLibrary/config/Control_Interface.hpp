#ifndef EMBLCONTROL_INTERFACE_H
#define EMBLCONTROL_INTERFACE_H

#include <cstdint>
#include "TempObs_Types.h"
#include "AcCommon_Types.h"
#include "MotCtrlHal_Types.h"
#include "ExteriorMirror_Types.h"

#ifdef __cplusplus
namespace emblex {
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
/** \brief Init block notification from nvm
  * \return E_OK if the block is initialized successfully, E_NOT_OK if the block initialization failed
  *  */
extern Std_ReturnType EmblCtrl_InitBlock(void);

/**********************************************************************/
/** \brief Init block notification from nvm
  * \param[in] ServiceId Service id
  * \param[in] JobResult job result
  *  */
extern void EmblCtrl_JobFinished(uint8 ServiceId, NvM_RequestResultType_cil JobResult);

/**********************************************************************/
/** \brief Shall set control session state
  * \param state TRUE IO Control session active, FALSE Session inactive
  *  */
extern void EmblCtrl_setIOControlSessionState(boolean state);

/**********************************************************************/
/** \brief Read embl_MirrorFoldCmd
  * \param[out] data Command to fold or unfold the mirror
  *  */
extern void EmblCtrl_Read_embl_MirrorFoldCmd(t_emblMirrorFoldCmd* data);

/*!@}*/ /* [RPort] EMBLC_MirrorFoldCmd */

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
/** \brief read block
  * \param[in] DstPtr Pointer to store the read data
  * \return E_OK if the request was accepted, E_NOT_OK if the request was not accepted.
  *  */
extern Std_ReturnType EmblCtrl_ReadBlock(NvM_DstPtrType_cil DstPtr);


/**********************************************************************/
/** \brief Invalidate Nv block
  * \param[in] DstPtr Pointer to contain the default data
  *  */
extern void EmblCtrl_RestoreBlockDefaults(NvM_DstPtrType_cil DstPtr);

/**********************************************************************/
/** \brief restore PRAM block defaults
  *  */
extern void EmblCtrl_RestorePRAMBlockDefaults(void);

/**********************************************************************/
/** \brief write block
  * \param[in] SrcPtr Pointer to the data to be written
  * \return E_OK if the request was accepted, E_NOT_OK if the request was not accepted.

  *  */
extern Std_ReturnType EmblCtrl_WriteBlock(NvM_SrcPtrType_cil SrcPtr);

/**********************************************************************/
/** \brief write pram block
  *  */
extern void EmblCtrl_WritePRAMBlock(void);
/**********************************************************************/
/** \brief Read embl_BlockState
  * \param BlockDetMotor block detection motor number
  * \param[out] data Block state of the mirror motor
  *  */
extern void EmblCtrl_Read_embl_BlockState(uint8 const BlockDetMotor, t_emblMirrorBlockState* data);

/**********************************************************************/
/** \brief Read Temperature
  * \param[out] data temperature [1.0 degree Celsius + 40 degree offset] or TEMPOBS_TEMPERATURE_LOW_RES_WITH_OFFSET_SNA on error
  *  */
extern void EmblCtrl_OutsideTemp_Read_Temperature(TempObs_TemperatureLowResWithOffsetType* data);

/**********************************************************************/
/** \brief Read embl_MirrorInitRoutineCmd
  * \param[out] data Frameless mirror initialization routine command trigger
  *  */
extern void EmblCtrl_Read_embl_MirrorInitRoutineCmd(t_embl_MirrorInitRoutineCmd* data);


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
} // namespace emblex
#endif
#endif