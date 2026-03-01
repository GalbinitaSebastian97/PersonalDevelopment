#ifndef emblInternalTypes_H
#define emblInternalTypes_H

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

typedef unsigned short uint16;

/** Structure containing frameless fold control data */
typedef struct {
  /** Frameless fold structure on axis */
  mirrorFramelessFoldAxis axis[2];
  /** Frameless fold structure singleton */
  mirrorFramelessFoldSingle single;
} mirrorFramelessFoldData;

/** Structure containing frameless fold control axis X and Y data */
typedef struct {
  /** Frameless target position */
  uint16 FramelessTargetPos_u16;
} mirrorFramelessFoldAxis;

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

#endif //emblInternalTypes_H



