#ifndef ARP_PARSER_H_
#define ARP_PARSER_H_

#include <stddef.h>
#include <stdint.h>

#define ARP_MIN_LEN 28

struct arp_packet {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hsize;
  uint8_t psize;
  uint16_t op;
  uint8_t sha[6];
  uint8_t spa[4];
  uint8_t tha[6];
  uint8_t tpa[4];
};

int parse_arp(const uint8_t *data, size_t len, struct arp_packet *packet);
uint8_t *read_file(const char *filename, size_t *out_size);
void handle_parse_error(int error_code);
void print_arp(const struct arp_packet *packet);

#endif  // ARP_PARSER_H_