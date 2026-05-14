#!/bin/bash
erlc arp_parser.erl
erl -noshell -s arp_parser test -s init stop