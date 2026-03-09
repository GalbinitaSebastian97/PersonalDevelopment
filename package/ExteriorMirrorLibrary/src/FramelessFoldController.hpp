// FramelessFoldController.hpp
#ifndef FRAMELESS_FOLD_CONTROLLER_HPP
#define FRAMELESS_FOLD_CONTROLLER_HPP

#include "mirrorConfiguration.hpp"
#include "InternalTypes.hpp"
#include "NvMHandling.hpp"

#if EMBL_FRAMELESS_MIRROR_SUPPORT
namespace emblex
{

class FramelessFoldController
{
public:
    FramelessFoldController();
    ~FramelessFoldController() = default;

    void Init();
    void MainTask();

    // query interface
    t_emblGlassAutoAdjustCmd  getAutoAdjCmd();
    uint16                    getYTargetPos();
    uint16                    getXTargetPos();
    t_emblMirrorFoldCmd       getFoldCommand();

private:
    static constexpr unsigned HORIZONTAL_AXIS = 0u;
    static constexpr unsigned VERTICAL_AXIS   = 1u;

    mirrorFramelessFoldData      _data{};
    mirrorCtrlNvMData*           _nvMem = nullptr;
    embl::control::EmblControlNvMHandler& _nvmHandler;
    
    void mirrorControl();
    void triggerFoldMovement();
    void triggerUnfoldMovement();
    t_emblAbortReason checkAbortConditions(bool requestOngoing);

    // helpers for member access
    auto& single() { return _data.single; }
    auto& axis(unsigned i) { return _data.axis[i]; }
};

} // namespace emblex
#endif
#endif // FRAMELESS_FOLD_CONTROLLER_HPP