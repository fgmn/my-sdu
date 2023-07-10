# # 类似变量
# src = tpipe.c
# obj = tpipe.o 
# opt = -g -c
# all: tpipe
# # 链接
# tpipe: $(obj)
# 	gcc $(obj) -l pthread -o tpipe
# # 编译
# tpipe.o: $(src)
# 	gcc $(opt) $(src)
# clean:
# 	rm tpipe *.o


# src = ppipe.c
# obj = ppipe.o 
# opt = -g -c
# all: ppipe
# ppipe: $(obj)
# 	gcc $(obj) -o ppipe
# ppipe.o: $(src)
# 	gcc $(opt) $(src)
# clean:
# 	rm ppipe *.o

src = func_calc.c
obj = func_calc.o 
opt = -g -c
all: func_calc
func_calc: $(obj)
	gcc $(obj) -l pthread -o func_calc -lm
# -lm：连接数学库
func_calc.o: $(src)
	gcc $(opt) $(src)
clean:
	rm func_calc *.o