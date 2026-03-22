#!/bin/bash
# ==============================================================================
# AUTOSAR Adaptive Platform - Container Health Check Script
# ==============================================================================
# This script is called periodically by Docker to verify the container
# is functioning correctly.
#
# Exit codes:
#   0 = healthy
#   1 = unhealthy
# ==============================================================================

# ============================================================================
# HEALTH CHECKS
# ============================================================================

# Check 1: Verify the application process is running
if [ "${APP_ROLE}" = "provider" ]; then
    if ! pgrep -x "MirrorServiceProvider" > /dev/null; then
        echo "UNHEALTHY: MirrorServiceProvider process not running"
        exit 1
    fi
elif [ "${APP_ROLE}" = "consumer" ]; then
    if ! pgrep -x "MirrorServiceConsumer" > /dev/null; then
        echo "UNHEALTHY: MirrorServiceConsumer process not running"
        exit 1
    fi
elif [ "${APP_ROLE}" = "integrated" ]; then
    if ! pgrep -x "MirrorServiceProvider" > /dev/null; then
        echo "UNHEALTHY: MirrorServiceProvider process not running"
        exit 1
    fi
    if ! pgrep -x "MirrorServiceConsumer" > /dev/null; then
        echo "UNHEALTHY: MirrorServiceConsumer process not running"
        exit 1
    fi
fi

# Check 2: For provider, verify shared memory registry exists
if [ "${APP_ROLE}" = "provider" ] || [ "${APP_ROLE}" = "integrated" ]; then
    if [ ! -f "/dev/shm/mirror_fold_service_registry" ]; then
        echo "UNHEALTHY: Shared memory service registry not found"
        exit 1
    fi
fi

# Check 3: Verify log directory is writable
if [ ! -w "${ARA_LOG_PATH}" ]; then
    echo "UNHEALTHY: Log directory not writable"
    exit 1
fi

# All checks passed
echo "HEALTHY: All checks passed"
exit 0

3.3 docker/.dockerignore

# ==============================================================================
# Docker Build Context Ignore File
# ==============================================================================
# Files and directories listed here will NOT be copied to the Docker build
# context, reducing build time and image size.
# ==============================================================================

# Build outputs
dist/
build/
*.o
*.a
*.so
*.exe

# IDE and editor files
.vscode/
.idea/
*.swp
*.swo
*~

# Git
.git/
.gitignore

# Documentation
docs/
*.md
!README.md

# Test data
test/data/

# Logs
*.log
logs/

# Temporary files
tmp/
temp/
*.tmp

# Python cache
__pycache__/
*.pyc
*.pyo
.pytest_cache/

# Docker files (prevent recursive copy)
docker/Dockerfile.*

# CI/CD
.github/
.gitlab-ci.yml
Jenkinsfile

# macOS
.DS_Store

# Windows
Thumbs.db