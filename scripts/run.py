"""
================================================================================
AUTOSAR Adaptive Platform - Build Orchestrator
================================================================================
Usage: python run.py <ProjectName> <BuildType> [options]

Examples:
    python run.py MirrorServiceProvider Debug
    python run.py MirrorServiceConsumer Release --ci
    python run.py all Debug                    # Build all apps
    python run.py all Debug --docker           # Build and run in Docker
    python run.py clean                        # Clean all build artifacts
    python run.py docker-down                  # Stop Docker containers

Author: Exterior Mirror Development Team
================================================================================
"""

import sys
import os
import subprocess
import shutil
import argparse
import json
from pathlib import Path
from datetime import datetime
from enum import Enum
from typing import List, Optional, Dict

# ============================================================================
# CONFIGURATION
# ============================================================================

class BuildType(Enum):
    DEBUG = "Debug"
    RELEASE = "Release"
    RELWITHDEBINFO = "RelWithDebInfo"

class Colors:
    """ANSI color codes for terminal output"""
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    CYAN = '\033[96m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'

# Project root directory (where this script lives)
PROJECT_ROOT = Path(__file__).parent.parent.resolve()

# Directory structure
APPS_DIR = PROJECT_ROOT / "apps/variant"
LIBS_DIR = PROJECT_ROOT / "package"
DIST_DIR = PROJECT_ROOT / "dist"
IDL_DIR = PROJECT_ROOT / "idl"
CONFIG_DIR = PROJECT_ROOT / "config"
DOCKER_DIR = PROJECT_ROOT / "docker"

# Docker Compose file location
DOCKER_COMPOSE_FILE = DOCKER_DIR / "docker-compose.yml"


# ============================================================================
# UTILITY FUNCTIONS
# ============================================================================

def get_available_apps() -> List[str]:
    """Discover all available applications from apps/ directory"""
    if not APPS_DIR.exists():
        return []
    return [d.name for d in APPS_DIR.iterdir() if d.is_dir() and (d / "CMakeLists.txt").exists()]

def get_available_libs() -> List[str]:
    """Discover all available libraries from libs/ directory"""
    if not LIBS_DIR.exists():
        return []
    return [d.name for d in LIBS_DIR.iterdir() if d.is_dir() and (d / "CMakeLists.txt").exists()]


# ============================================================================
# LOGGING UTILITIES
# ============================================================================

def log_header(message: str):
    """Print a header message"""
    print(f"\n{Colors.HEADER}{Colors.BOLD}{'='*80}{Colors.ENDC}")
    print(f"{Colors.HEADER}{Colors.BOLD}  {message}{Colors.ENDC}")
    print(f"{Colors.HEADER}{Colors.BOLD}{'='*80}{Colors.ENDC}\n")

def log_step(step: int, message: str):
    """Print a step message"""
    print(f"{Colors.BLUE}[STEP {step}]{Colors.ENDC} {message}")

def log_success(message: str):
    """Print a success message"""
    print(f"{Colors.GREEN}[SUCCESS]{Colors.ENDC} {message}")

def log_warning(message: str):
    """Print a warning message"""
    print(f"{Colors.YELLOW}[WARNING]{Colors.ENDC} {message}")

def log_error(message: str):
    """Print an error message"""
    print(f"{Colors.RED}[ERROR]{Colors.ENDC} {message}")

def log_info(message: str):
    """Print an info message"""
    print(f"[INFO] {message}")

def log_docker(message: str):
    """Print a Docker-specific message"""
    print(f"{Colors.CYAN}[DOCKER]{Colors.ENDC} {message}")


# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

