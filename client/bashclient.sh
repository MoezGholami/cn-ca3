g++ *.h
g++ clientMain.cpp -o client clientCore.cpp ../connection/connection.o ../message/message.o ../util/util.o
