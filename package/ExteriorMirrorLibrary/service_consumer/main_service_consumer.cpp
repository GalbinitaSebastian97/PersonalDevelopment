#include "MirrorFoldServiceProxy.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

std::atomic<bool> g_running{true};

void signalHandler(int signal)
{
    std::cout << "\n[ServiceConsumer] Shutdown signal received" << std::endl;
    g_running = false;
}

int main(int argc, char** argv)
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Mirror Fold Service Consumer" << std::endl;
    std::cout << "  Test Application" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    try {
        // Step 1: Find service
        std::cout << "[ServiceConsumer] Searching for service..." << std::endl;
        
        auto handles = emblex::services::MirrorFoldServiceProxy::findService("/sebastianGalbinita/embl/mirror_fold");
        
        if (handles.empty()) {
            std::cerr << "[ServiceConsumer] No service found! Make sure service provider is running." << std::endl;
            return 1;
        }
        
        // Step 2: Create proxy
        std::cout << "[ServiceConsumer] Creating proxy..." << std::endl;
        emblex::services::MirrorFoldServiceProxy proxy(handles[0]);
        
        // Step 3: Subscribe to events
        std::cout << "[ServiceConsumer] Subscribing to events..." << std::endl;
        proxy.subscribeToAbortReasonEvent([](t_emblAbortReason reason) {
            std::cout << "[ServiceConsumer] *** Event: Abort reason changed to " << static_cast<int>(reason) << " ***" << std::endl;
        });
        
        std::cout << "========================================" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  Will automatically test fold/unfold" << std::endl;
        std::cout << "  Press Ctrl+C to stop" << std::endl;
        std::cout << "========================================" << std::endl;
        
        // Step 4: Test sequence
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Test 1: Request Fold
        std::cout << "\n[ServiceConsumer] TEST 1: Requesting FOLD..." << std::endl;
        bool result = proxy.requestFold();
        std::cout << "[ServiceConsumer] Fold request result: " << (result ? "Accepted" : "Rejected") << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Read status
        std::cout << "\n[ServiceConsumer] Reading status..." << std::endl;
        auto cmd = proxy.getCurrentCommand();
        auto xPos = proxy.getXTargetPosition();
        auto yPos = proxy.getYTargetPosition();
        auto abortReason = proxy.getAbortReason();
        
        std::cout << "  Current Command: " << static_cast<int>(cmd) << std::endl;
        std::cout << "  X Target Position: " << xPos << std::endl;
        std::cout << "  Y Target Position: " << yPos << std::endl;
        std::cout << "  Abort Reason: " << static_cast<int>(abortReason) << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Test 2: Request Unfold
        std::cout << "\n[ServiceConsumer] TEST 2: Requesting UNFOLD..." << std::endl;
        /*I need a feedback to request another fold, currently it fails due to the fact that
        the command is still on*/
        result = proxy.requestUnfold();
        std::cout << "[ServiceConsumer] Unfold request result: " << (result ? "Accepted" : "Rejected") << std::endl;
        
        // Keep running to receive events
        std::cout << "\n[ServiceConsumer] Monitoring events... (Ctrl+C to exit)" << std::endl;
        while (g_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "[ServiceConsumer] Shutdown complete." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "[ServiceConsumer] Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}