g++ *.h
g++ groupServerMain.cpp -o groupServer groupServerCore.cpp ../connection/connection.o ../message/message.o ../util/util.o
