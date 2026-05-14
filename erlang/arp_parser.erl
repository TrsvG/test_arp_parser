-module(arp_parser).
-export([
    parse/1, 
    print_arp/1, 
    test/0
]).

-record(arp_packet, {
    htype,
    ptype,
    hsize,
    psize,
    op,
    sha,
    spa,
    tha,
    tpa
}).

parse(Binary) ->
    case Binary of
        <<HType:16, PType:16, HSize:8, PSize:8, Op:16, 
          SHa:6/binary, SPa:4/binary, THa:6/binary, TPa:4/binary>> ->
        #arp_packet{
            htype = HType, 
            ptype = PType, 
            hsize = HSize, 
            psize = PSize, 
            op = Op,
            sha = SHa, 
            spa = SPa, 
            tha = THa, 
            tpa = TPa
            };
        _ -> 
            {error, incomplete_packet}
    end.

print_arp(#arp_packet{
    htype = HType,
    ptype = PType,
    hsize = HSize,
    psize = PSize,
    op = Op,
    sha = SHa,
    spa = SPa,
    tha = THa,
    tpa = TPa
}) ->
    % Функция вывода record
    io:format("ARP Packet:~n"),
    io:format("  Hardware type: ~p~n", [HType]),
    io:format("  Protocol type: 0x~4.16.0B~n", [PType]),
    io:format("  Hardware size: ~p~n", [HSize]),
    io:format("  Protocol size: ~p~n", [PSize]),
    io:format("  Opcode: ~p~n", [Op]),
    io:format("  Sender MAC: ~s~n", [io_lib:format("~.16B:~.16B:~.16B:~.16B:~.16B:~.16B", binary_to_list(SHa))]),
    io:format("  Sender IP: ~p~n", [SPa]),
    io:format("  Target MAC: ~s~n", [io_lib:format("~.16B:~.16B:~.16B:~.16B:~.16B:~.16B", binary_to_list(THa))]),
    io:format("  Target IP: ~p~n", [TPa]),
    ok.

    test() ->
    TestPacket = <<
    16#00, 16#01, 16#08, 16#00, 16#06, 16#04, 16#00, 16#01,
    16#08, 16#00, 16#27, 16#12, 16#34, 16#56, 16#C0, 16#A8,
    16#01, 16#01, 16#00, 16#00, 16#00, 16#00, 16#00, 16#00,
    16#C0, 16#A8, 16#01, 16#02
    >>,
    case parse(TestPacket) of
        {error, Reason} -> io:format("error: ~p~n", [Reason]);
        Packet -> print_arp(Packet)
    end.