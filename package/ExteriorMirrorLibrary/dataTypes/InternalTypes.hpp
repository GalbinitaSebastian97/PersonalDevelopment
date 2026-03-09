#ifndef emblInternalTypes_H
#define emblInternalTypes_H

typedef  signed char        int8;
typedef  unsigned char      uint8;
typedef  signed short       int16;
typedef  unsigned short     uint16;
typedef  signed long        int32;
typedef  unsigned long      uint32;
typedef  signed long long   int64;
typedef  unsigned long long uint64;
typedef  uint8 Std_ReturnType;

/* type for mirror position (8 bit unsigned)*/
typedef uint8 t_embl_position8u;
/* type for mirror position (16 bit unsigned)*/
typedef uint16 t_embl_position16u;
/* Type for motor movement timeout (16bit)*/
typedef uint16 t_bmcs_MaxMoveTime;
/* src ptr type */
typedef void const* NvM_SrcPtrType_cil;
/* dest ptr type */
typedef void const* NvM_DstPtrType_cil;
/** Mirror fold command */
typedef enum {
  MIRRFLD_COMMAND_IDLE = 0,   /**< Stop command */
  MIRRFLD_COMMAND_FOLD = 1,   /**< Fold the mirror */
  MIRRFLD_COMMAND_UNFOLD = 2  /**< Unfold the mirror */
} t_emblMirrorFoldCmd;

/** Mirror glass automatic adjustment command */
typedef enum {
  MIRR_GLASS_ADJ_AUTO_CMD_OFF = 0, /**< Automatic adjust command OFF */
  MIRR_GLASS_ADJ_AUTO_CMD_ON = 1   /**< Automatic adjust command ON */
} t_emblGlassAutoAdjustCmd;

/** Abort reason for mirror requests */
typedef enum {
  ABORT_REASON_NONE = 0,             /**< No abort reason */
  ABORT_REASON_VOLTAGE = 1,          /**< Request was stopped by supply voltage out of range */
  ABORT_REASON_TIMEOUT = 2,          /**< Request was stopped by timeout exceed */
  ABORT_REASON_DISABLED = 3,         /**< Request was stopped by disabling enable parameter */
  ABORT_REASON_AUTHORIZATION = 4,    /**< Request was stopped by authorization */
  ABORT_REASON_LOW_PRIO = 5,         /**< Request was stopped by another request with higher prio */
  ABORT_REASON_POS_UNREACHABLE = 6,  /**< Request was stopped because target position can not be reached */
  ABORT_REASON_POS_ERROR = 7,        /**< Request was stopped due to positioning error */
  ABORT_REASON_IGN_OFF = 8,          /**< Request was stopped by vehicle ignition status */
  ABORT_REASON_AMB_TEMP = 9,         /**< Request was stopped by outside temperature conditions */
  ABORT_REASON_DRIVER_ERROR = 10,    /**< Request was stopped due to driver error */
  ABORT_REASON_NVM_ERROR = 11,       /**< Request was stopped due to NvM error */
  ABORT_REASON_POS_LIMIT = 12,       /**< Request has exceeded/reached position limit */
  ABORT_REASON_BLOCK_DETECTED = 13,  /**< Request stopped due to block detection */
  ABORT_REASON_SPEED_LIMIT = 14,     /**< Request stopped due to vehicle speed above limit */
  ABORT_REASON_INVALID_REQ = 15,     /**< Request stopped due to invalid request */
  ABORT_REASON_PLAY_PROTECTION = 16, /**< Request stopped due to play protection */
  ABORT_REASON_ENGINE_CRANKING = 17, /**< Request stopped due to engine cranking event */
  ABORT_REASON_POS_REACHED = 18      /**< Request stopped due to target position reached */
} t_emblAbortReason;

/** Structure containing frameless fold control single data */
typedef struct {
  /** Frameless fold abort reason */
  t_emblAbortReason FramelessFoldAbortReason_e;
  /** Frameless fold request */
  t_emblMirrorFoldCmd FramelessFoldCmd_e;
  /** Frameless auto adjustment command */
  t_emblGlassAutoAdjustCmd FramelessAutoAdjCmd_e;
  /** Frameless Fold output command */
  t_emblMirrorFoldCmd FramelessFoldActiveCmd_e;
} mirrorFramelessFoldSingle;

