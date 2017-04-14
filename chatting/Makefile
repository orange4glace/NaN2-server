CXX = g++

SRC_PATH        = ./src
SRCS            = $(SRC_PATH)/*.cpp
CHATTING_SERVER = chatting_server

all: $(CHATTING_SERVER)

$(CHATTING_SERVER) : $(SRCS)
	$(CXX) -o $@ $(SRCS) -lboost_thread -lboost_system -lmysqlcppconn -I/usr/local/include -std=c++1z

