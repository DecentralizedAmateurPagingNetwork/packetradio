CPP=g++
CFLAGS=-O -Iinclude -Wall
CC=g++
.cpp.o:
	@$(CPP) $(CFLAGS) -O2 -DHAVE_CONFIG_H -c -g --static $<

all:m_filter
	@mv m_filter m_filter.prg

m_filter: m_filter.o

clean:
	@rm m_filter.prg
	@rm *.o
