server.out: serverMain.cpp serverCore.h serverCore.cpp ../message/message.h ../message/message.cpp ../connection/connection.h ../connection/connection.h ../util/util.h ../util/util.cpp
	g++ -Wall -g serverMain.cpp serverCore.cpp ../message/message.cpp ../connection/connection.cpp ../util/util.cpp -o server.out

clean:
	rm -rfv *.o *.out *.out.*
