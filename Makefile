# Makefile for Dun-geon

PROG = dun-geon
SRCDIR = src
OBJDIR = obj
DATADIR = data
CXX = g++
CXXFLAGS = -std=c++14 -Wall -pedantic
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(notdir $(SRCS)))

all: $(PROG)

$(PROG): $(addprefix $(OBJDIR)/, $(OBJS)) $(DATADIR)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(addprefix $(OBJDIR)/,$(OBJS))

$(OBJDIR) $(DATADIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(PROG)

.PHONY: all clean $(OBJDIR) $(DATADIR)