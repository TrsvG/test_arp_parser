#include "../include/arp_parser.h"

#include <arpa/inet.h>
#include <string.h>

int parse_arp(const uint8_t *data, size_t len, struct arp_packet *packet) {
  if (data == NULL || packet == NULL) return -1;
  if (len < ARP_MIN_LEN) return -1;

  uint16_t htype = ntohs(*(uint16_t *)(data + 0));
  uint16_t ptype = ntohs(*(uint16_t *)(data + 2));
  uint8_t hsize = data[4];
  uint8_t psize = data[5];
  uint16_t op = ntohs(*(uint16_t *)(data + 6));

  if (hsize != 6 || psize != 4) {
    return -2;
  }

  if (htype != 1 || ptype != 0x0800) {
    return -3;
  }

  if (op < 1 || op > 2) {
    return -4;
  }

  packet->htype = htype;
  packet->ptype = ptype;
  packet->hsize = hsize;
  packet->psize = psize;
  packet->op = op;

  memcpy(packet->sha, data + 8, 6);
  memcpy(packet->spa, data + 14, 4);
  memcpy(packet->tha, data + 18, 6);
  memcpy(packet->tpa, data + 24, 4);

  return 0;
}
