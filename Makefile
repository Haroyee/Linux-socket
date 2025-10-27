CC:=g++
CFLAGS:=-Wall -Iinclude -g -std=c++11
LDFLAGS:= -Llib
LDLIBS:= -lpthread

# 为 .cpp 文件指定搜索目录：src/
vpath %.cpp src/
#为 .o 文件指定搜索目录：build/
vpath %.o build/


all:server client

server: build/server.o build/socket.o
	$(CC) $^ -o output/$@ $(LDFLAGS) $(LDLIBS)

client: build/client.o build/socket.o
	$(CC) $^ -o output/$@ $(LDFLAGS) $(LDLIBS)

server_select: build/server_select.o build/socket.o
	$(CC) $^ -o output/$@ $(LDFLAGS) $(LDLIBS)

build/%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)



clean:
	rm -f build/*.o
	rm -f output/*.exe


.PHONY: clean all

run: all
	./output/server
	./output/client
	echo "执行完毕"


