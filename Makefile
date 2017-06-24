# Compiler flags
CC			= g++ -std=c++11
CXXFLAGS	= -Wall -Wextra 

# The target and sources
SRCS		= endianFlip.cpp
TARGET		= endianFlip

# Directories ./obj ./src ./include
OBJDIR	= obj
SRCDIR	= src
INCDIR	= include
CPPFLAGS	+= $(addprefix -I ,$(INCDIR))

# Objects and sources
OBJS	:= $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
SRCS	:= $(addprefix $(SRCDIR)/,$(SRCS))

# Build .o from .cpp files, only using my directory structure
# n.b. this is a variation of the Make builtin rule '%.o: %.c'
# n.b. run 'make -p' to print them out
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(dir_guard)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# Ditto for the link stage (augmenting '%: %.o)
# '$@' is the name of the target of the rule (the exe file)
# '$^' is the name of (all) the pre-requisite of the rule (the .o files)
$(TARGET): $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: all clean dir_guard
all: $(TARGET)

clean:
	@$(RM) $(TARGET)
	@$(RM) -r $(OBJDIR) 

dir_guard=@mkdir -p $(@D)
