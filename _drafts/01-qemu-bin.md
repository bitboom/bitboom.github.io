## qemu에 바이너리 올리기
- Arm 코어에 전원이 들어오면, 리셋 벡터(reset vector)에 있는 명령어를 실행함
- 메모리 주소 0x0000에 있는 명령어 32bit를 바로 실행

## 0x00에 명령어를 넣어주기 - dummy 바이너리
```as
@ boot/Entry.S 
.text                  @ text section start
  .code 32             @ instruction size

  .global vector_start @ extern
  .global vector_end   @ extern

  vector_start:        @ label
    mov r0, r1         @ meaningless instruction
  vector_end:          @ label
    .space 1024, 0     @ fill 0 - 1024th
.end                   @ text section end
```

- 오브젝트 파일(*.o)는 심볼 + 바이너리
- `objcopy -O binary` 바이너리 추출
- Arm은 4바이트 단위 얼라인 -- XXX

```sh
$ arm-none-eabi-as -march=armv7-a -mcpu=cortex-a8 -o Entry.o ./Entry.S
$ arm-none-eabi-objcopy -O binary Entry.o Entry.bin
$ hexdump Entry.bin
0000000 01 00 a0 e1 00 00 00 00 00 00 00 00 00 00 00 00
0000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
0000400 00 00 00 00                                    
0000404
```

## qemu에 올릴 수 있도록 -> elf 포맷
- qemu가 펌웨어 파일을 읽어서 부팅하려면, 펌웨어 바이너리 파일이 elf 형식
- elf 파일을 만들려면 링커를 사용
- 링커는 **오브젝트 파일들을** 링킹시켜 하나의 실행파일로 만듦
- 링커 스크립트를 링커에 넘겨서 동작시킴

```ld
/* memory.ld */ 
ENTRY(vector_start)  /* set start symbol */
SECTIONS
{
  . = 0X0;

  .text :
  {
    *(vector_start)
    *(.text .rodata)
  }
  .data :
  {
    *(.data)
  }
  .bss :
  {
    *(.bss)
  }
}
```

```
$ arm-none-eabi-objdump -D kernel.axf

kernel.axf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <vector_start>:
   0:	e1a00001 	mov	r0, r1

00000004 <vector_end>:
	...
```

## gdb로 확인
```sh
$ qemu-system-arm -M realview-pb-a8 -kernel kernel.axf -S -gdb tcp::1234,ipv4
$ arm-none-eabi-gdb
(gdb) target remote:1234
0x00000000 in ?? ()
(gdb) x/4x 0
0x0:	0xe1a00001	0x00000000	0x00000000	0x00000000
```
