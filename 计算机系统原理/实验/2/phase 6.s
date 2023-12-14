#一些常规的开栈，设置
   0x00401500 <+0>:     addiu   sp,sp,-96
   0x00401504 <+4>:     sw      ra,92(sp)
   0x00401508 <+8>:     sw      s8,88(sp)
   0x0040150c <+12>:    move    s8,sp
   0x00401510 <+16>:    lui     gp,0x42
   0x00401514 <+20>:    addiu   gp,gp,-20080
   0x00401518 <+24>:    sw      gp,16(sp)
   0x0040151c <+28>:    sw      a0,96(s8)
#正文开始
   0x00401520 <+32>:    lui     v0,0x41
   0x00401524 <+36>:    addiu   v0,v0,12592        #读取了node1
   0x00401528 <+40>:    sw      v0,32(s8)          #将node1存入32(s8)
   0x0040152c <+44>:    addiu   v0,s8,36           #v0=s8+36
   0x00401530 <+48>:    lw      a0,96(s8)          #a0存下入栈前栈顶指向的地址
   0x00401534 <+52>:    move    a1,v0              #将输入的6个数保存在a1=v0为起始的空间中，即(36)s8~(60)s8
   0x00401538 <+56>:    jal     0x401ba8 <read_six_numbers>
   0x0040153c <+60>:    nop
   0x00401540 <+64>:    lw      gp,16(s8)          #恢复全局指针gp
   0x00401544 <+68>:    sw      zero,28(s8)        28(s8)=0
   0x00401548 <+72>:    b       0x40163c <phase_6+316>
   0x0040154c <+76>:    nop

#判断输入的6是否有数字相等，相等则爆炸
{
#0<i<7
@2 0x00401550 <+80>:    lw      v0,28(s8)          #获取当前循环变量
   0x00401554 <+84>:    nop
   0x00401558 <+88>:    sll     v0,v0,0x2          #i*4
   0x0040155c <+92>:    addiu   v1,s8,24           #v1=24(s8)
   0x00401560 <+96>:    addu    v0,v1,v0           #v0=24(s8)+i*4
   0x00401564 <+100>:   lw      v0,12(v0)          #v0=*(v0+12)=*(s8+36+i*4) 即读取第i个数字
   0x00401568 <+104>:   nop
   0x0040156c <+108>:   slti    v0,v0,7            #判断v0是否<7
   0x00401570 <+112>:   beqz    v0,0x40159c <phase_6+156> #>=7则跳转,爆炸
   0x00401574 <+116>:   nop
   0x00401578 <+120>:   lw      v0,28(s8)          #获取循环遍量i
   0x0040157c <+124>:   nop
   0x00401580 <+128>:   sll     v0,v0,0x2          #i*4
   0x00401584 <+132>:   addiu   v1,s8,24           
   0x00401588 <+136>:   addu    v0,v1,v0 
   0x0040158c <+140>:   lw      v0,12(v0)          #v0=*(v0+12)=*(s8+36+i*4) 即读取第i个数字
   0x00401590 <+144>:   nop
   0x00401594 <+148>:   bgtz    v0,0x4015a8 <phase_6+168>   #v0>0则跳转，否则爆炸
   0x00401598 <+152>:   nop
   0x0040159c <+156>:   jal     0x4021f0 <explode_bomb>
   0x004015a0 <+160>:   nop
   0x004015a4 <+164>:   lw      gp,16(s8)
#j=i+1
@3 0x004015a8 <+168>:   lw      v0,28(s8)                   #获取循环变量
   0x004015ac <+172>:   nop
   0x004015b0 <+176>:   addiu   v0,v0,1                     #i++
   0x004015b4 <+180>:   sw      v0,24(s8)                   #存到24(s8)，j? or i+1?
   0x004015b8 <+184>:   b       0x401618 <phase_6+280>      #跳转
   0x004015bc <+188>:   nop
#判断array[i]与array[j]是否相等
@5 0x004015c0 <+192>:   lw      v0,28(s8)                   #获取循环变量
   0x004015c4 <+196>:   nop
   0x004015c8 <+200>:   sll     v0,v0,0x2                   #i*4
   0x004015cc <+204>:   addiu   v1,s8,24                    #v1=s8+24
   0x004015d0 <+208>:   addu    v0,v1,v0                    #v0=s8+24+i*4
   0x004015d4 <+212>:   lw      v1,12(v0)                   #v1=s8+36+i*4
   0x004015d8 <+216>:   lw      v0,24(s8)                   #获取循环变量j
   0x004015dc <+220>:   nop
   0x004015e0 <+224>:   sll     v0,v0,0x2                   #j*4
   0x004015e4 <+228>:   addiu   a0,s8,24              
   0x004015e8 <+232>:   addu    v0,a0,v0
   0x004015ec <+236>:   lw      v0,12(v0)                   #v0=*(s8+36+j*4)
   0x004015f0 <+240>:   nop
   0x004015f4 <+244>:   bne     v1,v0,0x401608 <phase_6+264>  #不相等，跳转264，否则爆炸（输入数字不得相等）
   0x004015f8 <+248>:   nop
   0x004015fc <+252>:   jal     0x4021f0 <explode_bomb>
   0x00401600 <+256>:   nop
   0x00401604 <+260>:   lw      gp,16(s8)
#j++
@6 0x00401608 <+264>:   lw      v0,24(s8)       #获取循环变量j
   0x0040160c <+268>:   nop
   0x00401610 <+272>:   addiu   v0,v0,1         #j++
   0x00401614 <+276>:   sw      v0,24(s8)       #存回24(s8)
#判断j<6确定是否循环结束
@4 0x00401618 <+280>:   lw      v0,24(s8)       #获取循环变量j
   0x0040161c <+284>:   nop
   0x00401620 <+288>:   slti    v0,v0,6         #j<6,跳转
   0x00401624 <+292>:   bnez    v0,0x4015c0 <phase_6+192>
   0x00401628 <+296>:   nop
   0x0040162c <+300>:   lw      v0,28(s8)       #j>=6,循环结束，读取i
   0x00401630 <+304>:   nop
   0x00401634 <+308>:   addiu   v0,v0,1
   0x00401638 <+312>:   sw      v0,28(s8)             #i++
@1 0x0040163c <+316>:   lw      v0,28(s8)             #存读取28(s8)
   0x00401640 <+320>:   nop
   0x00401644 <+324>:   slti    v0,v0,6               #v0<6，置为1
   0x00401648 <+328>:   bnez    v0,0x401550 <phase_6+80> #v0!=0，跳转（循环变量i<6，循环继续）
   0x0040164c <+332>:   nop
}
   0x00401650 <+336>:   sw      zero,28(s8)           #i=0       
   0x00401654 <+340>:   b       0x4016f8 <phase_6+504>
   0x00401658 <+344>:   nop
