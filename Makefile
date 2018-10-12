OBJS 	= main.o interface.o datastructures.o
SOURCE	= main.cpp interface.cpp datastructures.cpp
HEADER  = datastructures.h util.h
OUT  	= mygraph
CXX		= g++
FLAGS   = -g3 -c -pedantic -std=c++98 -Wall

$(OUT): $(OBJS)
	$(CXX) -g3 $(OBJS) -o $@

main.o: $(HEADER) main.cpp
	$(CXX) $(FLAGS) main.cpp

interface.o: $(HEADER) interface.cpp
	$(CXX) $(FLAGS) interface.cpp

datastructures.o: $(HEADER) datastructures.cpp
	$(CXX) $(FLAGS) datastructures.cpp


clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

