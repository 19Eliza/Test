Тестовый проект для стажировки C++ YADRO.
Проект использует систему сборки CMake с пресетами. Определить пользовательские пресеты, унаследованные от пресетов release и default. 
Для конфигурирования необходимо выполнить команду из каталога, содержащего CMakeLists.txt:
cmake --preset <name-of-preset>
Для сборки необходимо выполнить команду из сгенерированного каталога (обычно это build/<name-of-preset>):
cmake --build --preset <name-of-preset>
