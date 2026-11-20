cd C:\Users\Yaroslav\Desktop\PROJECTS\1seminar\homeworks\Task5_5\task5_5
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
mkdir build
cd build

cmake ..
cmake .. -DBUILD_TESTS=OFF
cmake --build . --config Debug


# Приложение
.\Debug\app.exe

# Тесты — способ 1
ctest --output-on-failure -C Debug

# Тесты — способ 2
.\tests\Debug\lib_tests.exe