class BuildConfig:
    """Build configuration container"""
    
    def __init__(
        self,
        project_name: str,
        build_type: BuildType,
        ci_mode: bool = False,
        clean_build: bool = False,
        run_tests: bool = False,
        generate_idl: bool = False,
        verbose: bool = False,
        docker_mode: bool = False,
        docker_service: Optional[str] = None
    ):
        self.project_name = project_name
        self.build_type = build_type
        self.ci_mode = ci_mode
        self.clean_build = clean_build
        self.run_tests = run_tests
        self.generate_idl = generate_idl
        self.verbose = verbose
        self.docker_mode = docker_mode
        self.docker_service = docker_service
        
        # Derived paths
        self.source_dir = APPS_DIR / project_name
        self.build_dir = DIST_DIR / project_name / build_type.value / "build"
        self.output_dir = DIST_DIR / project_name / build_type.value
        self.bin_dir = self.output_dir / "bin"
        self.lib_dir = self.output_dir / "lib"
        self.manifest_dir = self.output_dir / "manifests"
    
    def validate(self) -> bool:
        """Validate the build configuration"""
        if self.docker_mode:
            return True  # Docker handles validation
            
        if not self.source_dir.exists():
            log_error(f"Project source directory not found: {self.source_dir}")
            log_info(f"Available projects: {', '.join(get_available_apps())}")
            return False
        
        if not (self.source_dir / "CMakeLists.txt").exists():
            log_error(f"CMakeLists.txt not found in: {self.source_dir}")
            return False
        
        return True
    
    def __str__(self) -> str:
        return f"""
Build Configuration:
  Project:     {self.project_name}
  Build Type:  {self.build_type.value}
  CI Mode:     {self.ci_mode}
  Clean Build: {self.clean_build}
  Docker Mode: {self.docker_mode}
  Source Dir:  {self.source_dir}
  Output Dir:  {self.output_dir}
"""


# ============================================================================
# DOCKER MANAGER
# ============================================================================

class DockerManager:
    """
    Docker Build and Orchestration Manager
    ============================================================================
    Handles all Docker-related operations including:
    - Building base and application images
    - Running containers via docker-compose
    - Managing container lifecycle
    """
    
    def __init__(self, build_type: BuildType, ci_mode: bool = False, verbose: bool = False):
        self.build_type = build_type
        self.ci_mode = ci_mode
        self.verbose = verbose
        self.compose_file = DOCKER_COMPOSE_FILE
    
    def _run_command(
        self,
        command: List[str],
        cwd: Optional[Path] = None,
        env: Optional[Dict] = None,
        capture_output: bool = False
    ) -> subprocess.CompletedProcess:
        """Execute a command"""
        if self.verbose:
            log_docker(f"Running: {' '.join(command)}")
        
        run_env = os.environ.copy()
        if env:
            run_env.update(env)
        
        return subprocess.run(
            command,
            cwd=cwd or PROJECT_ROOT,
            env=run_env,
            capture_output=capture_output,
            text=True
        )
    
    def _check_docker_installed(self) -> bool:
        """Verify Docker and Docker Compose are installed"""
        # Check Docker
        result = self._run_command(["docker", "--version"], capture_output=True)
        if result.returncode != 0:
            log_error("Docker is not installed or not in PATH")
            return False
        log_docker(f"Found: {result.stdout.strip()}")
        
        # Check Docker Compose
        result = self._run_command(["docker", "compose", "version"], capture_output=True)
        if result.returncode != 0:
            # Try legacy docker-compose
            result = self._run_command(["docker-compose", "--version"], capture_output=True)
            if result.returncode != 0:
                log_error("Docker Compose is not installed")
                return False
        log_docker(f"Found: {result.stdout.strip()}")
        
        return True
    
    def _get_compose_command(self) -> List[str]:
        """Get the correct docker-compose command"""
        # Try modern 'docker compose' first
        result = self._run_command(["docker", "compose", "version"], capture_output=True)
        if result.returncode == 0:
            return ["docker", "compose"]
        return ["docker-compose"]
    
    def build_base_image(self) -> bool:
        """Build the base image with all build tools"""
        log_docker("Building base image (ara-base)...")
        
        command = [
            "docker", "build",
            "-f", str(DOCKER_DIR / "Dockerfile.base"),
            "-t", "ara-base:latest",
            str(PROJECT_ROOT)
        ]
        
        if self.verbose:
            log_docker(f"Command: {' '.join(command)}")
        
        result = self._run_command(command)
        
        if result.returncode != 0:
            log_error("Failed to build base image")
            return False
        
        log_success("Base image built successfully")
        return True
    
    def build_and_run(self, service: Optional[str] = None, detach: bool = False) -> bool:
        """Build images and start containers using docker-compose"""
        
        log_header("Docker Build and Run")
        
        # Check prerequisites
        if not self._check_docker_installed():
            return False
        
        if not self.compose_file.exists():
            log_error(f"Docker Compose file not found: {self.compose_file}")
            return False
        
        # Build base image first
        if not self.build_base_image():
            return False
        
        # Prepare environment variables for docker-compose
        env = {
            "BUILD_TYPE": self.build_type.value,
            "CI_BUILD": "ON" if self.ci_mode else "OFF",
            "LOG_LEVEL": "debug" if self.verbose else "info"
        }
        
        # Get compose command
        compose_cmd = self._get_compose_command()
        
        # Build command
        command = compose_cmd + [
            "-f", str(self.compose_file),
            "up",
            "--build"
        ]
        
        if detach:
            command.append("-d")
        
        if service:
            command.append(service)
        
        log_docker(f"Starting containers with BUILD_TYPE={self.build_type.value}")
        
        result = self._run_command(command, env=env)
        
        if result.returncode != 0:
            log_error("Docker Compose failed")
            return False
        
        log_success("Docker containers started successfully")
        return True
    
    def stop(self) -> bool:
        """Stop all running containers"""
        log_docker("Stopping containers...")
        
        compose_cmd = self._get_compose_command()
        command = compose_cmd + ["-f", str(self.compose_file), "down"]
        
        result = self._run_command(command)
        
        if result.returncode != 0:
            log_error("Failed to stop containers")
            return False
        
        log_success("Containers stopped")
        return True
    
    def logs(self, service: Optional[str] = None, follow: bool = False) -> bool:
        """Show container logs"""
        compose_cmd = self._get_compose_command()
        command = compose_cmd + ["-f", str(self.compose_file), "logs"]
        
        if follow:
            command.append("-f")
        
        if service:
            command.append(service)
        
        result = self._run_command(command)
        return result.returncode == 0
    
    def status(self) -> bool:
        """Show container status"""
        compose_cmd = self._get_compose_command()
        command = compose_cmd + ["-f", str(self.compose_file), "ps"]
        
        result = self._run_command(command)
        return result.returncode == 0


