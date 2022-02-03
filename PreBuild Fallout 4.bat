RMDIR dist /S /Q
cmake -S . --preset=f4 --check-stamp-file "build/f4/CMakeFiles/generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build build/f4 --config Release
if %ERRORLEVEL% NEQ 0 exit 1

xcopy "build\f4\release\*.dll" "dist\F4SE\Plugins\" /I /Y
cd dist
powershell Compress-Archive -Force -Path * -DestinationPath "ENBHelper-F4.zip"
pause