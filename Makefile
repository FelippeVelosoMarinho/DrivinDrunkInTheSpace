ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: Rtype.exe

Rtype.exe: Rtype.o
	gcc -o Rtype.exe Rtype.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

Rtype.o: Rtype.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c Rtype.c	
	
clean:
	del Rtype.o
	del Rtype.exe