# ============================================================================
# BUILD SYSTEM (Native)
# ============================================================================

class BuildSystem:
    """Main Build System Orchestrator for native builds"""
    
    def __init__(self, config: BuildConfig):
        self.config = config
        self.start_time = None
        self.cmake_generator = self._detect_generator()
    
    def _detect_generator(self) -> str:
        """Detect the best available CMake generator"""
        if shutil.which("ninja"):
            return "Ninja"
        if shutil.which("make"):
            return "Unix Makefiles"
        if sys.platform == "win32":
            return "Visual Studio 18 2026"
        return "Unix Makefiles"
    
    def _run_command(
        self,
        command: List[str],
        cwd: Optional[Path] = None,
        env: Optional[Dict] = None
    ) -> bool:
        """Execute a command and handle output"""
        if self.config.verbose:
            log_info(f"Running: {' '.join(command)}")
        
        try:
            run_env = os.environ.copy()
            if env:
                run_env.update(env)
            
            result = subprocess.run(
                command,
                cwd=cwd or PROJECT_ROOT,
                env=run_env,
                capture_output=not self.config.verbose,
                text=True
            )
            
            if result.returncode != 0:
                if not self.config.verbose:
                    print(result.stdout)
                    print(result.stderr)
                return False
            
            return True
            
        except FileNotFoundError as e:
            log_error(f"Command not found: {command[0]}")
            return False
        except Exception as e:
            log_error(f"Command failed: {e}")
            return False
    
    def setup_directories(self) -> bool:
        """Create output directory structure"""
        log_step(1, "Setting up directory structure")
        
        try:
            if self.config.clean_build and self.config.output_dir.exists():
                log_info(f"Cleaning: {self.config.output_dir}")
                shutil.rmtree(self.config.output_dir)
            
            directories = [
                self.config.build_dir,
                self.config.bin_dir,
                self.config.lib_dir,
                self.config.manifest_dir
            ]
            
            for directory in directories:
                directory.mkdir(parents=True, exist_ok=True)
                log_info(f"Created: {directory.relative_to(PROJECT_ROOT)}")
            
            log_success("Directory structure ready")
            return True
            
        except Exception as e:
            log_error(f"Failed to setup directories: {e}")
            return False
    
    def generate_idl_code(self) -> bool:
        """Generate proxy/skeleton code from ARXML/FIDL definitions"""
        if not self.config.generate_idl:
            log_step(2, "Skipping IDL code generation (not requested)")
            return True
        
        log_step(2, "Generating proxy/skeleton code from IDL")
        log_success("IDL code generation complete")
        return True
    
    def configure_cmake(self) -> bool:
        """Run CMake configuration"""
        log_step(3, f"Configuring CMake ({self.cmake_generator})")
        
        cmake_command = [
            "cmake",
            "-S", str(PROJECT_ROOT),
            "-B", str(self.config.build_dir),
            f"-DCMAKE_BUILD_TYPE={self.config.build_type.value}",
            f"-DTARGET_APP={self.config.project_name}",
            f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={self.config.bin_dir}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={self.config.lib_dir}",
            f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={self.config.lib_dir}",
        ]
        
        if self.cmake_generator:
            cmake_command.extend(["-G", self.cmake_generator])
        
        if self.config.ci_mode:
            cmake_command.extend([
                "-DCI_BUILD=ON",
                "-DENABLE_COVERAGE=ON",
                "-DWARNINGS_AS_ERRORS=ON"
            ])
        
        cmake_command.append("-DENABLE_SHARED_MEMORY_IPC=ON")
        
        if not self._run_command(cmake_command):
            log_error("CMake configuration failed")
            return False
        
        log_success("CMake configuration complete")
        return True
    
    def build(self) -> bool:
        """Run the actual build"""
        log_step(4, "Building project")
        
        import multiprocessing
        jobs = multiprocessing.cpu_count()
        
        build_command = [
            "cmake",
            "--build", str(self.config.build_dir),
            "--config", self.config.build_type.value,
            "--parallel", str(jobs)
        ]
        
        if self.config.verbose:
            build_command.append("--verbose")
        
        if not self._run_command(build_command):
            log_error("Build failed")
            return False
        
        log_success(f"Build complete (using {jobs} parallel jobs)")
        return True
    
    def run_tests(self) -> bool:
        """Run unit and integration tests"""
        if not self.config.run_tests:
            log_step(5, "Skipping tests (not requested)")
            return True
        
        log_step(5, "Running tests")
        
        test_command = [
            "ctest",
            "--test-dir", str(self.config.build_dir),
            "--build-config", self.config.build_type.value,
            "--output-on-failure"
        ]
        
        if not self._run_command(test_command):
            log_error("Tests failed")
            return False
        
        log_success("All tests passed")
        return True
    
    def deploy_manifests(self) -> bool:
        """Copy manifest files to output directory"""
        log_step(6, "Deploying manifests")
        
        source_manifest_dir = self.config.source_dir / "manifests"
        
        if not source_manifest_dir.exists():
            log_warning(f"No manifests directory found: {source_manifest_dir}")
            return True
        
        try:
            manifest_files = list(source_manifest_dir.glob("*.json"))
            manifest_files.extend(source_manifest_dir.glob("*.xml"))
            
            for manifest in manifest_files:
                dest = self.config.manifest_dir / manifest.name
                shutil.copy2(manifest, dest)
                log_info(f"Deployed: {manifest.name}")
            
            log_success(f"Deployed {len(manifest_files)} manifest files")
            return True
            
        except Exception as e:
            log_error(f"Failed to deploy manifests: {e}")
            return False
    
    def generate_build_info(self) -> bool:
        """Generate build information file"""
        log_step(7, "Generating build info")
        
        build_info = {
            "project": self.config.project_name,
            "build_type": self.config.build_type.value,
            "ci_build": self.config.ci_mode,
            "timestamp": datetime.now().isoformat(),
            "generator": self.cmake_generator,
            "paths": {
                "binaries": str(self.config.bin_dir),
                "libraries": str(self.config.lib_dir),
                "manifests": str(self.config.manifest_dir)
            }
        }
        
        try:
            info_file = self.config.output_dir / "build_info.json"
            with open(info_file, 'w') as f:
                json.dump(build_info, f, indent=2)
            
            log_success(f"Build info written to: {info_file.name}")
            return True
            
        except Exception as e:
            log_error(f"Failed to write build info: {e}")
            return False
    
    def execute(self) -> bool:
        """Execute the complete build pipeline"""
        self.start_time = datetime.now()
        
        log_header(f"Building: {self.config.project_name} ({self.config.build_type.value})")
        print(self.config)
        
        steps = [
            self.setup_directories,
            self.generate_idl_code,
            self.configure_cmake,
            self.build,
            self.run_tests,
            self.deploy_manifests,
            self.generate_build_info
        ]
        
        for step in steps:
            if not step():
                log_error("Build pipeline failed!")
                return False
        
        elapsed = datetime.now() - self.start_time
        
        log_header("BUILD SUCCESSFUL")
        print(f"  Output Directory: {self.config.output_dir}")
        print(f"  Elapsed Time:     {elapsed.total_seconds():.2f} seconds")
        print()
        
        return True


