server.out: server_main.cpp server_core.h server_core.cpp ../message/message.h ../message/message.cpp ../connection/connection.h ../connection/connection.h ../util/util.h ../util/util.cpp
	g++ -Wall -g server_main.cpp server_core.cpp ../message/message.cpp ../connection/connection.cpp ../util/util.cpp -o server.out

clean:
	rm -rfv *.o *.out *.out.*
