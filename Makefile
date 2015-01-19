COPTS=-Wall -g -c  -O0
OBJS=main.o \
	 langlex.o \
	 parse.o   \

all: lang

clean:
	rm -f $(OBJS)
	rm -f langlex.c
	rm -f lang
	rm -f out

.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langlex.c 
	g++ $(COPTS) main.cpp -o main.o

langlex.c: lang.l
	flex -o langlex.c lang.l

lang: $(OBJS)
	g++ $(OBJS) -o lang