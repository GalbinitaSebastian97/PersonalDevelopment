/*============================================================================*/
#ifndef emblFramelessFold_H
#define emblFramelessFold_H

/* Includes */
/*============================================================================*/

/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/
#if EMBL_FRAMELESS_MIRROR_SUPPORT
void emblFramelessFold_Init(void);
void emblFramelessFold_MainTask(void);
t_emblGlassAutoAdjustCmd emblFramelessFold_getAutoAdjCmd(void);
uint16 emblFramelessFold_getYTargetPos(void);
uint16 emblFramelessFold_getXTargetPos(void);
t_emblMirrorFoldCmd emblFramelessFold_getFoldCommand(void);
#endif // EMBL_FRAMELESS_MIRROR_SUPPORT
#endif //emblFramelessFold_H
