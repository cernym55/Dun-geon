# Makefile for Dun-geon

PROG = dun-geon
SRCDIR = src
OBJDIR = obj
DATADIR = data
TESTDIR = test
CXX = g++
INCDIRS = $(shell find $(SRCDIR) -type d)
INCFLAGS = $(addprefix -I,$(INCDIRS))
LDFLAGS = -lncurses -lmenu
CXXFLAGS = -g -O2 -pipe -std=c++17 -Wall -pedantic $(INCFLAGS)
DEPFLAGS = -MMD -MP
SRCS = $(shell find $(SRCDIR) -name *.cpp)
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
TESTS = $(shell find $(TESTDIR) -name *.cpp)
TESTBINS = $(patsubst %Test.cpp,%.test,$(TESTS))
DEPS = $(OBJS:.o=.d)

# Default target
all: $(PROG)

# Program target
$(PROG): $(OBJS) $(DATADIR)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS)

$(OBJDIR) $(DATADIR):
	@mkdir -p $@

%.o: %.cpp

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Test target
TESTCOUNT = $(words $(TESTBINS))
TESTFMT = printf "*** [%d/$(TESTCOUNT)]: $(subst test/,,$(testbin))\n"
test: $(OBJS) $(PROG) $(TESTBINS)
	@echo '*****************************************'
	@echo "  Running unit tests: $(TESTCOUNT) modules ready"
	@echo '*****************************************'
	@echo 
	@i=1 ; $(foreach testbin,$(TESTBINS),$(TESTFMT) $$((i)) ; $(testbin) ; i=$$(( i + 1 )))

%.test: %Test.cpp $(filter-out %/main.o,$(OBJS)) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lboost_unit_test_framework $^ -o $@

# Clean target
clean:
	@rm -rf $(OBJDIR) $(PROG)
	@rm -f $(TESTBINS)

.PHONY: all clean test

-include $(DEPS)