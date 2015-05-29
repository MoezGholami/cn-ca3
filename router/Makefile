all: router_main.cpp router_shell.cpp router_shell.h router_kernel.cpp router_kernel.h ../connection/connection.cpp ../connection/connection.h ../message/message.cpp ../message/message.h ../util/util.cpp ../util/util.h
	g++ -Wall -g router_main.cpp router_shell.cpp router_kernel.cpp ../connection/connection.cpp ../message/message.cpp ../util/util.cpp -o router.out

clean:
	rm -rfv *.o *.out *.out.*
