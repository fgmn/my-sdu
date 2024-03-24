	.file	1 "shell.c"
gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"Invalid Command, Enter again, or try \"help\"\n\000"
	.align	2
$LC1:
	.ascii	"Command \"\000"
	.align	2
$LC2:
	.ascii	"\" Execute Completely.\n\000"
	.text
	.align	2
	.globl	main
	.ent	main
main:
	.frame	$fp,304,$31		# vars= 280, regs= 2/0, args= 16, extra= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,304
	sw	$31,300($sp)
	sw	$fp,296($sp)
	move	$fp,$sp
	jal	__main
	sw	$0,20($fp)
	li	$2,1			# 0x00000001
	sw	$2,24($fp)
	li	$2,78			# 0x0000004e
	sb	$2,32($fp)
	li	$2,97			# 0x00000061
	sb	$2,33($fp)
	li	$2,99			# 0x00000063
	sb	$2,34($fp)
	li	$2,104			# 0x00000068
	sb	$2,35($fp)
	li	$2,111			# 0x0000006f
	sb	$2,36($fp)
	li	$2,115			# 0x00000073
	sb	$2,37($fp)
	li	$2,36			# 0x00000024
	sb	$2,38($fp)
	li	$2,58			# 0x0000003a
	sb	$2,39($fp)
$L2:
	j	$L4
	j	$L3
$L4:
	addu	$2,$fp,32
	move	$4,$2
	li	$5,8			# 0x00000008
	lw	$6,24($fp)
	jal	Write
	sw	$0,272($fp)
	sw	$0,280($fp)
	sw	$0,284($fp)
	sw	$0,288($fp)
$L5:
	addu	$2,$fp,292
	move	$4,$2
	li	$5,1			# 0x00000001
	lw	$6,20($fp)
	jal	Read
	lw	$2,272($fp)
	addu	$3,$fp,48
	addu	$2,$3,$2
	lbu	$3,292($fp)
	sb	$3,0($2)
	addu	$2,$fp,284
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,112
	addu	$4,$5,$4
	lw	$5,272($fp)
	addu	$6,$fp,48
	addu	$5,$6,$5
	lbu	$6,0($5)
	sb	$6,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	lw	$2,280($fp)
	addu	$3,$2,1
	sw	$3,280($fp)
$L7:
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$6,$5,$4
	lb	$4,0($6)
	addu	$3,$3,1
	sw	$3,0($2)
	li	$2,10			# 0x0000000a
	bne	$4,$2,$L8
	j	$L6
$L8:
	j	$L5
$L6:
	lw	$2,284($fp)
	addu	$3,$2,-1
	move	$2,$3
	sw	$2,284($fp)
	addu	$3,$fp,112
	addu	$2,$3,$2
	sb	$0,0($2)
	lw	$2,272($fp)
	addu	$3,$2,-1
	move	$2,$3
	sw	$2,272($fp)
	addu	$3,$fp,48
	addu	$2,$3,$2
	li	$3,46			# 0x0000002e
	sb	$3,0($2)
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$4,$5,$4
	li	$5,46			# 0x0000002e
	sb	$5,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$4,$5,$4
	li	$5,110			# 0x0000006e
	sb	$5,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$4,$5,$4
	li	$5,111			# 0x0000006f
	sb	$5,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$4,$5,$4
	li	$5,102			# 0x00000066
	sb	$5,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	addu	$2,$fp,272
	lw	$3,0($2)
	move	$4,$3
	addu	$5,$fp,48
	addu	$4,$5,$4
	li	$5,102			# 0x00000066
	sb	$5,0($4)
	addu	$3,$3,1
	sw	$3,0($2)
	lw	$2,272($fp)
	addu	$3,$fp,48
	addu	$2,$3,$2
	sb	$0,0($2)
	lw	$2,280($fp)
	li	$3,1			# 0x00000001
	bne	$2,$3,$L9
	j	$L2
$L9:
	lw	$2,284($fp)
	blez	$2,$L10
	addu	$2,$fp,112
	move	$4,$2
	jal	Exec
	sw	$2,16($fp)
	lw	$2,16($fp)
	li	$3,-1			# 0xffffffff
	bne	$2,$3,$L11
	la	$4,$LC0
	li	$5,44			# 0x0000002c
	lw	$6,24($fp)
	jal	Write
	j	$L10
$L11:
	lw	$2,16($fp)
	li	$3,127			# 0x0000007f
	beq	$2,$3,$L10
	lw	$4,16($fp)
	jal	Join
	la	$4,$LC1
	li	$5,9			# 0x00000009
	lw	$6,24($fp)
	jal	Write
	addu	$2,$fp,112
	move	$4,$2
	lw	$5,280($fp)
	lw	$6,24($fp)
	jal	Write
	la	$4,$LC2
	li	$5,22			# 0x00000016
	lw	$6,24($fp)
	jal	Write
$L13:
$L12:
$L10:
	j	$L2
$L3:
$L1:
	move	$sp,$fp
	lw	$31,300($sp)
	lw	$fp,296($sp)
	addu	$sp,$sp,304
	j	$31
	.end	main
