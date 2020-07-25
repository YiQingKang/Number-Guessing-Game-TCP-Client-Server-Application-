# Number-Guessing Game: TCP Client-Server Application 
*[Ray Jasson](mailto:holmesqueen2070@yahoo.com)* <br>
*[Yi Qing](mailto:yiqing0519@gmail.com)* <br>
*25/07/2020*

<br>

## :notebook: Description
This is a multi-player number-guessing game developed using BSD Unix socket API in C. 
In the game, the server generates a random number between 1 to 99 for each connected client, while the client(s) has to guess the number correctly to win. When the client made the correct guess, the server will broadcast the results. The server keeps track of the best score (i.e. least attempts for correct guess) for each client. This application includes keep-alive mechanism, server backup and restoring of disconnected game to handle unreachable situation of the client and the server.

<br>

## :computer: How to run the application
This application can run on any Unix system in terminal mode. To run the client-server application:

**Download Unix Network Programming Library**
Download the `unpv13e` folder and run configuration as follows:
```
./configure
cd lib
make
cd ../libfree
make
```
**Compile the files**
Download the `project` folder and run
```
$ make
```
**Run the server program**
```
$ ./server
```
**Run the client program**
```
$ ./client 127.0.0.1 UserA
```
:point_right: Format: `./client [server’s_IP_address] [client_username]` <br>
:point_right: The server identifies and communicates with a client by its username. 

<br>

## :computer:  Server implementation
The server can play the game with multiple clients simultaneously. To communicate with the clients correctly, the server keeps track of the state of its interaction with each of its connected client. There are 5 different states defined for the server program. The transitions from one state to another in the state machine are dictated by the current state and the client’s message received.

<p align=center><img src="/docs/pics/ServerStateTransitionDiagram.png" width=75% height=75%></p>
<p align="center"><i>Server State Transition Diagram</i></p>

`select()` is used to handle the listening socket, connected clients and the standard input.

<br>

### :arrow_down_small: Basic interactions between the server and client

 - After the server sends the game instructions, the client can start or
   decline the game.   
  - If the client is ready to play the game, the server checks if the
   client has any previous unfinished game. If the client has
   disconnected game record, the server will first ask if the client
   would like to continue the previous game before playing with the
   client.   
 - The server receives guesses from the client and gives appropriate feedback. `range()` function is called to narrow down the range of selection and help the client make the correct guess in fewer attempts.
  - Once the client guessed the number correctly, the server broadcasts
   the result and the client can start a new game or exit.   
  - For every winning game, the server will check for the client’s high
   score. If it is the first winning game of the client or if the client
   beats its own high score, the server updates the high score that is stored in `past_game_record.txt` file.
   -  During the game, the client can quit and leave the game voluntarily.   
  
<br>

###  :arrow_down_small: Other features
**Keep-Alive Mechanism** <br>
Application-level keep-alive mechanism is implemented to detect the unreachable or idle state of the client. A maximum keep-alive period is defined to be 120 seconds. 1 keep-alive probe is sent to the peer program after 60 seconds of inactivity. If no response is received by the local program within 60 seconds after the first keep-alive probe is sent, a keep-alive timeout occurs and the local program will terminate the connection by closing the socket.

-----------------------------------

**Periodic Server Backup** <br>
The server performs backup for all active game records every 60 seconds. During backup, all active game records will be written to `active_game_record.txt` file and the existing active game records in the file will be removed. If the server crashes and reboots, the server will copy the game records in the backup file to the `disconnect_game_record.txt` file to be restored when the client reconnects.
<br><br>
:point_right: Refer to `saveActiveRecords` and `appendActiveRecords` functions in `game.h`

-----------------------------------

**Restore Disconnected Games** <br>
If the client’s connection is terminated abruptly (client disconnects abruptly during `CLI_SERV_PLAY` state), the server stores the game record in `disconnect_game_record.txt` file. The disconnect game records will be restored from the file when the client reconnects to continue the previously unfinished game.
<br><br>
:point_right: Refer to `disconnUpdate`, `getDisconnRecord`, `disconnWrite` and `disconnRead` functions in `game.h`

-----------------------------------

**Use of commands** <br>
The server program can receive input commands from keyboard. Commands are used to display the specific game details and simulation of unreachable server.
| Command | Purpose  |
|--|--|
| a | Displays active game records (ongoing game details).   |
| d | Displays disconnect game records (unfinished games of the clients that have disconnected abruptly).  |
| p  | Displays past game records (best attempt of each client).  |
| sleep1, sleep2   | Pauses the server (to show the behavior of the client if the server is unreachable).   |
| help  | Displays valid input commands.| 

<br>

## :computer:  Client implementation
The client program is relatively simpler than the server program. The client uses `select()` to handle the standard input and its socket. Initially, the client sends the username to the server. The client then alternates between the `CLI_SENT` state and the `CLI_RCVD` state.
<p align=center><img src="/docs/pics/ClientStateTransitionDiagram.png" width=75% height=75%></p>
<p align="center"><i>Client State Transition Diagram</i></p>

Application-level keep-alive mechanism is also implemented in the client program.

<br>

## :black_nib: Reference
W. R. Stevens, B. Fenner and A. M. Rudoff, UNIX Network Programming: The Sockets Networking API, 3rd ed., vol. 1, Boston, Massachusetts: Addison-Wesley Professional, 2004.
