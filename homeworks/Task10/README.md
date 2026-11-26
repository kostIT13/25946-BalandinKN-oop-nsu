## Сборка

```
cmake -S homeworks/Task10/Task10_1 -B homeworks/Task10/Task10_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task10/Task10_1/build
homeworks\Task10\Task10_1\build\robot.exe
```

`not_allowed_example.cpp` в сборку не входит: он доказывает границами доступа
(отсутствующая дружба в п.0, недоступная база в п.2, запрет наследования от
`final`), поэтому обязан не компилироваться. Подробности — в
[`Task10_1/README.md`](Task10_1/README.md).
