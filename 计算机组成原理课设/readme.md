## 简单模型机的设计

Update：完整实验报告 [report](https://github.com/fgmn/Computer-Organization-Course-Design/blob/main/202000130143-%E9%83%91%E5%87%AF%E9%A5%B6.pdf)

#### Innovation Point

用自己设计的指令系统实现快速幂应用程序（ex3）。

#### 指令系统特点

①实现访存，四则运算，取模运算，与、或、非、异或等逻辑运算以及停机等等基本指令；

②实现基于有、无符号数大小比较的多种条件跳转指令，为模型机实现复杂功能打下基础；

③实现递归功能、循环功能的复杂指令集合；

④变量式指令系统，由于运算结果直接回写，汇编指令中可用变量名替代变量地址，对应地

址存储指定变量。

#### 设计总体结构和数据通路框图

![image-20230416113535231](https://github.com/fgmn/Computer-Organization-Course-Design/blob/main/img/image-20230416113535231.png)

#### 微程序实现的CU框图

![image-20230416113612313](https://github.com/fgmn/Computer-Organization-Course-Design/blob/main/img/image-20230416113612313.png)

#### 硬布线实现的CU框图

![image-20230416113642786](https://github.com/fgmn/Computer-Organization-Course-Design/blob/main/img/image-20230416113642786.png)
