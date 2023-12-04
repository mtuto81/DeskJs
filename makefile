CXX = ccache g++
OUTPUT_FILE=bin/DeskJS

define INCLUDE
     jsapi/mozjs-122a1
endef

define LIBUV
       libuv/include
endef

define APP
	main.cc
	src/deskjs.cc
endef
 
define OBJ
	jsapi/libmozjs-122a1.so
	libuv/libuv.a
endef

export INCLUDE
export LIBUV
export OUTPUT_FILE

export APP
export LIB
export OBJ
export APP



# will output the exectuable to ./bin folder
build:
	mkdir -p bin
		$(CXX)  -std=c++17 -I $$INCLUDE -L $$OBJ $$APP -o $$OUTPUT_FILE  -lmozjs-122a1 -lz -lpthread -ldl -DDEBUG 


# make uv-threads (or any from examples)
#$(examples): % : examples/%.cpp
#mkdir -p bin
#	 $(CXX) -I $$INCLUDE -I $$INCLUDEUV  -std=c++17 -pthread -o bin/$@  $< -DV8_COMPRESS_POINTERS $$OBJ -Wl,--no-as-needed -ldl
#	./bin/$@
clean:
	rm -rf bin