#ifndef FRAMELESS_FOLD_CONTROLLER_HPP
#define FRAMELESS_FOLD_CONTROLLER_HPP

#include "mirrorConfiguration.hpp"
#include "InternalTypes.hpp"
#include "PersistencyManager.hpp"
#include <optional>

#if EMBL_FRAMELESS_MIRROR_SUPPORT
namespace emblex
{

class FramelessFoldController
{
public:
    FramelessFoldController();
    ~FramelessFoldController() = default;

    static FramelessFoldController& getInstance();
    // ========== Methods (Request/Response pattern) ==========

    /*************************************************************/
    /*! \brief Initialize FramelessFoldControl functionality
    * Initializes the sframeless fold data persistent data from NvM, 
    * and prepares for operation.
    **************************************************************/
    void Init();
    /*************************************************************/
    /*! @brief FramelessFoldControl Main cyclic task
    * This task reads commands from the application's implementation
    * and executes the mirror control logic accordingly.
    *************************************************************/
    void MainTask();
    /*************************************************************/
    /*! @brief Service Method: Request Fold (called by application)
     *  @return true if request accepted, false otherwise
     *  TBD: combine with requestUnfold and cancelOperation into a 
     *  single method with command parameter
    *************************************************************/
    bool requestFold();
    /*************************************************************/
    /*! @brief Service Method: Request Unfold (called by application)
     *  @return true if request accepted, false otherwise
     *  TBD: combine with requestFold and cancelOperation into a 
     *  single method with command parameter
    *************************************************************/
    bool requestUnfold();
    /*************************************************************/
    /** @brief Cancel ongoing operation (fold/unfold)
     *  @return true if cancelled successfully
     *  TBD: combine with requestFold and requestUnfold into a 
     *  single method with command parameter
    *************************************************************/
    bool cancelOperation();

    // ========== Status Getters (Read by Applications) ==========
    /*************************************************************/
    /*! @brief In theory, these informations should be read from my 
     *  glass adjustment module, but for the sake of simplicity and 
     *  since I don't have a real glass adjustment module implemented, 
     *  I will read them from application's implementation of the service interface
     *  TBD: implement a real glass adjustment module and read the actual values 
     *  from it instead of reading from application
     *********************************************************************/
    t_emblGlassAutoAdjustCmd  getAutoAdjCmd() const;
    uint16                    getYTargetPos() const;
    uint16                    getXTargetPos() const;
    /*************************************************************/
    /*! @brief Get the current mirror fold command
     *  @return Current mirror fold command
     *************************************************************/
    t_emblMirrorFoldCmd       getCurrentCommand() const;
    /*************************************************************/
    /*! @brief Get the current abort reason
     *  @return Current abort reason
     *************************************************************/
    t_emblAbortReason         getAbortReason() const;

private:
    /*************************************************************/
    /*! @brief Private constants for axis identification
     *************************************************************/
    static constexpr unsigned HORIZONTAL_AXIS = 0u;
    static constexpr unsigned VERTICAL_AXIS   = 1u;

    /* FramelessFold data object*/
    mirrorFramelessFoldData _data{};
    mirrorCtrlNvMData*      _nvMem = nullptr;
    
    // Reference to NvM handler
    std::optional<mirrorCtrlNvMData> _persistentData;
    
    // Pointer to application's service implementation
    emblex::persistency::PersistencyManager& _persistencyMgr;

    /* Private functions */
    /*============================================================================*/
    /*********************************************************************/
    /*! @brief Mirror control state machine.
    * This function implements the main state machine for handling fold and unfold operations, including:
    * - Evaluating abort conditions
    * - Triggering fold/unfold movements
    * - Handling ongoing operations and auto-adjustment status
    * - Updating the state based on the evaluated conditions and operation status
    * The function is called cyclically in the MainTask to continuously monitor and control the fold operations.
    *********************************************************************/
    void mirrorControl();
    /*********************************************************************/
    /*! @brief Triggers the fold movement.
    * The target position is determined based on saved positions in persistent storage.
    * If valid saved positions are not available, default parameters are used.
    *********************************************************************/
    void triggerFoldMovement();
    /*********************************************************************/
    /*! @brief Triggers the unfold movement.
    * The target position is determined based on saved positions in persistent storage. 
    * If valid saved positions are not available, default parameters are used.
    *********************************************************************/
    void triggerUnfoldMovement();
    /*********************************************************************/
    /*! @brief Check abort conditions.
    * Evaluates various conditions that could lead to aborting the fold/unfold operation, such as:
    * - Position errors
    * - Manual adjustment requests
    * - Block detection (if enabled)
    * - TBD: During glass adjustment, check the abort conditions from glassAdjustment(not implemented)
    * The function returns the appropriate abort reason based on the evaluated conditions.
    *********************************************************************/
    t_emblAbortReason checkAbortConditions(bool requestOngoing);
    
    /* Return a modifiable reference to the single data -> You can read and write */
    auto& single() { return _data.single; }
    auto& axis(unsigned i) { return _data.axis[i]; }

    /* Return a const reference to the single data -> You can only read 
    First const means the return type is read only, the second const means the member function does not modify the object */
    const auto& single() const { return _data.single; }
    const auto& axis(unsigned i) const { return _data.axis[i]; }
};

} // namespace emblex
#endif
#endif // FRAMELESS_FOLD_CONTROLLER_HPP