#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arp_parser/arp_parser.h"

uint8_t test_arp_packet[] = {0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00,
                             0x01, 0x08, 0x00, 0x27, 0x12, 0x34, 0x56,
                             0xC0, 0xA8, 0x01, 0x01, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0xC0, 0xA8, 0x01, 0x02};

int main() {
  struct arp_packet packet;

  size_t data_size = sizeof(test_arp_packet);

  int res = parse_arp(test_arp_packet, data_size, &packet);

  if (res == 0) {
    print_arp(&packet);
  } else {
    handle_parse_error(res);
  }

  return (res == 0) ? 0 : 1;
}
