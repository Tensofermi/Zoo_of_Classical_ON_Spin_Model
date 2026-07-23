.DEFAULT_GOAL := all

TARGET := a.out
TEST_TARGET := common_tests

SRC_DIR ?= src
BIN_DIR := bin
BUILD_ROOT ?= build

CXX_STANDARD ?= c++11
CXX_CANDIDATES ?= icpx icpc clang++ g++ c++
CXX_INPUT_ORIGIN := $(origin CXX)

ifeq ($(CXX_INPUT_ORIGIN),default)
CXX_SELECTION := automatic
CXX := $(strip $(shell for candidate in $(CXX_CANDIDATES); do \
	if command -v "$$candidate" >/dev/null 2>&1 && \
		printf '%s\n' 'int main() { return 0; }' | "$$candidate" -x c++ -std=$(CXX_STANDARD) -fsyntax-only - >/dev/null 2>&1; then \
		printf '%s' "$$candidate"; \
		break; \
	fi; \
done))
else
CXX_SELECTION := user-specified ($(CXX_INPUT_ORIGIN))
endif

sanitize = $(strip $(shell printf '%s' "$(strip $(1))" | LC_ALL=C tr -c 'A-Za-z0-9._-' '_'))
SOURCE_TAG := $(or $(call sanitize,$(SRC_DIR)),source)
COMPILER_TAG := $(or $(call sanitize,$(CXX)),unavailable)
STANDARD_TAG := $(or $(call sanitize,$(CXX_STANDARD)),standard)
override BUILD_DIR := $(BUILD_ROOT)/$(SOURCE_TAG)/$(COMPILER_TAG)/$(STANDARD_TAG)

SRC := $(shell find "$(SRC_DIR)" -type f -name '*.cpp' 2>/dev/null)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

INC_DIR := $(shell find "$(SRC_DIR)" -type d 2>/dev/null)
INC_FLAG := $(addprefix -I,$(INC_DIR))

ARMADILLO_LIB := -larmadillo
LAPACK_LIB := -llapack
BLAS_LIB := -lblas
GMP_LIB := -lgmp
GMPXX_LIB := -lgmpxx
GSL_LIB := -lgsl
BOOST_LIB := -lboost_mpi -lboost_serialization -lboost_filesystem -lboost_system
FFT_LIB := -lfftw3
ALL_LIBS :=

CXX_WARNINGS ?= -Wall -pedantic-errors
CXX_DEBUG ?=
CXX_OPTIM ?= -O3
CXX_DEP ?= -MMD -MP

USER_CPPFLAGS := $(CPPFLAGS)
USER_CXXFLAGS := $(CXXFLAGS)
USER_LDFLAGS := $(LDFLAGS)
USER_LDLIBS := $(LDLIBS)

override CPPFLAGS := $(strip $(INC_FLAG) $(USER_CPPFLAGS))
override CXXFLAGS := $(strip -std=$(CXX_STANDARD) $(CXX_DEBUG) $(CXX_WARNINGS) $(CXX_OPTIM) $(CXX_DEP) $(USER_CXXFLAGS))
override LDFLAGS := $(strip $(USER_LDFLAGS))
override LDLIBS := $(strip $(ALL_LIBS) $(USER_LDLIBS))
TEST_CXXFLAGS := $(filter-out -MMD -MP,$(CXXFLAGS))

CONFIG_TARGET := $(BUILD_DIR)/$(TARGET)
TEST_BINARY := $(BUILD_DIR)/$(TEST_TARGET)
TEST_SOURCES := tests/test_common.cpp \
	$(SRC_DIR)/system/RandomNumGen/RandomNumGen.cpp \
	$(SRC_DIR)/config/Lattice/Hyperrectangle.cpp

.PHONY: all test clean help compiler-info check-compiler FORCE

all: $(BIN_DIR)/$(TARGET)

check-compiler:
	@if [ -z "$(strip $(CXX))" ]; then \
		if [ "$(CXX_SELECTION)" = "automatic" ]; then \
			echo "Error: no usable C++ compiler was found."; \
			echo "Tried candidates: $(CXX_CANDIDATES)"; \
		else \
			echo "Error: the explicitly specified CXX value is empty."; \
		fi; \
		echo "Required standard: $(CXX_STANDARD)"; \
		echo "Install or load a compiler module, or specify one explicitly:"; \
		echo "  make CXX=clang++"; \
		echo "  make CXX=g++"; \
		exit 1; \
	fi
	@compiler_command='$(firstword $(CXX))'; \
	if ! command -v "$$compiler_command" >/dev/null 2>&1; then \
		echo "Error: requested CXX '$(CXX)' was not found."; \
		echo "Required standard: $(CXX_STANDARD)"; \
		echo "Install/load that compiler or choose another, for example: make CXX=clang++"; \
		exit 1; \
	fi
	@if ! printf '%s\n' 'int main() { return 0; }' | $(CXX) -x c++ -std=$(CXX_STANDARD) -fsyntax-only - >/dev/null 2>&1; then \
		if [ "$(CXX_SELECTION)" = "automatic" ]; then \
			echo "Error: automatically selected CXX '$(CXX)' is no longer usable."; \
		else \
			echo "Error: requested CXX '$(CXX)' exists but cannot compile a minimal $(CXX_STANDARD) program."; \
		fi; \
		echo "Install/load a working compiler or choose another, for example: make CXX=g++"; \
		exit 1; \
	fi

compiler-info: check-compiler
	@printf '%s\n' "Selection: $(CXX_SELECTION)"
	@printf '%s\n' "CXX: $(CXX)"
	@version="$$( $(CXX) --version 2>&1 | sed -n '1p' )"; printf '%s\n' "Compiler version: $$version"
	@printf '%s\n' "CXX_STANDARD: $(CXX_STANDARD)"
	@printf '%s\n' "SRC_DIR: $(SRC_DIR)"
	@printf '%s\n' "BUILD_DIR: $(BUILD_DIR)"
	@printf '%s\n' "CXXFLAGS: $(CXXFLAGS)"
	@printf '%s\n' "Include flags (CPPFLAGS): $(CPPFLAGS)"
	@printf '%s\n' "Link flags (LDFLAGS): $(LDFLAGS)"
	@printf '%s\n' "Link libraries (LDLIBS): $(LDLIBS)"

help:
	@printf '%s\n' \
		"Build:                    make" \
		"Test:                     make test" \
		"Compiler information:     make compiler-info" \
		"Choose a compiler:        make CXX=clang++" \
		"Candidate priority:       make CXX_CANDIDATES=\"clang++ g++\"" \
		"Choose a source tree:     make SRC_DIR=src" \
		"Clean generated files:    make clean"

$(CONFIG_TARGET): $(OBJ) | check-compiler
	@mkdir -p "$(@D)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJ) $(LDLIBS) -o "$@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | check-compiler
	@mkdir -p "$(@D)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o "$@" "$<"

FORCE:

$(BIN_DIR)/$(TARGET): $(CONFIG_TARGET) FORCE
	@mkdir -p "$(@D)"
	cp "$(CONFIG_TARGET)" "$@"

test: $(TEST_BINARY)
	"$(TEST_BINARY)"

$(TEST_BINARY): $(TEST_SOURCES) | check-compiler
	@mkdir -p "$(@D)"
	$(CXX) $(CPPFLAGS) $(TEST_CXXFLAGS) $(TEST_SOURCES) $(LDFLAGS) $(LDLIBS) -o "$@"

clean:
	$(RM) -rf "$(BUILD_ROOT)" "$(BIN_DIR)"

-include $(DEP)
