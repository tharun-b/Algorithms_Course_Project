CXXFLAGS =	-O3 -Wall -fmessage-length=0

OBJS =		routing.o

LIBS = 

TARGET =	routing.out

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	
	
	