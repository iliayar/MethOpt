# MethOpt
## Лаба 1
Структура:
- `lib/include/` - методы и интерфейсы к ним
  - `function.hpp`
  - `methods.hpp`
  - `optimizer.hpp`
- `lab1/` - графический интерфейс
- `lab1cli/` - интерфейс для работы с методами из командной строки
## Лаба 2
Структура:
- `lib/include/` - методы и интерфейсы к ним
  - `mutli_helpers.hpp`
  - `mutli_methods.hpp`
  - `mutli_optimizer.hpp`
- `lab2cli/` - интерфейс для работы с методами из командной строки

Архив со скомпилированной под Windows программой в [релизе](https://github.com/iliayar/MethOpt/releases/tag/1.1.0)

## Сборка под Linux
- требуется `qt5` и `cmake`
- `git clone https://github.com/iliayar/MethOpt`
- `cd MethOpt`
- `mkdir build && cd build`
- `cmake ..`
- `make`

## Запуск
Из той же директории `build` `lab1/MetOptLab1`
