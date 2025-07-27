CC=g++
NASM=nasm
NASMFLAGS=-f elf64

# Основные флаги компиляции
CFLAGS=-ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# Флаги для профилирования (без sanitizers)
PROFILE_CFLAGS = -ggdb3 -std=c++17 -O0 -Wall -Wextra -fno-omit-frame-pointer

SOURCE_DIR = source
BUILD_DIR = build
DUMP_DIR = dump
HEADERS_DIR = include
ASM_DIR = asm

INCLUDE= -I include
CFLAGS+=$(INCLUDE)

HEADERS=$(wildcard $(HEADERS_DIR)/*.h)
SOURCES=$(wildcard $(SOURCE_DIR)/*.cpp)
ASM_SOURCES=$(wildcard $(ASM_DIR)/*.asm)
OBJECTS=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
ASM_OBJECTS=$(patsubst $(ASM_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
EXEC_NAME := hash_table.exe
PROFILER_OUT_NAME = callgrind_without_optimization.out

.PHONY: clean all run profile_build callgrind

all: $(BUILD_DIR)/$(EXEC_NAME)

$(BUILD_DIR)/$(EXEC_NAME): $(OBJECTS) $(ASM_OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

# Правило для компиляции ассемблерных файлов
$(BUILD_DIR)/%.o: $(ASM_DIR)/%.asm | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DUMP_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DUMP_DIR)
	rm -rf $(PROFILER_OUT_NAME)

run:
	$(BUILD_DIR)/$(EXEC_NAME)

profile_build:
	$(CC) $(PROFILE_CFLAGS) $(INCLUDE) $(SOURCES) $(ASM_OBJECTS) -o $(BUILD_DIR)/$(EXEC_NAME)_profile

callgrind: profile_build
	valgrind --tool=callgrind --callgrind-out-file=$(PROFILER_OUT_NAME) ./$(BUILD_DIR)/$(EXEC_NAME)_profile
	@echo "Callgrind data saved to $(PROFILER_OUT_NAME). Use: kcachegrind $(PROFILER_OUT_NAME)"