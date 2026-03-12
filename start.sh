#!/bin/bash

# 1. Start the Provider in the background
./build/MirrorFoldServiceProvider &

# 2. Wait a moment for the 's_serviceInstance' pointer to be set
sleep 2

# 3. Start the Consumer in the foreground
./build/MirrorFoldServiceConsumer