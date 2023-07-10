head = dp.h 
srcs = dp.cc 
objs = dp.o 
opts = -w -g -c 
all: dp 

dp: $(objs)
	g++ $(objs) -o dp 
	
dp.o: $(srcs) $(head)
	g++ $(opts) $(srcs) 
	
clean:
	rm dp *.o