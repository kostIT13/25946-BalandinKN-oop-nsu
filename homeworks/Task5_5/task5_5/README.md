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

# Отличие ASSERT от EXPECT видно только в выводе gtest:
# в первом тесте 3 блока error и все 4 маркера >>>, во втором — 1 блок error и ни одного >>>
.\tests\Debug\lib_tests.exe --gtest_filter=AssertVsExpect.*

# По ctest отличия не видно: gtest_discover_tests делает из каждого теста отдельный
# пункт, и оба он помечает ***Failed. --output-on-failure обязателен, иначе stdout скрыт.
ctest --output-on-failure -C Debug -R AssertVsExpect