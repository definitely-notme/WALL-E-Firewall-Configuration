@echo off
setlocal enabledelayedexpansion

:: Define the log file
set "logFile=t.log"

echo Enabling outgoing HTTP and HTTPS connections...

:: Define the rules for HTTP (port 80) and HTTPS (port 443)
set "http_rule=Block_HTTP_Outbound"
set "https_rule=Block_HTTPS_Outbound"

:: Check if the log file already exists, and if not, create it
if not exist "!logFile!" (
    echo Creating log file...
    echo. > "!logFile!"
)

:: Check if the rules exist and remove them
netsh advfirewall firewall show rule name="%http_rule%" > nul 2>&1
if %errorlevel% equ 0 (
    echo Removing outgoing HTTP rule... >> "!logFile!"
    netsh advfirewall firewall delete rule name="%http_rule%" >> "!logFile!"
) else (
    echo Outgoing HTTP rule does not exist. >> "!logFile!"
)

netsh advfirewall firewall show rule name="%https_rule%" > nul 2>&1
if %errorlevel% equ 0 (
    echo Removing outgoing HTTPS rule... >> "!logFile!"
    netsh advfirewall firewall delete rule name="%https_rule%" >> "!logFile!"
) else (
    echo Outgoing HTTPS rule does not exist. >> "!logFile!"
)

echo Outgoing HTTP and HTTPS connections are now enabled. >> "!logFile!"

endlocal
