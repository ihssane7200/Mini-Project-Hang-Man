@echo off
echo Downloading Raylib for Windows...
curl -L -o raylib.zip "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win32_mingw-w64.zip"

echo Extracting Raylib...
tar -xf raylib.zip

echo Renaming folder to 'raylib' for easier access...
rename raylib-5.0_win32_mingw-w64 raylib

echo Cleaning up...
del raylib.zip

echo.
echo Raylib installation complete.
pause
