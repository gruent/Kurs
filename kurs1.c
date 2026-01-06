#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

#define MAX_RECORDS 100

typedef enum { REST_API, SOAP, GRAPHQL, RPC, WEB_SOCKET } ServiceType;
typedef enum { WINDOWS, LINUX, MACOS, DOCKER, KUBERNETES } Platform;

typedef struct {
    char name[50];
    ServiceType type;
    char ver[20];
    char dev[50];
    Platform plat;
    double perf;
    int resp_time;
    float avail;
} WebService;

WebService* db = NULL;
int rec_count = 0;
int cap = 0;

int view_all();
int search_range(int min_t, int max_t);
int sort_by_multilevel();
int add_manual_record();
int input_ws(WebService* ws);
int print_ws(const WebService* ws);
const char* get_type_name(ServiceType type);
const char* get_plat_name(Platform plat);
int loadFromFile(char* fname);
int saveToFile(char* fname);
void compare_swap(WebService* a, WebService* b);

int main() {
    setlocale(LC_CTYPE, "RUS");
    int choice, result;
    char filename[100];

    puts("\n\n\t\t=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    puts("\t\t|                                                                                   |");
    puts("\t\t|     Курсовой проект по дисциплине: \"Основы программирования и алгоритмизации\"     |");
    puts("\t\t|                     Тема: \"База данных веб-сервисов\"                              |");
    puts("\t\t|                  Выполнила: Кобелев М.С., группа бТИИ-251                         |");
    puts("\t\t|                                                                                   |");
    puts("\t\t=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    puts("\t\t Программа представляет собой базу данных веб-сервисов.");
    puts("\t\t Основные функции: отображение записей, загрузка из файла, добавление новых записей,");
    puts("\t\t сортировка по многоуровневым критериям, поиск по времени отклика.");
    puts("\t\t Дополнительные функции: сохранение данных в файл, загрузка данных из файла.");

    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Просмотр всех записей\n");
        printf("2. Поиск по диапазону времени отклика\n");
        printf("3. Многоуровневая сортировка\n");
        printf("4. Добавление записи вручную\n");
        printf("5. Загрузка данных из файла\n");
        printf("6. Сохранение данных в файл\n");
        printf("7. Выход\n");
        printf("Выберите: ");

        scanf("%d", &choice);
        getchar();


        switch (choice) {
        case 1:
            result = view_all();
            if (result == 0) printf("База данных пуста\n");
            break;
        case 2: {
            int min_t, max_t;
            printf("Минимальное время отклика: ");
            scanf("%d", &min_t);
            printf("Максимальное время отклика: ");
            scanf("%d", &max_t);
            getchar();
            result = search_range(min_t, max_t);
            if (result == 0) printf("Записей не найдено\n");
            break;
        }
        case 3:
            result = sort_by_multilevel();
            if (result == 0) printf("Не удалось отсортировать данные\n");
            break;
        case 4:
            result = add_manual_record();
            if (result == 0) printf("Не удалось добавить запись\n");
            break;
        case 5:
            printf("Введите имя файла для загрузки: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            result = loadFromFile(filename);
            if (result > 0) printf("Загружено %d записей\n", result);
            else printf("Не удалось загрузить данные\n");
            break;
        case 6:
            printf("Введите имя файла для сохранения: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            result = saveToFile(filename);
            if (result > 0) printf("Сохранено %d записей\n", result);
            else if (result == 0) printf("База данных пуста\n");
            else printf("Ошибка при сохранении\n");
            break;
        case 7:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 7);

    if (db != NULL) {
        free(db);
        db = NULL;
    }

    return 0;
}

int view_all() {
    if (rec_count == 0) return 0;
    printf("\n=== ВСЕ ЗАПИСИ (%d) ===\n", rec_count);
    for (int i = 0; i < rec_count; i++) {
        printf("\n--- Запись %d ---\n", i + 1);
        print_ws(&db[i]);
    }
    return rec_count;
}

int search_range(int min_t, int max_t) {
    if (rec_count == 0) return 0;
    printf("\n=== ПОИСК (время от %d до %d мс) ===\n", min_t, max_t);
    int found = 0;
    for (int i = 0; i < rec_count; i++) {
        if (db[i].resp_time >= min_t && db[i].resp_time <= max_t) {
            printf("\n--- Запись %d ---\n", i + 1);
            print_ws(&db[i]);
            found++;
        }
    }
    return found;
}

void compare_swap(WebService* a, WebService* b) {
    WebService temp;

    if (b->perf > a->perf) {
        temp = *a;
        *a = *b;
        *b = temp;
        return;
    }

    if (b->perf == a->perf && b->resp_time < a->resp_time) {
        temp = *a;
        *a = *b;
        *b = temp;
        return;
    }

    if (b->perf == a->perf && b->resp_time == a->resp_time && b->avail > a->avail) {
        temp = *a;
        *a = *b;
        *b = temp;
        return;
    }
}

int sort_by_multilevel() {
    if (rec_count == 0) return 0;
    printf("Выполняется многоуровневая сортировка...\n");
    printf("Критерии сортировки:\n");
    printf("1. По производительности (по убыванию)\n");
    printf("2. При равной производительности - по времени отклика (по возрастанию)\n");
    printf("3. При равном времени отклика - по доступности (по убыванию)\n");

    for (int i = 0; i < rec_count - 1; i++) {
        for (int j = 0; j < rec_count - i - 1; j++) {
            compare_swap(&db[j], &db[j + 1]);
        }
    }

    printf("Данные отсортированы по многоуровневым критериям.\n");
    return 1;
}

int add_manual_record() {
    printf("\n=== ДОБАВЛЕНИЕ ЗАПИСИ ВРУЧНУЮ ===\n");

    if (rec_count >= cap) {
        cap += 10;
        WebService* new_db = realloc(db, cap * sizeof(WebService));
        if (new_db == NULL) {
            printf("Ошибка памяти!\n");
            return 0;
        }
        db = new_db;
    }


    int result = input_ws(&db[rec_count]);
    if (result == 1) {
        rec_count++;
        printf("\nЗапись успешно добавлена.\n");
        printf("Всего записей: %d\n", rec_count);
        return 1;
    }
    else {
        printf("Ошибка ввода данных\n");
        return 0;
    }
}

int input_ws(WebService* ws) {
    if (ws == NULL) return 0;

    printf("Название: ");
    fgets(ws->name, sizeof(ws->name), stdin);
    ws->name[strcspn(ws->name, "\n")] = 0;

    printf("Тип (0-4): ");
    int t;
    scanf("%d", &t);
    if (t < 0 || t > 4) {
        printf("Неверный тип!\n");
        return 0;
    }
    ws->type = (ServiceType)t;
    getchar();

    printf("Версия: ");
    fgets(ws->ver, sizeof(ws->ver), stdin);
    ws->ver[strcspn(ws->ver, "\n")] = 0;

    printf("Разработчик: ");
    fgets(ws->dev, sizeof(ws->dev), stdin);
    ws->dev[strcspn(ws->dev, "\n")] = 0;

    printf("Платформа (0-4): ");
    int p;
    scanf("%d", &p);
    if (p < 0 || p > 4) {
        printf("Неверная платформа!\n");
        return 0;
    }
    ws->plat = (Platform)p;

    printf("Производительность: ");
    scanf("%lf", &ws->perf);
    if (ws->perf < 0) {
        printf("Неверная производительность!\n");
        return 0;
    }

    printf("Время отклика (мс): ");
    scanf("%d", &ws->resp_time);
    if (ws->resp_time < 0) {
        printf("Неверное время отклика!\n");
        return 0;
    }

    printf("Доступность (%%): ");
    scanf("%f", &ws->avail);
    if (ws->avail < 0 || ws->avail > 100) {
        printf("Неверная доступность!\n");
        return 0;
    }

    getchar();
    return 1;
}

int print_ws(const WebService* ws) {
    if (ws == NULL) return 0;

    printf("Название: %s\n", ws->name);
    printf("Тип: %s\n", get_type_name(ws->type));
    printf("Версия: %s\n", ws->ver);
    printf("Разработчик: %s\n", ws->dev);
    printf("Платформа: %s\n", get_plat_name(ws->plat));
    printf("Производительность: %.2f\n", ws->perf);
    printf("Время отклика: %d мс\n", ws->resp_time);
    printf("Доступность: %.1f%%\n", ws->avail);

    return 1;
}

const char* get_type_name(ServiceType type) {
    switch (type) {
    case REST_API: return "REST API";
    case SOAP: return "SOAP";
    case GRAPHQL: return "GraphQL";
    case RPC: return "RPC";
    case WEB_SOCKET: return "Web Socket";
    default: return "Неизвестно";
    }
}

const char* get_plat_name(Platform plat) {
    switch (plat) {
    case WINDOWS: return "Windows";
    case LINUX: return "Linux";
    case MACOS: return "macOS";
    case DOCKER: return "Docker";
    case KUBERNETES: return "Kubernetes";
    default: return "Неизвестно";
    }
}

int loadFromFile(char* fname) {
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Не удалось открыть файл: %s\n", fname);
        return 0;
    }

    char line[256];
    int loaded = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        WebService ws;
        int t, p;

        if (sscanf(line, "%49[^;];%d;%19[^;];%49[^;];%d;%lf;%d;%f",
            ws.name, &t, ws.ver, ws.dev, &p, &ws.perf, &ws.resp_time, &ws.avail) == 8) {

            if (t < 0 || t > 4 || p < 0 || p > 4 ||
                ws.perf < 0 || ws.resp_time < 0 ||
                ws.avail < 0 || ws.avail > 100) {
                printf("Некорректная запись: %s\n", line);
                continue;
            }

            ws.type = (ServiceType)t;
            ws.plat = (Platform)p;

            if (rec_count >= cap) {
                cap += 10;
                WebService* new_db = realloc(db, cap * sizeof(WebService));
                if (!new_db) {
                    fclose(f);
                    return loaded;
                }
                db = new_db;
            }

            db[rec_count++] = ws;
            loaded++;
            printf("Загружена: %s\n", ws.name);
        }
    }

    fclose(f);
    return loaded;
}

int saveToFile(char* fname) {
    if (rec_count == 0) {
        printf("База данных пуста\n");
        return 0;
    }


    int result = input_ws(&db[rec_count]);
    if (result == 1) {
        rec_count++;
        printf("\nЗапись успешно добавлена.\n");
        printf("Всего записей: %d\n", rec_count);
        return 1;
    }
    else {
        printf("Ошибка ввода данных\n");
        return 0;
    }
}

int input_ws(WebService* ws) {
    if (ws == NULL) return 0;

    printf("Название: ");
    fgets(ws->name, sizeof(ws->name), stdin);
    ws->name[strcspn(ws->name, "\n")] = 0;

    printf("Тип (0-4): ");
    int t;
    scanf("%d", &t);
    if (t < 0 || t > 4) {
        printf("Неверный тип!\n");
        return 0;
    }
    ws->type = (ServiceType)t;
    getchar();

    printf("Версия: ");
    fgets(ws->ver, sizeof(ws->ver), stdin);
    ws->ver[strcspn(ws->ver, "\n")] = 0;

    printf("Разработчик: ");
    fgets(ws->dev, sizeof(ws->dev), stdin);
    ws->dev[strcspn(ws->dev, "\n")] = 0;

    printf("Платформа (0-4): ");
    int p;
    scanf("%d", &p);
    if (p < 0 || p > 4) {
        printf("Неверная платформа!\n");
        return 0;
    }
    ws->plat = (Platform)p;

    printf("Производительность: ");
    scanf("%lf", &ws->perf);
    if (ws->perf < 0) {
        printf("Неверная производительность!\n");
        return 0;
    }

    printf("Время отклика (мс): ");
    scanf("%d", &ws->resp_time);
    if (ws->resp_time < 0) {
        printf("Неверное время отклика!\n");
        return 0;
    }

    printf("Доступность (%%): ");
    scanf("%f", &ws->avail);
    if (ws->avail < 0 || ws->avail > 100) {
        printf("Неверная доступность!\n");
        return 0;
    }

    getchar();
    return 1;
}

int print_ws(const WebService* ws) {
    if (ws == NULL) return 0;

    printf("Название: %s\n", ws->name);
    printf("Тип: %s\n", get_type_name(ws->type));
    printf("Версия: %s\n", ws->ver);
    printf("Разработчик: %s\n", ws->dev);
    printf("Платформа: %s\n", get_plat_name(ws->plat));
    printf("Производительность: %.2f\n", ws->perf);
    printf("Время отклика: %d мс\n", ws->resp_time);
    printf("Доступность: %.1f%%\n", ws->avail);

    return 1;
}

const char* get_type_name(ServiceType type) {
    switch (type) {
    case REST_API: return "REST API";
    case SOAP: return "SOAP";
    case GRAPHQL: return "GraphQL";
    case RPC: return "RPC";
    case WEB_SOCKET: return "Web Socket";
    default: return "Неизвестно";
    }
}

const char* get_plat_name(Platform plat) {
    switch (plat) {
    case WINDOWS: return "Windows";
    case LINUX: return "Linux";
    case MACOS: return "macOS";
    case DOCKER: return "Docker";
    case KUBERNETES: return "Kubernetes";
    default: return "Неизвестно";
    }
}

int loadFromFile(char* fname) {
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("Не удалось открыть файл: %s\n", fname);
        return 0;
    }

    char line[256];
    int loaded = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        WebService ws;
        int t, p;

        if (sscanf(line, "%49[^;];%d;%19[^;];%49[^;];%d;%lf;%d;%f",
            ws.name, &t, ws.ver, ws.dev, &p, &ws.perf, &ws.resp_time, &ws.avail) == 8) {

            if (t < 0 || t > 4 || p < 0 || p > 4 ||
                ws.perf < 0 || ws.resp_time < 0 ||
                ws.avail < 0 || ws.avail > 100) {
                printf("Некорректная запись: %s\n", line);
                continue;
            }

            ws.type = (ServiceType)t;
            ws.plat = (Platform)p;

            if (rec_count >= cap) {
                cap += 10;
                WebService* new_db = realloc(db, cap * sizeof(WebService));
                if (!new_db) {
                    fclose(f);
                    return loaded;
                }
                db = new_db;
            }

            db[rec_count++] = ws;
            loaded++;
            printf("Загружена: %s\n", ws.name);
        }
    }

    fclose(f);
    return loaded;
}

int saveToFile(char* fname) {
    if (rec_count == 0) {
        printf("База данных пуста\n");
        return 0;
    }


    FILE* f = fopen(fname, "w");
    if (!f) {
        printf("Не удалось создать файл: %s\n", fname);
        return -1;
    }

    for (int i = 0; i < rec_count; i++) {
        WebService* ws = &db[i];
        fprintf(f, "%s;%d;%s;%s;%d;%.2lf;%d;%.1f\n",
            ws->name, ws->type, ws->ver, ws->dev,
            ws->plat, ws->perf, ws->resp_time, ws->avail);
    }

    fclose(f);
    return rec_count;
}
