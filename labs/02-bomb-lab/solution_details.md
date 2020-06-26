# Bomb Lab Step-by-step Solutions
## Pre-knowledge
To finish this lab, you have to get familiar with the debugger tool, gdb. A few instructions are frequently used.
1. `break` 
The program stops execution when it hit the break_point.
    1. break function_name (for example, `break explode_bomb`)
    2. break *address (for example, `break *0x0000000000400ee9)`
2. `disas`
The short form of `disassemble`. It will show the assembly language of the executing function.
3. `stepi`
This instruction will help you step over one single instruction in the assembly language. You could also use `stepi [n]` to step over n instructions.
4. `finish`
This instruction will finish the current function and return back to the caller of the current function.
5. `x`
This instruction helps you to examine the value inside registers, locations and so on.
6. `c`
Type `c` in gdb will continue the exection of the program until it ends or hit another break_point.
7. `help`
In case you are not familiar with any command, type `help [instruction]` to get more information.

## Phase_1
1. We set a break_point at phase_1 by `break phase_1`.
2. Type in `disas`, the assembly language of the function is as follows:
```
(gdb) disas
Dump of assembler code for function phase_1:
=> 0x0000000000400ee0 <+0>:	    sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:	    mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:	    callq  0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:	test   %eax,%eax
   0x0000000000400ef0 <+16>:	je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:	callq  0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:	add    $0x8,%rsp
   0x0000000000400efb <+27>:	retq
End of assembler dump.
```
Let's see what is going on here. It sets the `%esi` to the value `$0x402400` and calls the function `<strings_not_equal>`.  

As we know that `rsi` or `esi` is the second argument of the function. `rdi` is the first argument. We examine the values stored in `rdi` and `rsi` respectively by `x/s $rdi` and `x/s $rsi`. We could find out `rdi` stores the address of our input string. `rsi` stores the address of another string. In my bomb, the value is given by 
```
(gdb) x/s $rsi
0x402400:	"Border relations with Canada have never been better."
```

As we could imagine, the function check whether our input string is the same as the given string or not. Thus the answer of the phase_1 is the given string.

## Phase_2
```
(gdb) disas
Dump of assembler code for function phase_2:
=> 0x0000000000400efc <+0>:	    push   %rbp
   0x0000000000400efd <+1>:	    push   %rbx
   0x0000000000400efe <+2>:	    sub    $0x28,%rsp
   0x0000000000400f02 <+6>:	    mov    %rsp,%rsi
   0x0000000000400f05 <+9>:	    callq  0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:	cmpl   $0x1,(%rsp)
   0x0000000000400f0e <+18>:	je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:	callq  0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:	jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:	mov    -0x4(%rbx),%eax
   0x0000000000400f1a <+30>:	add    %eax,%eax
   0x0000000000400f1c <+32>:	cmp    %eax,(%rbx)
   0x0000000000400f1e <+34>:	je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:	callq  0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:	add    $0x4,%rbx
   0x0000000000400f29 <+45>:	cmp    %rbp,%rbx
   0x0000000000400f2c <+48>:	jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:	jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:	lea    0x4(%rsp),%rbx
   0x0000000000400f35 <+57>:	lea    0x18(%rsp),%rbp
   0x0000000000400f3a <+62>:	jmp    0x400f17 <phase_2+27>
   0x0000000000400f3c <+64>:	add    $0x28,%rsp
   0x0000000000400f40 <+68>:	pop    %rbx
   0x0000000000400f41 <+69>:	pop    %rbp
   0x0000000000400f42 <+70>:	retq
End of assembler dump.
```
If you step in to the function `read_six_numbers`, you will find out it only reads six numbers to the given address. In this case, it store the six nubmers in a row to the `rsi`, which is the `rsp` in `0x0000000000400f02 <+6>`. When the function returns from `read_six_numbers`, the first number is stored in `rsp`, the second in `rsp+0x4`. You could check it by `x/d $rsp`.

Let's look at `0x0000000000400f0a <+14>:	cmpl   $0x1,(%rsp)`. This instruction compares the first input number to 1. The following instruction `je     0x400f30 <phase_2+52>` check whether they are equal or not. If equal, continue execution, otherwise, `explode_bomb`.

Thus, we know that the first number it wants is `1`. How about the following numbers? The following code is a loop structure, It stores the next value at `rbx` and `-0x4(%rbx)` calculate the address of the current nubmer. It checks whether the next nubmer is two times of the current one. Once fail, it will `explode_bomb`. Thus we know that the six numbers the program wants are `1 2 4 8 16 32`.

## Phase_3
Let's first look at the first few lines of instructions.
```
=> 0x0000000000400f43 <+0>:	    sub    $0x18,%rsp
   0x0000000000400f47 <+4>:	    lea    0xc(%rsp),%rcx
   0x0000000000400f4c <+9>:	    lea    0x8(%rsp),%rdx
   0x0000000000400f51 <+14>:	mov    $0x4025cf,%esi
   0x0000000000400f56 <+19>:	mov    $0x0,%eax
   0x0000000000400f5b <+24>:	callq  0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:	cmp    $0x1,%eax
   0x0000000000400f63 <+32>:	jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:	callq  0x40143a <explode_bomb>
```
As we are getting quite familiar with the scanf function, the `rsi=0x4025cf` stores the format string, which is `"%d %d"` in my case. Thus, I know that phase 3 takes two integers as input. Next, the first input is stored in the address of `rdx`, the second is in `rcx`. The return value of the scanf is the number of input arguments. If it is greater than 1, jump to the address `<phase_3+39>`. Let's look at the remaining lines of instructions.

```
   0x0000000000400f6a <+39>:	cmpl   $0x7,0x8(%rsp)
   0x0000000000400f6f <+44>:	ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:	mov    0x8(%rsp),%eax
   0x0000000000400f75 <+50>:	jmpq   *0x402470(,%rax,8)
   0x0000000000400f7c <+57>:	mov    $0xcf,%eax
   0x0000000000400f81 <+62>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:	mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:	mov    $0x100,%eax
   0x0000000000400f8f <+76>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:	mov    $0x185,%eax
   0x0000000000400f96 <+83>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:	mov    $0xce,%eax
   0x0000000000400f9d <+90>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:	mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:	mov    $0x147,%eax
   0x0000000000400fab <+104>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:	callq  0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:	mov    $0x0,%eax
   0x0000000000400fb7 <+116>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:	mov    $0x137,%eax
   0x0000000000400fbe <+123>:	cmp    0xc(%rsp),%eax
   0x0000000000400fc2 <+127>:	je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:	callq  0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:	add    $0x18,%rsp
   0x0000000000400fcd <+138>:	retq
```

We could see the logic over here. First, it requries the first input to be less than or equal to 7. It will use the first input as an index to jump to an address using the command `jmpq *0x402470(,%rax,8)`. So the key here is to find out what is stored in the address `0x402470`.

```
(gdb) x/a 0x402470
0x402470:	0x400f7c <phase_3+57>
(gdb) x/a 0x402470+0x8
0x402478:	0x400fb9 <phase_3+118>
(gdb) x/a 0x402470+0x10
0x402480:	0x400f83 <phase_3+64>
```

Here is a few address stored following `0x402470`. We could see according to our first input, it will jump to different locations in the phase_3 function. It is like a switch structure. Here I choose the first input as `0`, so I have to see what is going on at `<phase_3+57>`. It assigns a value to `eax`, and check whether my second input is the same as the assigned value. If they are the same, return to the main function. There are different pairs you could use to pass phase_3. My choice is `0 207`.

## Phase_4
```
=> 0x000000000040100c <+0>:	    sub    $0x18,%rsp
   0x0000000000401010 <+4>:	    lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:	    lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:	mov    $0x4025cf,%esi
   0x000000000040101f <+19>:	mov    $0x0,%eax
   0x0000000000401024 <+24>:	callq  0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:	cmp    $0x2,%eax
   0x000000000040102c <+32>:	jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:	cmpl   $0xe,0x8(%rsp)
   0x0000000000401033 <+39>:	jbe    0x40103a <phase_4+46>
   0x0000000000401035 <+41>:	callq  0x40143a <explode_bomb>
   0x000000000040103a <+46>:	mov    $0xe,%edx
   0x000000000040103f <+51>:	mov    $0x0,%esi
   0x0000000000401044 <+56>:	mov    0x8(%rsp),%edi
   0x0000000000401048 <+60>:	callq  0x400fce <func4>
   0x000000000040104d <+65>:	test   %eax,%eax
   0x000000000040104f <+67>:	jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:	cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:	callq  0x40143a <explode_bomb>
   0x000000000040105d <+81>:	add    $0x18,%rsp
   0x0000000000401061 <+85>:	retq
```

The first half of the phase_4 is like the previous ones, scanf two numbers and stored in `rsp+0x8` and `rsp+0xc`. It use the first input number (requires to be smaller than 14), together with 0, 14 as the input the `func4`. After `func4` returns, it requires the return value to be 0. To find out what the first input number will be requires us to examine the details in `func4`. To find ou what the number would be for the second number is very easy, 0 in this case.

Let's look at the detail of `func4` to find out what will produce a zero return value.

```
   0x0000000000400fce <+0>:	    sub    $0x8,%rsp
   0x0000000000400fd2 <+4>:	    mov    %edx,%eax
   0x0000000000400fd4 <+6>:	    sub    %esi,%eax
=> 0x0000000000400fd6 <+8>:	    mov    %eax,%ecx
   0x0000000000400fd8 <+10>:	shr    $0x1f,%ecx
   0x0000000000400fdb <+13>:	add    %ecx,%eax
   0x0000000000400fdd <+15>:	sar    %eax
   0x0000000000400fdf <+17>:	lea    (%rax,%rsi,1),%ecx
   0x0000000000400fe2 <+20>:	cmp    %edi,%ecx
   0x0000000000400fe4 <+22>:	jle    0x400ff2 <func4+36>
   0x0000000000400fe6 <+24>:	lea    -0x1(%rcx),%edx
   0x0000000000400fe9 <+27>:	callq  0x400fce <func4>
   0x0000000000400fee <+32>:	add    %eax,%eax
   0x0000000000400ff0 <+34>:	jmp    0x401007 <func4+57>
   0x0000000000400ff2 <+36>:	mov    $0x0,%eax
   0x0000000000400ff7 <+41>:	cmp    %edi,%ecx
   0x0000000000400ff9 <+43>:	jge    0x401007 <func4+57>
   0x0000000000400ffb <+45>:	lea    0x1(%rcx),%esi
   0x0000000000400ffe <+48>:	callq  0x400fce <func4>
   0x0000000000401003 <+53>:	lea    0x1(%rax,%rax,1),%eax
   0x0000000000401007 <+57>:	add    $0x8,%rsp
   0x000000000040100b <+61>:	retq
```

By examine the code, it is a recursive function call. The code performs a easy algorithm, binary search. The `rdi` is the number to be searched, `rsi` is the lower bound, and `rdx` is the upper bound. If the input value is smaller than `mid_value = (upper_bound + lower_bound)/2`, set the upper bound `mid_value-1`. Else, set the lower bound to `mid_value+1`. To get `0` as the return value, there are multiple ways. The simplest way is to input `7`, thus the program returns with `0` immediately.

## Phase_5
```
=> 0x0000000000401062 <+0>:	    push   %rbx
   0x0000000000401063 <+1>:	    sub    $0x20,%rsp
   0x0000000000401067 <+5>:	    mov    %rdi,%rbx
   0x000000000040106a <+8>:	    mov    %fs:0x28,%rax
   0x0000000000401073 <+17>:	mov    %rax,0x18(%rsp)
   0x0000000000401078 <+22>:	xor    %eax,%eax
   0x000000000040107a <+24>:	callq  0x40131b <string_length>
   0x000000000040107f <+29>:	cmp    $0x6,%eax
   0x0000000000401082 <+32>:	je     0x4010d2 <phase_5+112>
   0x0000000000401084 <+34>:	callq  0x40143a <explode_bomb>
   0x0000000000401089 <+39>:	jmp    0x4010d2 <phase_5+112>
   0x000000000040108b <+41>:	movzbl (%rbx,%rax,1),%ecx
   0x000000000040108f <+45>:	mov    %cl,(%rsp)
   0x0000000000401092 <+48>:	mov    (%rsp),%rdx
   0x0000000000401096 <+52>:	and    $0xf,%edx
   0x0000000000401099 <+55>:	movzbl 0x4024b0(%rdx),%edx
   0x00000000004010a0 <+62>:	mov    %dl,0x10(%rsp,%rax,1)
   0x00000000004010a4 <+66>:	add    $0x1,%rax
   0x00000000004010a8 <+70>:	cmp    $0x6,%rax
   0x00000000004010ac <+74>:	jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:	movb   $0x0,0x16(%rsp)
   0x00000000004010b3 <+81>:	mov    $0x40245e,%esi
   0x00000000004010b8 <+86>:	lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:	callq  0x401338 <strings_not_equal>
   0x00000000004010c2 <+96>:	test   %eax,%eax
   0x00000000004010c4 <+98>:	je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:	callq  0x40143a <explode_bomb>
   0x00000000004010cb <+105>:	nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:	jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:	mov    $0x0,%eax
   0x00000000004010d7 <+117>:	jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:	mov    0x18(%rsp),%rax
   0x00000000004010de <+124>:	xor    %fs:0x28,%rax
   0x00000000004010e7 <+133>:	je     0x4010ee <phase_5+140>
   0x00000000004010e9 <+135>:	callq  0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:	add    $0x20,%rsp
   0x00000000004010f2 <+144>:	pop    %rbx
   0x00000000004010f3 <+145>:	retq
``` 
First, it reads the input string, and check whether the string length is 6 or not. If it is 6, continue execution. Then it steps into a loop structure.

```
   0x000000000040108b <+41>:	movzbl (%rbx,%rax,1),%ecx
   0x000000000040108f <+45>:	mov    %cl,(%rsp)
   0x0000000000401092 <+48>:	mov    (%rsp),%rdx
   0x0000000000401096 <+52>:	and    $0xf,%edx
   0x0000000000401099 <+55>:	movzbl 0x4024b0(%rdx),%edx
   0x00000000004010a0 <+62>:	mov    %dl,0x10(%rsp,%rax,1)
   0x00000000004010a4 <+66>:	add    $0x1,%rax
   0x00000000004010a8 <+70>:	cmp    $0x6,%rax
   0x00000000004010ac <+74>:	jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:	movb   $0x0,0x16(%rsp)
   0x00000000004010b3 <+81>:	mov    $0x40245e,%esi
   0x00000000004010b8 <+86>:	lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:	callq  0x401338 <strings_not_equal>
   0x00000000004010c2 <+96>:	test   %eax,%eax
   0x00000000004010c4 <+98>:	je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:	callq  0x40143a <explode_bomb>
   0x00000000004010cb <+105>:	nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:	jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:	mov    $0x0,%eax
   0x00000000004010d7 <+117>:	jmp    0x40108b <phase_5+41>
```

`rbx` stores the input string, `movzbl (%rbx,%rax,1),%ecx` stores the `rax`th char of `rbx` into `ecx`. The following operation checks the last byte of the `char`, and map the char to another char by the string stored in `0x4024b0`, which is `maduiersnfotvbyl`. If the last byte of the char is 0, it will map to `m`. `1` maps to `a` ... `f` maps to `l`.

After the mapping, it checks whether the mapped string is the same as the string stored in `0x40245e` or not. The string stored is `flyers`. So we have to decode it to get the input string. The index of `f` is `9` in `maduiersnfotvbyl`, so we have to input a char which has 9 as the least significant byte, `i` is `69` in ascii code, satisfying this condition. We perform this decoding for each char, and get the input string back, `ionefg`.

## Phase_6
The phase_6 function is quite long and performs multiple functionalities.
1. It reads six numbers. There is a nested-loop to check that every input number is different from each others.
2. ```
   0x0000000000401153 <+95>:	lea    0x18(%rsp),%rsi
   0x0000000000401158 <+100>:	mov    %r14,%rax
   0x000000000040115b <+103>:	mov    $0x7,%ecx
   0x0000000000401160 <+108>:	mov    %ecx,%edx
   0x0000000000401162 <+110>:	sub    (%rax),%edx
   0x0000000000401164 <+112>:	mov    %edx,(%rax)
   0x0000000000401166 <+114>:	add    $0x4,%rax
   0x000000000040116a <+118>:	cmp    %rsi,%rax
   0x000000000040116d <+121>:	jne    0x401160 <phase_6+108>
   ```
   It substitutes every input value `i` to `7-i`.
3. ```
   0x000000000040116f <+123>:	mov    $0x0,%esi
   0x0000000000401174 <+128>:	jmp    0x401197 <phase_6+163>
   0x0000000000401176 <+130>:	mov    0x8(%rdx),%rdx
   0x000000000040117a <+134>:	add    $0x1,%eax
   0x000000000040117d <+137>:	cmp    %ecx,%eax
   0x000000000040117f <+139>:	jne    0x401176 <phase_6+130>
   0x0000000000401181 <+141>:	jmp    0x401188 <phase_6+148>
   0x0000000000401183 <+143>:	mov    $0x6032d0,%edx
   0x0000000000401188 <+148>:	mov    %rdx,0x20(%rsp,%rsi,2)
   0x000000000040118d <+153>:	add    $0x4,%rsi
   0x0000000000401191 <+157>:	cmp    $0x18,%rsi
   0x0000000000401195 <+161>:	je     0x4011ab <phase_6+183>
   0x0000000000401197 <+163>:	mov    (%rsp,%rsi,1),%ecx
   0x000000000040119a <+166>:	cmp    $0x1,%ecx
   0x000000000040119d <+169>:	jle    0x401183 <phase_6+143>
   0x000000000040119f <+171>:	mov    $0x1,%eax
   0x00000000004011a4 <+176>:	mov    $0x6032d0,%edx
   0x00000000004011a9 <+181>:	jmp    0x401176 <phase_6+130>
    ```
    In `0x6032d0`, it stores a list of `structure node`. Each `node` stores a value and a pointer to the next `node`. This block of code re-order the nodes as the sequence of the converted input numbers.
4. The remaining instructions is relatively simple. It re-set the pointer of each node such that the pointer of each node points to the re-ordered next node.
5. Then, it compares the stored value of the re-ordered nodes. If it is in a monotonically decreasing sequence, the function returns successfully.
