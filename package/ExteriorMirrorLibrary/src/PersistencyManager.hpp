/* Prevent file from being included multiple times in a single compilation*/
#ifndef PERSISTENCY_MANAGER_HPP
#define PERSISTENCY_MANAGER_HPP

#include <memory> /* For smart pointers*/
#include <string> 
#include <optional> /* For optional values */
#include "InternalTypes.hpp"
#include "mirrorParam.hpp"
#include <iostream>

struct NvMState {
    /* Separate storage for default and actual data -> actual data can change during runtime */
    mirrorCtrlNvMData dataStorage;
    mirrorCtrlNvMData defaultData;
    bool dataValid{false};
    bool defaultSet{false};
    /** Holds the current state of the EMBLA NvM write request */
    EMBLC_NVM_REQ_STATE NvMWriteState;
    /** Holds the current state of the EMBLA NvM read request */
    EMBLC_NVM_REQ_STATE NvMReadState;
    /** Timeout for NvM requests */
    uint16 NvMReqTimeout;

};

/* Avoid name conflicts in the future*/
namespace emblex {
namespace persistency {

/**
 * @brief Manages persistent storage using Adaptive AUTOSAR ara::per concepts
 * 
 * In real Adaptive AUTOSAR, this would use ara::per::KeyValueStorage
 * For now, this is a simplified implementation following ara::per patterns
 */
class PersistencyManager {
public:
    /* Tell the compiler that you need the memory address of the one and only instance that already exists
    If & was not used than C++ would create an a new "clone". In the end you would end up with dozens of it, not optimal at all*/
    static PersistencyManager& getInstance();
    
    /* Don t clone 2 objects of PersistencyManager */
    PersistencyManager(const PersistencyManager&) = delete;
    /* If you overwrite an object that already exists -> delete it*/
    PersistencyManager& operator=(const PersistencyManager&) = delete;
    
    /**
     * @brief Main task to be called cyclically for managing NvM operations
     * @details Handles timeouts and state transitions for ongoing NvM read/write requests.
     * Should be called in the main scheduler loop.
     */
    void mainTask();
    /**
     * @brief Read persistent data (returns valid data, initializes defaults if needed)
     * @return mirrorCtrlNvMData (always valid)
     */
    mirrorCtrlNvMData readData();

    /**
     * @brief Write persistent data
     * @param data Data to persist
     * @return true if successful, false otherwise
     */
    void writeData(const mirrorCtrlNvMData& data);

    /**
     * @brief Check if persistency operations are pending
     * @return true if busy, false otherwise
     */
    bool isBusy() const;

    /**
     * @brief Set default initialization logic for NvM data
     * @param defaults The default data to use if no data is present
     */
    void setDefaultData(const mirrorCtrlNvMData& defaults);

private:
    PersistencyManager();
    ~PersistencyManager() = default;

    NvMState m_state;
};

} // namespace persistency
} // namespace emblex

#endif // PERSISTENCY_MANAGER_HPP