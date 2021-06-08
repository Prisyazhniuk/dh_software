## Инструменты

* CMake (3.10.0 и свежее)
* Qt 5.15.1 (mingw81_64)
* IPP 2019 Update 1 [(download)](https://yadi.sk/d/xUVWGAoz6W5ZBw)
* OpenCV 4.5.1 (mingw810_64) [(download)](https://yadi.sk/d/ktwLS7un6dZ1Uw)

## Настройка

### Переменные среды для IPP и OpenCV

Необходимо установить пути до библиотек. Достаточно сделать это для используемого kit'а. 

QtCreator:  
*Tools -> Options -> Kits -> Desktop Qt 5.15.1 MinGW 64-bit -> Environment -> Change*  
Добавить следующие переменные (установив актуальные пути):

```bash
    IPP_ROOT=c:\Program Files (x86)\IntelSWTools\compilers_and_libraries\windows\ipp
    OPENCV_ROOT=d:\3rdparty\opencv-4.5.1-mingw810
```

### Сборка сторонних библиотек

Сборка выполняется после клонирования основного репозитория.

1. Запустить консоль `Qt 5.15.1 (MinGW 8.1.0 64-bit)`
2. Перейти в каталог основного репозитория `dh_software`
3. Выполнить команду обновления подрепозиториев `git submodule update --init`
4. Запустить скрипт сборки `build_3rdparty.sh`

При успешном завершении сборки, артефакты перемещаются в каталог `3rdparty-mingw`

