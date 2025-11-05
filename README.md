# melodia-code

Deps:
sudo apt install make cmake g++ git libncurses5-dev

CMake:
# 1. Gera os arquivos de build na pasta "build/"
cmake -B build -S .

ln -s build/compile_commands.json compile_commands.json

# 2. Compila o projeto
cmake --build build

# 3. Roda o executável gerado
./build/cpp_project

Quando quiser recompilar do zero (por exemplo, se mudar muita coisa), basta apagar a pasta build:

rm -rf build
cmake -B build
cmake --build build
