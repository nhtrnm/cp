# := is immediate assignment - evaluated once, not re-evaluated each use (contrast with
# = which is lazy and re-evaluated every time).
CXX := g++

# -MMD: writes a .d file per .o listing every project header included; Make reads these
# to recompile the right .o files when a header changes.
# -MP: adds an empty phony rule per header in the .d so Make does not error with "No
# rule to make target '...'" when a header is deleted or renamed.
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wshadow -Wpedantic -Iinclude -MMD -MP

BUILD  := build
TARGET := $(BUILD)/tests  # $() expands a variable: $(BUILD) becomes "build"

# $(shell ...) runs a shell command at parse time and captures its stdout.
SRCS := $(shell find tests -name '*.cpp' ! -path '*/framework/*')

# $(patsubst pattern, replacement, text) - substitutes each match of tests/%.cpp with
# build/%.o, e.g. tests/core/test_common.cpp -> build/core/test_common.o.
OBJS := $(patsubst tests/%.cpp, $(BUILD)/%.o, $(SRCS))

# $(var:.o=.d) is a substitution reference - shorthand for patsubst, replaces .o with .d
# for each entry.
DEPS := $(OBJS:.o=.d)

# First build: no .d files yet - -include skips them silently (plain include errors).
# Later builds: Make loads them to recompile only the .o files whose headers changed.
-include $(DEPS)

# .PHONY declares targets that are not real files. Without it, if a file named "test" or
# "clean" existed on disk, make would skip running that target.
.PHONY: build test clean fmt

# A target follows the pattern:
#   target: prerequisites
#   <TAB> recipe
# make runs the recipe when the target is out of date relative to its prerequisites.
# Without target, make runs the first-defined target's recipe.

build: $(TARGET)

# $@    = target            -> build/tests
# $(@D) = directory of $@   -> build
# $^    = all prerequisites -> build/test_main.o build/core/test_common.o
$(TARGET): $(OBJS)
	mkdir -p $(@D) && $(CXX) $(CXXFLAGS) $^ -o $@

# Pattern rule - instantiated once per .cpp file.
# $@    = target             -> build/core/test_common.o
# $(@D) = directory of $@    -> build/core
# $<    = first prerequisite -> tests/core/test_common.cpp
$(BUILD)/%.o: tests/%.cpp
	mkdir -p $(@D) && $(CXX) $(CXXFLAGS) -c $< -o $@

test: build
	./$(TARGET)

clean:
	rm -rf $(BUILD)

# Formats only files changed since the last commit.
# --diff-filter=ACM limits to Added, Copied, and Modified files.
# $$ is a literal $ in make - needed here for the regex end-of-string anchor.
# xargs -r skips running clang-format-19 if the file list is empty.
fmt:
	{ git diff --name-only --diff-filter=ACM; git ls-files --others --exclude-standard; } | \
	  grep -E '\.(cpp|hpp|h|cc|cxx)$$' | \
	  xargs -r clang-format-19 -i
