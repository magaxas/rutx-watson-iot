CFLAGS:=-Wall
LDFLAGS:=-lpaho-mqtt3as -liotp-as-device -lblobmsg_json -lubox -lubus
BIN:=watsoniot-daemon

SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(BIN)
