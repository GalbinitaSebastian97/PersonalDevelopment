#ifndef MIRROR_FOLD_SERVICE_PROXY_HPP
#define MIRROR_FOLD_SERVICE_PROXY_HPP

#include "IMirrorFoldService.hpp"
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include <iostream>
#include <thread>
/*============================================================================*/
// SHARED MEMORY HEADERS
/*============================================================================*/
/************************************************************/
/*! @brief These are linux system headers that allow us to create memory that
 * that can be shared between different processes. This is a common technique 
 * for simple IPC (Inter-Process Communication).
/************************************************************/
#include <sys/mman.h>   // mmap(), shm_open() - functions to create/access shared memory
#include <sys/stat.h>   // Permission flags (S_IRUSR, S_IWUSR, etc.)
#include <fcntl.h>      // O_CREAT, O_RDWR - flags for opening shared memory
#include <unistd.h>     // ftruncate(), getpid() - functions for setting size and getting process ID
#include <cstring>      // memset(), strcpy() - string operations

namespace emblex {
namespace services {
/*============================================================================*/
// SHARED SERVICE REGISTRY STRUCTURE
/*============================================================================*/
/************************************************************/
/*! @brief This structure will be stored in SHARED MEMORY, meaning both the provider
 * and consumer processes can read and write to it.
 * IMPORTANT: We use simple types (bool, char arrays, integers) instead of
 * pointers or complex objects because:
 * 1. Pointers are only valid within ONE process's memory
 * 2. The shared memory must contain raw data, not references to other memory
/************************************************************/
struct SharedServiceRegistry {
    
    /************************************************************/
    /*! @brief serviceAvailable (1 byte) 
     * Purpose: Flag indicating if a service provider is currently running
     * Set by:  Provider (true when starting, false when stopping)
     * Read by: Consumer (to check if service exists)
    /************************************************************/
    bool serviceAvailable;
    
    /************************************************************/
    /*! @brief instanceId (256 bytes)
     * Purpose: The unique identifier for this service instance
     * Example: "/sebastianGalbinita/embl/mirror_fold"
     *
     * Why char[] instead of std::string?
     * - std::string uses heap memory with internal pointers
     * - Pointers are only valid within ONE process
     * - char[] stores actual bytes directly in shared memory
    /************************************************************/
    char instanceId[256];
    
    /************************************************************/
    /*! @brief providerPid (4 bytes)
     * Purpose: Process ID of the provider (for debugging/verification)
     * Use:     Consumer can verify provider is still running
     *          kill(providerPid, 0) returns 0 if process exists
    /************************************************************/
    int providerPid;
    // --- IPC command channel ---
    enum class Command : uint8_t { NONE, FOLD, UNFOLD, CANCEL };
    Command  pendingCommand;   // consumer writes, provider reads
    bool     commandPending;   // consumer sets true, provider clears after handling
    bool     commandResult;    // provider writes result back
    
    // --- Status fields (provider writes, consumer reads) ---
    t_emblMirrorFoldCmd  currentCommand;
    t_emblAbortReason    abortReason;
    uint16_t             xTargetPos;
    uint16_t             yTargetPos;
};

/************************************************************/
/*! @brief Unique name for the shared memory region
 * Both provider and consumer use this SAME name to access the SAME memory
 * The name appears as a file in /dev/shm/ on Linux systems
/************************************************************/
constexpr const char* SHM_NAME = "/mirror_fold_service_registry";
/**
 * @brief ara::com Proxy - Service Consumer Implementation
 * 
 * This class allows applications to use the MirrorFold service remotely.
 * In a real implementation, this would inherit from ara::com::proxy::ProxyBase
 * 
 * The proxy handles:
 * - Service discovery
 * - Remote method calls
 * - Event subscription
 * - Field access
 */
class MirrorFoldServiceProxy {
public:
    /************************************************************/
    /*! @brief Handle for service instance
    /************************************************************/
    struct ServiceHandle {
        std::string instanceId;
    };
    
