#g++ main.cpp EventLoop.cpp TcpServer.cpp TcpConnection.cpp Buffer.cpp -std=c++11 -I/home/shared/boost_1_77_0 -L/home/shared/boost_1_77_0/stage/lib -o test -lpthread -lboost_system

CXXFLAGS = -std=c++11
INCLUDEFLAGS = -I.
#-L/home/shared/boost_1_77_0/stage/lib
LDFLAGS = -lpthread

SRCS = main.cpp EventLoop.cpp TcpServer.cpp TcpConnection.cpp Buffer.cpp EventLoopPool.cpp TcpClient.cpp Logger.cpp TimingWheel.cpp
OBJS = $(SRCS:.cpp=.o)
BIN = server
CXX = g++

.PHONY: all install clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c $< -o $@

include $(SRCS:.cpp=.d)

%.d: %.cpp
	set -e; rm -f $@; \
	$(CXX) $(INCLUDEFLAGS) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"

clean:
	@echo "cleanning project"
	-rm -f $(BIN) $(OBJS)
	@echo "clean completed"
