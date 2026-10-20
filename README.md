# AI Skills

Коллекция навыков (skills) для Claude.

## Навыки

| Навык | Описание |
|---|---|
| [modern-cpp-style](modern-cpp-style/SKILL.md) | Генерация и ревью кода на Modern C++ (C++17/20/23): `{}`-инициализация, `const`/`constexpr` по умолчанию, `static_assert`, `enum class` с унарным `operator+`, snake_case для функций, `_` в конце имён приватных членов, STL-алгоритмы и ranges вместо сырых циклов, RAII, `explicit`-конструкторы. |

## Установка

**Claude.ai / приложение Claude:** скачайте готовый архив из [`dist/`](dist/) (например, `modern-cpp-style.skill`) и загрузите его в разделе навыков (Skills) в настройках Claude.

**Claude Code:** скопируйте папку навыка в `~/.claude/skills/` (для всех проектов) или в `.claude/skills/` конкретного проекта:

```bash
git clone https://github.com/AlexKilim/ai-skills.git
cp -r ai-skills/modern-cpp-style ~/.claude/skills/
```

## Структура

```
ai-skills/
├── README.md
├── dist/                      # packaged .skill archives
└── <skill-name>/
    ├── SKILL.md               # frontmatter (name, description) + instructions
    └── references/            # supporting files loaded on demand
```

## Добавление нового навыка

1. Создайте папку `<skill-name>/` с файлом `SKILL.md`.
2. Соберите архив и положите его в `dist/`.
3. Добавьте строку в таблицу выше.
