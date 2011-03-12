CC=g++
CFLAGS=-c -Wall -D_SERVER -D_LINUX -w -I../../Shared -I.
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard scripting/natives/*.cpp)
SOURCES+=../../Shared/CBitStream.cpp ../../Shared/CExceptionHandler.cpp ../../Shared/CHttpClient.cpp ../../Shared/CLibrary.cpp ../../Shared/CLogFile.cpp ../../Shared/CMutex.cpp ../../Shared/CNetModule.cpp ../../Shared/CNetworkPadState.cpp ../../Shared/CPacketHandler.cpp ../../Shared/CRPCHandler.cpp ../../Shared/CString.cpp ../../Shared/CThreadedHttpClient.cpp ../../Shared/CXML.cpp ../../Shared/Linux.cpp ../../Shared/SharedUtility.cpp ../../Shared/CRootEntity.cpp ../../Shared/CEntity.cpp
SOURCES+=$(wildcard ../../Shared/scripting/*.cpp)
SOURCES+=$(wildcard ../../Shared/scripting/natives/*.cpp)
SOURCES+=$(wildcard ../../Shared/TinyXML/*.cpp)
SOURCES+=$(wildcard ../../Shared/squirrel/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Debug/Server.Core.so
PCH=StdInc.h

all: $(PCH) $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -shared -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

pch:
	$(CC) $(CFLAGS) $(PCH) -o $(PCH).gch

clean:
	rm -Rf *.o $(EXECUTABLE)
