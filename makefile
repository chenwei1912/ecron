#g++ main.cpp EventLoop.cpp TcpServer.cpp TcpConnection.cpp Buffer.cpp -std=c++11 -I/mnt/d/linux/boost_1_68_0 -L/mnt/d/linux/boost_1_68_0/stage/lib -o test -lpthread -lboost_system

CXXFLAGS = -std=c++11 -I/mnt/d/linux/boost_1_68_0 -I.
LIBS = -L/mnt/d/linux/boost_1_68_0/stage/lib -lpthread

SRCS = main.cpp EventLoop.cpp TcpServer.cpp TcpConnection.cpp Buffer.cpp EventLoopPool.cpp TcpClient.cpp Logger.cpp TimingWheel.cpp
OBJS = $(SRCS:.cpp=.o)
BIN = server
CXX = g++

.PHONY: all clean install

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"

clean:
	@echo "cleanning project"
	rm -vf $(BIN) $(OBJS)
	@echo "clean completed"


