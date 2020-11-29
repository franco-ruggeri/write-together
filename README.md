# Write TOgether
Write TOgether is a real-time collaborative text editor made in PoliTO (Politecnico di Torino) as project of the course System Programming (Programmazione di Sistema). It is inspired by [Conclave](https://conclave-team.github.io/conclave-site/) and [Overleaf](https://www.overleaf.com/).

## Install dependencies
### On Ubuntu
```bash
cd scripts/
./install_dependencies.sh
```

## Run the server
### On Linux
#### Server:
1. [Optional] If you want to use the server not only on localhost:
    1. Change the password in the [SQL script](https://github.com/franco-ruggeri/collaborative-text-editor/blob/master/src/database/sql/database_create.sql) and in the [code](https://github.com/franco-ruggeri/collaborative-text-editor/blob/master/src/database/database_utility_secret.cpp).
    2. Replace certificate and private key for the server. Read the [instructions](https://github.com/franco-ruggeri/collaborative-text-editor/blob/master/docs/setup_ssl.md).
2. Start MySQL and create the database using the [SQL script](https://github.com/franco-ruggeri/collaborative-text-editor/blob/master/src/database/sql/database_create.sql).
3. Run the server (on port 5000 and with saving period 5 seconds):
    ```bash
    cd scripts/
    ./run.sh server 5000 5000
    ```

## Run the client
### On Linux
1. Run these commands:
    ```bash
    cd scripts/
    ./install.sh
    ```
2. Launch the application (connect to localhost on port 5000).

## Functionalities
### Requirements
- [x] Client-server architecture
- [x] Each user has a profile (username, name, surname, email, icon)
- [x] Authentication with username and password
- [x] Client can sign up and log in
- [x] Client can edit their profile
- [x] Client can create or open documents
- [x] Clients can collaboratively edit documents (coherence guaranteed)
- [x] Server automatically saves the documents
- [x] Client shows number and identity of users editing a document
- [x] Client shows the cursors of the other users
- [x] Client can highlight the text with colors according to the author

### Extensions
- [x] Export document to PDF
- [x] Format of characters (bold, italic, underline)
- [x] Undo, Redo, Cut, Copy, Paste (with menu entries, toolbar and shortcuts)
- [x] Invite to collaboration (by means of URI)
- [x] Multiple sessions of same user (e.g. a user can log in from different devices)
- [x] Multiple open documents (i.e. a user can open more documents at the same time)
- [x] Remote cursor update also for position changed (not just insert/erase as in Conclave)
- [x] Encrypted connection with TLS
- [x] List of both online and offline users with access to the document (possibility of text coloring also for offline users)
- [x] View full profile of other users
- [x] Filters for document list
- [x] Word and char count

## Design choices:
- Both client and server are portable and multi-platform, being written with the powerful [Qt framework](https://www.qt.io/). They have been tested on Ubuntu 18.04, Ubuntu 20.04 and Windows 10.
- The application protocol is designed ad hoc and is built on top of TCP. Currently it is textual and uses JSON. Have a look at the documentation [here](https://github.com/franco-ruggeri/collaborative-text-editor/blob/master/docs/protocol.md).
- The client uses 1 thread.
- The server uses an ideal number of threads based on the number of cores. The sockets are distributed uniformly among the threads. 
- The server uses MySQL to save periodically the open documents. All the queries are prepared as prepared statements to guarantee protection against SQL injection. 
- The server does not store password as plain-text, it stores hashes computed using [Crypto++](https://www.cryptopp.com/).
- Both client and server use UTF-16 encoding.
