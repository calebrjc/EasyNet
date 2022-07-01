CXX = g++
CXXFLAGS = -g -Wall -Werror --pedantic-errors -std=c++17

SRC = src
INC = include
BIN = bin

ASIO_DIR = vendor/asio/asio
ASIO_INC = $(ASIO_DIR)/include

SPDLOG_DIR = vendor/spdlog
SPDLOG_INC = $(SPDLOG_DIR)/include

all: check_syntax

check_syntax: CXXFLAGS += -fsyntax-only
check_syntax: $(SRC)/easy_net.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) -I$(SPDLOG_INC) $<

test_server: $(SRC)/test_server.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) -I$(SPDLOG_INC) $< -o $(BIN)/$@ -lpthread

clean:
	rm -rf $(BIN)

$(BIN):
	mkdir -p $@
