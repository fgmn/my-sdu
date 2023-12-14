   0x00400ed4 <+0>:     addiu   sp,sp,-56
   0x00400ed8 <+4>:     sw      ra,52(sp)
   0x00400edc <+8>:     sw      s8,48(sp)
   0x00400ee0 <+12>:    move    s8,sp
   0x00400ee4 <+16>:    lui     gp,0x42
   0x00400ee8 <+20>:    addiu   gp,gp,-20080
   0x00400eec <+24>:    sw      gp,24(sp)
   0x00400ef0 <+28>:    sw      a0,56(s8)
   0x00400ef4 <+32>:    lw      a0,56(s8)

   0x00400ef8 <+36>:    lui     v0,0x40    
   0x00400efc <+40>:    addiu   a1,v0,10112     #a1中存了未知的地址 //通过阅读给出的.s源码，可猜测这一段是给出了标准输入的格式
   0x00400f00 <+44>:    addiu   v1,s8,44        #s8+44 指向<input_numbers+160>，从此向后存储着本次输入的内容 //推测每次的输入均保存在<input_strings>中一个80字节的空间里
   0x00400f04 <+48>:    addiu   v0,s8,40        #s8+40 不知道是什么//经实验为输入的字符串的长度（包括'\0'）
   0x00400f08 <+52>:    addiu   a2,s8,36        #s8+36 这个应该是输入的数据存储的起点
   0x00400f0c <+56>:    sw      a2,16(sp)       #sp+16中存入了s8+36的地址
   0x00400f10 <+60>:    move    a2,v1           #a2=v1 a2设为存储本次输入的字符串的首地址
   0x00400f14 <+64>:    move    a3,v0           #a3=v0 a3设为输入的字符串的长度（包括'\0'）
   0x00400f18 <+68>:    lw      v0,-32636(gp)   #读入了一个地址，存着15，经对比发现是%call16(__isoc99_scanf)(gp)
   0x00400f1c <+72>:    nop
   0x00400f20 <+76>:    move    t9,v0           #t9=v0=15
   0x00400f24 <+80>:    jalr    t9              #跳转到这个未知的地址，然后一路运行，不知道什么时候，怎样的莫名其妙返回到0x00400f2c,可以使用形如“disas 0x 0x”来查看其中存储的语句
   0x00400f28 <+84>:    nop                     #下次记得在0x7f665190处设置断点（在phase_3开始运行后！），看看这里是否回跳转回来
                                                #似乎执行phase_2时也会到0x7f665190此处
                                                #徒劳追踪了半天，后来翻给出的源码，好家伙，正是系统的输入函数！！！！或者说，是对输入内容的格式化处理函数
   0x00400f2c <+88>:    lw      gp,24(s8)       #取回保存的全局指针的值
   0x00400f30 <+92>:    slti    v0,v0,3         #比较v0与3的大小，小于3置1，观察源码中给出的输入格式，可知应该输入三个东西//"%d %c %d\000"，也就是说输入应该>=3,v0恰好为输入的返回值，即输入元素数
   0x00400f34 <+96>:    beqz    v0,0x400f48 <phase_3+116> #等于0,跳转
   0x00400f38 <+100>:   nop
   0x00400f3c <+104>:   jal     0x4021f0 <explode_bomb>
   0x00400f40 <+108>:   nop
   0x00400f44 <+112>:   lw      gp,24(s8)
   0x00400f48 <+116>:   lw      v0,44(s8)       #加载输入第一个数，这里倒序存储，输入1 b 214，sp+36~sp+44存214 b 1//看之前这里应该是存第一个的地址啊，不知道怎么变化的，回头追踪一下sp-->后来发现实在%call16()中变化的
   0x00400f4c <+120>:   nop
   0x00400f50 <+124>:   sltiu   v1,v0,8         #比较输入第一个数与8的关系,小于则不跳转，大于则跳转引爆
   0x00400f54 <+128>:   beqz    v1,0x401190 <phase_3+700> #跳到+700，发现会引爆炸弹
   0x00400f58 <+132>:   nop
   0x00400f5c <+136>:   sll     v1,v0,0x2       #v0<<2
   0x00400f60 <+140>:   lui     v0,0x40
   0x00400f64 <+144>:   addiu   v0,v0,10124     #v0中存储了0x4028c,其指向一条指令的地址（就在phase_3里，后面行处）
   0x00400f68 <+148>:   addu    v0,v1,v0        #v0=v0+v1 （跳转和输入有关，感觉有点switch的味道）
   0x00400f6c <+152>:   lw      v0,0(v0)
   0x00400f70 <+156>:   nop
   0x00400f74 <+160>:   jr      v0              #跳转到v0所存的地址所指的地址（执行指令）
