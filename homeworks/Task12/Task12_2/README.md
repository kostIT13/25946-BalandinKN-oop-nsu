# Задание 12.II — документация, сгенерированная из комментариев в коде

Инструмент — **clang-doc** (LLVM 21.1.4, установлен на машине). Он разбирает
исходник настоящим парсером C++ (libTooling), а не ищет комментарии регуляркой,
поэтому в документацию попадают реальные сигнатуры, порядок наследования и
принадлежность члена классу.

## Как получено

Комментарии в коде — блоки `///` с тегами Doxygen (`\brief`, `\param`, `\return`,
`\throws`), они стоят в заголовках `../Task12_1/*.h`:

```cpp
/// \brief Расход заряда за шаг с учётом груза.
///
/// Переопределение protected-хука базы: base-реализация move() сама спросит
/// стоимость шага у самого конкретного класса.
/// \return базовый расход плюс надбавка за каждые 100 кг груза.
double stepBatteryCost() const override;
```

Команда генерации (выполнена из корня репозитория, вывод — в `clangdoc/`):

```
clang-doc homeworks/Task12/Task12_1/robot.cpp ^
          homeworks/Task12/Task12_1/quadruped_robot.cpp ^
          homeworks/Task12/Task12_1/cargo_quadruped_robot.cpp ^
          homeworks/Task12/Task12_1/autonomous_cargo_quadruped_robot.cpp ^
          --format=md --output=homeworks/Task12/Task12_2/clangdoc ^
          --extra-arg=--target=x86_64-pc-windows-msvc --extra-arg=-std=c++17
```

Формат вывода меняется на `html`, `yaml` или `json` тем же флагом `--format`.

### Почему именно так (две особенности окружения)

- `--extra-arg=--target=x86_64-pc-windows-msvc` — без него clang-doc подставляет
  MSVC-заголовки в режим компилятора, который их не понимает, и тонет в
  `STL1003: Unexpected compiler`. С этим таргетом clang сам находит MSVC-STL и
  разбирает проект без ошибок.
- подаём `.cpp`, а не `.h`: clang-doc определяет язык по расширению, и на `.h`
  отказается принимать `-std=c++17` («not allowed with 'C'»). Трансляционная
  единица тянет за собой весь заголовок, поэтому документация полная.
- заголовки MinGW 6.3 для clang 21 несовместимы (`__float128`, `::atoll`), поэтому
  MinGW-вариант не использовался; сам код по-прежнему собирается MinGW-ом.

## Что получилось

| Файл | Содержимое |
|---|---|
| `clangdoc/index.md`, `clangdoc/GlobalNamespace/index.md` | указатель классов иерархии |
| `clangdoc/GlobalNamespace/Robot.md` | абстрактная вершина: `move()` как pure virtual, protected-помощники, контракт исключений |
| `clangdoc/GlobalNamespace/QuadrupedRobot.md` | п.2 |
| `clangdoc/GlobalNamespace/CargoQuadrupedRobot.md` | п.3 |
| `clangdoc/GlobalNamespace/AutonomousCargoQuadrupedRobot.md` | п.4 |
| `clangdoc/all_files.md` | всё одним файлом |

Фрагмент сгенерированного `CargoQuadrupedRobot.md`:

```markdown
# class CargoQuadrupedRobot
*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.h#8*

**brief** Грузовой квадропедальный робот — прямой наследник QuadrupedRobot.
 Своя реализация move(): тот же шаг, но расход заряда зависит от массы груза…

Inherits from QuadrupedRobot

## Functions
### move
*public void move()*
**brief** Шаг с грузом: реализация move() этого класса.
```

Строка «Inherits from QuadrupedRobot» и список членов взяты не из комментариев, а
из разобранного AST — то есть документация не может разойтись с кодом:
добавишь метод без комментария, и он всё равно появится в выводе (без текста),
переименуешь — ссылка обновится сама.

## Ограничения генератора

- clang-doc не рисует диаграмму наследования (в отличие от Doxygen + Graphviz) —
  иерархия видна только по строкам «Inherits from …» на страницах классов;
  диаграмма иерархии делалась отдельно в задании 11.II.
- В markdown-выводе `const` и модификаторы в сигнатурах теряются (`*public void
  move()*` вместо `virtual void move() const`), тип возвращаемого значения для
  конструктора печатается как `void`. YAML/JSON-вывод (`--format=yaml`) точнее:
  там есть `Access: Private|Protected|Public`, `Parent`, полные имена типов в
  `QualName` (`std::string` вместо `basic_string`);
- Нет кросс-ссылок между страницами классов, как в Doxygen.

Если нужен более привычный результат, тот же комментарийный слой понимает
Doxygen: `doxygen -g`, в `Doxyfile` — `RECURSIVE = YES`, `INPUT = ../Task12_1`,
`GENERATE_TREEVIEW = YES`. На этой машине Doxygen не установлен, поэтому
использован clang-doc.
