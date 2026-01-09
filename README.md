<h1>Курсовой проект</h1>
<h3>По теме: База данных "Веб-сервисы"</h3>
<h2>1. Постановка задачи</h2>
<b>Разработать консольное приложение для управления файлами базы данных Веб-сервисов с возможностями:</b>

- Хранение информации о сервисах (название, тип, разработчик, платформа, производительность, время отклика, доступность)

- Загрузки и сохранение данных в текстовых файлах

- Добавление новых записей с валидацией ввода

- Сортировка по каждому случаю (По производительности(убывание) → времени отклика → уровню доступности)

- Поиск записей по диапазону времени отклика

- Представление данных в виде списка

<h2>2. Структура проекта</h2>
КП/ 

├── README.md

├── kurs1.c

└── f.txt

<h2>3. Ключевые функции</h2>
<b>Основные функции управления:</b>

<blockquote><code>int main()</code></blockquote>

- Назначение: Главная функция.

<blockquote><code>int view_all(WebService* db, int rec_count)</code></blockquote>

- Назначение: Вывод списка всех записей, находящихся в базе данных.

<blockquote><code>int search_range(Database* db, int min_t, int max_t)</code></blockquote>

- Назначение: Поиск записе в БД по диапазону времени отклика

<blockquote><code>void compare_swap(WebService* a, WebService* b)</code></blockquote>

- Назначение: Функция <code>compare_swap</code> реализует логику сравнения и обмена двух записей о веб-сервисах для многоуровневой сортировки. Она определяет порядок сортировки по трем критериям и при необходимости меняет местами элементы.

<blockquote><code>int sort_by_multilevel(Database* db)</code></blockquote>

- Назначение: Сортирует записи по трем критериям:

 1. Производительность (по убыванию)

 2. Время отклика (по возрастанию, при равной производительности)

 3. Доступность (по убыванию, при равном времени отклика)

<blockquote><code>int add_manual_record(Database* db)</code></blockquote>

- Назначение: Добавление записи в ручную.

<blockquote><code>int input_ws(WebService* ws)</code></blockquote>

- Назначение: Обрабатывает ввод данных для добавления записи вручную.

<blockquote><code>int print_ws(const WebService* ws)</code></blockquote>

- Назначение: Выводит итоги ручного ввода данных

<blockquote><code>int loadFromFile(Database* db, char* fname)</code></blockquote>

- Назначение: Загружает данные из файла.

<blockquote><code>int saveToFile(Database* db, char* fname)</code></blockquote>

- Назначение: Выгружает данные в файл
  

<h2>4. Контрольные образцы</h2>
<h3>Пример 1: Запуск программы и загрузка данных</h3>
<img width="919" height="462" alt="image" src="https://github.com/user-attachments/assets/83a318cf-a68c-4877-98ca-fd716de9d297" />

<h3>Пример 2: Отображение всей базы данных (команда 1)</h3>
<img width="498" height="443" alt="image" src="https://github.com/user-attachments/assets/2be53a67-e923-43dd-9e08-f314e41d14fe" />

<h3>Пример 3: Добавление новых записей (команда 4)</h3>
<img width="815" height="478" alt="image" src="https://github.com/user-attachments/assets/c38ba5ed-9415-4b30-bc06-e4108f215944" />

<h3>Пример 4: Сортировка (команда 3)</h3>
<img width="1351" height="243" alt="image" src="https://github.com/user-attachments/assets/908ce2d7-e993-44da-9474-7c8508e01d29" />

<h3>Пример 5: Поиск по заданному диапазону (команда 2)</h3>
<img width="628" height="84" alt="image" src="https://github.com/user-attachments/assets/b2c295df-3308-4ebb-865f-e35d6359f746" />

<h2>5. Формат данных</h2>

<b>Типы Веб-сервисов:</b>
<blockquote><code>typedef enum { REST_API, SOAP, GRAPHQL, RPC, WEB_SOCKET } ServiceType;
typedef enum { WINDOWS, LINUX, MACOS, DOCKER, KUBERNETES } Platform;
</code></blockquote>

<b>Основная структура WebService</b>
<blockquote><code>typedef struct {
    char name[50];
    ServiceType type;
    char ver[20];
    char dev[50];
    Platform plat;
    double perf;
    int resp_time;
    float avail;
} WebService;</code></blockquote>

<h2>Формат файлов данных</h2>

<h3>Текстовый формат (.txt)</h3>

<b>Структура строки</b>

Название; Тип(0-4); Версия; Разработчик; Платформа(0-4; Производительность; Время отклика; Доступность

<b>Политестер</b>

<code>webserv;1;01;Matvey;3;200.00;12;46.0</code>
<h3>Ограничения</h3>

- Текстовые поля: <code>name, dev</code> - до 49 символов; <code>ver</code> - до 19 символов

- Максимальное количество записей: 100

- Числовые диапазоны <code>Тип/платформа</code>: 0-4; <code>Доступность</code>: 0-100%

<h2>Информация о разработчике</h2>
<b>Кобелев Матвей, бТИИ - 251</b>
