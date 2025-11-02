# Настройка Cocos2d-x для разработки в Cursor (VS Code) на macOS

## ✅ Выполненные шаги

### 1. Конвертация в Python 3
- ✅ `setup.py` обновлен для Python 3
- ✅ `download-deps.py` обновлен для Python 3 (убрана зависимость от устаревшего `distutils`)

### 2. Установка зависимостей
- ✅ Внешние библиотеки скачаны через `python3 download-deps.py -r yes`

### 3. Настройка CMake
- ✅ CMake проект сконфигурирован с поддержкой `compile_commands.json` для IntelliSense
- ✅ Используется Ninja build system для быстрой сборки

### 4. Исправление ошибок компиляции
- ✅ Исправлена проблема с `iconv` в `CCFontAtlas.h/cpp` для совместимости с новым macOS SDK

### 5. Конфигурация VS Code/Cursor
Созданы следующие файлы в `.vscode/`:

- ✅ `tasks.json` - задачи сборки CMake
- ✅ `launch.json` - конфигурации запуска и отладки
- ✅ `settings.json` - настройки проекта и IntelliSense
- ✅ `c_cpp_properties.json` - настройки C++ IntelliSense
- ✅ `extensions.json` - рекомендуемые расширения
- ✅ `README.md` - документация по использованию

## 🚀 Как начать работу

### Установка рекомендуемых расширений

При открытии проекта Cursor предложит установить расширения. Установите:
- **C/C++** (ms-vscode.cpptools)
- **CMake Tools** (ms-vscode.cmake-tools)
- **CMake** (twxs.cmake)
- **CodeLLDB** (vadimcn.vscode-lldb)

### Сборка проекта

#### Через UI:
1. Нажмите `⌘⇧B` (Cmd+Shift+B) - сборка всего проекта
2. Или `⌘⇧P` → `Tasks: Run Task` → выберите нужную задачу

#### Доступные задачи:
- **CMake: Build All** - сборка всех целей (по умолчанию)
- **CMake: Build cpp-tests** - C++ тесты
- **CMake: Build cpp-empty-test** - пустой тест
- **CMake: Build HelloCpp** - шаблон приложения
- **CMake: Build cocos2d** - только библиотека
- **CMake: Clean** - очистка
- **CMake: Clean and Reconfigure** - полная пересборка

#### Через терминал:
```bash
cd build
ninja                    # Собрать все
ninja cpp-tests          # Собрать тесты
ninja cocos2d            # Собрать только библиотеку
```

### Запуск и отладка

1. Откройте панель отладки: `⌘⇧D` (Cmd+Shift+D)
2. Выберите конфигурацию:
   - **Debug cpp-tests** - запуск тестов с отладкой
   - **Run cpp-tests (No Debug)** - запуск без отладки
   - **Debug cpp-empty-test** - отладка пустого теста
   - **Debug HelloCpp** - отладка шаблона
3. Нажмите `F5` для запуска

### Горячие клавиши

- `F5` - начать отладку
- `⇧F5` - остановить
- `F9` - точка останова
- `F10` - шаг с обходом
- `F11` - шаг с заходом
- `⌘⇧B` - сборка

## 📁 Структура проекта

```
cocos2d-x/
├── .vscode/                  # Конфигурация VS Code/Cursor
│   ├── tasks.json           # Задачи сборки
│   ├── launch.json          # Конфигурации запуска
│   ├── settings.json        # Настройки проекта
│   ├── c_cpp_properties.json # C++ IntelliSense
│   └── README.md            # Документация
├── build/                   # Директория сборки (создается)
│   ├── bin/                 # Исполняемые файлы
│   ├── lib/                 # Библиотеки
│   └── compile_commands.json # База данных компиляции
├── cocos/                   # Исходники движка
├── external/                # Внешние библиотеки
├── tests/                   # Тестовые проекты
├── templates/               # Шаблоны проектов
├── setup.py                 # Настройка окружения (Python 3)
└── download-deps.py         # Скачивание зависимостей (Python 3)
```

## 🔧 Решение проблем

### CMake не настраивается
```bash
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### IntelliSense не работает
1. `⌘⇧P` → `C/C++: Reset IntelliSense Database`
2. Перезапустите Cursor
3. Убедитесь, что `build/compile_commands.json` существует

### Ошибки компиляции с iconv
Уже исправлено в `cocos/2d/CCFontAtlas.h` и `CCFontAtlas.cpp`

### Отсутствуют зависимости
```bash
python3 download-deps.py -r yes
```

### Xcode Command Line Tools
Если lldb не работает:
```bash
xcode-select --install
```

## 📝 Дополнительные команды

### Настройка переменных окружения
```bash
python3 setup.py -q
source ~/.zshrc  # Применить изменения
```

### Полная пересборка
```bash
cd /Users/andreizenkovich/cocos2d-x
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ninja
```

### Сборка в Release режиме
```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ninja
```

## 🎯 Следующие шаги

1. **Изучите примеры**: запустите `cpp-tests` для просмотра возможностей движка
2. **Создайте свой проект**: используйте шаблон `HelloCpp` как основу
3. **Настройте отладку**: установите точки останова и изучите код движка
4. **Прочитайте документацию**: [docs.cocos2d-x.org](https://docs.cocos2d-x.org/)

## ℹ️ Полезные ссылки

- [Официальная документация Cocos2d-x](https://docs.cocos2d-x.org/)
- [Cocos2d-x на GitHub](https://github.com/cocos2d/cocos2d-x)
- [VS Code C++ Documentation](https://code.visualstudio.com/docs/languages/cpp)
- [CMake Documentation](https://cmake.org/documentation/)

---

**Готово к работе!** 🚀
Проект полностью настроен для разработки в Cursor на macOS.