############################################################################
#0x40278c:       0x00400f7c      0x00400fc0      0x00401004      0x00401048
#0x40279c:       0x00401088      0x004010cc      0x00401110      0x0040114c
#可以看到，从v0一开始赋予的初值开始，向后连续存了8个跳转的位置，这恰好对应0~7这8中输入（这也解释了为什么会判断我们的输入是否<8）
############################################################################
   0x00400f78 <+164>:   nop
@0 0x00400f7c <+168>:   li      v0,113
   0x00400f80 <+172>:   sb      v0,32(s8)
   0x00400f84 <+176>:   lw      v0,-32660(gp)
   0x00400f88 <+180>:   nop
   0x00400f8c <+184>:   lw      v1,44(v0)
   0x00400f90 <+188>:   lw      v0,36(s8)
   0x00400f94 <+192>:   nop
   0x00400f98 <+196>:   mult    v1,v0
   0x00400f9c <+200>:   mflo    v1
   0x00400fa0 <+204>:   li      v0,777
   0x00400fa4 <+208>:   beq     v1,v0,0x4011ac <phase_3+728>
   0x00400fa8 <+212>:   nop
   0x00400fac <+216>:   jal     0x4021f0 <explode_bomb>
   0x00400fb0 <+220>:   nop
   0x00400fb4 <+224>:   lw      gp,24(s8)
   0x00400fb8 <+228>:   b       0x4011f8 <phase_3+804>
   0x00400fbc <+232>:   nop
@1 0x00400fc0 <+236>:   li      v0,98           #v0=98=0x62
   0x00400fc4 <+240>:   sb      v0,32(s8)       #s8+32十六进制数的后两位更改为62（按字节更改）
   0x00400fc8 <+244>:   lw      v0,-32660(gp)   #存入了地址，指向<ID_num>
   0x00400fcc <+248>:   nop
   0x00400fd0 <+252>:   lw      v1,44(v0)       #提取ID_num的最后一位
   0x00400fd4 <+256>:   lw      v0,36(s8)       #存的是本次输入的第一位
   0x00400fd8 <+260>:   nop
   0x00400fdc <+264>:   mult    v1,v0           #相乘，跳转
   0x00400fe0 <+268>:   mflo    v1              #存到v1
   0x00400fe4 <+272>:   li      v0,214          #v0=214
   0x00400fe8 <+276>:   beq     v1,v0,0x4011b8 <phase_3+740>   #比较v1与214
   0x00400fec <+280>:   nop
   0x00400ff0 <+284>:   jal     0x4021f0 <explode_bomb>
   0x00400ff4 <+288>:   nop
   0x00400ff8 <+292>:   lw      gp,24(s8)
   0x00400ffc <+296>:   b       0x4011f8 <phase_3+804>
   0x00401000 <+300>:   nop
@2 0x00401004 <+304>:   li      v0,98
   0x00401008 <+308>:   sb      v0,32(s8)
   0x0040100c <+312>:   lw      v0,-32660(gp)
   0x00401010 <+316>:   nop
   0x00401014 <+320>:   lw      v1,44(v0)
   0x00401018 <+324>:   lw      v0,36(s8)
   0x0040101c <+328>:   nop
   0x00401020 <+332>:   mult    v1,v0
   0x00401024 <+336>:   mflo    v1
   0x00401028 <+340>:   li      v0,755
   0x0040102c <+344>:   beq     v1,v0,0x4011c4 <phase_3+752>
   0x00401030 <+348>:   nop
   0x00401034 <+352>:   jal     0x4021f0 <explode_bomb>
   0x00401038 <+356>:   nop
   0x0040103c <+360>:   lw      gp,24(s8)
   0x00401040 <+364>:   b       0x4011f8 <phase_3+804>
   0x00401044 <+368>:   nop
