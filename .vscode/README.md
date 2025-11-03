# VS Code / Cursor Configuration для Cocos2d-x

## Созданные файлы конфигурации

- `tasks.json` - задачи сборки CMake
- `launch.json` - конфигурации запуска и отладки
- `settings.json` - настройки проекта
- `c_cpp_properties.json` - настройки IntelliSense для C++
- `extensions.json` - рекомендуемые расширения

## Рекомендуемые расширения

Установите следующие расширения для лучшего опыта разработки:

1. **C/C++** (ms-vscode.cpptools) - IntelliSense, отладка
2. **CMake Tools** (ms-vscode.cmake-tools) - интеграция CMake
3. **CMake** (twxs.cmake) - подсветка синтаксиса CMake
4. **CodeLLDB** (vadimcn.vscode-lldb) - отладчик LLDB для macOS

## Быстрый старт

### 1. Сборка проекта

**Через Command Palette (⇧⌘P):**
- `Tasks: Run Build Task` - собрать все (Ctrl+Shift+B)
- Или выбрать конкретную цель через `Tasks: Run Task`

**Доступные задачи сборки:**
- `CMake: Configure` - конфигурация CMake
- `CMake: Build All` - сборка всех целей (по умолчанию)
- `CMake: Build cpp-tests` - сборка C++ тестов
- `CMake: Build cpp-empty-test` - сборка пустого теста
- `CMake: Build HelloCpp` - сборка шаблона HelloCpp
- `CMake: Build cocos2d` - сборка только библиотеки cocos2d
- `CMake: Clean` - очистка артефактов сборки
- `CMake: Clean and Reconfigure` - полная пересборка

### 2. Запуск и отладка

**Через панель отладки (⇧⌘D):**
1. Выберите конфигурацию из выпадающего списка:
   - `Debug cpp-tests` - запуск тестов с отладкой
   - `Run cpp-tests (No Debug)` - запуск без отладки (быстрее)
   - `Debug cpp-empty-test` - отладка пустого теста
   - `Debug HelloCpp` - отладка шаблона
   - `Attach to Process` - подключение к запущенному процессу

2. Нажмите F5 или кнопку "Start Debugging"

**Горячие клавиши:**
- `F5` - начать отладку
- `⇧F5` - остановить отладку
- `F9` - установить/снять точку останова
- `F10` - шаг с обходом
- `F11` - шаг с заходом

### 3. Сборка через терминал

```bash
# Из корневой директории проекта
cd build

# Сборка всех целей (Ninja)
ninja

# Или сборка конкретной цели
ninja cpp-tests
ninja HelloCpp
ninja cocos2d

# Очистка
ninja clean
```

## Структура проекта после сборки

```
cocos2d-x/
├── build/                          # Директория сборки
│   ├── bin/                        # Исполняемые файлы
│   │   ├── cpp-tests/
│   │   │   └── cpp-tests.app/
│   │   ├── cpp-empty-test/
│   │   ├── HelloCpp/
│   │   └── lua-tests/
│   ├── lib/                        # Библиотеки
│   └── compile_commands.json       # Для IntelliSense
├── cocos/                          # Исходники движка
├── tests/                          # Тестовые проекты
└── templates/                      # Шаблоны проектов
```

## IntelliSense

IntelliSense настроен автоматически через:
- `compile_commands.json` - база данных компиляции
- Настроенные пути включения (include paths)
- Правильные стандарты C++14/C11

Если IntelliSense не работает:
1. Нажмите `⇧⌘P` → `C/C++: Reset IntelliSense Database`
2. Перезапустите VS Code/Cursor

## Решение проблем

### Проблема: "Generator does not match"
```bash
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### Проблема: не находится lldb
Установите Xcode Command Line Tools:
```bash
xcode-select --install
```

### Проблема: отсутствуют зависимости
```bash
python3 download-deps.py -r yes
```

## Дополнительная информация

- [Cocos2d-x Documentation](https://docs.cocos2d-x.org/)
- [CMake Documentation](https://cmake.org/documentation/)
- [VS Code C++ Documentation](https://code.visualstudio.com/docs/languages/cpp)

