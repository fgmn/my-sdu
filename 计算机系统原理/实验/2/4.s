Dump of assembler code for function phase_4:
   0x004012bc <+0>:     addiu   sp,sp,-40
   0x004012c0 <+4>:     sw      ra,36(sp)
   0x004012c4 <+8>:     sw      s8,32(sp)
   0x004012c8 <+12>:    move    s8,sp # s8 = sp


   0x004012cc <+16>:    lui     gp,0x42 # break
   0x004012d0 <+20>:    addiu   gp,gp,-20080 # 0x41b190  zeros!
   0x004012d4 <+24>:    sw      gp,16(sp) # 16(sp) = gp
   0x004012d8 <+28>:    sw      a0,40(s8) # <input_strings+240>
   0x004012dc <+32>:    lw      v1,40(s8) # v1 = <input_strings+240>
   0x004012e0 <+36>:    lui     v0,0x40
   0x004012e4 <+40>:    addiu   v0,v0,10156 # v0 = 0x4027ac
   0x004012e8 <+44>:    move    a0,v1 # a0 = <input_strings+240>
   0x004012ec <+48>:    move    a1,v0 # a1 = 0x4027ac
   0x004012f0 <+52>:    addiu   v0,s8,24
   0x004012f4 <+56>:    move    a2,v0 # a2 = 15
   0x004012f8 <+60>:    lw      v0,-32636(gp)
   0x004012fc <+64>:    nop
   0x00401300 <+68>:    move    t9,v0
   0x00401304 <+72>:    jalr    t9 # t9 = 0x3c1c0015 jalr:寄存器的跳转指令


   0x00401308 <+76>:    nop # break
   0x0040130c <+80>:    lw      gp,16(s8)
   0x00401310 <+84>:    move    v1,v0
   0x00401314 <+88>:    li      v0,1
   0x00401318 <+92>:    bne     v1,v0,0x401330 <phase_4+116>
   0x0040131c <+96>:    nop
   0x00401320 <+100>:   lw      v0,24(s8) # now v0 = 5
   0x00401324 <+104>:   nop
   0x00401328 <+108>:   bgtz    v0,0x401340 <phase_4+132> # ？
   0x0040132c <+112>:   nop
   0x00401330 <+116>:   jal     0x4021f0 <explode_bomb>
   0x00401334 <+120>:   nop
   0x00401338 <+124>:   lw      gp,16(s8) # <+24>存入，现取出 
   0x0040133c <+128>:   nop
   0x00401340 <+132>:   lw      v0,-32660(gp)
   0x00401344 <+136>:   nop
   0x00401348 <+140>:   lw      v0,44(v0) # ID的最后一位

