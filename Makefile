#### Adapted from ADTs Assignment/Polymorphic Hashing Makefile ###
CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDL= `sdl2-config --cflags`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm `sdl2-config --libs`

parse : Parser/parser.c Parser/testing.c Parser/turtle.h ADTs/General/general.h ADTs/General/general.c
	$(CC) Parser/parser.c Parser/testing.c ADTs/General/general.c -o parse $(PRODUCTION) $(LDLIBS)

parse_s : Parser/parser.c Parser/testing.c Parser/turtle.h ADTs/General/general.h ADTs/General/general.c
	$(CC) Parser/parser.c Parser/testing.c ADTs/General/general.c -o parse_s $(SANITIZE) $(LDLIBS)

parse_v : Parser/parser.c Parser/testing.c Parser/turtle.h ADTs/General/general.h ADTs/General/general.c
	$(CC) Parser/parser.c Parser/testing.c ADTs/General/general.c -o parse_v $(VALGRIND) $(LDLIBS)

interp : Interp/interp.c Interp/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c 
	$(CC) Interp/interp.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o interp $(PRODUCTION) $(SDL) $(LDLIBS)

#Interpreter without SDL for testing purposes
interp_t : Interp/noSDL/interp.c Interp/noSDL/testing.c Interp/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Interp/noSDL/interp.c Interp/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o interp_t $(PRODUCTION) $(SDL) $(LDLIBS)

interp_s : Interp/noSDL/interp.c Interp/noSDL/testing.c Interp/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Interp/noSDL/interp.c Interp/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o interp_s $(SANITIZE) $(SDL) $(LDLIBS)

interp_v : Interp/noSDL/interp.c Interp/noSDL/testing.c Interp/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Interp/noSDL/interp.c Interp/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o interp_v $(VALGRIND) $(SDL) $(LDLIBS)

extension : Extension/ext.c Extension/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Extension/ext.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o extension $(PRODUCTION) $(SDL) $(LDLIBS)

#Extension without SDL for testing purposes - some warnings on compile due to unused variables, -Werror will prevent these from running - TO SHOW TESTING ONLY
extension_t : Extension/noSDL/ext.c Extension/noSDL/testing.c Extension/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Extension/noSDL/ext.c Extension/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o extension_t $(PRODUCTION) $(SDL) $(LDLIBS)

extension_s : Extension/noSDL/ext.c Extension/noSDL/testing.c Extension/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Extension/noSDL/ext.c Extension/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o extension_s $(SANITIZE) $(SDL) $(LDLIBS)

extension_v : Extension/noSDL/ext.c Extension/noSDL/testing.c Extension/noSDL/turtle.h ADTs/Stack/stack.h ADTs/Stack/Linked/specific.h ADTs/Stack/Linked/linked.c ADTs/General/general.h ADTs/General/general.c SDL/neillsdl2.h SDL/neillsdl2.c
	$(CC) Extension/noSDL/ext.c Extension/noSDL/testing.c ADTs/Stack/Linked/linked.c ADTs/General/general.c SDL/neillsdl2.c -o extension_v $(VALGRIND) $(SDL) $(LDLIBS)

clean :
	rm -f parse parse_s parse_v interp interp_t interp_s interp_v extension extension_t extension_s extension_v

pall: parse parse_s parse_v
	./parse_s Files/simp.ttl
	valgrind --leak-check=full ./parse_v Files/spiral2_c.ttl

iall: interp_t interp_s interp_v
	./interp_s Files/simp_c.ttl
	valgrind --leak-check=full ./interp_v Files/spiral1_c.ttl

eall: extension_t extension_s extension_v
	./extension_s Files/spiral1_c.ttl
	valgrind --leak-check=full ./extension_v Files/cube.ttl
