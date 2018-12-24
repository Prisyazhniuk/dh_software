## Инструменты

* Qt 5.12.0 (mingw73_64)
* IPP 2019 Update 1 [(download)](https://yadi.sk/d/xUVWGAoz6W5ZBw)
* OpenCV 4.0.0 (mingw730_64) [(download)](https://yadi.sk/d/weYym11MsmXHeA)

## Настройка

### Переменные среды для IPP и OpenCV

Необходимо установить пути до библиотек. Достаточно сделать это для используемого kit'а. 

QtCreator:  
*Tools -> Options -> Kits -> Desktop Qt 5.12.0 MinGW 64-bit -> Environment -> Change *  
Добавить следующие переменные (установив актуальные пути):

```bash
    IPP_ROOT=c:\Program Files (x86)\IntelSWTools\compilers_and_libraries\windows\ipp
    OPENCV_ROOT=d:\3rdparty\opencv-4.0.0-mingw730
```