    /************************************************************/
    /*! @brief Find available service instances
     *  @param instanceId Instance to search for (empty = find all)
     *  @return Vector of available service handles
    /************************************************************/
    static std::vector<ServiceHandle> findService(const std::string& instanceId = "");
    // ========================================================================
    // SHARED MEMORY HELPER FUNCTION
    // ========================================================================
    /************************************************************/
    /*! @brief This function creates or opens the shared memory region and returns
     *  a pointer to it. Both provider and consumer call this function.
     *
     *  @note How it works:
     *  1. shm_open() creates a "shared memory object" (like a file in /dev/shm/)
     *  2. ftruncate() sets the size of this memory region
     *  3. mmap() maps this memory into our process so we can use it like a normal pointer
    /************************************************************/
    static SharedServiceRegistry* getSharedRegistry() {
        
        /* Step 1: Open (or create) the shared memory object
           -------------------------------------------------
           shm_open() is like opening a file, but for shared memory
           Parameters:
             SHM_NAME: The name of the shared memory (like a filename)
             O_CREAT:  Create it if it doesn't exist
             O_RDWR:   Open for both reading and writing
             0666:     Permission mode (read/write for everyone)
           Returns: A file descriptor (fd) - a number that identifies this shared memory */
        int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        
        if (fd == -1) {
            /* shm_open failed - this is a critical error */
            std::cerr << "[SharedMemory] ERROR: Failed to open shared memory!" << std::endl;
            return nullptr;
        }
        
        /* Step 2: Set the size of the shared memory region
           ------------------------------------------------
           ftruncate() sets how big the shared memory should be
           We need exactly enough space for one SharedServiceRegistry structure */
        ftruncate(fd, sizeof(SharedServiceRegistry));
        
        /* Step 3: Map the shared memory into our process's address space
           --------------------------------------------------------------
           mmap() makes the shared memory accessible via a normal pointer
           Parameters:
            nullptr:                    Let the system choose the address
            sizeof(SharedServiceRegistry): How much memory to map
            PROT_READ | PROT_WRITE:     We want to read AND write
            MAP_SHARED:                 Changes are visible to other processes!
            fd:                         The shared memory file descriptor
            0:                          Start at the beginning

           Returns: A pointer to the shared memory that we can use like any other pointer */
        void* ptr = mmap(
            nullptr,                          // Let OS choose the address
            sizeof(SharedServiceRegistry),    // Size of memory to map
            PROT_READ | PROT_WRITE,           // Read and write access
            MAP_SHARED,                       // KEY: Share with other processes!
            fd,                               // The shared memory descriptor
            0                                 // Offset (start from beginning)
        );
        
        /* We can close the file descriptor now - the mapping stays valid */
        close(fd);
        
        if (ptr == MAP_FAILED) {
            std::cerr << "[SharedMemory] ERROR: Failed to map shared memory!" << std::endl;
            return nullptr;
        }
        
        /* Cast the void pointer to our structure type and return it */
        return static_cast<SharedServiceRegistry*>(ptr);
    }   
    /************************************************************/
    /*! @brief Constructor - connects to a specific service instance
     *  @param handle Service handle from findService()
    /************************************************************/
    explicit MirrorFoldServiceProxy(const ServiceHandle& handle);
    
    ~MirrorFoldServiceProxy() = default;
    
    // Method calls (synchronous - in real ara::com could be async with Future)
    
    /************************************************************/
    /*! @brief Call requestFold method on remote service
     *  @return Result of the operation
    /************************************************************/
    bool requestFold();
    
    /************************************************************/
    /*! @brief Call requestUnfold method on remote service
     *  @return Result of the operation
    /************************************************************/
    bool requestUnfold();
    
    // Field getters (read remote values)
        /********************************************************/
    /*! @brief Get the current abort reason
     * @return The current abort reason
     ************************************************************/
    t_emblAbortReason getAbortReason() const;
    /************************************************************/
    /*! @brief Get the current fold command
     *  @return The current fold command
     ************************************************************/
    t_emblMirrorFoldCmd getCurrentCommand() const;
    /************************************************************/
    /*! @brief Get the target X position for folding/unfolding
     *  @return Target X position
     ************************************************************/
    uint16_t getXTargetPosition() const;
    /************************************************************/
    /*! @brief Get the target Y position for folding/unfolding
     *  @return Target Y position
    *************************************************************/
    uint16_t getYTargetPosition() const;
    /**********************************************************************/
    /*! @brief offerService() - Called by the PROVIDER
     * This function does TWO things:
     * 1. Stores the service pointer locally (for method calls within provider process)
     * 2. Writes to SHARED MEMORY so the consumer can discover this service
    ***********************************************************************/
    static void offerService(IMirrorFoldService* instance);
    /**********************************************************************/
    /*! @brief stopOfferService() - Called by the PROVIDER when shutting down
    * This cleans up by marking the service as unavailable in shared memory
    ***********************************************************************/
    static void stopOfferService();

    using AbortReasonCallback = std::function<void(t_emblAbortReason)>;
    /*********************************************************************/
    /*! @brief Subscribe to abort reason changes
      * @param callback Function to call when abort reason changes
    *********************************************************************/
    void subscribeToAbortReasonEvent(AbortReasonCallback callback);

private:
    std::string m_instanceId;
    
    // We still keep this for the actual method calls within the provider process. The consumer will need a different IPC mechanism for actual method calls
    static IMirrorFoldService* s_serviceInstance;
    
    std::vector<AbortReasonCallback> m_eventCallbacks;
    mutable std::mutex m_callbacksMutex;
    
    friend class MirrorFoldServiceSkeleton;
};

} // namespace services
} // namespace emblex

#endif // MIRROR_FOLD_SERVICE_PROXY_HPP