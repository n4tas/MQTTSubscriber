SRC:=$(wildcard src/*.c)
BIN:=MQTTSubscriber
OBJ:=$(SRC:.c=.o)
LDFLAGS +=-largp -luci -lsqlite3 -lmosquitto -lcjson -lcurl
CFLAGS +=-Wall
DEBUG +=-DDEBUG

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LDFLAGS)  #$(DEBUG)

clean:
	$(RM) $(OBJ) $(BIN)