# ============================================================================
# SPECIAL COMMANDS
# ============================================================================

def wipe_dist_folder():
    """Remove the dist folder completely"""
    log_header("Wiping dist folder")
    
    if DIST_DIR.exists():
        shutil.rmtree(DIST_DIR)
        log_success(f"Removed: {DIST_DIR}")
    else:
        log_info("dist folder does not exist. Nothing to wipe.")
    
    return True

def clean_all():
    """Remove all build artifacts"""
    log_header("Cleaning all build artifacts")
    
    if DIST_DIR.exists():
        shutil.rmtree(DIST_DIR)
        log_success(f"Removed: {DIST_DIR}")
    else:
        log_info("Nothing to clean")
    
    return True

def build_all(build_type: BuildType, ci_mode: bool, docker_mode: bool = False, verbose: bool = False):
    """Build all available applications"""
    
    # Always wipe dist folder before building all
    wipe_dist_folder()
    
    # If Docker mode, use Docker Compose
    if docker_mode:
        docker_manager = DockerManager(build_type, ci_mode, verbose)
        return docker_manager.build_and_run()
    
    # Native build
    apps = get_available_apps()
    
    if not apps:
        log_error("No applications found in apps/ directory")
        return False
    
    log_header(f"Building all applications ({len(apps)} found)")
    
    success_count = 0
    failed = []
    
    for app in apps:
        config = BuildConfig(
            project_name=app,
            build_type=build_type,
            ci_mode=ci_mode,
            verbose=verbose
        )
        
        if config.validate():
            builder = BuildSystem(config)
            if builder.execute():
                success_count += 1
            else:
                failed.append(app)
        else:
            failed.append(app)
    
    log_header("BUILD ALL SUMMARY")
    print(f"  Successful: {success_count}/{len(apps)}")
    if failed:
        print(f"  Failed:     {', '.join(failed)}")
    
    return len(failed) == 0

