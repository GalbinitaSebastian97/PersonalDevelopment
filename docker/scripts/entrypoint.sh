#!/bin/bash
# ==============================================================================
# AUTOSAR Adaptive Platform - Container Entrypoint Script
# ==============================================================================
# This script runs when the container starts. It performs setup tasks
# and then executes the main application.
# ==============================================================================

set -e  # Exit on error

# ============================================================================
# LOGGING FUNCTIONS
# ============================================================================

log_info() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [INFO] $1"
}

log_warn() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [WARN] $1" >&2
}

log_error() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [ERROR] $1" >&2
}

# ============================================================================
# ENVIRONMENT SETUP
# ============================================================================

log_info "Starting AUTOSAR Adaptive container..."
log_info "Application: ${APP_NAME:-Unknown}"
log_info "Role: ${APP_ROLE:-Unknown}"

# ─────────────────────────────────────────────────────────────────────────────
# Verify shared memory is available
# ─────────────────────────────────────────────────────────────────────────────

if [ -d "/dev/shm" ]; then
    log_info "Shared memory (/dev/shm) is available"
    
    # Clean up any stale shared memory from previous runs
    if [ -f "/dev/shm/mirror_fold_service_registry" ]; then
        log_warn "Cleaning up stale shared memory registry"
        rm -f /dev/shm/mirror_fold_service_registry
    fi
else
    log_error "Shared memory (/dev/shm) is NOT available!"
    log_error "Please run the container with --shm-size option"
    exit 1
fi

# ─────────────────────────────────────────────────────────────────────────────
# Verify network configuration
# ─────────────────────────────────────────────────────────────────────────────

log_info "Network configuration:"
ip addr show | grep -E "inet |link/ether" | head -10

# Check if multicast is available (required for SOME/IP)
if ip link show | grep -q "MULTICAST"; then
    log_info "Multicast networking is available"
else
    log_warn "Multicast may not be available - SOME/IP service discovery may fail"
fi

# ─────────────────────────────────────────────────────────────────────────────
# Verify manifests are present
# ─────────────────────────────────────────────────────────────────────────────

MANIFEST_COUNT=$(find ${ARA_MANIFESTS_PATH} -name "*.json" 2>/dev/null | wc -l)
log_info "Found ${MANIFEST_COUNT} manifest files in ${ARA_MANIFESTS_PATH}"

if [ "${MANIFEST_COUNT}" -eq 0 ]; then
    log_warn "No manifest files found - application may not function correctly"
fi

# ─────────────────────────────────────────────────────────────────────────────
# Create log directory if needed
# ─────────────────────────────────────────────────────────────────────────────

mkdir -p ${ARA_LOG_PATH}

# ─────────────────────────────────────────────────────────────────────────────
# Wait for dependencies (if consumer)
# ─────────────────────────────────────────────────────────────────────────────

if [ "${APP_ROLE}" = "consumer" ]; then
    log_info "Consumer role detected - waiting for provider to be ready..."
    
    WAIT_TIME=0
    MAX_WAIT=30
    
    while [ ! -f "/dev/shm/mirror_fold_service_registry" ] && [ ${WAIT_TIME} -lt ${MAX_WAIT} ]; do
        log_info "Waiting for service registry... (${WAIT_TIME}/${MAX_WAIT}s)"
        sleep 1
        WAIT_TIME=$((WAIT_TIME + 1))
    done
    
    if [ -f "/dev/shm/mirror_fold_service_registry" ]; then
        log_info "Service registry found - provider is ready"
    else
        log_warn "Service registry not found after ${MAX_WAIT}s - starting anyway"
    fi
fi

# ============================================================================
# EXECUTE APPLICATION
# ============================================================================

log_info "Executing: $@"
log_info "============================================================"

# If no arguments provided, show usage
if [ $# -eq 0 ]; then
    log_info "No command specified. Available applications:"
    ls -la ${ARA_ROOT}/bin/
    exit 0
fi

# Execute the application
# The exec replaces this shell with the application, ensuring signals
# are properly forwarded (important for graceful shutdown)
exec ${ARA_ROOT}/bin/"$@"