# ID numbers
(gdb) x /32d $v0
0x413264 <ID_num>:      -48     -48     -48     -48
0x413274 <ID_num+16>:   -48     -48     1       1
0x413284 <ID_num+32>:   1       1       1       1
0x413294 <input_strings>:       661939532       1700929651    544106855        561475438
0x4132a4 <input_strings+16>:    0       0       0       0
0x4132b4 <input_strings+32>:    0       0       0       0
0x4132c4 <input_strings+48>:    0       0       0       0
0x4132d4 <input_strings+64>:    0       0       0       0

   0x0040134c <+144>:   nop
   0x00401350 <+148>:   andi    v0,v0,0x1 # v0 = v0 & 0x00...01
   0x00401354 <+152>:   andi    v0,v0,0xff
   0x00401358 <+156>:   beqz    v0,0x40139c <phase_4+224> # 偶数时跳转

   # odd
   0x0040135c <+160>:   nop
   0x00401360 <+164>:   lw      v0,24(s8) # v0 = 5 input?
   0x00401364 <+168>:   nop
   0x00401368 <+172>:   move    a0,v0 # a0 = 5
   0x0040136c <+176>:   jal     0x401230 <func4> # ?     $v0 == $2
   0x00401370 <+180>:   nop
   0x00401374 <+184>:   lw      gp,16(s8)
   0x00401378 <+188>:   move    v1,v0 # v1 = 8
   0x0040137c <+192>:   li      v0,8
   0x00401380 <+196>:   beq     v1,v0,0x4013d0 <phase_4+276>
   0x00401384 <+200>:   nop
   0x00401388 <+204>:   jal     0x4021f0 <explode_bomb>
   0x0040138c <+208>:   nop
   0x00401390 <+212>:   lw      gp,16(s8)
   0x00401394 <+216>:   b       0x4013d0 <phase_4+276>
   0x00401398 <+220>:   nop

   # even
   0x0040139c <+224>:   lw      v0,24(s8)
   0x004013a0 <+228>:   nop
   0x004013a4 <+232>:   move    a0,v0
   0x004013a8 <+236>:   jal     0x401230 <func4>
   0x004013ac <+240>:   nop
   0x004013b0 <+244>:   lw      gp,16(s8)
   0x004013b4 <+248>:   move    v1,v0 # 0 1 2 3 4 5 6
   0x004013b8 <+252>:   li      v0,13 # 1 1 2 3 5 8 13 
   0x004013bc <+256>:   beq     v1,v0,0x4013d0 <phase_4+276>
   0x004013c0 <+260>:   nop
   0x004013c4 <+264>:   jal     0x4021f0 <explode_bomb>
   0x004013c8 <+268>:   nop

   0x004013cc <+272>:   lw      gp,16(s8)
   0x004013d0 <+276>:   move    sp,s8
   0x004013d4 <+280>:   lw      ra,36(sp)
   0x004013d8 <+284>:   lw      s8,32(sp)
   0x004013dc <+288>:   addiu   sp,sp,40
   0x004013e0 <+292>:   jr      ra
   0x004013e4 <+296>:   nop


   Dump of assembler code for function func4:
   0x00401230 <+0>:     addiu   sp,sp,-40
   0x00401234 <+4>:     sw      ra,36(sp)
   0x00401238 <+8>:     sw      s8,32(sp)
   0x0040123c <+12>:    sw      s0,28(sp)
   0x00401240 <+16>:    move    s8,sp

   0x00401244 <+20>:    sw      a0,40(s8) # input
   0x00401248 <+24>:    lw      v0,40(s8) # v0 = 5
   0x0040124c <+28>:    nop
   0x00401250 <+32>:    slti    v0,v0,2 # 5 4 3 2 1 执行5次 v0 = 1
   0x00401254 <+36>:    bnez    v0,0x40129c <func4+108> # 若n < 2,直接返回1
   0x00401258 <+40>:    nop
   0x0040125c <+44>:    lw      v0,40(s8)
   0x00401260 <+48>:    nop
   0x00401264 <+52>:    addiu   v0,v0,-1
   0x00401268 <+56>:    move    a0,v0
   0x0040126c <+60>:    jal     0x401230 <func4> # 递归调用自身 计算func4（5 - 1）
   0x00401270 <+64>:    nop
   0x00401274 <+68>:    move    s0,v0 # s0 = func4（5 - 1）
   0x00401278 <+72>:    lw      v0,40(s8)
   0x0040127c <+76>:    nop
   0x00401280 <+80>:    addiu   v0,v0,-2
   0x00401284 <+84>:    move    a0,v0
   0x00401288 <+88>:    jal     0x401230 <func4> # 计算func4（5 - 2）
   0x0040128c <+92>:    nop
   0x00401290 <+96>:    addu    v0,s0,v0 # func4(5) = func4(4) + func4(3)
   0x00401294 <+100>:   b       0x4012a0 <func4+112>
   0x00401298 <+104>:   nop
   0x0040129c <+108>:   li      v0,1 # 若n < 2,直接返回1
   
   0x004012a0 <+112>:   move    sp,s8
   0x004012a4 <+116>:   lw      ra,36(sp)
   0x004012a8 <+120>:   lw      s8,32(sp)
   0x004012ac <+124>:   lw      s0,28(sp)
   0x004012b0 <+128>:   addiu   sp,sp,40
   0x004012b4 <+132>:   jr      ra
   0x004012b8 <+136>:   nop
End of assembler dump.