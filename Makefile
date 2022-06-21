CXX = g++
CXXFLAGS = -g -Wall -Werror --pedantic-errors -std=c++17

SRC = src
INC = include

ASIO_DIR = vendor/asio/asio
ASIO_INC = $(ASIO_DIR)/include

all: check_syntax

check_syntax: CXXFLAGS += -fsyntax-only
check_syntax: $(SRC)/easy_net.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) $<
