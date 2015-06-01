client.out: client_main.cpp client_core.cpp client_core.h ../connection/connection.h ../connection/connection.cpp ../message/message.h ../message/message.cpp ../util/util.h ../util/util.cpp
	g++ -Wall -g client_core.cpp client_main.cpp ../connection/connection.cpp ../message/message.cpp ../util/util.cpp -o client.out

clean:
	rm -rfv *.o *.out *.out.*
