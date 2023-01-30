# hdrs = ipc.h
# opts = -g -c
# c_src = consumer.c ipc.c
# c_obj = consumer.o ipc.o
# p_src = producer.c ipc.c
# p_obj = producer.o ipc.o
# all: producer consumer
# consumer: $(c_obj)
# 	gcc $(c_obj) -o consumer
# consumer.o: $(c_src) $(hdrs)
# 	gcc $(opts) $(c_src)
# producer: $(p_obj)
# 	gcc $(p_obj) -o producer
# producer.o: $(p_src) $(hdrs)
# 	gcc $(opts) $(p_src)
# clean: 
# 	rm consumer producer *.o

hdrs = ipc.h
opts = -g -c
c_src = smoker_v2.c ipc.c
c_obj = smoker_v2.o ipc.o
p_src = producer_v2.c ipc.c
p_obj = producer_v2.o ipc.o
all: producer_v2 smoker_v2
smoker_v2: $(c_obj)
	gcc $(c_obj) -o smoker_v2
smoker_v2.o: $(c_src) $(hdrs)
	gcc $(opts) $(c_src)
producer_v2: $(p_obj)
	gcc $(p_obj) -o producer_v2
producer_v2.o: $(p_src) $(hdrs)
	gcc $(opts) $(p_src)
clean: 
	rm smoker_v2 producer_v2 *.o