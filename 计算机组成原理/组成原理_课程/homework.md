**Q1.在RISC指令集下函数调用流程**

流程主要分为以下几个步骤：

1.函数调用前的准备：将函数的参数从右往左的顺序传入寄存器或堆栈中；如果有需要，会将某些寄存器的值(如edi等)入栈；将函数返回地址入栈，以保证函数返回位置正确

2.执行调用指令，利用指令集中特定的分支指令跳转到目标函数的地址

3.通过修改帧指针和栈指针分配函数执行的栈空间，按需保存函数修改的寄存器原始值

4.进行函数的执行，在函数调用的尾声恢复寄存器旧值，释放栈空间，恢复原先的帧指针，利用返回指令跳转回调用者

5.清理栈空间(比如传递的栈参数)，并恢复栈中之前保存的临时寄存器

**Q1.1在RiscV指令集下在函数调用过程中所需要用到哪些寄存器呢？参数存放在哪里？返回地址呢？**

在RiscV指令集中，函数调用过程中所需要用到的寄存器如下：

1.a0-a7主要用于传递函数的参数，函数调用前传入的前八个整型参数用这些寄存器保存，其余的参数会通过栈传递；如果遇到浮点参数，则前八个参数利用fa0-fa7保存，剩下的浮点参数同样通过栈进行传递；返回值在较小结构体下会通过a0和a1返回

2.ra为返回地址寄存器，用于保存函数的返回地址，一般来说为当前的程序计数器+4的值

一般来说参数储存于寄存器或者栈中，返回地址通过特定的返回地址寄存器ra进行存储，除了这两类寄存器外，在函数调用全过程中，还会有一些寄存器频繁使用

3.sp为栈指针寄存器，记录栈帧中栈顶地址，调用者一般通过调整sp来分配栈空间

4.s0-s11为需要被调用者保存的寄存器

5.t0-t6为临时寄存器，若函数调用结束后其中的值仍需要使用，则需要由调用者保存它们

![a8609fd650b39e3c84368fb0fcbc3270](C:\Users\32096\OneDrive\Desktop\a8609fd650b39e3c84368fb0fcbc3270.png)

**Q1.2在RiscV指令集下函数调用具体是如何实现的？每个步骤是通过什么样的指令进行的？**

具体函数的调用大致可分为caller(调用函数)和callee(被调用函数)中的操作

caller中的步骤有：

（1）使用li指令将函数参数存入寄存器(a0-a7或者fa0-fa7)或利用sw指令将参数压入栈中；

（2）若是在调用后仍需要使用某些特定的寄存器(t0-t6)，那么需要caller将这些寄存器原先的数值保存下来，此处使用的是sw指令，将这些寄存器数值压入栈中

（3）跳转到被调用函数并将返回地址保存，在此使用jal指令跳转到目标函数中，并将返回地址存入ra

``````asm
jal ra my_function  //即调用my_function并将返回地址存于ra寄存器中
``````

callee中的步骤有：

（1）若是函数中需要调用其他函数，那么此时callee需要像caller一样保存某些可能仍需要使用的寄存器旧数值，这些数值一般存于s0-s11寄存器中，此处同样使用sw指令将这些寄存器数值压入栈中

（2）通过add指令(实际上一般为sub)调整栈指针sp来分配给定大小的栈空间，并通过a0-a7或栈空间访问参数来进行函数体内的计算，并通过mv(实际上一般为addi)或者算术指令将结果储存于a0（或 a1 和 fa0 等）中

（3）通过lw指令恢复callee中保存的寄存器和ra，并通过add释放栈空间，随后借jalr指令跳转回ra中的地址

在返回到调用者后，caller内部从a0/a1中读取返回值，并恢复保存的寄存器值(t0-t6)

