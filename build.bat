@echo off

set BUILD_DIR=build
set CMAKE_COMMAND=cmake
set BUILD_COMMAND=cmake --build .

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

%CMAKE_COMMAND% -G Ninja .. > nul 2>&1
if %errorlevel% neq 0 (
    echo Ninja not found. Using default build system.
    %CMAKE_COMMAND% ..
) else (
    %BUILD_COMMAND%
)

if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)

echo Build successful.
exit /b 0