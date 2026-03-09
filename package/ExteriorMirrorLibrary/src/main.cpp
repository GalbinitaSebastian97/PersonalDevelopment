#include <iostream>
#include "mirrorConfiguration.hpp"
#if EMBL_FRAMELESS_MIRROR_SUPPORT
#include "FramelessFoldController.hpp"
#endif /*EMBL_FRAMELESS_MIRROR_SUPPORT*/
using namespace emblex;

int main()
{
#if EMBL_FRAMELESS_MIRROR_SUPPORT
    FramelessFoldController framelessFoldController;
    framelessFoldController.Init();
    framelessFoldController.MainTask();
    std::cout << "Finished"<< "\n";
#endif /*EMBL_FRAMELESS_MIRROR_SUPPORT*/
    return 0;
}
