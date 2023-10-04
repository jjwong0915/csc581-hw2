OBJS = src/SfmlTimeline/SfmlTimeline.o src/Timeline/Timeline.o
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread
CXXFLAGS = -std=c++17 -Isrc

all: single-player server client

single-player: src/SinglePlayer/SinglePlayer.o $(OBJS)
	$(CXX) -o single-player src/SinglePlayer/SinglePlayer.o $(OBJS) $(LIBS)

server: src/Server/Server.o $(OBJS)
	$(CXX) -o server src/Server/Server.o $(OBJS) $(LIBS)

client: src/Client/Client.o $(OBJS)
	$(CXX) -o client src/Client/Client.o $(OBJS) $(LIBS)

####

src/SfmlTimeline/SfmlTimeline.o: src/SfmlTimeline/SfmlTimeline.hpp src/BasicTimeline/BasicTimeline.hpp

src/Timeline/Timeline.o: src/Timeline/Timeline.hpp src/BasicTimeline/BasicTimeline.hpp

src/SinglePlayer/SinglePlayer.o: src/SfmlTimeline/SfmlTimeline.hpp src/Timeline/Timeline.hpp

src/Server/Server.o: src/Object/Object.hpp src/UserAction/UserAction.hpp

src/Server/Client.o: src/Object/Object.hpp src/UserAction/UserAction.hpp

####

.PHONY: run
run: server client
	./server

.PHONY: clean
clean:
	rm single-player server client $(OBJS)
