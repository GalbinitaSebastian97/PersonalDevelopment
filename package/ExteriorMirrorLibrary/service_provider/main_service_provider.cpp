#include "MirrorFoldServicesSkeleton.hpp"
#include "MirrorFoldServiceProxy.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

std::atomic<bool> g_running{true};
void signalHandler(int signal)
{ 
    std::cout << "\n[Provider] Shutdown signal" << std::endl; 
    g_running = false;
}
int main()
{ 
    std::cout << "========================================" << std::endl; 
    std::cout << " Mirror Fold Service Provider" << std::endl; 
    std::cout << " Using Adaptive AUTOSAR Persistency" << std::endl; 
    std::cout << "========================================" << std::endl; 
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler); 
    // Create skeleton 
    std::cout << "[Provider] Creating service..." << std::endl; 
    emblex::services::MirrorFoldServiceSkeleton skeleton("/sebastianGalbinita/embl/mirror_fold"); 
    // Initialize controller (PersistencyManager is initialized automatically via singleton) 
    std::cout << "[Provider] Initializing controller..." << std::endl; 
    skeleton.getController().Init(); 
    // Offer service 
    std::cout << "[Provider] Offering service..." << std::endl; skeleton.offerService(); 
    // Make accessible to proxies 
    emblex::services::MirrorFoldServiceProxy::offerService(&skeleton); 
    std::cout << "[Provider] Service running (Ctrl+C to stop)" << std::endl; 
    std::cout << "========================================\n" << std::endl; 
    // Main loop (10ms cycle) 
    while (g_running) 
    { 
        auto cycleStart = std::chrono::steady_clock::now(); 
        // Execute controller logic 
        skeleton.getController().MainTask(); 
        // Process events 
        skeleton.processCycle(); 
        // Maintain 10ms cycle 
        auto elapsed = std::chrono::steady_clock::now() - cycleStart; 
        auto sleepTime = std::chrono::milliseconds(10) - elapsed; 
        if (sleepTime > std::chrono::milliseconds(0)) { 
            std::this_thread::sleep_for(sleepTime); 
        } 
    } 
    std::cout << "[Provider] Shutting down..." << std::endl; 
    skeleton.stopOfferService(); 
    return 0;
}