/** Structure containing frameless fold control axis data */
typedef struct {
  uint16 FramelessTargetPos_u16; /**< Target position for the axis */
} mirrorFramelessFoldAxis;

/** Structure containing frameless fold control data */
typedef struct
{
  mirrorFramelessFoldSingle single; /**< Single data */
  mirrorFramelessFoldAxis axis[2];  /**< Axis data: 0: horizontal, 1: vertical */
} mirrorFramelessFoldData;

/** Possible return values for motor request halmo_setMotor */
typedef enum {
  MOTOR_RSP_OK = 0,        /**< requested motor / H-bridge command will be executed synchronously. */
  MOTOR_RSP_NOT_OK = 1,    /**< requested motor command / H-bridge won't be executed due to internal incompatibility. */
  MOTOR_RSP_BUSY = 2,      /**< requested motor command won't be executed since other SW instance is in charge of H-bridge. */
  MOTOR_RSP_ERROR = 3,     /**< requested motor command won't be executed due to error state of H-bridge. */
  MOTOR_RSP_OK_QUEUED = 4  /**< requested motor / H-bridge command will be executed asynchronously. */
} t_MotorResponse;

/** Mirror glass position validity status */
typedef enum {
  MIRR_GLASS_POS_VALID = 0,   /**< position is valid */
  MIRR_GLASS_POS_INVALID = 1  /**< position is invalid */
} t_emblMirrorGlassAdjPosValidStat;

/* Glass mirror selection */
typedef enum {
  GLASS_ADJUST_CMD_NO_REQUEST = 0, /**< No request for glass adjustment */
  GLASS_ADJUST_CMD_LEFT = 1,       /**< Request for left glass adjustment */
  GLASS_ADJUST_CMD_RIGHT = 2,      /**< Request for right glass adjustment */
  GLASS_ADJUST_CMD_UP = 3,         /**< Request for up glass adjustment */
  GLASS_ADJUST_CMD_DOWN = 4        /**< Request for down glass adjustment */
} t_emblGlassManualAdjustCmd;

/* Mirror frameless init routine */
typedef enum {
  MIRR_INIT_ROUTINE_CMD_OFF = 0,   /**< No request for frameless mirror init routine */
  MIRR_INIT_ROUTINE_CMD_ON = 1     /**< Request for frameless mirror init routine */
} t_embl_MirrorInitRoutineCmd;

typedef enum {
  MIRROR_INIT_ROUTINE_NOT_DONE = 0, /**< Frameless mirror init routine not done */
  MIRROR_INIT_ROUTINE_REJECTED = 1, /**< Frameless mirror init routine rejected due to preconditions not met */
  MIRROR_INIT_ROUTINE_ONGOING = 2,  /**< Frameless mirror init routine ongoing */
  MIRROR_INIT_ROUTINE_ABORTED = 3,  /**< Frameless mirror init routine aborted due to error or abort condition */
  MIRROR_INIT_ROUTINE_SUCCESSFUL = 4  /**< Frameless mirror init routine finished successfully */
} t_embl_MirrorInitRoutineStatus;

typedef struct {
  t_embl_MirrorInitRoutineStatus InitRoutineStatus_e; /**< Status of the frameless mirror initialization routine */
  bool   FoldPosSaveAllowed_u1; /**< Indicates if saving fold position in NvM is allowed */
  uint16 LowPositionFold_u16;    /**< Fold lower limit position */
  uint16 LowPositionX_u16;       /**< Horizontal lower limit position */
  uint16 HighPositionX_u16;      /**< Horizontal upper limit position */
  uint16 LowPositionY_u16;       /**< Vertical lower limit position */
  uint16 HighPositionY_u16;      /**< Vertical upper limit position */
  uint16 FoldDrivePosX_u16;     /**< Fold drive position on horizontal axis */
  uint16 FoldDrivePosY_u16;     /**< Fold drive position on vertical axis */
} mirrorCtrlNvMData;

