#ifndef IMIRROR_FOLD_SERVICE_HPP
#define IMIRROR_FOLD_SERVICE_HPP

#include <cstdint>
#include <functional>
#include "InternalTypes.hpp"

namespace emblex {
namespace services {

/**
 * @brief Service Interface for Mirror Fold Control
 * 
 * This would typically be auto-generated from ARXML service definition
 * For Adaptive AUTOSAR, this defines the contract between provider and consumer
 */
class IMirrorFoldService {
public:
    virtual ~IMirrorFoldService() = default;
    
    // ========== Methods (Request/Response pattern) ==========

    /**********************************************************************/
    /** @brief Request to fold the mirror
      * @return true if request accepted, false otherwise
    ***********************************************************************/
    virtual bool requestFold() = 0;

    /**********************************************************************/
    /** @brief Request to unfold the mirror
      * @return true if request accepted, false otherwise
    ***********************************************************************/
    virtual bool requestUnfold() = 0;

    // ========== Fields (Status Getters) ==========

    /** @brief Get the current abort reason
      * @return The current abort reason
    ***********************************************************************/
    virtual t_emblAbortReason getAbortReason() const = 0;

    /**********************************************************************/
    /** @brief Get the current fold command
      * @return The current fold command
    ***********************************************************************/
    virtual t_emblMirrorFoldCmd getCurrentCommand() const = 0;
    /**********************************************************************/
    /** @brief Get the target X position for folding/unfolding
      * @return Target X position
    ***********************************************************************/
    virtual uint16_t getXTargetPosition() const = 0;

    /**********************************************************************/
    /** @brief Get the target Y position for folding/unfolding
      * @return Target Y position
    ***********************************************************************/
    virtual uint16_t getYTargetPosition() const = 0;
    
    // ========== Events (notifications to Application) ==========

    /**********************************************************************/
    /** @brief Subscribe to abort reason changes
      * @param callback Function to call when abort reason changes
    ***********************************************************************/
    using AbortReasonCallback = std::function<void(t_emblAbortReason)>;

    /**********************************************************************/
    /** @brief Subscribe to fold command changes
      * @param callback Function to call when fold command changes
     **********************************************************************/
    using CommandEventCallback = std::function<void(t_emblMirrorFoldCmd)>;

};

} // namespace services
} // namespace emblex

#endif // IMIRROR_FOLD_SERVICE_HPP