#这一段实现了对链表中指定结点的读取，相当于读取输入的数字指定位置的结点
#存的链表结点中的元素分别为：0xfd,0x2d5,0x12d,0x3e5,0xd4,0x1b0 按降序：4，2，6，3，1，5
{
   
@2 0x0040165c <+348>:   lui     v0,0x41               
   0x00401660 <+352>:   addiu   v0,v0,12592           #读入node1
   0x00401664 <+356>:   sw      v0,32(s8)             #存入32(s8)，emmm我记得之前读过一次啊
   0x00401668 <+360>:   li      v0,1                  #v0=1
   0x0040166c <+364>:   sw      v0,24(s8)             #j=1
   0x00401670 <+368>:   b       0x40169c <phase_6+412>
   0x00401674 <+372>:   nop
@4 0x00401678 <+376>:   lw      v0,32(s8)             #读入某个node(看来是链表了)
   0x0040167c <+380>:   nop
   0x00401680 <+384>:   lw      v0,8(v0)              #v0设为node1第3位的内容（猜测为nextNode）
   0x00401684 <+388>:   nop
   0x00401688 <+392>:   sw      v0,32(s8)             #将32(s8)设为v0=node1第3位的内容
   0x0040168c <+396>:   lw      v0,24(s8)             #j
   0x00401690 <+400>:   nop
   0x00401694 <+404>:   addiu   v0,v0,1
   0x00401698 <+408>:   sw      v0,24(s8)             #j++
@3 0x0040169c <+412>:   lw      v0,28(s8)             #i
   0x004016a0 <+416>:   nop
   0x004016a4 <+420>:   sll     v0,v0,0x2             #i*4
   0x004016a8 <+424>:   addiu   v1,s8,24              
   0x004016ac <+428>:   addu    v0,v1,v0
   0x004016b0 <+432>:   lw      v1,12(v0)             #v1=*(s8+36+i*4)
   0x004016b4 <+436>:   lw      v0,24(s8)             #v0=j
   0x004016b8 <+440>:   nop
   0x004016bc <+444>:   slt     v0,v0,v1              #j<*(s8+36+i*4)
   0x004016c0 <+448>:   bnez    v0,0x401678 <phase_6+376>   #若*(s8+36+i*4)>j,跳转，直到j>=*(s8+36+i*4)
#这里对读取到的结点进行存储
   0x004016c4 <+452>:   nop
   0x004016c8 <+456>:   lw      v0,28(s8)             #i       
   0x004016cc <+460>:   nop
   0x004016d0 <+464>:   sll     v0,v0,0x2             #i*4
   0x004016d4 <+468>:   addiu   v1,s8,24
   0x004016d8 <+472>:   addu    v0,v1,v0              #v0=s8+24+i*4
   0x004016dc <+476>:   lw      v1,32(s8)             #v1=*32(s8)，即当前结点的地址
   0x004016e0 <+480>:   nop
   0x004016e4 <+484>:   sw      v1,36(v0)             #当前结点存入s8+60+i*4,也就是说，会读取6个结点，分别存到60(s8)~80(s8)
   0x004016e8 <+488>:   lw      v0,28(s8)             #i
   0x004016ec <+492>:   nop
   0x004016f0 <+496>:   addiu   v0,v0,1               
   0x004016f4 <+500>:   sw      v0,28(s8)             #i++
@1 0x004016f8 <+504>:   lw      v0,28(s8) #获取循环变量i
   0x004016fc <+508>:   nop
   0x00401700 <+512>:   slti    v0,v0,6   
   0x00401704 <+516>:   bnez    v0,0x40165c <phase_6+348> #i<6跳转，循环6次
   0x00401708 <+520>:   nop
}
#这一段循环相当于将结点数组中的各个节点连为新的链表
{
   0x0040170c <+524>:   lw      v0,60(s8)                 #读取第一个链表
   0x00401710 <+528>:   nop
   0x00401714 <+532>:   sw      v0,32(s8)                 #存到32(s8) //相当于一个迭代器p
   0x00401718 <+536>:   li      v0,1                      #v0=1
   0x0040171c <+540>:   sw      v0,28(s8)                 #i=1
   0x00401720 <+544>:   b       0x40177c <phase_6+636>
   0x00401724 <+548>:   nop
   0x00401728 <+552>:   lw      v0,28(s8)                 #i
   0x0040172c <+556>:   nop
   0x00401730 <+560>:   sll     v0,v0,0x2                 #i*4
   0x00401734 <+564>:   addiu   v1,s8,24
   0x00401738 <+568>:   addu    v0,v1,v0
   0x0040173c <+572>:   lw      v1,36(v0)                 #v1=*(s8+i*4+60)
   0x00401740 <+576>:   lw      v0,32(s8)                 #v0=*(s8+32) 即迭代器p
   0x00401744 <+580>:   nop
   0x00401748 <+584>:   sw      v1,8(v0)                  #p->next=查找出的结点数组中下一个元素
   0x0040174c <+588>:   lw      v0,28(s8)                 #i
   0x00401750 <+592>:   nop
   0x00401754 <+596>:   sll     v0,v0,0x2                 #i*4
   0x00401758 <+600>:   addiu   v1,s8,24                  #
   0x0040175c <+604>:   addu    v0,v1,v0
   0x00401760 <+608>:   lw      v0,36(v0)                 #v0=*(s8+60+i*4)
   0x00401764 <+612>:   nop
   0x00401768 <+616>:   sw      v0,32(s8)                 #存到p
   0x0040176c <+620>:   lw      v0,28(s8)                 #i
   0x00401770 <+624>:   nop
   0x00401774 <+628>:   addiu   v0,v0,1                   
   0x00401778 <+632>:   sw      v0,28(s8)                 #i++
@1 0x0040177c <+636>:   lw      v0,28(s8)                   #i
   0x00401780 <+640>:   nop      
   0x00401784 <+644>:   slti    v0,v0,6                     #i<6
   0x00401788 <+648>:   bnez    v0,0x401728 <phase_6+552>   #小于6就一直循环
   0x0040178c <+652>:   nop
   0x00401790 <+656>:   lw      v0,32(s8)                   
   0x00401794 <+660>:   nop
   0x00401798 <+664>:   sw      zero,8(v0)                  #尾结点next置为0(NULL)
}

   0x0040179c <+668>:   lw      v0,60(s8)                   #找到头结点
   0x004017a0 <+672>:   nop
   0x004017a4 <+676>:   sw      v0,32(s8)                   #p指向header
   0x004017a8 <+680>:   sw      zero,28(s8)                 #i=0
   0x004017ac <+684>:   b       0x401878 <phase_6+888>
   0x004017b0 <+688>:   nop
