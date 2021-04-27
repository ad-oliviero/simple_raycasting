set -e 
git clone https://github.com/raysan5/raylib.git raylib-git 2>/dev/null
cd raylib-git
mkdir build
cd build
cmake ..
make
mv raylib/ ../..
