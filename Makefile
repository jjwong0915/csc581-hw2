OBJS = src/App/App.o src/SfmlTimeline/SfmlTimeline.o src/Timeline/Timeline.o

sfml-app: $(OBJS)
	$(CXX) -o sfml-app $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

src/SfmlTimeline/SfmlTimeline.o: src/SfmlTimeline/SfmlTimeline.hpp src/BasicTimeline/BasicTimeline.hpp

src/Timeline/Timeline.o: src/Timeline/Timeline.hpp src/BasicTimeline/BasicTimeline.hpp

.PHONY: run
run: sfml-app
	./sfml-app

.PHONY: clean
clean:
	rm sfml-app $(OBJS)
