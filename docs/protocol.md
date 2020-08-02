# Application protocol

## State diagram
![](plantuml/protocol.svg)

## Message types
Here are the fields and the explanation of each message type appearing in the state diagram:
- *error \<reason>* : either a runtime error due to user input or a programming error due to bug.
- *sign_up \<username> \<password>* : client requests to create a new account.
- *login \<username> \<password>* : client requests to log in with an existing account.
- *logout* : client logs out.
- *profile \<profile> [\<password>]* : client requests a profile update.
- *profile_ok* : server confirms success of profile update.
- *documents [\<list_of_documents>]* : client requests (without optional argument) or server sends (with optional argument) list of documents accessible by the client (created or obtained with sharing link).
- *create \<document_name>* : client requests to create a new document.
- *open \<document|sharing_link> [\<site_id> \<profile>]* : client requests to open an existing document (without optional arguments) or server notifies a new user with access to the document (with *\<document>* and optional arguments).
- *close \<document> [\<username>]* : client requests to close an opened document (without optional argument) or server notifies a close to the other editing users (with optional argument).
- *document \<document> \<text> \<site_id> \<site_counter> \<site_ids> \<profiles> \<cursors> \<sharing_link>* : server sends all the info about the requested (open/create) document.
- *insert \<document> \<symbol>* : insert symbol in an opened document.
- *erase \<document> \<symbol>* : erase symbol from an opened document.
- *cursor \<document> \<symbol> [\<username>]* : move cursor of a client in an opened document (client to server without optional argument, server to client with optional argument).

Further details:
- *\<document>* contains: *owner* (username) and *document_name*.
- *\<symbol>* contains: *site_id*, *site_counter*, *value*, and *position* (array of integers).
- *\<profile>* contains: *username*, *name*, *surname*, and *icon*.
- *\<text>* (in *document*) contains a sorted array of *\<symbol>*.
- *\<site_id>* and *\<site_counter>* (in *document*) are the *site_id* and starting *site_counter* assigned to the client.
- *\<site_ids>* (in *document*) contains a map username -> *site_id*, for all the other users with access to the document (even if currently offline).
- *\<profiles>* (in *document*) contains a map username -> *\<profile>*, for all the other users with access to the document (even if currently offline).
- *\<cursors>* (in *document*) contains a map username -> *\<symbol>*, for all the other users currently editing the document.
