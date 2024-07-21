@echo off
setlocal enabledelayedexpansion

:: Define the log file
set "logFile=t.log"

echo Disabling outgoing HTTP and HTTPS connections...

:: Define the rules for HTTP (port 80) and HTTPS (port 443)
set "http_rule=Block_HTTP_Outbound"
set "https_rule=Block_HTTPS_Outbound"

:: Check if the log file already exists, and if not, create it
if not exist "!logFile!" (
    echo Creating log file...
    echo. > "!logFile!"
)

:: Check if the rules already exist
netsh advfirewall firewall show rule name="%http_rule%" > nul 2>&1
if %errorlevel% equ 0 (
    echo Outgoing HTTP rule already exists. >> "!logFile!"
) else (
    echo Creating outgoing HTTP rule... >> "!logFile!"
    netsh advfirewall firewall add rule name="%http_rule%" dir=out action=block protocol=TCP remoteport=80 >> "!logFile!"
)

netsh advfirewall firewall show rule name="%https_rule%" > nul 2>&1
if %errorlevel% equ 0 (
    echo Outgoing HTTPS rule already exists. >> "!logFile!"
) else (
    echo Creating outgoing HTTPS rule... >> "!logFile!"
    netsh advfirewall firewall add rule name="%https_rule%" dir=out action=block protocol=TCP remoteport=443 >> "!logFile!"
)

echo Outgoing HTTP and HTTPS connections are now blocked. >> "!logFile!"

endlocal
