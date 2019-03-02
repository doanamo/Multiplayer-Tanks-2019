@echo off
echo -- Generating CMake project...

rem Create empty project directory.
mkdir "CMake-Example-VS2017" >nul 2>&1
cd "CMake-Example-VS2017"

rem Generate solution files.
cmake -G "Visual Studio 15" "CMake-Example-VS2017" "../../"

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
