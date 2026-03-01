#ifndef EMBLCONFIG_H
#define EMBLCONFIG_H

/* Selects the block detection type used for mirror folding motor:
    0: No Block Detection
    1: Inrush current based
    2: Current limit based
*/
#define EMBL_FOLD_BLOCK_DET_TYPE = 2u

/* Enables mirror glass adjustment feature */
#define EMBL_GLASS_AUTO_ADJUST_AVAILABLE 1u

/* Enables control of framed mirrors */
#define EMBL_FRAMED_MIRROR_SUPPORT 1u

/* Enables control of frameless mirrors */
#define EMBL_FRAMELESS_MIRROR_SUPPORT 1u

#endif //EMBLCONFIG_H