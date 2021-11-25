#g++ main.cpp EventLoop.cpp TcpServer.cpp TcpConnection.cpp Buffer.cpp -std=c++11 -I/home/shared/boost_1_77_0 -L/home/shared/boost_1_77_0/stage/lib -o test -lpthread -lboost_system

DIR_SRC = .
DIR_OBJ = ../build
DIR_BIN = ../bin

SRCS = $(DIR_SRC)/echo.cpp
OBJS = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRCS}))
DEPS = $(patsubst %.cpp,${DIR_OBJ}/%.d,$(notdir ${SRCS}))
TARGET = $(DIR_BIN)/echo

CXX = g++
CXXFLAGS = -std=c++11 -Wall
INCLUDEFLAGS = -I../code
LDFLAGS = -lpthread


.PHONY: all install clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ $(DIR_BIN)/libnetlib.a -o $@ $(LDFLAGS)

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
