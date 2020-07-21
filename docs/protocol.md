# Application protocol

## State diagram
![](plantuml/protocol.svg)

## Message types
Here are the fields and the explanation of each message type appearing in the state diagram:
- *error \<reason>* : either a runtime error due to user input or a programming error due to bug.
- *sign_up \<username> \<password>* : client requests to create a new account.
- *login \<username> \<password>* : client requests to logs in with an existing account.
- *logout* : client logs out.
- *documents \<list_of_document_names>* : server sends the list of documents owned by the user (created or obtained with sharing link).
- *create \<document_name>* : client requests to create a new document
- *open \<document_name>* : client requests to open an existing document
- *text \<document_name> \<document_text>* : server sends full text of the opened/created document (created => empty text)
- *insert \<document_name> \<symbol>* : insert symbol in a document
- *erase \<document_name> \<symbol>* : erase symbol from a document
- *cursor \<document_name> \<username> \<symbol>* : move cursor of a client in a document
- *close \<document_name> \<username>* : close a document for a client 

*insert*, *erase*, *cursor* and *close* travel both:
- From client to server: client notifies a local action
- From server to client: server dispatches actions

Finally:
- When a document is opened by a client, the server sends *text* and a sequence of *cursor*, one for each user currently editing that document.
- When a document is closed by a client, the server sends *close* to the others so that they can remove the cursor.
