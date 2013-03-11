# install directory
INSTALL_DIR=/usr/local/bin

# enable debug messages 
DEBUG = -DDEBUG

CC = gcc
LIBS = -lX11
LDFLAGS = -L/usr/lib
CFLAGS = -O2 -Wall -I/usr/X11R6/include 

SRC = main.o events.o manage.o list.o bar.o
HEADERS = bar.h conf.h data.h events.h list.h manage.h antiwm.h

antiwm: $(SRC)
	gcc $(SRC) -o $@ $(CFLAGS) $(LDFLAGS) $(LIBS) 

install: antiwm
	cp antiwm $(INSTALL_DIR)

%.o : %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $(DEBUG) $< -o $@

clean :
	rm -f *.o antiwm
