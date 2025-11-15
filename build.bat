@echo off
REM Simple build script for Windows
REM Requires g++ (MinGW) to be in PATH

echo Building Basic Placement Simulator...
echo.

REM Check if g++ is available
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ not found in PATH!
    echo Please install MinGW-w64 and add it to your PATH.
    echo.
    echo Alternative: Use CMake or Visual Studio
    pause
    exit /b 1
)

REM Create object files directory structure
if not exist obj mkdir obj
if not exist obj\model mkdir obj\model
if not exist obj\io mkdir obj\io
if not exist obj\cost mkdir obj\cost
if not exist obj\opt mkdir obj\opt
if not exist obj\legal mkdir obj\legal
if not exist obj\detail mkdir obj\detail
if not exist obj\viz mkdir obj\viz

echo Compiling source files...
g++ -std=c++17 -Wall -Wextra -O2 -I. -c main.cpp -o obj\main.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c model\placement.cc -o obj\model\placement.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c io\reader.cc -o obj\io\reader.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c cost\cost.cc -o obj\cost\cost.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c opt\anneal.cc -o obj\opt\anneal.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c legal\legalize.cc -o obj\legal\legalize.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c detail\detail_place.cc -o obj\detail\detail_place.o
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -Wall -Wextra -O2 -I. -c viz\write_json.cc -o obj\viz\write_json.o
if %ERRORLEVEL% NEQ 0 goto :error

echo Linking executable...
g++ -std=c++17 -Wall -Wextra -O2 -o placement_simulator.exe obj\main.o obj\model\placement.o obj\io\reader.o obj\cost\cost.o obj\opt\anneal.o obj\legal\legalize.o obj\detail\detail_place.o obj\viz\write_json.o
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo Build successful! Executable: placement_simulator.exe
echo.
echo To run: placement_simulator.exe test_input.txt output.json
goto :end

:error
echo.
echo Build failed! Check the error messages above.
pause
exit /b 1

:end
pause
