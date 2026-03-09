#include "NvMHandling.hpp"
#include "InternalTypes.hpp"
#include "mirrorParam.hpp"
#include <cstring>
#if EMBL_FRAMELESS_MIRROR_SUPPORT
namespace embl {
    namespace control {
/*============================================================================*/
/* Constructor */
/*============================================================================*/
EmblControlNvMHandler::EmblControlNvMHandler() : m_nvmData{}
{ 
    // Initialize member variables 
    m_nvmData.NvMReadState = EMBLC_NVM_REQ_IDLE; 
    m_nvmData.NvMWriteState = EMBLC_NVM_REQ_IDLE; 
    m_nvmData.NvMReqTimeout = 0u;
}
/*============================================================================*/
/* Public Methods */
/*============================================================================*/
void EmblControlNvMHandler::init()
{ 
    // Trigger NvM read block start
    startReadOperation();
}
void EmblControlNvMHandler::mainTask()
{ 
    // Decrement timeout if higher than 0 
    if (m_nvmData.NvMReqTimeout > 0u) 
    { 
        m_nvmData.NvMReqTimeout--; 
    } 
    else 
    { 
        // Timeout occurred 
        if (m_nvmData.NvMWriteState == EMBLC_NVM_REQ_ONGOING) 
        { 
            handleWriteTimeout(); 
        } 
        else if (m_nvmData.NvMReadState == EMBLC_NVM_REQ_ONGOING) 
        { 
            handleReadTimeout(); 
        } 
    }
}

bool EmblControlNvMHandler::writeBlock()
{ 
    bool retVal = false; 
    // Do not allow write request if an ongoing request is present 
    if ((m_nvmData.NvMWriteState != EMBLC_NVM_REQ_ONGOING) && (m_nvmData.NvMReadState != EMBLC_NVM_REQ_ONGOING)) 
    { 
        // Allow write of NvM in case NvM is NOK to potentially fix the issue 
        EmblCtrl_WriteBlock(&m_nvmData.emblcNvMData); 
        m_nvmData.NvMWriteState = EMBLC_NVM_REQ_ONGOING; 
        m_nvmData.NvMReqTimeout = EMBL_MM_PARAM(embl_ctrl_NvMResponseTimeout); 
        retVal = true; 
    } 
    return retVal;
}

EMBLC_NVM_REQ_STATE EmblControlNvMHandler::getWriteOperationStatus() const
{ 
    return m_nvmData.NvMWriteState;
}
EMBLC_NVM_REQ_STATE EmblControlNvMHandler::getReadOperationStatus() const{ 
    return m_nvmData.NvMReadState;
}
bool EmblControlNvMHandler::isBusy() const
{ 
    return (m_nvmData.NvMReadState == EMBLC_NVM_REQ_ONGOING) || (m_nvmData.NvMWriteState == EMBLC_NVM_REQ_ONGOING);
}
mirrorCtrlNvMData* EmblControlNvMHandler::getNvMRamImageAddress()
{ 
    return &m_nvmData.emblcNvMData;
}
Std_ReturnType EmblControlNvMHandler::initBlock()
{ 
    setDefaultValues(); return 0u;
}
void EmblControlNvMHandler::jobFinished(uint8 serviceId, NvM_RequestResultType_cil jobResult)
{ 
    // Suppress compiler warnings about unused arguments 
    static_cast<void>(serviceId); 
    // Handle write operation completion 
    if (m_nvmData.NvMWriteState == EMBLC_NVM_REQ_ONGOING) 
    { 
        if (jobResult == NVM_CIL_REQ_OK) 
        { 
            m_nvmData.NvMWriteState = EMBLC_NVM_REQ_SUCCESFULL; 
            m_nvmData.NvMReqTimeout = 0u; 
            // Stop timeout 
        } else 
        { 
            m_nvmData.NvMWriteState = EMBLC_NVM_REQ_FAILED; 
            // Because write job failed, trigger a read block to resync the RAM image with NvM s
            startReadOperation(); 
        } 
    } 
    // Handle read operation completion 
    else 
    { 
        if ((jobResult == NVM_CIL_REQ_OK) || (jobResult == NVM_CIL_REQ_RESTORED_DEFAULTS)) 
        { 
            m_nvmData.NvMReadState = EMBLC_NVM_REQ_SUCCESFULL; 
        } 
        else 
        { 
            m_nvmData.NvMReadState = EMBLC_NVM_REQ_FAILED; 
        } 
        m_nvmData.NvMReqTimeout = 0u; 
        // Stop timeout 
    }
}
/*============================================================================*/
/* Private Methods */
/*============================================================================*/
void EmblControlNvMHandler::startReadOperation()
{ 
    static_cast<void>(EmblCtrl_ReadBlock(&m_nvmData.emblcNvMData));
    m_nvmData.NvMReadState = EMBLC_NVM_REQ_ONGOING;
    m_nvmData.NvMReqTimeout = EMBL_MM_PARAM(embl_ctrl_NvMResponseTimeout);
}
void EmblControlNvMHandler::handleWriteTimeout()
{ 
    m_nvmData.NvMWriteState = EMBLC_NVM_REQ_FAILED; 
    // Something went wrong, restore RAM image with NvM values startReadOperation();
}
void EmblControlNvMHandler::handleReadTimeout()
{ 
    // A read block has failed therefore the NvM RAM Image is not synchronized anymore with NvM 
    m_nvmData.NvMReadState = EMBLC_NVM_REQ_FAILED;
}
void EmblControlNvMHandler::setDefaultValues()
{ 
    // Set default values to EMBL NvM RAM image 
    std::memset(&m_nvmData.emblcNvMData, 0, sizeof(m_nvmData.emblcNvMData));
    m_nvmData.emblcNvMData.LowPositionFold_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessFoldLowerLimitPos); 
    m_nvmData.emblcNvMData.LowPositionX_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessHorizontalLowerLimitPos); 
    m_nvmData.emblcNvMData.HighPositionX_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessHorizontalUpperLimitPos); 
    m_nvmData.emblcNvMData.LowPositionY_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessVerticalLowerLimitPos); 
    m_nvmData.emblcNvMData.HighPositionY_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessVerticalUpperLimitPos); 
    m_nvmData.emblcNvMData.FoldDrivePosX_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosX); 
    m_nvmData.emblcNvMData.FoldDrivePosY_u16 = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosY); 
    m_nvmData.emblcNvMData.FoldPosSaveAllowed_u1 = true;
}
} 
// namespace control
} // namespace embl
/*============================================================================*/
/* C Interface Implementation - Singleton Pattern */
/*============================================================================*/
// Singleton instance
static embl::control::EmblControlNvMHandler* g_nvmHandlerInstance = nullptr;
embl::control::EmblControlNvMHandler& emblControl_getNvMHandlerInstance()
{ 
    if (g_nvmHandlerInstance == nullptr) 
    { 
        static embl::control::EmblControlNvMHandler instance; 
        g_nvmHandlerInstance = &instance; 
    } 
    return *g_nvmHandlerInstance;
}
/*============================================================================*/
/* C Wrapper Functions for Backward Compatibility */
/*============================================================================*/
extern "C" {
    void emblControl_NvMHandling_Init(void)
    {
         emblControl_getNvMHandlerInstance().init();
    }
    void emblControl_NvMHandling_MainTask(void)
    { 
        emblControl_getNvMHandlerInstance().mainTask();
    }
    bool emblControl_NvMWriteBlock(void){ return static_cast<bool>(emblControl_getNvMHandlerInstance().writeBlock());}
    EMBLC_NVM_REQ_STATE emblControl_getNvMWriteOperationStatus(void){ return emblControl_getNvMHandlerInstance().getWriteOperationStatus();}
    EMBLC_NVM_REQ_STATE emblControl_getNvMReadOperationStatus(void){ return emblControl_getNvMHandlerInstance().getReadOperationStatus();}
    bool emblControl_IsNvMBusy(void){ return static_cast<bool>(emblControl_getNvMHandlerInstance().isBusy());}
    mirrorCtrlNvMData* emblControl_getNvMRamImageAddress(void){ return emblControl_getNvMHandlerInstance().getNvMRamImageAddress();}
    Std_ReturnType EmblCtrl_InitBlock(void){ return emblControl_getNvMHandlerInstance().initBlock();}
    void EmblCtrl_JobFinished(uint8 ServiceId, NvM_RequestResultType_cil JobResult){ emblControl_getNvMHandlerInstance().jobFinished(ServiceId, JobResult);}
} // extern "C"

#endif /* EMBL_FRAMELESS_MIRROR_SUPPORT */