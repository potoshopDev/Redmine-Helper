call cd build
call cmake --build . --target RedmineHelper_ImGui
call cd bin\Debug\
call .\RedmineHelper_ImGui.exe
call cd ..\..\..
