
FLAGS := -Wall -Wextra -fsanitize=address -Iinclude -std=c++17

test:
	mkdir -p build
	clang++ $(FLAGS) -o build/test test.cc $(shell find src -name '*.cc')
