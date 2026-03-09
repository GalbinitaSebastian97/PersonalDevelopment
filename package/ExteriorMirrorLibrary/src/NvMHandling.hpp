#ifndef EMBLCONTROLNVMHANDLER_HPP
#define EMBLCONTROLNVMHANDLER_HPP

/*============================================================================*/
/* Includes */
/*============================================================================*/
#include "Control_Interface.hpp"
#include "InternalTypes.hpp"
#include "mirrorConfiguration.hpp"

#if EMBL_FRAMELESS_MIRROR_SUPPORT

/*============================================================================*/
/* Class Declaration */
/*============================================================================*/

namespace embl {
namespace control {

/**
 * @class EmblControlNvMHandler
 * @brief Handles non-volatile memory operations for EMBL mirror control
 * 
 * This class encapsulates all NvM (Non-Volatile Memory) operations including
 * read, write, status tracking, and timeout management for the EMBL 
 * (Electronic Mirror BackLight) control system.
 */
class EmblControlNvMHandler {
public:
    /**
     * @brief Constructor
     * Initializes the NvM handler with default state
     */
    EmblControlNvMHandler();

    /**
     * @brief Destructor
     */
    ~EmblControlNvMHandler() = default;

    // Disable copy and move operations (Singleton-like behavior)
    EmblControlNvMHandler(const EmblControlNvMHandler&) = delete;
    EmblControlNvMHandler& operator=(const EmblControlNvMHandler&) = delete;
    EmblControlNvMHandler(EmblControlNvMHandler&&) = delete;
    EmblControlNvMHandler& operator=(EmblControlNvMHandler&&) = delete;

    /**
     * @brief Initialize NvM handling and trigger read operation
     * Should be called during system initialization
     */
    void init();

    /**
     * @brief Main task for handling ongoing NvM operations and timeouts
     * Should be called periodically (e.g., every cycle)
     */
    void mainTask();

    /**
     * @brief Trigger a write operation to NvM
     * @return true if write request was accepted, false if NvM is busy
     */
    bool writeBlock();

    /**
     * @brief Get the status of the last write operation
     * @return Current state of write operation
     */
    EMBLC_NVM_REQ_STATE getWriteOperationStatus() const;

    /**
     * @brief Get the status of the last read operation
     * @return Current state of read operation
     */
    EMBLC_NVM_REQ_STATE getReadOperationStatus() const;

    /**
     * @brief Check if NvM has any ongoing operations
     * @return true if NvM is busy (read or write in progress)
     */
    bool isBusy() const;

    /**
     * @brief Get pointer to NvM RAM image
     * @return Pointer to the NvM data structure
     */
    mirrorCtrlNvMData* getNvMRamImageAddress();

    /**
     * @brief Initialize NvM block with default values
     * Called after first SW flash or when defaults are needed
     * @return Standard return type (0 = success)
     */
    Std_ReturnType initBlock();

    /**
     * @brief Callback notification when NvM job is finished
     * @param serviceId The NvM service ID
     * @param jobResult Result of the NvM operation
     */
    void jobFinished(uint8 serviceId, NvM_RequestResultType_cil jobResult);

private:
    /**
     * @brief Start a read operation from NvM
     */
    void startReadOperation();

    /**
     * @brief Handle write timeout scenario
     */
    void handleWriteTimeout();

    /**
     * @brief Handle read timeout scenario
     */
    void handleReadTimeout();

    /**
     * @brief Set default values to NvM data structure
     */
    void setDefaultValues();

    // Member variables
    mirrorCtrlNvM m_nvmData;  ///< NvM data structure containing state and RAM image
};

} // namespace control
} // namespace embl

/*============================================================================*/
/* C Interface for backward compatibility */
/*============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the singleton instance of EmblControlNvMHandler
 * @return Reference to the singleton instance
 */
embl::control::EmblControlNvMHandler& emblControl_getNvMHandlerInstance();

// C-compatible wrapper functions
void emblControl_NvMHandling_Init(void);
void emblControl_NvMHandling_MainTask(void);
bool emblControl_NvMWriteBlock(void);
EMBLC_NVM_REQ_STATE emblControl_getNvMWriteOperationStatus(void);
EMBLC_NVM_REQ_STATE emblControl_getNvMReadOperationStatus(void);
bool emblControl_IsNvMBusy(void);
mirrorCtrlNvMData* emblControl_getNvMRamImageAddress(void);
Std_ReturnType EmblCtrl_InitBlock(void);
void EmblCtrl_JobFinished(uint8 ServiceId, NvM_RequestResultType_cil JobResult);

#ifdef __cplusplus
}
#endif

#endif /* EMBL_FRAMELESS_MIRROR_SUPPORT */

#endif /* EMBLCONTROLNVMHANDLER_HPP */