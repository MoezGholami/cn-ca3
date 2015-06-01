client.out: clientMain.cpp clientCore.cpp clientCore.h ../connection/connection.h ../connection/connection.cpp ../message/message.h ../message/message.cpp ../util/util.h ../util/util.cpp
	g++ -Wall -g clientMain.cpp clientCore.cpp ../connection/connection.cpp ../message/message.cpp ../util/util.cpp -o client.out

clean:
	rm -rfv *.o *.out *.out.*
