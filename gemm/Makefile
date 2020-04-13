CC = gcc

#NANOX_LIB=-L/home/afilgueras/install/nanox/lib/debug/ -lnanox -lnanox-c -lnanox-ompss -lnanox-max-api -lnanox-pe-max -Wl,-rpath=/home/afilgueras/install/nanox/lib/debug/
#NANOX_LIB=-L/home/maxeler/install/nanox/lib/performance/ -lnanox -lnanox-c -lnanox-ompss -lnanox-max-api -lnanox-pe-max -Wl,-rpath=/home/maxeler/install/nanox/lib/performance/
NANOX_LIB=-L/home/jusers/martorell1/jumax/ompss/install/nanox-mx/lib/performance/ -lnanox -lnanox-c -lnanox-ompss -lnanox-max-api -lnanox-pe-max -lpthread -Wl,-rpath=/home/jusers/martorell1/jumax/ompss/install/nanox-mx/lib/performance/

CFLAGS+=$(NANOX_LIB)
CFLAGS+= -g -std=gnu99 -O3

include $(MAXCOMPILERDIR)/lib/Makefile.include


mm_test_max: mm_test_max.c slic_gemm.o
	$(CC) $(CFLAGS) $^ -o $@

mm_test_panels: mm_test_panels.c slic_gemm.o
	$(CC) $(CFLAGS) $^ -o $@

mm_test_panels_nodep: mm_test_panels_nodep.c slic_gemm.o
	$(CC) $(CFLAGS) $^ -o $@

slic_gemm.o: cpu_src/gemm.max
	$(SLICCOMPILE) cpu_src/gemm.max slic_gemm.o


