# Multiple-Client-Server-chat
Chat application that allows multiple clients to chat through a server
Multi threaded implementation of Client to Client communication through server.
Functionalities: 
• Multiple clients can connect to server.
• A list of active clients shall be displayed, each time a client connects successfully 
• The messages are to be specified as [destination client id]  [msg] 
• The message will be sent to client with id same as  the destination client id. 
• A client can disconnect to the network by sending a message “logout” • Communication with client that had logged out isn’t allowed.

Implementation in code: 
Server.c: 
• Each time a new client is connected, a new thread is spawned
• A linked list of structure cl_details is used to store details of connected clients 
• Disconnected client details are removed from the list using delete_client function. 
• Search function returns corresponding socket id, given client id.      

Client.c: 
• 2 threads are spawned, one for send and other for receive
• A client can quit by sending “logout” message.
• Structures are used to send arguments to the threads.
• (Make sure to change ip address before testing the code)

