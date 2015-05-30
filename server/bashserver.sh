g++ *.h
g++ serverMain.cpp -o server serverCore.cpp ../message/message.o ../connection/connection.o ../util/util.o
