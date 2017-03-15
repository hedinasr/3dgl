ECHO 	 = printf
CAT 	 = cat
RM	 = rm
FIND 	 = find
UNAME	 = $(shell uname)
CPPFLAGS = -ggdb -Wall -pedantic -ansi
LDFLAGS	 =
CC   	 = g++
LD   	 = g++

SRCD = src
INCD = include #-I/usr/include
OBJD = obj
BIND = bin
LIBD = #-L/usr/local/lib/
LIBS = -lGL -lGLU -lglut #-L/usr/X11/lib

EXEC =  anim
SRCS =  $(wildcard $(SRCD)/*.cpp)
OBJ  =  $(SRCS:.cpp=.o)
OBJS := $(addprefix $(OBJD)/, $(notdir $(OBJ)))

default: $(BIND)/$(EXEC)

$(BIND)/$(EXEC): $(OBJS)
	@$(ECHO) "Linking ..\n"
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBD) $(LIBS) -Wl,-rpath ./

$(OBJD)/%.o: $(SRCD)/%.cpp
	@$(ECHO) "Compiling source file ..\n"
	$(CC) -o $@ -c $< $(CPPFLAGS) -I$(INCD)

clean:
	@$(FIND) $(OBJD) -name *.o -delete

mrproper: clean
	@$(RM) -f $(BIND)/$(EXEC)
