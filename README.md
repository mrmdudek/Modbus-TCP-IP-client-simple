# Modbus-TCP-IP-client-simple
Modbus TCP/IP client simple. Written in C, compiled in Cygwin

Supported functions: Write_multiple_registers and read_holding_registers
Not implemented: write_multiple_coils and read_coils 

Instruction in Cygwin
0. open cygwin
1. go to directory with files
2. check if you have all files  (ls)
3. compile (gcc modbus_client.c  modbus_client_AP.c  modbus_client_TCP.c  socket_client.c)
4. open (./a.exe)
