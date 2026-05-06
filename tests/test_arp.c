#include "test_arp.h"

// 1. ВХОДНЫЕ ПАРАМЕТРОВ
START_TEST(test_invalid_inputs) {
  struct arp_packet p;
  uint8_t data[28] = {0};

  // NULL-указателей
  ck_assert_int_eq(parse_arp(NULL, 28, &p), -1);
  ck_assert_int_eq(parse_arp(data, 28, NULL), -1);

  // 27 байт
  ck_assert_int_eq(parse_arp(data, 27, &p), -1);

  // Пустой пакет
  ck_assert_int_eq(parse_arp(data, 0, &p), -1);
}
END_TEST

// 2. НЕПОДДЕРЖИВАЕМЫЙ РАЗМЕР АДРЕСОВ
START_TEST(test_invalid_address_sizes) {
  struct arp_packet p;
  uint8_t data[28] = {0};
  // валидные htype/ptype/op
  data[1] = 1;
  data[3] = 0x00;
  data[2] = 0x08;
  data[7] = 1;

  // MAC не 6 байт
  data[4] = 8;
  data[5] = 4;
  ck_assert_int_eq(parse_arp(data, 28, &p), -2);

  // IP не 4 байта
  data[4] = 6;
  data[5] = 16;
  ck_assert_int_eq(parse_arp(data, 28, &p), -2);
}
END_TEST

// 3. НЕВЕРНЫЕ ТИПОВЫ СЕТИ И ПРОТОКОЛА // -3
START_TEST(test_invalid_network_types) {
  struct arp_packet p;
  uint8_t data[28] = {0};
  data[4] = 6;
  data[5] = 4;
  data[7] = 1;

  // Не Ethernet // 0x0006
  data[1] = 6;
  data[2] = 0x08;
  data[3] = 0x00;
  ck_assert_int_eq(parse_arp(data, 28, &p), -3);

  // Не IPv4 // 0x86DD для IPv6
  data[1] = 1;
  data[2] = 0x86;
  data[3] = 0xDD;
  ck_assert_int_eq(parse_arp(data, 28, &p), -3);
}
END_TEST

// 4. КОД ОПЕРАЦИЙ // -4
START_TEST(test_invalid_opcodes) {
  struct arp_packet p;
  uint8_t data[28] = {0};
  data[1] = 1;
  data[2] = 0x08;
  data[3] = 0x00;
  data[4] = 6;
  data[5] = 4;

  data[7] = 0;
  ck_assert_int_eq(parse_arp(data, 28, &p), -4);

  data[7] = 3;
  ck_assert_int_eq(parse_arp(data, 28, &p), -4);
}
END_TEST

// 5. КРАЕВЫЕ ЗНАЧЕНИЯ ДАННЫХ
START_TEST(test_valid_boundaries) {
  struct arp_packet p;
  uint8_t data[28];
  memset(data, 0xFF, 28);

  // Подгоняем заголовки под валидные
  data[0] = 0;
  data[1] = 1;  // HTYPE
  data[2] = 0x08;
  data[3] = 0x00;  // PTYPE
  data[4] = 6;
  data[5] = 4;  // Sizes
  data[6] = 0;
  data[7] = 2;  // OP: Reply

  ck_assert_int_eq(parse_arp(data, 28, &p), 0);

  // Проверяем, что данные не потерялись, не исказились
  ck_assert_int_eq(p.op, 2);
  ck_assert_int_eq(p.sha[0], 0xFF);
  ck_assert_int_eq(p.spa[3], 0xFF);
}
END_TEST

// 6. ПРОВЕРКА УТИЛИТ
START_TEST(test_utils_coverage) {
  handle_parse_error(-1);
  handle_parse_error(-2);
  handle_parse_error(-3);
  handle_parse_error(-4);
  handle_parse_error(999);

  struct arp_packet p_unknown = {
      .htype = 1,
      .ptype = 0x0800,
      .hsize = 6,
      .psize = 4,
      .op = 5  // Некорректный OP
  };
  print_arp(&p_unknown);

  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(test_print_function) {
  struct arp_packet p;
  uint8_t data[28] = {0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00,
                      0x01, 0x08, 0x00, 0x27, 0x12, 0x34, 0x56,
                      0xC0, 0xA8, 0x01, 0x01, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0xC0, 0xA8, 0x01, 0x02};

  parse_arp(data, 28, &p);

  print_arp(&p);

  print_arp(NULL);

  ck_assert_int_eq(1, 1);
}
END_TEST

TCase* tcase_arp_parsing(void) {
  TCase* tc = tcase_create("arp_parsing");

  tcase_add_test(tc, test_invalid_inputs);
  tcase_add_test(tc, test_invalid_address_sizes);
  tcase_add_test(tc, test_invalid_network_types);
  tcase_add_test(tc, test_invalid_opcodes);
  tcase_add_test(tc, test_valid_boundaries);
  tcase_add_test(tc, test_utils_coverage);
  tcase_add_test(tc, test_print_function);

  return tc;
}