FLAGS = -Wall

all: cclus sim2corr

cclus: main.o clus_handle.o eclus.o graph.o cdb.o
	g++ ${FLAGS} -o $@ $^

sim2corr: clus_handle.o graph.o sim2corr.o
	g++ ${FLAGS} -o $@ $^

sim2corr.o: sim2corr.cpp clus_handle.h graph.h
	g++ ${FLAGS} -c $^

main.o: main.cpp clus_handle.h eclus.h graph.h cdb.h
	g++ ${FLAGS} -c $^

clus_handle.o: clus_handle.cpp clus_handle.h
	g++ ${FLAGS} -c $^

eclus.o: eclus.cpp eclus.h cdb.h graph.h
	g++ ${FLAGS} -c $^

vclus.o: vclus.cpp vclus.h cdb.h graph.h
	g++ ${FLAGS} -c $^

%.o: %.cpp %.h
	g++ ${FLAGS} -c $^

clean: 
	rm *.o
