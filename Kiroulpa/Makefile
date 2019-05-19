# Makefile ENSIM'Elec 2017

# Configuration {{{1

# Nom des exécutables à créer
TEST=bottest
MAIN=main
ACTION=action
CODEURSERIE = codeurSerie
ASSERVT = asservt
AX12 = AX12

# Dossiers sources
SRC_DIR:=src
INC_DIR:=src

# Dossier source de wiringPi pour la cross-compilation (submodule git)
WPI_DIR:=wiringPi

# Flags de compilation
CXXFLAGS:= -std=c++14 -Wall -Wextra -Wshadow -Wpedantic -I$(INC_DIR)
LDFLAGS:= -pthread -lpthread -lwiringPi -ldxl_sbc_cpp

# Dossiers de compilation
BUILD_DIR:=build
OBJ_DIR:=$(BUILD_DIR)/obj
OBJ_DIR_MAIN:=$(BUILD_DIR)/obj/main
BIN_DIR:=$(BUILD_DIR)/bin

# Choix automatique de toolchain {{{1

CURRENT_ARCH=$(shell uname -m)

ifneq ($(filter arm%,$(CURRENT_ARCH)),)
	# Si on compile directement sur la Rasp on utilise le g++ disponible
	CXX=g++
else
	# Si on compile sur un PC alors on crosscompile
	CXX=arm-unknown-linux-gnueabi-g++
	CXXFLAGS+=-I$(WPI_DIR)/wiringPi
	LDFLAGS+=-Llib
	CXXFLAGS+=-I/usr/include/
endif

# Actions automatisées {{{1

# Création du dossier de build et des sous dossiers nécessaires
ifneq ($(BUILD_DIR),)
$(shell [ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR))
$(shell [ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR))
$(shell [ -d $(OBJ_DIR_MAIN) ] || mkdir -p $(OBJ_DIR_MAIN))
$(shell [ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR))
endif

# Recherche des fichiers sources
SRCS:=$(wildcard $(SRC_DIR)/*.cpp)

# Déduction des fichiers objets .o à produire à partir de chaque fichier source
OBJS:=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Règles de construction {{{1

# Règle appelée avec 'make': on reconstruit asserv
all: $(MAIN) $(CODEURSERIE) $(TEST) $(ASSERVT) $(ACTION) $(AX12)


# Construction du programme action
$(ACTION): $(OBJS) $(OBJ_DIR_MAIN)/action.o
	$(CXX) $(LDFLAGS) $^ -o $@
	
# Construction du programme asservt
$(ASSERVT): $(OBJS) $(OBJ_DIR_MAIN)/asservt.o
	$(CXX) $(LDFLAGS) $^ -o $@	

# Construction du programme AX12
$(AX12): $(OBJS) $(OBJ_DIR_MAIN)/AX12.o
	$(CXX) $(LDFLAGS) $^ -o $@	
	
# Construction du programme principal
$(MAIN): $(OBJS) $(OBJ_DIR_MAIN)/main.o
	$(CXX) $(LDFLAGS) $^ -o $@

# Construction du programme de bottest
$(TEST): $(OBJS) $(OBJ_DIR_MAIN)/bottest.o
	$(CXX) $(LDFLAGS) $^ -o $@

# Construction du programme de codeurSerie
$(CODEURSERIE): $(OBJS) $(OBJ_DIR_MAIN)/codeurSerie.o
	$(CXX) $(LDFLAGS) $^ -o $@

# Règle de fabrication des fichiers objets communs
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle de fabrication des fichiers main qui sont isolés pour n'en choisir qu'un
$(OBJ_DIR)/main/%.o: $(SRC_DIR)/main/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR) $(TEST) $(MAIN) $(CODEURSERIE) $(ASSERVT) $(ACTION) $(AX12)

# Compilation de wiringPi en cross
wiringpi:
	rm -f lib/libwiringPi.so
	make -C wiringPi/wiringPi clean
	make -C wiringPi/wiringPi CC=arm-unknown-linux-gnueabi-gcc
	cd lib && ln -s ../wiringPi/wiringPi/libwiringPi.so* libwiringPi.so


# vim: foldmethod=marker
