CXX = ccache g++
OUTPUT_FILE=bin/DeskJS

define INCLUDE
	include/
endef

define LIBUV
       libuv/include
endef

define APP
	main.cc
endef
 
define OBJ
	include/libv8_monolith.a
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
	$(CXX) $$APP -I $$INCLUDE -I $$LIBUV -std=c++17 -pthread -o $$OUTPUT_FILE -DV8_COMPRESS_POINTERS $$OBJ -Wl,--no-as-needed -ldl -DV8_ENABLE_SANDBOX

# make uv-threads (or any from examples)
#$(examples): % : examples/%.cpp
#mkdir -p bin
#	 $(CXX) -I $$INCLUDE -I $$INCLUDEUV  -std=c++17 -pthread -o bin/$@  $< -DV8_COMPRESS_POINTERS $$OBJ -Wl,--no-as-needed -ldl
#	./bin/$@
clean:
	rm -rf bin