@3 0x00401048 <+372>:   li      v0,107
   0x0040104c <+376>:   sb      v0,32(s8)
   0x00401050 <+380>:   lw      v0,-32660(gp)
   0x00401054 <+384>:   nop
   0x00401058 <+388>:   lw      v1,44(v0)
   0x0040105c <+392>:   lw      v0,36(s8)
   0x00401060 <+396>:   nop
   0x00401064 <+400>:   mult    v1,v0
   0x00401068 <+404>:   mflo    v0
   0x0040106c <+408>:   beqz    v0,0x4011d0 <phase_3+764>
   0x00401070 <+412>:   nop
   0x00401074 <+416>:   jal     0x4021f0 <explode_bomb>
   0x00401078 <+420>:   nop
   0x0040107c <+424>:   lw      gp,24(s8)
   0x00401080 <+428>:   b       0x4011f8 <phase_3+804>
   0x00401084 <+432>:   nop
@4 0x00401088 <+436>:   li      v0,111
   0x0040108c <+440>:   sb      v0,32(s8)
   0x00401090 <+444>:   lw      v0,-32660(gp)
   0x00401094 <+448>:   nop
   0x00401098 <+452>:   lw      v1,44(v0)
   0x0040109c <+456>:   lw      v0,36(s8)
   0x004010a0 <+460>:   nop
   0x004010a4 <+464>:   mult    v1,v0
   0x004010a8 <+468>:   mflo    v1
   0x004010ac <+472>:   li      v0,228
   0x004010b0 <+476>:   beq     v1,v0,0x4011dc <phase_3+776>
   0x004010b4 <+480>:   nop
   0x004010b8 <+484>:   jal     0x4021f0 <explode_bomb>
   0x004010bc <+488>:   nop
   0x004010c0 <+492>:   lw      gp,24(s8)
   0x004010c4 <+496>:   b       0x4011f8 <phase_3+804>
   0x004010c8 <+500>:   nop
@5 0x004010cc <+504>:   li      v0,116
   0x004010d0 <+508>:   sb      v0,32(s8)
   0x004010d4 <+512>:   lw      v0,-32660(gp)
   0x004010d8 <+516>:   nop
   0x004010dc <+520>:   lw      v1,44(v0)
   0x004010e0 <+524>:   lw      v0,36(s8)
   0x004010e4 <+528>:   nop
   0x004010e8 <+532>:   mult    v1,v0
   0x004010ec <+536>:   mflo    v1
   0x004010f0 <+540>:   li      v0,513
   0x004010f4 <+544>:   beq     v1,v0,0x4011e8 <phase_3+788>
   0x004010f8 <+548>:   nop
   0x004010fc <+552>:   jal     0x4021f0 <explode_bomb>
   0x00401100 <+556>:   nop
   0x00401104 <+560>:   lw      gp,24(s8)
   0x00401108 <+564>:   b       0x4011f8 <phase_3+804>
   0x0040110c <+568>:   nop
@6 0x00401110 <+572>:   li      v0,118
   0x00401114 <+576>:   sb      v0,32(s8)
   0x00401118 <+580>:   lw      v0,-32660(gp)
   0x0040111c <+584>:   nop
   0x00401120 <+588>:   lw      v1,44(v0)
   0x00401124 <+592>:   lw      v0,36(s8)
   0x00401128 <+596>:   nop
   0x0040112c <+600>:   mult    v1,v0
   0x00401130 <+604>:   mflo    v1
   0x00401134 <+608>:   li      v0,780
   0x00401138 <+612>:   beq     v1,v0,0x40114c <phase_3+632>
   0x0040113c <+616>:   nop
   0x00401140 <+620>:   jal     0x4021f0 <explode_bomb>
   0x00401144 <+624>:   nop
   0x00401148 <+628>:   lw      gp,24(s8)
