#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arp_parser/arp_parser.h"

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
    buffer = parse_hex_args(argc, argv, &data_size);
  }

  if (!buffer) {
    fprintf(stderr, "Ошибка: Не удалось получить данные.\n");
    return 1;
  }

  struct arp_packet packet;
  int res = parse_arp(buffer, data_size, &packet);

  if (res == 0) {
    print_arp(&packet);
  } else {
    handle_parse_error(res);
  }

  free(buffer);
  return (res == 0) ? 0 : 1;
}