@2 0x004017b4 <+692>:   lw      v0,-32660(gp)               #<ID_num>
   0x004017b8 <+696>:   nop
   0x004017bc <+700>:   lw      v0,44(v0)                   #ID_num的最后一位
   0x004017c0 <+704>:   nop
   0x004017c4 <+708>:   andi    v0,v0,0x1                   #+0x1
   0x004017c8 <+712>:   andi    v0,v0,0xff                  #+0xff      //判断奇偶，v0=奇：1 偶：2
   0x004017cc <+716>:   beqz    v0,0x401818 <phase_6+792>   #如果为偶则跳转
   0x004017d0 <+720>:   nop
   0x004017d4 <+724>:   lw      v0,32(s8)                   #p
   0x004017d8 <+728>:   nop
   0x004017dc <+732>:   lw      v1,0(v0)                    #v1=*p
   0x004017e0 <+736>:   lw      v0,32(s8)                   #p
   0x004017e4 <+740>:   nop
   0x004017e8 <+744>:   lw      v0,8(v0)                    #p->next
   0x004017ec <+748>:   nop
   0x004017f0 <+752>:   lw      v0,0(v0)                    #v0=*(p->next)
   0x004017f4 <+756>:   nop
   0x004017f8 <+760>:   slt     v0,v1,v0                    #比较*p与*(p->next)的大小
   0x004017fc <+764>:   beqz    v0,0x401854 <phase_6+852>   #如果小于关系，则爆炸
   0x00401800 <+768>:   nop
   0x00401804 <+772>:   jal     0x4021f0 <explode_bomb>
   0x00401808 <+776>:   nop
   0x0040180c <+780>:   lw      gp,16(s8)
   0x00401810 <+784>:   b       0x401854 <phase_6+852>
   0x00401814 <+788>:   nop
   0x00401818 <+792>:   lw      v0,32(s8)
   0x0040181c <+796>:   nop
   0x00401820 <+800>:   lw      v1,0(v0)
   0x00401824 <+804>:   lw      v0,32(s8)
   0x00401828 <+808>:   nop
   0x0040182c <+812>:   lw      v0,8(v0)
   0x00401830 <+816>:   nop
   0x00401834 <+820>:   lw      v0,0(v0)
   0x00401838 <+824>:   nop
   0x0040183c <+828>:   slt     v0,v0,v1        #这里判断的是*p->next与*p
   0x00401840 <+832>:   beqz    v0,0x401854 <phase_6+852>   #大于则跳转，过炸弹
   0x00401844 <+836>:   nop
   0x00401848 <+840>:   jal     0x4021f0 <explode_bomb>
   0x0040184c <+844>:   nop
   0x00401850 <+848>:   lw      gp,16(s8)
@3 0x00401854 <+852>:   lw      v0,32(s8)             #p       
   0x00401858 <+856>:   nop
   0x0040185c <+860>:   lw      v0,8(v0)              #p->next 
   0x00401860 <+864>:   nop
   0x00401864 <+868>:   sw      v0,32(s8)             #p=p->next
   0x00401868 <+872>:   lw      v0,28(s8)             #i
   0x0040186c <+876>:   nop
   0x00401870 <+880>:   addiu   v0,v0,1               #
   0x00401874 <+884>:   sw      v0,28(s8)             #i++
@1 0x00401878 <+888>:   lw      v0,28(s8)          #i
   0x0040187c <+892>:   nop
   0x00401880 <+896>:   slti    v0,v0,5            #i<5
   0x00401884 <+900>:   bnez    v0,0x4017b4 <phase_6+692>   #i小于5，循环继续，即循环5次
   0x00401888 <+904>:   nop
   0x0040188c <+908>:   move    sp,s8
   0x00401890 <+912>:   lw      ra,92(sp)
   0x00401894 <+916>:   lw      s8,88(sp)
   0x00401898 <+920>:   addiu   sp,sp,96
   0x0040189c <+924>:   jr      ra
   0x004018a0 <+928>:   nop