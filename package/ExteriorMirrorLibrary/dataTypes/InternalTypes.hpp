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
  ABORT_REASON_POS_UNREACHABLE = 6,  /**< Request was stopped because target position can not be reached */
  ABORT_REASON_POS_ERROR = 7,        /**< Request was stopped due to positioning error */
  ABORT_REASON_DRIVER_ERROR = 10,    /**< Request was stopped due to driver error */
  ABORT_REASON_NVM_ERROR = 11,       /**< Request was stopped due to NvM error */
  ABORT_REASON_POS_LIMIT = 12,       /**< Request has exceeded/reached position limit */
  ABORT_REASON_INVALID_REQ = 15,     /**< Request stopped due to invalid request */
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

typedef struct {
  bool   FoldPosSaveAllowed_u1;  /**< Indicates if saving fold position in NvM is allowed */
  uint16 FoldDrivePosX_u16;      /**< Fold drive position on horizontal axis */
  uint16 FoldDrivePosY_u16;      /**< Fold drive position on vertical axis */
  uint16 LowPositionFold_u16;    /**< Fold lower limit position */
  uint16 LowPositionX_u16;       /**< Horizontal lower limit position */
  uint16 LowPositionY_u16;       /**< Vertical lower limit position */
  uint16 HighPositionX_u16;      /**< Horizontal upper limit position */
  uint16 HighPositionY_u16;      /**< Vertical upper limit position */

} mirrorCtrlNvMData;

/* NvM job status */
typedef enum{
    NVM_REQ_IDLE = 0,      /**< No ongoing request */
    NVM_REQ_ONGOING = 1,   /**< Request is ongoing */
    NVM_REQ_SUCCESFULL = 2,/**< Request finished successfully */
    NVM_REQ_FAILED = 3     /**< Request finished with failure */
} NVM_REQ_STATE;

typedef struct {
  NVM_REQ_STATE NvMReadState;   /**< State of the ongoing NvM read request */
  NVM_REQ_STATE NvMWriteState;  /**< State of the ongoing NvM write request */
  uint16 NvMReqTimeout;               /**< Timeout for NvM operations */
  mirrorCtrlNvMData emblcNvMData;     /**< RAM image of the NvM block for mirror control */
} mirrorCtrlNvM;

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

typedef enum {
  MIRR_AUTO_GLASS_ADJ_OFF = 0,        /**< No ongoing automatic glass adjustment */
  MIRR_AUTO_GLASS_ADJ_ONGOING = 1,    /**< Automatic glass adjustment is ongoing */
  MIRR_AUTO_GLASS_ADJ_FINISHED = 2,   /**< Automatic glass adjustment finished successfully */
  MIRR_AUTO_GLASS_ADJ_ABORTED = 3     /**< Automatic glass adjustment was aborted due to error or abort condition */
} t_emblMirrorGlassAutoAdjustStatus;
#endif //emblInternalTypes_H



