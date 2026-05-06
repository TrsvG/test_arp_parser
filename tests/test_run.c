#include <check.h>

#include "arp_parser.h"

TCase* tcase_arp_parsing(void);


int main() {
  Suite *s = suite_create("ARP_Parser_Suite");
  SRunner *sr = srunner_create(s);

  suite_add_tcase(s, tcase_arp_parsing());

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}