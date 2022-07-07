CXX = g++
CXXFLAGS = -g -Wall -Werror --pedantic-errors -std=c++17

SRC = src
INC = include
BIN = bin

HEADERS = $(shell find include/ -type f -name '*.*pp')

ASIO_DIR = vendor/asio/asio
ASIO_INC = $(ASIO_DIR)/include

SPDLOG_DIR = vendor/spdlog
SPDLOG_INC = $(SPDLOG_DIR)/include

all: check_syntax

check_syntax: CXXFLAGS += -fsyntax-only
check_syntax: $(SRC)/easy_net.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) -I$(SPDLOG_INC) $<

test_exes: $(BIN)/test_server $(BIN)/test_client

$(BIN)/test_server: $(SRC)/test_server.cpp $(HEADERS) | $(BIN)
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) -I$(SPDLOG_INC) $< -o $@ -lpthread

$(BIN)/test_client: $(SRC)/test_client.cpp $(HEADERS) | $(BIN)
	$(CXX) $(CXXFLAGS) -I$(INC) -I$(ASIO_INC) -I$(SPDLOG_INC) $< -o $@ -lpthread

clean:
	rm -rf $(BIN)

$(BIN):
	mkdir -p $@
