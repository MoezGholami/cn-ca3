group_server.out: group_server_main.cpp group_server_core.h group_server_core.cpp ../message/message.h ../message/message.cpp ../connection/connection.h ../connection/connection.cpp ../util/util.h ../util/util.cpp
	g++ -Wall -g group_server_main.cpp group_server_core.cpp ../message/message.cpp ../connection/connection.cpp ../util/util.cpp -o group_server.out
clean:
	rm -rfv *.o *.out *.out.*
