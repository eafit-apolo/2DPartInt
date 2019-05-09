.DEFAULT_GOAL := default

program_name = 2DPartInt
c_src_folder = src/main/c
cpp_src_folder = src/main/cpp
test_src_folder = src/test/c
headers_folder = src/main/include
objects_folder = bin/objects
test_folder = bin/tests
c_flags = -std=c11 -O3 -Wall -Wextra -Werror
cpp_flags = -std=c++11 -O3 -Wall -Wextra -Werror

$(objects_folder)/config.o: $(cpp_src_folder)/config.cpp $(headers_folder)/config.h
	mkdir -p $(objects_folder)
	g++ $(cpp_flags) -I$(headers_folder) -o $@ -c $<

$(objects_folder)/functions.o: $(c_src_folder)/functions.c $(headers_folder)/data.h $(headers_folder)/functions.h
	mkdir -p $(objects_folder)
	gcc $(c_flags) -I$(headers_folder) -o $@ -c $<

$(objects_folder)/functions_spec.o: $(test_src_folder)/functions_spec.c $(headers_folder)/data.h $(headers_folder)/functions.h
	mkdir -p $(objects_folder)
	gcc $(c_flags) -I$(headers_folder) -o $@ -c $<

$(test_folder)/functions_spec: $(objects_folder)/functions.o $(objects_folder)/functions_spec.o
	mkdir -p $(test_folder)
	gcc $(c_flags) -o $@ $^ -lm

.PHONY: functions_spec
functions_spec: $(test_folder)/functions_spec
	$(test_folder)/functions_spec

.PHONY: test
test: functions_spec

$(objects_folder)/main.o: $(cpp_src_folder)/main.c $(headers_folder)/data.h $(headers_folder)/functions.h
	mkdir -p $(objects_folder)
	g++ $(cpp_flags) -I$(headers_folder) -o $@ -c $<

bin/$(program_name): $(objects_folder)/functions.o $(objects_folder)/config.o $(objects_folder)/main.o
	g++ $(cpp_flags) -o $@ $^ -lm

.PHONY: compile
compile: bin/$(program_name)

.PHONY: clean
clean:
	rm -rf bin

.PHONY: default
default: compile
