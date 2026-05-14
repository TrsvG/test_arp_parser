#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arp_parser/arp_parser.h"

// Функция для конвертации аргументов командной строки в массив байтов
uint8_t *parse_hex_args(int argc, char *argv[], size_t *out_size) {
    *out_size = (size_t)(argc - 1);
    uint8_t *buffer = malloc(*out_size);
    if (!buffer) return NULL;

    for (int i = 1; i < argc; i++) {
        // Преобразуем строку (например, "0x01", "08" или "10") в число
        buffer[i - 1] = (uint8_t)strtol(argv[i], NULL, 16);
    }
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Отсутствуют аргументы.\n");
        return 1;
    }

    uint8_t *buffer = NULL;
    size_t data_size = 0;

    if (argc == 2) {
        buffer = read_file(argv[1], &data_size);
    } else {
        // Если аргументов много — парсим их как байты
        printf("Чтение бинарных значений из терминала...\n");
        buffer = parse_hex_args(argc, argv, &data_size);
    }

    if (!buffer) {
        fprintf(stderr, "Ошибка: Не удалось получить данные для анализа.\n");
        return 1;
    }

    struct arp_packet packet;
    // Вызов вашей основной функции парсинга
    int res = parse_arp(buffer, data_size, &packet);

    if (res == 0) {
        print_arp(&packet);
    } else {
        handle_parse_error(res);
    }

    free(buffer);
    return (res == 0) ? 0 : 1;
}