Dump of assembler code for function phase_5:
   0x004013e8 <+0>:     addiu   sp,sp,-72
   0x004013ec <+4>:     sw      ra,68(sp)
   0x004013f0 <+8>:     sw      s8,64(sp)
   0x004013f4 <+12>:    move    s8,sp
   0x004013f8 <+16>:    sw      a0,72(s8） # break
   0x004013fc <+20>:    lw      a0,72(s8)

   0x00401400 <+24>:    jal     0x401c78 <string_length>
   0x00401404 <+28>:    nop
   0x00401408 <+32>:    move    v1,v0
   0x0040140c <+36>:    li      v0,6
   0x00401410 <+40>:    beq     v1,v0,0x401420 <phase_5+56> # the length of input_string is 6.
   0x00401414 <+44>:    nop
   0x00401418 <+48>:    jal     0x4021f0 <explode_bomb>
   0x0040141c <+52>:    nop
   0x00401420 <+56>:    sw      zero,24(s8)
   0x00401424 <+60>:    b       0x4014a8 <phase_5+192>


   0x00401428 <+64>:    nop
   0x0040142c <+68>:    lw      v0,24(s8) # int i
   0x00401430 <+72>:    lw      v1,24(s8)
   0x00401434 <+76>:    lw      a0,72(s8) # input_strings+320: a b c d e f
   0x00401438 <+80>:    nop
   0x0040143c <+84>:    addu    v1,a0,v1
   0x00401440 <+88>:    lb      v1,0(v1) # 读取1Byte，即1个字符
   0x00401444 <+92>:    nop
   0x00401448 <+96>:    andi    v1,v1,0xff
   0x0040144c <+100>:   andi    v1,v1,0xf # 提取后4位
   0x00401450 <+104>:   sll     v0,v0,0x2
   0x00401454 <+108>:   addiu   a0,s8,24
   0x00401458 <+112>:   addu    v0,a0,v0
   0x0040145c <+116>:   sw      v1,12(v0) # cal res 存入

   0x00401460 <+120>:   lw      a0,24(s8)
   0x00401464 <+124>:   lw      v0,24(s8)
   0x00401468 <+128>:   nop
   0x0040146c <+132>:   sll     v0,v0,0x2
   0x00401470 <+136>:   addiu   v1,s8,24
   0x00401474 <+140>:   addu    v0,v1,v0
   0x00401478 <+144>:   lw      v1,12(v0) # cal res 取出

   0x0040147c <+148>:   lui     v0,0x41
   0x00401480 <+152>:   addiu   v0,v0,12524 # 映射表 0 - 15

   0x00401484 <+156>:   addu    v0,v1,v0
   0x00401488 <+160>:   lb      v1,0(v0)
   0x0040148c <+164>:   addiu   v0,s8,24
   0x00401490 <+168>:   addu    v0,v0,a0
   0x00401494 <+172>:   sb      v1,4(v0)
   0x00401498 <+176>:   lw      v0,24(s8) # 自增
   0x0040149c <+180>:   nop
   0x004014a0 <+184>:   addiu   v0,v0,1
   0x004014a4 <+188>:   sw      v0,24(s8)


   0x004014a8 <+192>:   lw      v0,24(s8) # v0 = zero
   0x004014ac <+196>:   nop
   0x004014b0 <+200>:   slti    v0,v0,6 # 0 - 5
   0x004014b4 <+204>:   bnez    v0,0x40142c <phase_5+68>

   0x004014b8 <+208>:   nop
   0x004014bc <+212>:   sb      zero,34(s8) # break
   0x004014c0 <+216>:   addiu   v0,s8,28 # s8 + 28 save encrytion
   0x004014c4 <+220>:   move    a0,v0
   0x004014c8 <+224>:   lui     v0,0x40
   0x004014cc <+228>:   addiu   a1,v0,10160 # a1 = "gaints"
   0x004014d0 <+232>:   jal     0x401cf8 <strings_not_equal>
   0x004014d4 <+236>:   nop
   0x004014d8 <+240>:   beqz    v0,0x4014e8 <phase_5+256>
   0x004014dc <+244>:   nop
   0x004014e0 <+248>:   jal     0x4021f0 <explode_bomb>
   0x004014e4 <+252>:   nop
   0x004014e8 <+256>:   move    sp,s8
   0x004014ec <+260>:   lw      ra,68(sp)
   0x004014f0 <+264>:   lw      s8,64(sp)
   0x004014f4 <+268>:   addiu   sp,sp,72
   0x004014f8 <+272>:   jr      ra
   0x004014fc <+276>:   nop
End of assembler dump.


# 函数如其名：返回input_strings的长度

Dump of assembler code for function string_length:
   0x00401c78 <+0>:     addiu   sp,sp,-24
   0x00401c7c <+4>:     sw      s8,20(sp)
   0x00401c80 <+8>:     move    s8,sp
   0x00401c84 <+12>:    sw      a0,24(s8)
   0x00401c88 <+16>:    lw      v0,24(s8)
   0x00401c8c <+20>:    nop
   0x00401c90 <+24>:    sw      v0,12(s8)
   0x00401c94 <+28>:    sw      zero,8(s8)
   0x00401c98 <+32>:    b       0x401cb0 <string_length+56>
   0x00401c9c <+36>:    nop
   0x00401ca0 <+40>:    lw      v0,8(s8)
   0x00401ca4 <+44>:    nop
   0x00401ca8 <+48>:    addiu   v0,v0,1
   0x00401cac <+52>:    sw      v0,8(s8)
   0x00401cb0 <+56>:    lw      v0,12(s8)
   0x00401cb4 <+60>:    nop
   0x00401cb8 <+64>:    lb      v0,0(v0)
   0x00401cbc <+68>:    nop
   0x00401cc0 <+72>:    sltu    v0,zero,v0
   0x00401cc4 <+76>:    andi    v0,v0,0xff
   0x00401cc8 <+80>:    lw      v1,12(s8)
   0x00401ccc <+84>:    nop
   0x00401cd0 <+88>:    addiu   v1,v1,1
   0x00401cd4 <+92>:    sw      v1,12(s8)
   0x00401cd8 <+96>:    bnez    v0,0x401ca0 <string_length+40>
   0x00401cdc <+100>:   nop
   0x00401ce0 <+104>:   lw      v0,8(s8)
   0x00401ce4 <+108>:   move    sp,s8
   0x00401ce8 <+112>:   lw      s8,20(sp)
   0x00401cec <+116>:   addiu   sp,sp,24
   0x00401cf0 <+120>:   jr      ra
   0x00401cf4 <+124>:   nop
End of assembler dump.