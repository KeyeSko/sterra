Требования:

1. Код должен быть написан на языке C и собираться под Linux.
2. Исходники должны быть выложены на github.com с публичным доступом.

Задание:

Сформировать в памяти один двусвязный список, содержащий случайные значения, которые следует получить из генератора случайных чисел.
Программа должна принимать на вход количество элементов в двухсвязном списке.
Запустить два потока (threads).
Потоку №1: начиная с головы списка подсчитывать количество нулевых битов в значениях элементов и количество пройденных элементов, освобождать учтённый элемент сразу после учёта.
Потоку №2: начиная с хвоста списка подсчитывать количество единичных битов в значениях элементов и количество пройденных элементов, освобождать учтённый элемент сразу после учёта.
По окончании элементов списка вывести результаты подсчёта.
Обеспечить однократную обработку каждого элемента списка (каждый должен быть учтён, но только один раз, одним из потоков).
Очень желательно, чтобы в потоках работали не две разные функции, а одна, принимающая особенности работы через аргументы.
Желательно научиться отлаживать программу при помощи GDB (The GNU Project Debugger).
