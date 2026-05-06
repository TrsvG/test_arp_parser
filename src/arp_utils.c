#include <stdio.h>

#include "arp_parser/arp_parser.h"

void handle_parse_error(int error_code) {
  printf("Ошибка парсинга (%d): ", error_code);

  switch (error_code) {
    case -1:
      printf("Недостаточно данных.\n");
      break;
    case -2:
      printf("Неподдерживаемый размер адресов.\n");
      break;
    case -3:
      printf("Неверный тип сети/протокола.\n");
      break;
    case -4:
      printf("Неизвестная операция ARP.\n");
      break;
    default:
      printf("Неизвестный код ошибки.\n");
  }
}

void print_arp(const struct arp_packet *packet) {
  if (packet == NULL) return;

  printf("  Hardware type: 0x%04X\n", packet->htype);
  printf("  Protocol type: 0x%04X\n", packet->ptype);
  printf("  Hardware size: %u\n", packet->hsize);
  printf("  Protocol size: %u\n", packet->psize);
  printf("  Opcode: %u (%s)\n", packet->op,
         (packet->op == 1)   ? "Request"
         : (packet->op == 2) ? "Reply"
                             : "Unknown");
  printf("  Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", packet->sha[0],
         packet->sha[1], packet->sha[2], packet->sha[3], packet->sha[4],
         packet->sha[5]);
  printf("  Sender IP: %u.%u.%u.%u\n", packet->spa[0], packet->spa[1],
         packet->spa[2], packet->spa[3]);
  printf("  Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", packet->tha[0],
         packet->tha[1], packet->tha[2], packet->tha[3], packet->tha[4],
         packet->tha[5]);
  printf("  Target IP: %u.%u.%u.%u\n", packet->tpa[0], packet->tpa[1],
         packet->tpa[2], packet->tpa[3]);
}