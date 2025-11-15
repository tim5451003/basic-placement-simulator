@echo off
REM Visualization script for placement results
REM This script should be run from the project root directory

REM Change to script's directory
cd /d "%~dp0"

REM Refresh PATH to include Python
for /f "tokens=2*" %%a in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v PATH 2^>nul') do set "SYSTEM_PATH=%%b"
for /f "tokens=2*" %%a in ('reg query "HKCU\Environment" /v PATH 2^>nul') do set "USER_PATH=%%b"
set "PATH=%SYSTEM_PATH%;%USER_PATH%"

if "%1"=="" (
    set INPUT_FILE=output.json
) else (
    set INPUT_FILE=%1
)

if "%2"=="" (
    set OUTPUT_FILE=placement.png
) else (
    set OUTPUT_FILE=%2
)

echo Visualizing placement results...
echo Current directory: %CD%
echo Input: %INPUT_FILE%
echo Output: %OUTPUT_FILE%
echo.

REM Try python first, then python3, then py
where python >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    python viz\plot.py %INPUT_FILE% -o %OUTPUT_FILE%
) else (
    where py >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        py viz\plot.py %INPUT_FILE% -o %OUTPUT_FILE%
    ) else (
        echo ERROR: Python not found in PATH!
        echo Please restart your terminal or VS Code after installing Python.
        pause
        exit /b 1
    )
)

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Visualization saved to: %OUTPUT_FILE%
    echo.
    echo To view the image, open: %OUTPUT_FILE%
) else (
    echo.
    echo Visualization failed!
    echo Make sure you have matplotlib installed: pip install matplotlib
)

pause

