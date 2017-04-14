OS := $(shell uname)
ifeq ($(OS), Darwin)
	CXX = g++
else
	CXX = g++-6
endif

SRC_PATH        = ./src
SRCS            = $(SRC_PATH)/*.cpp
LOBBY_SERVER	= lobby_server

all: $(LOBBY_SERVER)

$(LOBBY_SERVER) : $(SRCS)
	$(CXX) -o $@ $(SRCS) -lboost_thread -lboost_system -lmysqlcppconn -lcpp_redis -ltacopie -lpthread -I/usr/local/include -std=c++1z

clean:
	rm -f $(LOBBY_SERVER)

