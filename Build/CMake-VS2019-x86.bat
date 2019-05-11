@echo off
echo -- Generating CMake project...

rem Create empty project directory.
mkdir "CMake-VS2019-x86" >nul 2>&1
cd "CMake-VS2019-x86"

rem Generate solution files.
cmake -G "Visual Studio 16" -A Win32 "CMake-VS2019-x86" "../../"

if %ERRORLEVEL% NEQ 0 (
    pause
    exit
)

rem Open solution file.
for /f %%f in ('dir /b "*.sln"') do (
    set solution=%%f
    goto found
)
:found

echo -- Opening solution file...
start "" "%solution%"
