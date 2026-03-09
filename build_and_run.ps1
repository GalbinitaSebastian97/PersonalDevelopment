# 1. Build the Docker Image
Write-Host "--- Building Docker Image ---" -ForegroundColor Cyan
docker build -t frameless-fold-poc .

if ($LASTEXITCODE -ne 0) { 
    Write-Host "Build Failed! Check the compiler errors above." -ForegroundColor Red
    exit 
}

# 2. Run the executable to see logic results
Write-Host "--- Running Executable ---" -ForegroundColor Cyan
docker run --rm frameless-fold-poc

# 3. Handle the local 'dist' folder
Write-Host "--- Preparing dist folder ---" -ForegroundColor Cyan
if (-not (Test-Path -Path "./dist")) {
    New-Item -ItemType Directory -Path "./dist"
    Write-Host "Created 'dist' folder." -ForegroundColor Gray
}

# 4. Extract the library file directly into dist
Write-Host "--- Extracting libFramelessFoldLib.a to ./dist ---" -ForegroundColor Cyan
$containerId = docker create frameless-fold-poc
docker cp "${containerId}:/app/build/libFramelessFoldLib.a" "./dist/libFramelessFoldLib.a"

docker rm $containerId

Write-Host "Success! Your library is located in: $(Get-Location)\dist\libFramelessFoldLib.a" -ForegroundColor Green