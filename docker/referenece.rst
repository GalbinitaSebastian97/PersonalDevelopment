# ============================================================================
# NATIVE BUILDS
# ============================================================================

# Build a specific project
python scripts/run.py MirrorServiceProvider Debug
python scripts/run.py MirrorServiceConsumer Release --ci

# Build all projects (wipes dist/ first)
python scripts/run.py all Debug

# Build with options
python scripts/run.py MirrorServiceProvider Debug --clean --test --verbose

# Clean all artifacts
python scripts/run.py clean

# List available projects
python scripts/run.py list


# ============================================================================
# DOCKER BUILDS
# ============================================================================

# Build and run all services in Docker
python scripts/run.py all Debug --docker

# Build and run in background (detached)
python scripts/run.py all Release --docker --detach

# Build and run specific service
python scripts/run.py MirrorServiceProvider Debug --docker

# Stop Docker containers
python scripts/run.py docker-down

# View Docker logs
python scripts/run.py docker-logs
python scripts/run.py docker-logs --follow
python scripts/run.py docker-logs --service mirror-provider --follow

# Check container status
python scripts/run.py docker-status


# ============================================================================
# DIRECT DOCKER COMPOSE (Alternative)
# ============================================================================

# Build and run from docker directory
cd docker
BUILD_TYPE=Debug docker compose up --build

# Run integrated mode (both provider + consumer in one container)
BUILD_TYPE=Debug docker compose --profile integrated up --build mirror-integrated

# View logs
docker compose logs -f mirror-provider

# Stop all
docker compose down

# ============================================================================
# DIRECT DOCKER COMPOSE (Alternative)
# ============================================================================
# 1. Verify Base Image
#   Check the base image is correctly built and ready to use -> Image is showed with the latest tag
docker image ls ara-base
# 2. Build Application Images
docker build -f docker/Dockerfile.provider -t mirror-provider:debug --build-arg BUILD_TYPE=Debug .
docker build -f docker/Dockerfile.consumer -t mirror-consumer:debug --build-arg BUILD_TYPE=Debug .
# docker-compose can be used to build all services
cd docker
docker compose build
# 3. Run the Application Containers
cd docker
cocker compose up
# Or
docker run --name mirror-provider --shm-size=64m mirror-provider:debug
# 4. Using the Extended run.py Script
# Extended run.py script wiwith the --docker flag can be also used
python scripts/run.py all Debug --docker