/* NvM job status */
typedef enum{
    EMBLC_NVM_REQ_IDLE = 0,      /**< No ongoing request */
    EMBLC_NVM_REQ_ONGOING = 1,   /**< Request is ongoing */
    EMBLC_NVM_REQ_SUCCESFULL = 2,/**< Request finished successfully */
    EMBLC_NVM_REQ_FAILED = 3     /**< Request finished with failure */
} EMBLC_NVM_REQ_STATE;

typedef struct {
  EMBLC_NVM_REQ_STATE NvMReadState;   /**< State of the ongoing NvM read request */
  EMBLC_NVM_REQ_STATE NvMWriteState;  /**< State of the ongoing NvM write request */
  uint16 NvMReqTimeout;               /**< Timeout for NvM operations */
  mirrorCtrlNvMData emblcNvMData;     /**< RAM image of the NvM block for mirror control */
} mirrorCtrlNvM;

/* Identification of software*/
typedef enum{
  SWC_INST_NONE = 0,                     /**< No software component */
  SWC_INST_EMBL = 1                      /**< Exterior Mirror Library SW instance */
} t_InstanceIdentification;

typedef enum{
  MOTOR_CMD_RELEASE_CTRL = 0,             /**< Release control of the motor, allowing it to be driven by other SW instances or physical controls. */
  MOTOR_CMD_IDLE = 1,                     /**< No movement, hold current position */
  MOTOR_CMD_BRAKE = 2,                    /**< Brake motor, actively hold current position */
  MOTOR_CMD_MOVE_OPEN_DIR = 3,            /**< Move in open direction (e.g., unfold or adjust up/right) */
  MOTOR_CMD_MOVE_CLOSE_DIR = 4            /**< Move in close direction (e.g., fold or adjust down/left) */
} t_MotorCmd;

typedef enum{
  MIRR_BLOCKSTATE_NOT_BLOCKED = 0,  /**< No block detected */
  MIRR_BLOCKSTATE_BLOCKED = 1,      /**< Block detected */
} t_emblMirrorBlockState;

/* Describe the way any error is reported outside */
typedef enum{
  NVM_CIL_REQ_OK = 0,               /**< Request was successful */
  NVM_CIL_REQ_NOT_OK = 1,           /**< Request failed due to an error */
  NVM_CIL_REQ_RESTORED_DEFAULTS = 2 /**< Request was successful by restoring default values */
} NvM_RequestResultType_cil;

/* Move types for motor control commands*/
typedef enum{
  BMCS_MOVE_TYPE_MOVE_IN_DIR_A = 0,  /**< Move in direction A (e.g., fold or adjust down/left) */
  BMCS_MOVE_TYPE_MOVE_IN_DIR_B = 1,  /**< Move in direction B (e.g., unfold or adjust up/right) */
  BMCS_MOVE_TYPE_MOVE_TO_POS = 2     /**< Move to a specific target position */
} t_bmcs_MoveType;

/* Pwm control mode */
typedef enum{
  BMCS_PWM_CONTROLMODE_NONE = 0,           /**< No PWM control, motor is driven with full voltage in the requested direction */
  BMCS_CONTROL_MODE_SPEED_PROFILE = 1,     /**< Speed profile control mode, where PwmOption defines the speed profile to be used */
  BMCS_PWM_CONTROL_MODE_VOLTAGE = 2,       /**< Voltage control mode, where PwmOption defines the applied voltage as percentage of the full voltage */
  BMCS_PWM_CONTROL_MODE_DUTYCYLE = 3,      /**< Duty cycle control mode, where PwmOption defines the duty cycle for PWM control */
} t_bmcs_PwmControlMode;

/*Stop type for motor control commands*/
typedef enum{
  BMCS_STOP_TYPE_IDLE = 0,           /**< No stop, hold current state */
  BMCS_STOP_TYPE_BRAKE = 1,          /**< Brake stop, actively hold current position */
  BMCS_STOP_TYPE_RAMP_DOWN = 2       /**< Ramp down stop, gradually reduce speed to zero before holding position */
} t_bmcs_StopType;

