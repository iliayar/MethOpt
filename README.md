# MethOpt
## Лаба 4
[Отчет](scripts/lab4/report.pdf) \
Структура:
- `liblab4/` - Реализация методов
- `lab4cli/` - интерфейс для работы с методами из командной строки
- [Jupyter notebook](scripts/lab4/Main.ipynb) - построение графиков и работа с функциями
## Лаба 3
[Отчет](scripts/lab3/report.pdf) \
Структура:
- `liblab3/` - Реализация методов и матриц
- `lab3cli/` - интерфейс для работы с методами из командной строки
## Лаба 2
[Отчет](scripts/lab2/report.pdf) \
Структура:
- `lib/include/` - методы и интерфейсы к ним
  - `mutli_helpers.hpp`
  - `mutli_methods.hpp`
  - `mutli_optimizer.hpp`
- `lab2cli/` - интерфейс для работы с методами из командной строки
## Лаба 1
Структура:
- `lib/include/` - методы и интерфейсы к ним
  - `function.hpp`
  - `methods.hpp`
  - `optimizer.hpp`
- `lab1/` - графический интерфейс
- `lab1cli/` - интерфейс для работы с методами из командной строки
### Визуализация
#### Windows
Архив со скомпилированной под Windows программой в [релизе](https://github.com/iliayar/MethOpt/releases/tag/1.1.0)
#### Linux
- требуется `qt5` и `cmake`
- `git clone https://github.com/iliayar/MethOpt`
- `cd MethOpt`
- `mkdir build && cd build`
- `cmake ..`
- `make`
- `lab1/MetOptLab1`
