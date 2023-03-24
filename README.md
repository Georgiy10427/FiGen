## 📐FiGen 
Программа для быстрого решения треугольников и их генерации. Сделано на C++ с применением Qt.
![Скриншот на KDE 5 с Breeeze Dark](https://github.com/Georgiy10427/FiGen/raw/main/screenshot.png)
Поддерживается:
* вычисление недостающих данных в треугольнике, в том числе по теореме косинусов и теореме Пифагора;
* расчет площади треугольника;
* отрисовка данного треугольника на панели;
* проверка существования треугольника (теорема синусов)
* расчет синусов, косинусов, тангенса, котангенса углов для дальнейших расчетов.
* генерация любого вида треугольника
* поддержка ввода минут и секунд помимо градусов (в формате градусы' минуты' секунды')

### Установка
FiGen не создает временных файлов на вашем устройстве, имея полностью портативный формат. 
| | Windows | Linux (glibc 2.33) |
| - | ----- | ------------- |
| Версия | [1.0.1](https://github.com/Georgiy10427/FiGen/releases/download/1.0.1/figen-1.0.1-windows-x64.exe) | [1.0.1](https://github.com/Georgiy10427/FiGen/releases/download/1.0.1/figen-1.0.1-linux-glibc2-33) |

# Сборка
Компиляция производится со статической компоновкой Qt 6, либо с версией Qt6 установленной в вашей системе.  
## Статическая сборка
Следуйте указаниям из официальной документации:  
* [Сборка Qt6 под Windows] (https://doc.qt.io/qt-6/windows-building.html)
* [Сборка Qt6 под Linux] (https://doc.qt.io/qt-6/linux.html)
Для сборки программы FiGen достаточно компонента `qtbase`. Далее необходимо указать папку со статической сборкой Qt и собрать проект в режиме "Релиз".  
Флаги сборки для Linux: `../configure -debug-and-release -commercial -confirm-license -static -glib -opengl -no-icu -xkbcommon -qt-libjpeg -qt-zlib -qt-libpng -system-freetype -qt-pcre -qt-harfbuzz -no-sm -fontconfig -nomake tests -prefix PATH_FOR_INSTALL`  
Флаг сборки для Windows: `-static`  
## Динамическая сборка
Установите cmake, git, qt6 в вашем дистрибутиве. Далее следуйте инструкциям:  
```git clone https://github.com/Georgiy10427/FiGen.git
cd FiGen
mkdir build && cd build
cmake ..
cmake --build ..
```


