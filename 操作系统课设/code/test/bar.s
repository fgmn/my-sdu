	.file	1 "bar.c"
gcc2_compiled.:
__gnu_compiled_c:
	.rdata
	.align	2
$LC0:
	.ascii	"../test/halt.noff\000"	;用户地址空间
	.text
	.align	2	;2字节对齐
	.globl	main	
	.ent	main
main:
	.frame	$fp,32,$31		# vars= 8, regs= 2/0, args= 16, extra= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	subu	$sp,$sp,32
	sw	$31,28($sp)
	sw	$fp,24($sp)	;将main函数返回地址和$fp入栈
	move	$fp,$sp	;将$fp指向栈顶，因为$sp在Exec调用时会修改
	jal	__main
	la	$4,$LC0	;将函数的前4个参数给子程序，不够的用堆栈
	jal	Exec	;PC+4->$31，转到start.s中Exec处执行
	sw	$2,16($fp)	保存函数返回值
	jal	Halt
$L1:
	move	$sp,$fp
	lw	$31,28($sp)
	lw	$fp,24($sp)	;从栈中恢复这两个数据
	addu	$sp,$sp,32	;释放main对应在栈中的frame，对应前面subu
	j	$31
	.end	main
