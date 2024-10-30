# TaskScheduler

## Описание

Этот проект реализует класс `TaskScheduler` на языке C++, который позволяет планировать и выполнять задачи в заданное время. Проект демонстрирует использование потоков и примитивов синхронизации, используя стандартную библиотеку C++.

## Структура проекта

```
project/
├── include/
│   └── TaskScheduler.h
├── src/
│   ├── main.cpp
│   └── TaskScheduler.cpp
```

- **`include/`**: Содержит заголовочные файлы (`.h`).
- **`src/`**: Содержит исходные файлы (`.cpp`).

## Требования

- Компилятор с поддержкой C++11 или выше (например, GCC или Clang).
- POSIX-совместимая система (Linux, macOS).

## Сборка и запуск

### Компиляция с помощью g++

1. **Перейдите в корневую директорию проекта:**

   ```bash
   cd project
   ```

2. **Компилируйте проект:**

   ```bash
   g++ src/*.cpp -I include -o scheduler -pthread
   ```

   - `src/*.cpp`: Компилирует все `.cpp` файлы в директории `src/`.
   - `-I include`: Указывает компилятору путь к заголовочным файлам.
   - `-o scheduler`: Имя выходного исполняемого файла.
   - `-pthread`: Включает поддержку потоков POSIX.

3. **Запустите программу:**

   ```bash
   ./scheduler
   ```

## Ожидаемый вывод

При запуске программы вы должны увидеть следующий вывод:

```
Task 2 executed
Task 1 executed
Task 3 executed
```

Это демонстрирует, что задачи выполняются в соответствии с запланированным временем.

## Детали реализации

- **TaskScheduler.h**: Объявление класса `TaskScheduler` и структуры `ScheduledTask`.
- **TaskScheduler.cpp**: Реализация методов класса `TaskScheduler`.
- **main.cpp**: Пример использования `TaskScheduler`, добавляющий несколько задач для выполнения.

## Примечания

- **Потоковая безопасность**: Класс `TaskScheduler` использует `std::mutex` и `std::condition_variable` для обеспечения безопасного доступа к общим данным между потоками.
- **Синхронизация**: Рабочий поток ожидает наступления времени выполнения задач и выполняет их в нужный момент.
- **Завершение работы**: Деструктор класса корректно завершает рабочий поток.

## Полезные команды

- **Проверка версии компилятора:**

  ```bash
  g++ --version
  ```

- **Обновление g++ до последней версии (если необходимо):**

  ```bash
  sudo apt-get update
  sudo apt-get install g++
  ```

## Дополнительные рекомендации

- **Организация проекта**: Убедитесь, что директории `src/` и `include/` содержат соответствующие файлы, и структура проекта соответствует описанной.
- **Совместимость**: Убедитесь, что ваш компилятор поддерживает стандарты C++11 или выше, поскольку используются современные возможности языка.
- **Отладка**: Если при компиляции возникают ошибки, используйте флаг `-Wall` для вывода всех предупреждений:

  ```bash
  g++ src/*.cpp -I include -o scheduler -pthread
  ```