#
SHELL=bash
CC=g++
CFLAGS=-Wall -Wextra -Werror -g -IUtil/ -IThirdparty/
LDFLAGS=-lm -lz -LUtil/ -LThirdparty/
OBJS=probe.o count.o cs_probe.o
O_UTIL=Util/SeqReader.o Util/Util.o Util/DNAString.o
O_TP=Thirdparty/gzstream.o
BINS=test egrl

all: $(BINS)

egrl: $(OBJS) $(O_UTIL) $(O_TP) egrl.o
	$(CC) $(LDFLAGS) -o $@ $^

test: $(O_UTIL) $(O_TP) $(OBJS) test.o probe.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

Thirdparty/gzstream.o: Thirdparty/gzstream.C
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	find . -name "*.o" | xargs rm -f ; rm -f $(BINS); rm -f a.out
	find . -name "*.a" | xargs rm -f

.PHONY: clean

