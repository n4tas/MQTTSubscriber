SRC:=$(wildcard *.c)
BIN:=MQTTSubscriber
OBJ:=$(SRC:.c=.o)
LDFLAGS +=-largp -luci -lsqlite3 -lmosquitto -lcjson -lcurl

CFLAGS +=-Wall

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ) $(BIN)