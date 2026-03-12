#ifndef MIRROR_FOLD_SERVICE_SKELETON_HPP
#define MIRROR_FOLD_SERVICE_SKELETON_HPP

#include "IMirrorFoldService.hpp"
#include "FramelessFoldControl.hpp"
#include "InternalTypes.hpp"
#include <memory>
#include <vector>
#include <mutex>

namespace emblex {
namespace services {

/**
 * @brief ara::com Skeleton - Service Provider Implementation
 * 
 * This class wraps your existing FramelessFoldController and exposes it
 * as an Adaptive AUTOSAR service. In a real implementation, this would
 * inherit from ara::com::skeleton::SkeletonBase.
 * 
 * The skeleton handles:
 * - Service offering/discovery
 * - Method call dispatching
 * - Event notification
 * - Field value updates
 */
class MirrorFoldServiceSkeleton : public IMirrorFoldService {
public:
    /*************************************************************/
    /*! @brief Constructor
     * @param instanceId Service instance identifier (e.g., "/sebastianGalbinita/embl/mirror_fold")
    **************************************************************/
    explicit MirrorFoldServiceSkeleton(const std::string& instanceId);
    
    ~MirrorFoldServiceSkeleton() override = default;
    
    // ========= Lifecycle management (ara::com pattern) ==========

    /*************************************************************/
    /*! @brief Offer the service to the network
     * Makes this service discoverable by consumers
     *************************************************************/
    void offerService();
    
    /*************************************************************/
    /*! @brief Stop offering the service
     * Makes this service no longer discoverable by consumers
     *************************************************************/
    void stopOfferService();
    
    /*************************************************************/
    /*! @brief Process cyclic updates (call from main loop)
     * Handles event notifications and field updates
     *************************************************************/
    void processCycle();
    
    // ========== IMirrorFoldService implementation - delegates to controller ==========
    
    /*************************************************************/
    /*! @brief Request to fold the mirror
     * @return true if request accepted, false otherwise
     *************************************************************/
    bool requestFold() override;
    /*************************************************************/
    /*! @brief Request to unfold the mirror
     * @return true if request accepted, false otherwise
     *************************************************************/
    bool requestUnfold() override;
    /*************************************************************/
    /*! @brief Get the current abort reason
     * @return The current abort reason
     *************************************************************/
    t_emblAbortReason getAbortReason() const override;
    /*************************************************************/
    /*! @brief Get the current fold command
     * @return The current fold command
     *************************************************************/
    t_emblMirrorFoldCmd getCurrentCommand() const override;
    /*************************************************************/
    /*! @brief Get the target X position for folding/unfolding
     * @return Target X position
     *************************************************************/
    uint16_t getXTargetPosition() const override;
    /*************************************************************/
    /*! @brief Get the target Y position for folding/unfolding
     * @return Target Y position
     *************************************************************/
    uint16_t getYTargetPosition() const override;
    /*************************************************************/
    /*! @brief Get access to the underlying controller (for init/main task)
     * @return Reference to FramelessFoldController
     *************************************************************/
    emblex::FramelessFoldController& getController() { return *m_controller; }

private:
    std::string m_instanceId;
    emblex::FramelessFoldController* m_controller;
    /* Flag to track if the service is currently offered */
    bool m_serviceOffered{false};
    /*The actual list of everyone who wants to be notified -> mailing list of callbacks, 
    one operation at a time, protected by mutex*/
    std::vector<AbortReasonCallback> m_abortReasonSubscribers;
    /* For multi-threading */
    mutable std::mutex m_subscribersMutex;
    // Last notified values (for change detection)
    t_emblAbortReason m_lastAbortReason{ABORT_REASON_NONE};
    /*************************************************************/
    /*! @brief Notifies all registered subscribers about a change in the abort reason.
    *
    * This function safely broadcasts an abort reason change to all subscribed callbacks
    * without blocking the notification delivery. It uses a copy-and-release pattern:
    * first, it quickly copies the subscriber list under lock, then releases the lock
    * before invoking the callbacks. This prevents potential deadlocks if a callback
    * tries to modify the subscriber list.
    *
    * @param newReason The new abort reason to notify subscribers about
    *
    * @note Callbacks are invoked sequentially outside of any mutex lock, allowing
    *       them to freely interact with other parts of the system without risk of deadlock
    *************************************************************/
    void notifyAbortReasonChange(t_emblAbortReason newReason);
};

} // namespace services
} // namespace emblex

#endif // MIRROR_FOLD_SERVICE_SKELETON_HPP