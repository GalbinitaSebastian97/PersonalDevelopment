#ifndef MIRROR_PARAM_HPP
#define MIRROR_PARAM_HPP

#include "InternalTypes.hpp"
/* Parameters block */
typedef struct{
    /* Fold position high threshold */
    uint8 embl_FramelessPosFoldHiighThreshold;
    /* Fold position low threshold */
    uint8 embl_FramelessPosFoldLowThreshold;
    /* X position high threshold */
    uint8 embl_FramelessPosXHighThreshold;
    /* X position low threshold */
    uint8 embl_FramelessPosXLowThreshold;
    /* Y position high threshold */
    uint8 embl_FramelessPosYHighThreshold;
    /* Y position low threshold */
    uint8 embl_FramelessPosYLowThreshold;
    /* Initialization routine fold lower limit raw value*/
    t_embl_position16u embl_FramelessFoldLowerLimitPos;
    /*Initialization routine fold upper limit raw value*/
    t_embl_position16u embl_FramelessHorizontalLowerLimitPos;
    /*Initialization routine upper glass adjust axis X limit raw value*/
    t_embl_position16u embl_FramelessHorizontalUpperLimitPos;
    /*Initialization routine lower glass adjust axis Y limit raw value*/
    t_embl_position16u embl_FramelessVerticalLowerLimitPos;
    /*Initialization routine upper glass adjust axis Y limit raw value*/
    t_embl_position16u embl_FramelessVerticalUpperLimitPos;
    /* Fold position offset for target position calculation */
    t_embl_position8u embl_FramelessFoldPositionOffSet;
    /* Default driving position for x motor*/
    t_embl_position16u embl_FramelessDrivePosX;
    /* Default driving position for y motor*/
    t_embl_position16u embl_FramelessDrivePosY;
    /* Frameless request arbitration between glass requests and fold requests; 1 - fold requests have bigger priority; 0 - glass requests have bigger priority */
    bool embl_FramelessFoldGlassRequestPriority:1;
    /* NvM response timeout */
    uint16 embl_NvMResponseTimeout;
} mirrorParam;

#define EMBL_MM_PARAM(param) (PARAM_##param)

/*TBD: Need to think of how these parameters can be generated and changed(Do it using an parameter excel)*/
#define PARAM_embl_FramelessPosFoldHiighThreshold 90u
#define PARAM_embl_FramelessPosFoldLowThreshold 10u
#define PARAM_embl_FramelessPosXHighThreshold 90u
#define PARAM_embl_FramelessPosXLowThreshold 10u
#define PARAM_embl_FramelessPosYHighThreshold 90u
#define PARAM_embl_FramelessPosYLowThreshold 10u
#define PARAM_embl_FramelessFoldLowerLimitPos 100u
#define PARAM_embl_FramelessHorizontalLowerLimitPos 100u
#define PARAM_embl_FramelessHorizontalUpperLimitPos 900u
#define PARAM_embl_FramelessVerticalLowerLimitPos 100u
#define PARAM_embl_FramelessVerticalUpperLimitPos 900u
#define PARAM_embl_FramelessFoldPositionOffSet 5u
#define PARAM_embl_FramelessDrivePosX 500u
#define PARAM_embl_FramelessDrivePosY 500u
#define PARAM_embl_FramelessFoldGlassRequestPriority 1u
#define PARAM_embl_NvMResponseTimeout 100u

#endif //MIRROR_PARAM_HPP