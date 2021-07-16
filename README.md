# COLIBRI-ELF

this project it's an example of running elf binary in memory.
You can use two inputs.

input | syscalls
----|---
socket | socket, connect, recvfrom, mmap, SYS_memfd_create, ftruncate, SYS_execveat
binary | open, stat, close, mmap, SYS_memfd_create, ftruncate, SYS_execveat
--------

## usage

from binary
```shell
$ ./colibri-elf -s binary -f test.elf
[+] Loading ELF file...
[+] ELF Magic : 'ELF'
[+] ELF type : (3) 'EXEC'
[+] Program headers : '13'
[+] PT_LOAD segment at : 0x00000000
[+] PT_LOAD segment at : 0x00001000
[+] PT_LOAD segment at : 0x00002000
[+] PT_LOAD segment at : 0x00002db8
[+] Total PT_LOAD segment size '27005'
hello from another ELF !
```

from socket
```shell
$ ./colibri-elf -s socket -h 127.0.0.1 -p 5555
[+] starting connection at 127.0.0.1:5555
[+] Loading ELF file...
[+] ELF Magic : 'ELF'
[+] ELF type : (3) 'EXEC'
[+] Program headers : '13'
[+] PT_LOAD segment at : 0x00000000
[+] PT_LOAD segment at : 0x00001000
[+] PT_LOAD segment at : 0x00002000
[+] PT_LOAD segment at : 0x00002db8
[+] Total PT_LOAD segment size '27005'
hello from another ELF !
```