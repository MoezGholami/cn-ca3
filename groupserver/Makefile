group_server.out: groupServerMain.cpp groupServerCore.h groupServerCore.cpp ../connection/connection.h ../connection/connection.cpp ../message/message.h ../message/message.cpp ../util/util.h ../util/util.cpp
	g++ -Wall -g groupServerMain.cpp groupServerCore.cpp ../connection/connection.cpp ../message/message.cpp ../util/util.cpp -o group_server.out
clean:
	rm -rfv *.o *.out *.out.*