![RISC-V 函数调用约定和Stack使用_riscv汇编为c语言函数设置程序栈-CSDN博客](https://img-blog.csdnimg.cn/23029fe1057046a5b2c48cc37784149e.png)

**Q1.2.1相较于MIPS指令集的函数调用，RiscV进行了哪些调整？**

1.RiscV使用了更灵活的寄存器组设计，在函数调用过程中，RiscV一般使用a0-a7进行函数参数的传递，ra来进行返回地址的储存，但实际上编译器可以灵活分配；而MIPS的寄存器有着固定的用途，用于参数传递和返回值保存的寄存器严格区分，调用者保存和被调用者保需要严格遵守规定，例如$s0至能由被调用者保存

2.RiscV支持压缩指令扩展的jalr，并且可以指定任意寄存器来储存返回地址，这样可以避免频繁保存ra

3.MIPS仅将前4个参数用寄存器传递，而RiscV扩展至8个，并且支持通过独立的浮点寄存器fa0-fa7来进行参数的传递，于整数寄存器a0-a7分离

4.RiscV有更灵活的栈操作，支持c.addi16sp快速调整栈指针，且没有对于栈帧调整的显式指令要求

5.在RiscV指令集下，若被调用的函数是叶子函数，则可跳过保存ra到栈中，直接通过ret（实际上为 `jalr x0, ra, 0`）返回。

6.RiscV取消了延迟槽并提供了jalr的独立指令，而MIPS的jal和分支延迟槽增加了复杂性

**Q1.2.2在RiscV指令集下，函数调用过程中用到了ret伪指令，使用ret伪指令有什么好处？相对于MIPS有什么区别和优势吗?**

为什么在RiscV中使用ret伪指令呢？一是因为ret相比于显式的`jr $ra`更加符合直觉，可以减少程序员的记忆负担，二是因为伪指令将会有更好的硬件兼容性，在不同的RiscV下实现中行为一致，而MIPS不同变种(如 MIPS32/64)可能有差异，三是因为ret伪指令支持叶子函数优化，可以直接使用ret而不保存ra

**Q1.2.1.1为什么叶子函数可以单独优化？RiscV指令集中对于叶子函数的优化具体是指什么？这个优化是借助什么实现的？这个优化在实际使用中有什么好处？**

首先，叶子函数是指不调用其他函数的函数。若是函数内部还需要调用其他函数的话，则在函数中必然会存在jal/jalr指令进行跳转，而由于跳转语句的执行，会覆盖ra寄存器(用于保留函数返回地址的寄存器)。而叶子函数并不会执行jal/jalr指令，则自然无需保存或者恢复ra寄存器的值，在函数仅使用寄存器且不需要保存临时数据时，甚至可以避免调整栈指针，因此可以对叶子函数进行特殊的优化

自然，对于叶子函数的优化围绕着叶子函数的特殊性进行：首先可以跳过对ra寄存器的保存，并直接使用ret指令(实际上来说为jalr x0，ra，0)跳转到ra存储的地址；其次，若在函数中仅使用临时寄存器t0-t6且没有局部变量存储到栈中，则可以不调整栈指针；

对叶子函数进行的优化，主要是依靠着现代RiscV编译器(例如GCC，Clang)进行实现的，这些编译器会自动检测叶子函数并对其进行应用优化，对于小型的叶子函数，甚至会进行内联展开，这样就会完全消除调用开销

这样子进行优化后，首先由于跳过了ra和栈操作，自然减少了对内存的访问，可以降低访存延迟；其次，提供专用 ret 指令简化返回逻辑；在仅使用寄存器时，允许完全避免栈操作，而这一系列优化都是通过RiscV的编译器自动应用优化，无需人为的调控

**Q1.2.1.1.1为什么MIPS不能对叶子函数进行特殊优化呢？是由于其什么样的特性造成的限制？**

不能对叶子函数进行特殊优化的主要原因，在于MIPS会强制性的保存返回地址，即使函数是叶子函数并未有保存返回地址的需求，MIPS 的jal指令会无条件覆盖 $ra寄存器（存储返回地址），而硬件没有提供跳过保存返回地址寄存器的机制。而RiscV的jal同样会修改ra，但通过编译器分析和优化，可跳过保存步骤。

另外，则是由于延迟槽的干扰。MIPS的跳转指令执行后，下一条指令会先执行再真正跳转，延迟槽的设计在此先不赘述，简单来说，是为了避免流水线停顿而设计的先执行跳转指令的下一条指令以填补流水线的空缺。而对于叶子函数而言，延迟槽的存在使得编译器难以安全移除 $ra的保存或恢复，这可能破坏延迟槽逻辑。而RiscV取消了延迟槽的设计，编译器则可自由的优化叶子函数。

**Q1.2.1.2RiscV为什么要设置单独的浮点寄存器？这在实际应用中有什么必要性？**

在MIPS早期版本中，浮点数和整数共用通用寄存器，会导致寄存器压力增大和频繁保存/恢复操作增加开销的问题，复杂的浮点运算可能占用大量寄存器，挤占整数运算的资源；在混合运算场景中（如整数和浮点交替操作），则需要频繁切换寄存器内容。自然想到的解决方案则是：设计独立的浮点寄存器，隔离浮点与整数数据流，减少资源冲突。

因此，在RiscV中设置了单独的浮点寄存器，其浮点支持通过可选扩展实现，分为F扩展(单精度32bit的float型数据)，D扩展(双精度64bit的double型数据)，Q 扩展(四精度浮点128bit），用户可以根据需求显式启用扩展。

在实际应用中，该优化通过编译器进行，编译器可分别为整数和浮点变量分配寄存器，减少溢出到内存的情况，有效避免整数与浮点运算的资源竞争，并且这种分离式的结构支持并行执行和专用浮点单元。

**Q1.2.1.3之前提到RiscV取消了延迟槽，什么是延迟槽？取消延迟槽起到了什么优化作用？MIPS为什么不对其进行优化？是有什么限制造成的吗？**

延迟槽是早期RISC架构（如MIPS、SPARC）中用于缓解控制指令流水线停顿的一种设计。当处理器执行分支/跳转指令时，由于指令需要经过取指、译码、执行等阶段，确定目标地址可能需要多个时钟周期，导致流水线出现停顿。延迟槽允许分支指令后面的1条指令（即延迟槽中的指令）无条件执行，无论分支是否成功，以此填充流水线的空闲周期。

取消延迟槽是由于现代处理器采用更深的流水线和高级分支预测技术（如动态分支预测），这导致了延迟槽的收益变低，还增加了硬件设计的复杂性。取消后硬件无需处理延迟槽指令的强制执行，减少了控制逻辑。并且，延迟槽要求程序员或编译器手动填充有效指令（如NOP），导致代码难以维护。取消后，指令顺序与逻辑完全一致，更符合直觉。编译器也无需再为延迟槽寻找有用的指令填充，降低了优化负担。

而MIPS之所以不对其进行优化，一是因为MIPS早期设计依赖延迟槽，后续架构为了保持二进制兼容性（如MIPS I至MIPS V），难以移除这一特性，二是因为大量现有代码（如嵌入式系统、传统编译器）已针对延迟槽优化，移除会导致性能回退或需大规模重写。毕竟，MIPS诞生于1980年代，当时分支预测技术的不成熟使得延迟槽的设计成为了较低成本的解决方案，而在现代处理器的高级分支预测技术下，RiscV取消延迟槽是自然的

**Q1.2.1.4为何提供jalr独立指令起到了优化作用？相较于jal指令来说，jalr指令适用于哪些情况？**

首先解释一下jal和jalr的区别：

jal使用固定偏移（PC-relative）跳转，适用于函数调用和短距离跳转，编码紧凑，jal的指令语句`jal ra my_function`的实际作用就是将当前程序计数器PC的值加上给定的偏移量，便可以定位到需要调用的函数my_function上，并且将PC+4，也就是下一条指令地址保存于ra寄存器下；而jalr采取的是寄存器间接跳转（基址 + 偏移），当目标地址数值与某个寄存器有关时，使用jalr更加便利；

可以发现，jal的目标地址计算仅依赖于程序计数器，而jalr的目标地址计算需要读取寄存器数值再进行计算，可能需要额外周期。将jalr单独设计后，硬件可以针对不同跳转类型优化流水线逻辑，根据当前目标选择更合适的指令jal/jalr，可以有效避免混合计算带来的额外延迟。

前面说过，jalr由于不依赖PC，更适合动态目标，例如RiscV的返回通常使用`jalr ra,0(ra)`，由于函数返回的目标地址是动态的(储存在ra寄存器中)，无法使用jal实现