@7 0x0040114c <+632>:   li      v0,98
   0x00401150 <+636>:   sb      v0,32(s8)
   0x00401154 <+640>:   lw      v0,-32660(gp)
   0x00401158 <+644>:   nop
   0x0040115c <+648>:   lw      v1,44(v0)
   0x00401160 <+652>:   lw      v0,36(s8)
   0x00401164 <+656>:   nop
   0x00401168 <+660>:   mult    v1,v0
   0x0040116c <+664>:   mflo    v1
   0x00401170 <+668>:   li      v0,824
   0x00401174 <+672>:   beq     v1,v0,0x4011f4 <phase_3+800>
   0x00401178 <+676>:   nop
   0x0040117c <+680>:   jal     0x4021f0 <explode_bomb>
   0x00401180 <+684>:   nop
   0x00401184 <+688>:   lw      gp,24(s8)
   0x00401188 <+692>:   b       0x4011f8 <phase_3+804>
   0x0040118c <+696>:   nop
   0x00401190 <+700>:   li      v0,120
   0x00401194 <+704>:   sb      v0,32(s8)
   0x00401198 <+708>:   jal     0x4021f0 <explode_bomb>
   0x0040119c <+712>:   nop
   0x004011a0 <+716>:   lw      gp,24(s8)
   0x004011a4 <+720>:   b       0x4011f8 <phase_3+804>
   0x004011a8 <+724>:   nop
   0x004011ac <+728>:   nop
   0x004011b0 <+732>:   b       0x4011f8 <phase_3+804>
   0x004011b4 <+736>:   nop
   0x004011b8 <+740>:   nop
   0x004011bc <+744>:   b       0x4011f8 <phase_3+804>
   0x004011c0 <+748>:   nop
   0x004011c4 <+752>:   nop
   0x004011c8 <+756>:   b       0x4011f8 <phase_3+804>
   0x004011cc <+760>:   nop
   0x004011d0 <+764>:   nop
   0x004011d4 <+768>:   b       0x4011f8 <phase_3+804>
   0x004011d8 <+772>:   nop
   0x004011dc <+776>:   nop
   0x004011e0 <+780>:   b       0x4011f8 <phase_3+804>
   0x004011e4 <+784>:   nop
   0x004011e8 <+788>:   nop
   0x004011ec <+792>:   b       0x4011f8 <phase_3+804>
   0x004011f0 <+796>:   nop
   0x004011f4 <+800>:   nop
   0x004011f8 <+804>:   lb      v0,40(s8)          #倒数第二个输入的后一字节
   0x004011fc <+808>:   lb      v1,32(s8)          #若输入1，跳转的结果98
   0x00401200 <+812>:   nop
   0x00401204 <+816>:   beq     v1,v0,0x401218 <phase_3+836> #要求两者相等
   0x00401208 <+820>:   nop
   0x0040120c <+824>:   jal     0x4021f0 <explode_bomb>
   0x00401210 <+828>:   nop
   0x00401214 <+832>:   lw      gp,24(s8)
   0x00401218 <+836>:   move    sp,s8
   0x0040121c <+840>:   lw      ra,52(sp)
   0x00401220 <+844>:   lw      s8,48(sp)
   0x00401224 <+848>:   addiu   sp,sp,56
   0x00401228 <+852>:   jr      ra
   0x0040122c <+856>:   nop








   #一波奇怪的跳转，最后发现是输入函数，笑了，白花两小时
   0x7f665130:  lui     gp,0x15
   0x7f665134:  addiu   gp,gp,-29552
   0x7f665138:  addu    gp,gp,t9
   0x7f66513c:  addiu   sp,sp,-240
   0x7f665140:  lw      t9,-31888(gp)
   0x7f665144:  sw      s2,232(sp)
   0x7f665148:  sw      a2,248(sp)
   0x7f66514c:  li      a2,-1
   0x7f665150:  sw      a3,252(sp)
   0x7f665154:  move    a3,zero
   0x7f665158:  sw      zero,16(sp)
   0x7f66515c:  lw      s2,-27668(gp)
   0x7f665160:  sw      s1,228(sp)
   0x7f665164:  move    s1,a0
   0x7f665168:  addiu   a0,sp,48
   0x7f66516c:  sw      s0,224(sp)
   0x7f665170:  move    s0,a1
   0x7f665174:  sw      ra,236(sp)
   0x7f665178:  sw      gp,24(sp)
   0x7f66517c:  li      a1,0x8000
   0x7f665180:  sw      a0,36(sp)
   0x7f665184:  sw      zero,120(sp)
   0x7f665188:  lw      v0,0(s2)
   0x7f66518c:  sw      v0,220(sp)
   0x7f665190:  jalr    t9
   #跳转
   0x7f6916e0:  lui     gp,0x12
   0x7f6916e4:  addiu   gp,gp,-14624
   0x7f6916e8:  addu    gp,gp,t9
   0x7f6916ec:  addiu   sp,sp,-40
   0x7f6916f0:  lw      t9,-30812(gp)
   0x7f6916f4:  sw      s1,28(sp)
   0x7f6916f8:  move    s1,a0
   0x7f6916fc:  sw      s2,32(sp)
   0x7f691700:  move    s2,a2
   0x7f691704:  sw      s0,24(sp)
   0x7f691708:  sw      gp,16(sp)
   0x7f69170c:  sw      ra,36(sp)
   0x7f691710:  bal     0x7f691658
   #跳转
   0x7f691658:  lui     gp,0x12
   0x7f69165c:  addiu   gp,gp,-14488
   0x7f691660:  addu    gp,gp,t9
   0x7f691664:  lw      v0,-32656(gp)
   0x7f691668:  lui     v1,0xfbad
   0x7f69166c:  sw      zero,60(a0)
   0x7f691670:  or      a1,a1,v1
   0x7f691674:  sw      a1,0(a0)
   0x7f691678:  lw      v0,15784(v0)
   0x7f69167c:  beqz    v0,0x7f691688
   #跳转
   0x7f691688:  lw      v0,72(a0)
   0x7f69168c:  sw      zero,28(a0)
   0x7f691690:  sw      zero,32(a0)
   0x7f691694:  sw      zero,12(a0)
   0x7f691698:  sw      zero,4(a0)
   0x7f69169c:  sw      zero,8(a0)
   0x7f6916a0:  sw      zero,16(a0)
   0x7f6916a4:  sw      zero,20(a0)
   0x7f6916a8:  sw      zero,24(a0)
   0x7f6916ac:  sw      zero,52(a0)
   0x7f6916b0:  sw      zero,36(a0)
   0x7f6916b4:  sw      zero,40(a0)
   0x7f6916b8:  sw      zero,44(a0)
   0x7f6916bc:  sw      zero,48(a0)
   0x7f6916c0:  sh      zero,68(a0)
   0x7f6916c4:  beqz    v0,0x7f6916d8
   #跳转
   0x7f6916d8:  jr      ra
   #跳转
   0x7f691718:  bltz    s2,0x7f691774
   #跳转
   0x7f691774:  lw      ra,36(sp)
   0x7f691778:  li      v0,-1
   0x7f69177c:  lw      s2,32(sp)
   0x7f691780:  lw      s0,24(sp)
   0x7f691784:  sw      v0,92(s1)
   0x7f691788:  sw      zero,96(s1)
   0x7f69178c:  lw      s1,28(sp)
   0x7f691790:  jr      ra
   #跳转
   0x7f665198:  move    a3,zero
   0x7f66519c:  lw      gp,24(sp)
   0x7f6651a0:  move    a2,zero
   0x7f6651a4:  lw      a0,36(sp)
   0x7f6651a8:  move    a1,s1
   0x7f6651ac:  lw      v0,-31680(gp)
   0x7f6651b0:  lw      t9,-31880(gp)
   0x7f6651b4:  jalr    t9
   #跳转
   0x7f693218:  lui     gp,0x12
   0x7f69321c:  addiu   gp,gp,-21592
   0x7f693220:  addu    gp,gp,t9
   0x7f693224:  addiu   sp,sp,-48
   0x7f693228:  sw      gp,16(sp)
   0x7f69322c:  sw      s2,36(sp)
   0x7f693230:  move    s2,a3
   0x7f693234:  sw      s1,32(sp)
   0x7f693238:  move    s1,a1
   0x7f69323c:  sw      s0,28(sp)
   0x7f693240:  move    s0,a0
   0x7f693244:  sw      ra,44(sp)
   0x7f693248:  bnez    a2,0x7f6932bc
   0x7f69324c:  sw      s3,40(sp)
   0x7f693250:  lw      t9,-32464(gp)
   0x7f693254:  move    a1,zero
   0x7f693258:  bal     0x7f6a39b0
   #跳转
   0x7f6a39b0:  lui     gp,0x11
   0x7f6a39b4:  addiu   gp,gp,-23536
   0x7f6a39b8:  addu    gp,gp,t9
   0x7f6a39bc:  addiu   sp,sp,-32
   0x7f6a39c0:  sw      gp,16(sp)
   0x7f6a39c4:  sw      ra,28(sp)
   0x7f6a39c8:  bnez    a1,0x7f6a39f0
   0x7f6a39cc:  sw      s0,24(sp)
   0x7f6a39d0:  lw      t9,-32648(gp)
   0x7f6a39d4:  bal     0x7f69f9b0
   #跳转
   0x7f69f9b0:  andi    v0,a0,0x3
   0x7f69f9b4:  beqz    v0,0x7f69f9e8
   #
   0x7f69f9e8:  lui     t0,0xfefe
   0x7f69f9ec:  lui     a3,0x8080
   0x7f69f9f0:  ori     t0,t0,0xfeff
   0x7f69f9f4:  ori     a3,a3,0x8080
   0x7f69f9f8:  lw      a1,0(v0)
   0x7f69f9fc:  addu    v1,a1,t0
   0x7f69fa00:  nor     a1,zero,a1
   0x7f69fa04:  and     v1,v1,a1
   0x7f69fa08:  and     v1,v1,a3
   0x7f69fa0c:  beqz    v1,0x7f69fa44
   #
   0x7f69fa44:  b       0x7f69f9f8
   #
   0x7f69f9f8:  lw      a1,0(v0)
   0x7f69f9fc:  addu    v1,a1,t0
   0x7f69fa00:  nor     a1,zero,a1
   0x7f69fa04:  and     v1,v1,a1
   0x7f69fa08:  and     v1,v1,a3
   0x7f69fa0c:  beqz    v1,0x7f69fa44
   0x7f69fa10:  addiu   a2,v0,4
   0x7f69fa14:  lb      v1,-4(a2)
   0x7f69fa18:  beqz    v1,0x7f69f9e0
   0x7f69fa1c:  nop
   0x7f69fa20:  lb      v1,-3(a2)
   0x7f69fa24:  beqz    v1,0x7f69fa58
   0x7f69fa28:  nop
   0x7f69fa2c:  lb      v1,-2(a2)
   0x7f69fa30:  beqz    v1,0x7f69fa64
   0x7f69fa34:  nop
   0x7f69fa38:  lb      v1,-1(a2)
   0x7f69fa3c:  beqz    v1,0x7f69fa4c
   #
   0x7f69fa4c:  subu    v0,v0,a0
   0x7f69fa50:  jr      ra
   #
   0x7f6a39dc:  lw      ra,28(sp)
   0x7f6a39e0:  addu    v0,s0,v0
   0x7f6a39e4:  lw      s0,24(sp)
   0x7f6a39e8:  jr      ra
   #
   0x7f693260:  lw      gp,16(sp)
   0x7f693264:  move    s3,v0
   0x7f693268:  lw      t9,-31220(gp)
   0x7f69326c:  move    a3,zero
   0x7f693270:  move    a2,s3
   0x7f693274:  move    a1,s1
   0x7f693278:  bal     0x7f690e44
   #
   0x7f690e44:  lui     gp,0x12
   0x7f690e48:  addiu   gp,gp,-12420
   0x7f690e4c:  addu    gp,gp,t9
   0x7f690e50:  addiu   sp,sp,-32
   0x7f690e54:  sw      s0,24(sp)
   0x7f690e58:  move    s0,a0
   0x7f690e5c:  sw      gp,16(sp)
   0x7f690e60:  sw      ra,28(sp)
   0x7f690e64:  lw      a0,28(a0)
   0x7f690e68:  beqz    a0,0x7f690e7c
   #
   0x7f690e7c:  sw      a1,28(s0)
   0x7f690e80:  beqz    a3,0x7f690ea4
   #
   0x7f690ea4:  lw      ra,28(sp)
   0x7f690ea8:  ori     v0,v0,0x1
   0x7f690eac:  sw      v0,0(s0)
   0x7f690eb0:  lw      s0,24(sp)
   0x7f690eb4:  jr      ra
   #
   0x7f693280:  sw      s1,16(s0)
   0x7f693284:  sw      s1,12(s0)
   0x7f693288:  beqz    s2,0x7f6932d4
   #
   0x7f6932d4:  lw      ra,44(sp)
   0x7f6932d8:  lw      s2,36(sp)
   0x7f6932dc:  sw      s1,20(s0)
   0x7f6932e0:  sw      s1,24(s0)
   0x7f6932e4:  sw      s3,8(s0)
   0x7f6932e8:  sw      zero,160(s0)
   0x7f6932ec:  lw      s3,40(sp)
   0x7f6932f0:  lw      s1,32(sp)
   0x7f6932f4:  lw      s0,28(sp)
   0x7f6932f8:  jr      ra
   #
   0x7f6651bc:  addiu   a2,sp,248
   0x7f6651c0:  lw      gp,24(sp)
   0x7f6651c4:  li      a3,2
   0x7f6651c8:  lw      a0,36(sp)
   0x7f6651cc:  move    a1,s0
   0x7f6651d0:  lw      t9,-31468(gp)
   0x7f6651d4:  bal     0x7f665a58
   #
   0x7f665a58:  lui     gp,0x15
   0x7f665a5c:  addiu   gp,gp,-31896
   0x7f665a60:  addu    gp,gp,t9
   0x7f665a64:  lw      v0,-27432(gp)
   0x7f665a68:  0x7c03e83b
   #
   0x7f665a58:  lui     gp,0x15
   0x7f665a5c:  addiu   gp,gp,-31896
   0x7f665a60:  addu    gp,gp,t9
   0x7f665a64:  lw      v0,-27432(gp)
   0x7f665a68:  0x7c03e83b
   0x7f665a6c:  addiu   sp,sp,-1576
   0x7f665a70:  lw      t1,-27668(gp)
   0x7f665a74:  addu    v0,v0,v1
   0x7f665a78:  sw      s8,1568(sp)
   0x7f665a7c:  move    s8,sp
   0x7f665a80:  sw      s5,1556(sp)
   0x7f665a84:  addiu   t2,s8,488
   0x7f665a88:  lw      v0,0(v0)
   0x7f665a8c:  move    s5,a0
   0x7f665a90:  lw      t0,0(t1)
   0x7f665a94:  sw      s0,1536(sp)
   0x7f665a98:  move    s0,a1
   0x7f665a9c:  sw      v0,152(s8)
   0x7f665aa0:  sw      ra,1572(sp)
   0x7f665aa4:  sw      s7,1564(sp)
   0x7f665aa8:  sw      s6,1560(sp)
   0x7f665aac:  sw      s4,1552(sp)
   0x7f665ab0:  sw      s3,1548(sp)
   0x7f665ab4:  sw      s2,1544(sp)
   0x7f665ab8:  sw      s1,1540(sp)
   0x7f665abc:  sw      gp,16(sp)
   0x7f665ac0:  lw      v0,0(v0)
   0x7f665ac4:  lb      v1,70(a0)
   0x7f665ac8:  sw      t1,112(s8)
   0x7f665acc:  sw      v0,52(s8)
   0x7f665ad0:  li      v0,1024
   0x7f665ad4:  sw      t2,108(s8)
   0x7f665ad8:  sw      t0,1532(s8)
   0x7f665adc:  sw      a2,76(s8)
   0x7f665ae0:  sw      a1,180(s8)
   0x7f665ae4:  sw      a3,1588(s8)
   0x7f665ae8:  sw      t2,480(s8)
   0x7f665aec:  sw      v0,484(s8)
   0x7f665af0:  bnez    v1,0x7f665b18
   0x7f665af4:  sw      a2,96(s8)
   0x7f665af8:  lw      v0,-27516(gp)
   0x7f665afc:  beqz    v0,0x7f6660a0
   0x7f665b00:  lw      t9,-31344(gp)
   0x7f665b04:  lw      v0,108(a0)
   0x7f665b08:  bnez    v0,0x7f6660b0
   #
   0x7f6660b0:  beq     v0,v1,0x7f665b18
   #
   0x7f665b18:  lw      v0,0(s5)
   0x7f665b1c:  andi    v1,v0,0x4
   0x7f665b20:  bnez    v1,0x7f668aec
   0x7f665b24:  nop
   0x7f665b28:  beqz    s0,0x7f66963c
   0x7f665b2c:  lw      v1,152(s8)
   0x7f665b30:  lw      a0,-32460(gp)
   0x7f665b34:  lw      v1,4(v1)
   0x7f665b38:  lw      a0,0(a0)
   0x7f665b3c:  lw      a1,40(v1)
   0x7f665b40:  lw      v1,36(v1)
   0x7f665b44:  sw      a0,104(s8)
   0x7f665b48:  sw      v1,56(s8)
   0x7f665b4c:  lb      v1,0(a1)
   0x7f665b50:  0x3280a
   0x7f665b54:  bnez    a0,0x7f665e10
   0x7f665b54:  bnez    a0,0x7f665e10
   0x7f665b58:  sw      a1,120(s8)
   0x7f665b5c:  lw      v1,-27696(gp)
   0x7f665b60:  sw      s5,192(s8)
   0x7f665b64:  sw      v1,188(s8)
   0x7f665b68:  andi    v0,v0,0x8000
   0x7f665b6c:  bnez    v0,0x7f665bcc
   #
   0x7f665bcc:  addiu   s7,s8,204
   0x7f665bd0:  sw      zero,204(s8)
   0x7f665bd4:  sw      zero,208(s8)
   0x7f665bd8:  lb      v1,0(v0)
   0x7f665bdc:  beqz    v1,0x7f667edc
   0x7f665be0:  move    s0,v0
   0x7f665be4:  lw      a0,-32592(gp)
   0x7f665be8:  move    s1,zero
   0x7f665bec:  move    s4,zero
   0x7f665bf0:  sw      s7,88(s8)
   0x7f665bf4:  move    s6,zero
   0x7f665bf8:  sw      zero,92(s8)
   0x7f665bfc:  sw      a0,44(s8)
   0x7f665c00:  move    s3,s4
   0x7f665c04:  lw      a0,-32732(gp)
   0x7f665c08:  move    s7,s1
   0x7f665c0c:  sw      zero,172(s8)
   0x7f665c10:  sw      zero,148(s8)
   0x7f665c14:  addiu   a0,a0,4172
   0x7f665c18:  sw      zero,132(s8)
   0x7f665c1c:  sw      zero,84(s8)
   0x7f665c20:  sw      a0,48(s8)
   0x7f665c24:  lw      a0,-31544(gp)
   0x7f665c28:  sw      zero,68(s8)
   0x7f665c2c:  sw      zero,72(s8)
   0x7f665c30:  addiu   a0,a0,6584
   0x7f665c34:  sw      a0,116(s8)
   0x7f665c38:  bltz    v1,0x7f665e34
   0x7f665c3c:  move    v1,v0
   0x7f665c40:  addiu   v0,v0,1
   0x7f665c44:  li      a0,37
   0x7f665c48:  sw      v0,180(s8)
   0x7f665c4c:  lbu     s1,0(v1)
   0x7f665c50:  beq     s1,a0,0x7f665e80
   #
   0x7f665e80:  lw      v0,480(s8)
   0x7f665e84:  lw      a0,484(s8)
   0x7f665e88:  sw      v0,472(s8)
   0x7f665e8c:  addu    v0,v0,a0
   0x7f665e90:  sw      v0,476(s8)
   0x7f665e94:  lb      v0,1(v1)
   0x7f665e98:  andi    v1,v0,0xff
   0x7f665e9c:  addiu   v1,v1,-48
   0x7f665ea0:  sltiu   v1,v1,10
   0x7f665ea4:  beqz    v1,0x7f666018
   #
   0x7f666018:  sw      zero,80(s8)
   0x7f66601c:  lw      a3,120(s8)
   0x7f666020:  move    a2,zero
   0x7f666024:  li      v1,42
   0x7f666028:  li      a0,39
   0x7f66602c:  li      a1,73
   0x7f666030:  beq     v0,v1,0x7f666048
   0x7f666034:  nop
   0x7f666038:  beq     v0,a0,0x7f666048
   0x7f66603c:  nop
   0x7f666040:  bne     v0,a1,0x7f665f28
   #
   0x7f665f28:  addiu   v0,v0,-48
   0x7f665f2c:  sltiu   v0,v0,10
   0x7f665f30:  bnez    v0,0x7f6661ec
   0x7f665f34:  sw      a2,144(s8)
   0x7f665f38:  addiu   v1,s0,1
   0x7f665f3c:  li      v0,-1
   0x7f665f40:  sw      v1,180(s8)
   0x7f665f44:  sw      v0,160(s8)
   0x7f665f48:  lbu     v0,0(s0)
   0x7f665f4c:  addiu   v0,v0,-76
   0x7f665f50:  andi    v0,v0,0xff
   0x7f665f54:  sltiu   a0,v0,47
   0x7f665f58:  bnez    a0,0x7f665f04
   #
   0x7f665f04:  sll     v0,v0,0x2
   0x7f665f08:  addiu   a0,a0,3984
   0x7f665f0c:  addu    v0,a0,v0
   0x7f665f10:  lw      v0,0(v0)
   0x7f665f14:  addu    v0,v0,gp
   0x7f665f18:  jr      v0
   #
   0x7f665f60:  sw      s0,180(s8)
   0x7f665f64:  move    v1,s0
   0x7f665f68:  lb      v0,0(s0)
   0x7f665f6c:  beqz    v0,0x7f665d4c
   0x7f665f70:  addiu   v0,v1,1
   0x7f665f74:  sw      v0,180(s8)
   0x7f665f78:  lb      s1,0(v1)
   0x7f665f7c:  bnez    s6,0x7f6662cc
   0x7f665f80:  andi    v0,s1,0xff
   0x7f665f84:  li      v1,91
   0x7f665f88:  beq     v0,v1,0x7f6668b8
   0x7f665f8c:  li      v1,99
   #
