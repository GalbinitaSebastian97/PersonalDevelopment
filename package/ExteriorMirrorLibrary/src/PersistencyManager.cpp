#include "PersistencyManager.hpp"
#include <cstring>

namespace emblex {
namespace persistency {

/**********************************************************************/
/** \brief Gets the singleton instance of PersistencyManager.
  * @return Reference to the static instance.
  * @note Patterns that ensures only one instance of PersistencyManager 
  * exists and provides global access to it. The instance is created on first 
  * use and destroyed when the program ends.
  * @warning Copy and assignment are deleted.
***********************************************************************/
PersistencyManager& PersistencyManager::getInstance()
{
    static PersistencyManager instance;
    return instance;
}
/**********************************************************************/
/** \brief Initializes the PersistencyManager with default values.
  * Sets up the NvM cache and loads default position limits and drive 
  * parameters from EMBL configuration macros for frameless mirror control.
  * The default data remains constant while actual data storage can change
  * during runtime.
***********************************************************************/
PersistencyManager::PersistencyManager()
{
    std::memset(&m_state, 0, sizeof(m_state));
    m_state.defaultData.LowPositionFold_u16   = EMBL_MM_PARAM(embl_ctrl_FramelessFoldLowerLimitPos);
    m_state.defaultData.LowPositionX_u16      = EMBL_MM_PARAM(embl_ctrl_FramelessHorizontalLowerLimitPos);
    m_state.defaultData.HighPositionX_u16     = EMBL_MM_PARAM(embl_ctrl_FramelessHorizontalUpperLimitPos);
    m_state.defaultData.LowPositionY_u16      = EMBL_MM_PARAM(embl_ctrl_FramelessVerticalLowerLimitPos);
    m_state.defaultData.HighPositionY_u16     = EMBL_MM_PARAM(embl_ctrl_FramelessVerticalUpperLimitPos);
    m_state.defaultData.FoldDrivePosX_u16     = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosX);
    m_state.defaultData.FoldDrivePosY_u16     = EMBL_MM_PARAM(embl_ctrl_FramelessDrivePosY);
    m_state.defaultData.FoldPosSaveAllowed_u1 = true;
    m_state.defaultSet = true;
    m_state.NvMWriteState = EMBLC_NVM_REQ_IDLE;
    m_state.NvMReadState = EMBLC_NVM_REQ_IDLE;
    /*TBD: Use a parameter for NvM request timeout */
    m_state.NvMReqTimeout = 1000;
}
/**********************************************************************/
/** \brief Main task for managing NvM operations and timeouts.
  * Should be called cyclically in the main scheduler loop to handle NvM 
  * request state transitions and timeouts. If a request is ongoing and the 
  * timeout expires, the request state is set to failed.
***********************************************************************/
void PersistencyManager::mainTask()
{
    if (m_state.NvMReqTimeout > 0)
    {
        m_state.NvMReqTimeout--;
    }
    else
    {
        if (m_state.NvMWriteState == EMBLC_NVM_REQ_ONGOING)
        {
            m_state.NvMWriteState = EMBLC_NVM_REQ_FAILED;
            // Optionally: trigger a read to resync, or notify error
            m_state.NvMReqTimeout = 1000;
        }
        else if (m_state.NvMReadState == EMBLC_NVM_REQ_ONGOING)
        {
            m_state.NvMReadState = EMBLC_NVM_REQ_FAILED;
        }
        else
        {
            // No ongoing request, nothing to do
        }
    }
}
/**********************************************************************/
/** \brief Reads and validates persistent data storage.
  * @details Ensures data validity by initializing from defaults or zeroing if unavailable.
  * @return The current mirror control NVM data structure.
***********************************************************************/
mirrorCtrlNvMData PersistencyManager::readData()
{
    // If data is not yet valid, attempt to load it from defaults
    if (!m_state.dataValid) 
    {
        if (m_state.defaultSet) {
            m_state.dataStorage = m_state.defaultData;
            m_state.dataValid = true;
        }
    }
    return m_state.dataStorage;
}
/**********************************************************************/
/** \brief Sets the default data for persistent storage.
  * @details Updates the default data used when no valid data is available.
  * @note This function should be called outside the class.
***********************************************************************/
void PersistencyManager::setDefaultData(const mirrorCtrlNvMData& defaults)
{
    m_state.defaultData = defaults;
    m_state.defaultSet = true;
}

/**********************************************************************/
/** \brief Writes data to persistent storage if valid and not busy.
  * @param data The mirror control NVM data to be persisted.
  * @details Writes data only if the manager is not currently busy and no valid 
  * data is already stored.
***********************************************************************/
void PersistencyManager::writeData(const mirrorCtrlNvMData& data)
{
    if (!isBusy())
    {
        m_state.dataStorage = data;
        m_state.dataValid = true;
        m_state.NvMWriteState = EMBLC_NVM_REQ_ONGOING;
        /*TBD: Use a parameter for NvM request timeout */
        m_state.NvMReqTimeout = 1000;
    }
}

/**********************************************************************/
/** \brief Checks if the persistency manager is busy with an ongoing operation.
  * @return true if busy, false otherwise.
***********************************************************************/
bool PersistencyManager::isBusy() const
{
    return m_state.NvMReadState == EMBLC_NVM_REQ_ONGOING || m_state.NvMWriteState == EMBLC_NVM_REQ_ONGOING;
}

} // namespace persistency
} // namespace emblex