def list_projects():
    """List all available projects"""
    log_header("Available Projects")
    
    print("Applications:")
    for app in get_available_apps():
        print(f"  - {app}")
    
    print("\nLibraries:")
    for lib in get_available_libs():
        print(f"  - {lib}")

def docker_down(verbose: bool = False):
    """Stop Docker containers"""
    docker_manager = DockerManager(BuildType.DEBUG, False, verbose)
    return docker_manager.stop()

def docker_logs(service: Optional[str] = None, follow: bool = False, verbose: bool = False):
    """Show Docker container logs"""
    docker_manager = DockerManager(BuildType.DEBUG, False, verbose)
    return docker_manager.logs(service, follow)

def docker_status(verbose: bool = False):
    """Show Docker container status"""
    docker_manager = DockerManager(BuildType.DEBUG, False, verbose)
    return docker_manager.status()


# ============================================================================
# ARGUMENT PARSING & MAIN
# ============================================================================

def parse_arguments():
    """Parse command line arguments"""
    parser = argparse.ArgumentParser(
        description="AUTOSAR Adaptive Platform Build Orchestrator",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python run.py MirrorServiceProvider Debug
  python run.py MirrorServiceConsumer Release --ci
  python run.py all Debug
  python run.py all Debug --docker              # Build and run in Docker
  python run.py all Release --docker --detach   # Run Docker in background
  python run.py docker-down                     # Stop Docker containers
  python run.py docker-logs --follow            # Follow Docker logs
  python run.py docker-status                   # Show container status
  python run.py clean
  python run.py list
        """
    )
    
    parser.add_argument(
        "project",
        help="Project name, 'all', 'clean', 'list', 'docker-down', 'docker-logs', 'docker-status'"
    )
    
    parser.add_argument(
        "build_type",
        nargs="?",
        default="Debug",
        choices=["Debug", "Release", "RelWithDebInfo"],
        help="Build type (default: Debug)"
    )
    
    parser.add_argument(
        "--ci", "-c",
        action="store_true",
        dest="ci_mode",
        help="Enable CI mode (coverage, warnings as errors)"
    )
    
    parser.add_argument(
        "--clean", "-C",
        action="store_true",
        help="Clean build (remove existing artifacts first)"
    )
    
    parser.add_argument(
        "--test", "-t",
        action="store_true",
        help="Run tests after build"
    )
    
    parser.add_argument(
        "--generate-idl", "-g",
        action="store_true",
        help="Generate proxy/skeleton code from IDL"
    )
    
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Verbose output"
    )
    
    # ─────────────────────────────────────────────────────────────────────────
    # Docker-specific arguments
    # ─────────────────────────────────────────────────────────────────────────
    
    parser.add_argument(
        "--docker", "-d",
        action="store_true",
        help="Build and run inside Docker containers"
    )
    
    parser.add_argument(
        "--detach",
        action="store_true",
        help="Run Docker containers in background (detached mode)"
    )
    
    parser.add_argument(
        "--follow", "-f",
        action="store_true",
        help="Follow Docker logs (used with docker-logs)"
    )
    
    parser.add_argument(
        "--service", "-s",
        type=str,
        default=None,
        help="Specific Docker service to target (mirror-provider, mirror-consumer, mirror-integrated)"
    )
    
    # Legacy support
    parser.add_argument(
        "legacy_ci",
        nargs="?",
        default=None,
        help=argparse.SUPPRESS
    )
    
    return parser.parse_args()


def main():
    """Main entry point"""
    args = parse_arguments()
    
    # Handle legacy "buildCI" positional argument
    ci_mode = args.ci_mode or (args.legacy_ci and args.legacy_ci.lower() == "buildci")
    
    # ─────────────────────────────────────────────────────────────────────────
    # Special commands
    # ─────────────────────────────────────────────────────────────────────────
    
    if args.project.lower() == "clean":
        sys.exit(0 if clean_all() else 1)
    
    if args.project.lower() == "list":
        list_projects()
        sys.exit(0)
    
    if args.project.lower() == "docker-down":
        sys.exit(0 if docker_down(args.verbose) else 1)
    
    if args.project.lower() == "docker-logs":
        sys.exit(0 if docker_logs(args.service, args.follow, args.verbose) else 1)
    
    if args.project.lower() == "docker-status":
        sys.exit(0 if docker_status(args.verbose) else 1)
    
    # ─────────────────────────────────────────────────────────────────────────
    # Build all
    # ─────────────────────────────────────────────────────────────────────────
    
    if args.project.lower() == "all":
        build_type = BuildType(args.build_type)
        sys.exit(0 if build_all(build_type, ci_mode, args.docker, args.verbose) else 1)
    
    # ─────────────────────────────────────────────────────────────────────────
    # Build specific project
    # ─────────────────────────────────────────────────────────────────────────
    
    # If Docker mode for specific project
    if args.docker:
        docker_manager = DockerManager(
            BuildType(args.build_type),
            ci_mode,
            args.verbose
        )
        
        # Map project name to Docker service
        service_map = {
            "MirrorServiceProvider": "mirror-provider",
            "MirrorServiceConsumer": "mirror-consumer",
            "MirrorAppIntegrated": "mirror-integrated"
        }
        
        service = service_map.get(args.project, args.project)
        sys.exit(0 if docker_manager.build_and_run(service, args.detach) else 1)
    
    # Native build
    config = BuildConfig(
        project_name=args.project,
        build_type=BuildType(args.build_type),
        ci_mode=ci_mode,
        clean_build=args.clean,
        run_tests=args.test,
        generate_idl=args.generate_idl,
        verbose=args.verbose
    )
    
    if not config.validate():
        sys.exit(1)
    
    builder = BuildSystem(config)
    sys.exit(0 if builder.execute() else 1)


if __name__ == "__main__":
    main()