/* Request result status */
typedef enum{
  BMCS_REQUEST_RESULT_DONE = 0,          /**< Request has been processed and completed */
  BMCS_REQUEST_RESULT_PENDING = 1,       /**< Request is still being processed */
  BMCS_REQUEST_RESULT_REJECTED = 2,      /**< Request has been rejected due to preconditions not met */
  BMCS_REQUEST_RESULT_ONGOING = 3,       /**< Request is currently being executed */
  BMCS_REQUEST_RESULT_FAILED = 4         /**< Request has been processed but failed due to an error */
} t_bmcs_RequestResult;

/* BMCS stop reason*/
typedef enum{
  BMCS_STOP_REASON_NONE = 0,             /**< No stop reason, movement is ongoing or completed successfully */
  BMCS_STOP_REASON_POS_REACHED = 1,      /**< Movement stopped because target position was reached */
  BMCS_STOP_REASON_TIMEOUT = 2,          /**< Movement stopped due to exceeding maximum allowed time */
  BMCS_STOP_REASON_BLOCK = 3,            /**< Movement stopped due to block detection */
  BMCS_STOP_REASON_VOLTAGE = 4,          /**< Movement stopped due to supply voltage out of range */
  BMCS_STOP_REASON_AUTHORIZATION = 5,    /**< Movement stopped due to authorization failure */
  BMCS_STOP_REASON_LOW_PRIO = 6,         /**< Movement stopped due to another request with higher priority */
  BMCS_STOP_REASON_POS_UNREACHABLE = 7   /**< Movement stopped because target position cannot be reached */
} t_bmcs_StopReason;

/* BMCS reject reason*/
typedef enum{
  BMCS_REJECT_REASON_NONE = 0,                  /**< No reject reason, request is accepted */
  BMCS_REJECT_REASON_PRECONDITIONS_NOT_MET = 1, /**< Request rejected due to unmet preconditions */
  BMCS_REJECT_REASON_INVALID_PARAMETER = 2,     /**< Request rejected due to invalid parameter */
  BMCS_REJECT_REASON_RESOURCE_UNAVAILABLE = 3,  /**< Request rejected due to unavailable resources */
  BMCS_REJECT_REASON_OTHER = 4                  /**< Request rejected due to other reasons such as internal errors */
} t_bmcs_RejectReason;

/* Motor driver error status */
typedef enum{
  BMCS_MOTOR_DRIVER_ERROR_NONE = 0,           /**< No error, motor is operational */
  BMCS_MOTOR_DRIVER_ERROR_OVER_CURRENT = 1,    /**< Over-current error detected */
  BMCS_MOTOR_DRIVER_ERROR_OVER_VOLTAGE = 2,    /**< Over-voltage error detected */
  BMCS_MOTOR_DRIVER_ERROR_UNDER_VOLTAGE = 3,   /**< Under-voltage error detected */
  BMCS_MOTOR_DRIVER_ERROR_OVER_TEMPERATURE = 4, /**< Over-temperature error detected */
  BMCS_MOTOR_DRIVER_ERROR_SHORT_CIRCUIT = 5,   /**< Short circuit detected */
  BMCS_MOTOR_DRIVER_ERROR_OPEN_LOAD = 6,       /**< Open load detected */
  BMCS_MOTOR_DRIVER_ERROR_OTHER = 7            /**< Other errors not categorized above */
} t_bmcs_MotorDriverError;

typedef enum {
  MIRR_AUTO_GLASS_ADJ_OFF = 0,        /**< No ongoing automatic glass adjustment */
  MIRR_AUTO_GLASS_ADJ_ONGOING = 1,    /**< Automatic glass adjustment is ongoing */
  MIRR_AUTO_GLASS_ADJ_FINISHED = 2,   /**< Automatic glass adjustment finished successfully */
  MIRR_AUTO_GLASS_ADJ_ABORTED = 3     /**< Automatic glass adjustment was aborted due to error or abort condition */
} t_emblMirrorGlassAutoAdjustStatus;
#endif //emblInternalTypes_H



