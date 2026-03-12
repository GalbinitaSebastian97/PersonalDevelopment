#include "MirrorFoldServicesSkeleton.hpp"
#include "MirrorFoldServiceProxy.hpp"
#include <iostream>
#include <algorithm>

namespace emblex {
namespace services {

/*************************************************************/
/*! @brief Constructor
    * @param instanceId Service instance identifier 
    (e.g., "/sebastianGalbinita/embl/mirror_fold")
**************************************************************/
MirrorFoldServiceSkeleton::MirrorFoldServiceSkeleton(const std::string& instanceId)
    : m_instanceId(instanceId)
    , m_controller(&emblex::FramelessFoldController::getInstance())
{
    std::cout << "[Skeleton] Service instance created: " << m_instanceId << std::endl;
}

// ========= Lifecycle management (ara::com pattern) ==========

/*************************************************************/
/*! @brief Offer the service to the network
 * Makes this service discoverable by consumers
**************************************************************/
void MirrorFoldServiceSkeleton::offerService()
{
    if (!m_serviceOffered) {
        // In real ara::com, this would be:
        // OfferService();
        m_serviceOffered = true;
        std::cout << "[Skeleton] Service offered: " << m_instanceId << std::endl;
    }
}

/*************************************************************/
/*! @brief Stop offering the service
 * Makes this service no longer discoverable by consumers
**************************************************************/
void MirrorFoldServiceSkeleton::stopOfferService()
{
    if (m_serviceOffered) {
        // In real ara::com, this would be:
        // StopOfferService();
        m_serviceOffered = false;
        std::cout << "[Skeleton] Service stopped: " << m_instanceId << std::endl;
    }
}

/*************************************************************/
/*! @brief Process cyclic updates (call from main loop)
 * Handles event notifications and field updates
**************************************************************/
void MirrorFoldServiceSkeleton::processCycle()
{
    SharedServiceRegistry* registry = MirrorFoldServiceProxy::getSharedRegistry();
    
    if (registry && registry->commandPending) {
        if (registry->pendingCommand == SharedServiceRegistry::Command::FOLD)
            registry->commandResult = m_controller->requestFold();
        else if (registry->pendingCommand == SharedServiceRegistry::Command::UNFOLD)
            registry->commandResult = m_controller->requestUnfold();
        registry->commandPending = false;
    }
    
    // Keep status fields up to date for consumer reads
    registry->currentCommand = m_controller->getCurrentCommand();
    registry->abortReason    = m_controller->getAbortReason();
    registry->xTargetPos     = m_controller->getXTargetPos();
    registry->yTargetPos     = m_controller->getYTargetPos();
    
    // ... existing abort reason event notification ...
}

// ========== IMirrorFoldService implementation - delegates to controller ==========

/*************************************************************/
/*! @brief Request to fold the mirror
 *  @return true if request accepted, false otherwise
**************************************************************/
bool MirrorFoldServiceSkeleton::requestFold()
{
    std::cout << "[Skeleton] Method call: requestFold()" << std::endl;
    return m_controller->requestFold();
}
/*************************************************************/
/*! @brief Request to unfold the mirror
 *  @return true if request accepted, false otherwise
*************************************************************/
bool MirrorFoldServiceSkeleton::requestUnfold()
{
    std::cout << "[Skeleton] Method call: requestUnfold()" << std::endl;
    return m_controller->requestUnfold();
}
/*************************************************************/
/*! @brief Get the current abort reason
 * @return The current abort reason
**************************************************************/
t_emblAbortReason MirrorFoldServiceSkeleton::getAbortReason() const
{
    return m_controller->getAbortReason();
}
/*************************************************************/
/*! @brief Get the current fold command
 *  @return The current fold command
**************************************************************/
t_emblMirrorFoldCmd MirrorFoldServiceSkeleton::getCurrentCommand() const
{
    return m_controller->getCurrentCommand();
}
/*************************************************************/
/*! @brief Get the target X position for folding/unfolding
 *  @return Target X position
**************************************************************/
uint16_t MirrorFoldServiceSkeleton::getXTargetPosition() const
{
    return m_controller->getXTargetPos();
}
/*************************************************************/
/*! @brief Get the target Y position for folding/unfolding
 *  @return Target Y position
 *************************************************************/
uint16_t MirrorFoldServiceSkeleton::getYTargetPosition() const
{
    return m_controller->getYTargetPos();
}

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
void MirrorFoldServiceSkeleton::notifyAbortReasonChange(t_emblAbortReason newReason)
{
    std::vector<AbortReasonCallback> currentSubscribers;
    
    // 1. Scope the lock as tightly as possible
    {
        /*When this line executes, the thread tries to grab the "key" for m_subscribersMutex
        If another thread already has it, this thread stops and waits (blocks).
        Once it gets the key, it proceeds to the loop.
        Because it's a std::lock_guard, the key is automatically returned (unlocked) as soon as the function reaches the closing brace*/
        std::lock_guard<std::mutex> lock(m_subscribersMutex);
        currentSubscribers = m_abortReasonSubscribers; // Copy the list
    } // Lock is automatically released here

    std::cout << "[Skeleton] Event notification: AbortReason changed to " << static_cast<int>(newReason) << std::endl;
    
    // 2. Notify outside the lock to prevent deadlocks
    for (const auto& callback : currentSubscribers) {
        if (callback) { // Safety check
            callback(newReason);
        }
    }
}

} // namespace services
} // namespace emblex