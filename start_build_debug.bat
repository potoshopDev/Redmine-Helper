call cd build
call cmake --build . --target RedmineHelper_imGui 
call cd bin\Debug\
call .\RedmineHelper_imGui.exe
call cd ..\..\..
