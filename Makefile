# for no cmake
#g++ -std=c++11 -Wall -Icode examples/echo.cpp code/EventLoop.cpp code/TcpServer.cpp code/TcpConnection.cpp code/Buffer.cpp code/EventLoopPool.cpp code/TcpClient.cpp code/Logger.cpp code/TimingWheel.cpp -o output/echo -lpthread

DIR_SRC = code
DIR_OBJ = build
DIR_BIN = output

SRCS = $(wildcard ${DIR_SRC}/*.cpp)
OBJS = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRCS}))
DEPS = $(patsubst %.cpp,${DIR_OBJ}/%.d,$(notdir ${SRCS}))

TARGET = $(DIR_BIN)/echo
CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDEFLAGS = -Icode
LDFLAGS = -lpthread

AR = ar
ARFLAGS = -rcs


.PHONY: all install clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -c $< -o $@

include $(DEPS)

$(DIR_OBJ)/%.d: $(DIR_SRC)/%.cpp
	set -e; rm -f $@; \
	$(CXX) $(INCLUDEFLAGS) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"

clean:
	@echo "cleanning project"
	-rm -f $(TARGET) $(OBJS)
	@echo "clean completed"
