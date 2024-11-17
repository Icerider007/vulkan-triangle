@echo off
setlocal
cd /D "%~dp0"

if not exist out mkdir out

cl /TC /d2cgsummary /Od /Zi /I %VULKAN_SDK%\Include\ /I ./code/ /I ./third_party/ ./code/main.c /link /OUT:out/game.exe %VULKAN_SDK%\lib\vulkan-1.lib data/SDL3.lib user32.lib kernel32.lib gdi32.lib

if not exist out\SDL3.dll xcopy /s /y data\SDL3.dll out
