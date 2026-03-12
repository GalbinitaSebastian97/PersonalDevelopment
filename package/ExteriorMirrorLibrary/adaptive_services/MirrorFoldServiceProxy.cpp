#include "MirrorFoldServiceProxy.hpp"


namespace emblex {
namespace services {

// Static service registry (simulates ara::com service discovery)
IMirrorFoldService* MirrorFoldServiceProxy::s_serviceInstance = nullptr;

/**********************************************************************/
/*! @brief findService() - Called by the CONSUMER
 * This function reads from SHARED MEMORY to check if a provider is available
 * @param instanceId Instance to search for (empty = find all)
 * @return Vector of available ServiceHandle objects (empty list if no provider found)
 **********************************************************************/
std::vector<MirrorFoldServiceProxy::ServiceHandle> 
MirrorFoldServiceProxy::findService(const std::string& instanceId)
{
    std::vector<ServiceHandle> handles;
    
    // ========================================================================
    // SHARED MEMORY: Check if a provider has registered itself
    // ========================================================================
    
    /* Get pointer to the shared memory region
       This accesses the SAME memory that the provider wrote to! */
    SharedServiceRegistry* registry = getSharedRegistry();
    
    if (registry == nullptr) {
        std::cerr << "[Proxy] ERROR: Could not access shared memory for findService!" << std::endl;
        return handles;  // Return empty list
    }
    
    /* Check if the provider has set serviceAvailable to true */
    if (registry->serviceAvailable) {
        /* Service found! Create a handle with the instance ID from shared memory */
        ServiceHandle handle;
        handle.instanceId = registry->instanceId;  // Copy from shared memory
        
        handles.push_back(handle);
        
        std::cout << "[Proxy] Found service instance in shared memory: " << registry->instanceId << " (Provider PID: " << registry->providerPid << ")" << std::endl;
    } else {
        std::cout << "[Proxy] No service instances found in shared memory" << std::endl;
    }
    
    return handles;
}
/**********************************************************************/
/*! @brief offerService() - Called by the PROVIDER
  * This function does TWO things:
  * 1. Stores the service pointer locally (for method calls within provider process)
  * 2. Writes to SHARED MEMORY so the consumer can discover this service
***********************************************************************/
void MirrorFoldServiceProxy::offerService(IMirrorFoldService* instance)
{
    /* Store locally for direct method calls (only works within same process) */
    s_serviceInstance = instance;
    
    // ========================================================================
    /* SHARED MEMORY: Announce service availability to OTHER processes
    // ========================================================================
    
    /* Get pointer to the shared memory region. Both provider and consumer access the SAME memory through this pointer */
    SharedServiceRegistry* registry = getSharedRegistry();
    
    if (registry == nullptr) {
        std::cerr << "[Proxy] ERROR: Could not access shared memory for offerService!" << std::endl;
        return;
    }
    
    /* Write service information to shared memory
       The consumer process will be able to READ these values */
    registry->serviceAvailable = true;  // "Yes, I'm available!"
    
    /* Copy the instance ID string into the shared memory
       We use strncpy to safely copy into a fixed-size buffer */
    strncpy(
        registry->instanceId,                    // Destination (in shared memory)
        "/sebastianGalbinita/embl/mirror_fold",  // Source string
        sizeof(registry->instanceId) - 1         // Max chars to copy (leave room for null)
    );
    registry->instanceId[sizeof(registry->instanceId) - 1] = '\0';  // Ensure null-terminated
    
    /* Store our process ID for debugging purposes */
    registry->providerPid = getpid();
    
    std::cout << "[Proxy] Service offered and registered in shared memory (PID: " << registry->providerPid << ")" << std::endl;
}
/**********************************************************************/
/*! @brief stopOfferService() - Called by the PROVIDER when shutting down
  * This cleans up by marking the service as unavailable in shared memory
***********************************************************************/
void MirrorFoldServiceProxy::stopOfferService()
{
    s_serviceInstance = nullptr;
    
    SharedServiceRegistry* registry = getSharedRegistry();
    
    if (registry != nullptr) {
        // Mark service as no longer available
        registry->serviceAvailable = false;
        registry->providerPid = 0;
        
        std::cout << "[Proxy] Service stopped and unregistered from shared memory" << std::endl;
    }
}
/************************************************************/
/*! @brief Constructor - connects to a specific service instance
 *  @param handle Service handle from findService()
/************************************************************/
MirrorFoldServiceProxy::MirrorFoldServiceProxy(const ServiceHandle& handle)
    : m_instanceId(handle.instanceId)
{
    std::cout << "[Proxy] Connected to service: " << m_instanceId << std::endl;
}
/************************************************************/
/*! @brief Call requestFold method on remote service
 *  @return Result of the operation
/************************************************************/
bool MirrorFoldServiceProxy::requestFold()
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (!registry || !registry->serviceAvailable) return false;
    
    registry->pendingCommand = SharedServiceRegistry::Command::FOLD;
    registry->commandPending = true;
    
    // Wait briefly for provider to respond
    // (in real ara::com this would be async with a Future)
    for (int i = 0; i < 100 && registry->commandPending; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    return registry->commandResult;
}
/************************************************************/
/*! @brief Call requestUnfold method on remote service
 *  @return Result of the operation
/************************************************************/
bool MirrorFoldServiceProxy::requestUnfold()
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (!registry || !registry->serviceAvailable) return false;
    
    registry->pendingCommand = SharedServiceRegistry::Command::UNFOLD;
    registry->commandPending = true;
    
    // Wait briefly for provider to respond
    // (in real ara::com this would be async with a Future)
    for (int i = 0; i < 100 && registry->commandPending; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    return registry->commandResult;
}
/************************************************************/
/*! @brief Get the current abort reason
 *  @return The current abort reason
************************************************************/
t_emblAbortReason MirrorFoldServiceProxy::getAbortReason() const
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (registry && registry->serviceAvailable)
        return registry->abortReason;
    return ABORT_REASON_NONE;
}
/***********************************************************/
/*! @brief Get the current fold command
 *  @return The current fold command
************************************************************/
t_emblMirrorFoldCmd MirrorFoldServiceProxy::getCurrentCommand() const
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (registry && registry->serviceAvailable)
        return registry->currentCommand;
    return MIRRFLD_COMMAND_IDLE;
}
/************************************************************/
/*! @brief Get the target X position for folding/unfolding
 *  @return Target X position
************************************************************/
uint16_t MirrorFoldServiceProxy::getXTargetPosition() const
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (registry && registry->serviceAvailable)
        return registry->xTargetPos;
    return 0;
}
/************************************************************/
/*! @brief Get the target Y position for folding/unfolding
 *  @return Target Y position
*************************************************************/
uint16_t MirrorFoldServiceProxy::getYTargetPosition() const
{
    SharedServiceRegistry* registry = getSharedRegistry();
    if (registry && registry->serviceAvailable)
        return registry->yTargetPos;
    return 0;
}
/*********************************************************************/
/*! @brief Subscribe to abort reason changes
 *  @param callback Function to call when abort reason changes
*********************************************************************/
void MirrorFoldServiceProxy::subscribeToAbortReasonEvent(AbortReasonCallback callback)
{
    std::lock_guard<std::mutex> lock(m_callbacksMutex);
    if (callback) {
        m_eventCallbacks.push_back(callback);
        std::cout << "[Proxy] Subscribed to abort reason events" << std::endl;
    }
}
} // namespace services
} // namespace emblex