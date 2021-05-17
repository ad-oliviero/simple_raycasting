#raylib
git clone https://github.com/raysan5/raylib.git raylib-git/
mkdir ./raylib/
mkdir ./raylib/include/
Set-Location raylib-git/src/
make PLATFORM=PLATFORM_DESKTOP
Copy-Item libraylib.a ../../raylib/
Copy-Item raylib.h ../../raylib/include/
Set-Location ../../

#raygui
git clone https://github.com/raysan5/raygui.git
Copy-Item ./raylib/include/raylib.h ./raygui/src/
