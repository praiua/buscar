
LIB = Libraries/
DIR = Source/
CPP = g++
OPTS = -Wall -g -I$(DIR) -I$(LIB)
NAME = buscar

FONTS = $(wildcard $(LIB)*.cc) $(wildcard $(DIR)*.cc)

OBJ=$(patsubst %.cc,%.o,$(FONTS))


all: $(NAME)

$(NAME): $(OBJ)
	${CPP} ${OPTS} $(OBJ) -o $(NAME)

%.o: %.cc %.h
	${CPP} -shared ${OPTS} $< -c -o $@
	
clean:
	rm -f *~ ${LIB}*.o ${LIB}*~ ${DIR}*.o ${DIR}*~ $(NAME)

