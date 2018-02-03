
root=$(shell pwd)
lib=$(root)/../lib


test:base_json.cpp data.cpp
	g++ -o $@ $^ -I $(lib)/include  -L$(lib)/lib -ljsoncpp #-D_STYLED_
.PHONY:clean
clean:
	rm -f test
