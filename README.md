# assignment-2-AshishKempwad
assignment-2-AshishKempwad created by GitHub Classroom
Peer to Peer File sharing

To run Tracker(server to bind the peer):-
g++ tracker.cpp -o tracker
./tracker (port number)

To run the peers:-
 PEER1
 g++ peer1.cpp -o peer -lpthread
 ./peer 127.0.0.1 (tracker port_no) (port_no)

 PEER2
 ./peer 127.0.0.1 (PEER1 port_no) (port_no)
  
