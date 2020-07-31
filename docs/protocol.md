# Application protocol

## State diagram
![](plantuml/protocol.svg)

## Message types
Here are the fields and the explanation of each message type appearing in the state diagram:
- *error \<reason>* : either a runtime error due to user input or a programming error due to bug.
- *sign_up \<username> \<password>* : client requests to create a new account.
- *login \<username> \<password>* : client requests to log in with an existing account.
- *logout* : client logs out.
- *documents [\<list_of_documents>]* : client requests (without optional argument) or server sends (with optional argument) list of documents accessible by the client (created or obtained with sharing link).
- *create \<document_name>* : client requests to create a new document.
- *open \<document|sharing_link> [\<site_id> \<profile>]* : client requests to open an existing document (without optional arguments) or server notifies a new user with access to the document (with *\<document>* and optional arguments).
- *document \<document> \<list_of_symbols> \<map_site_id_to_profile> \<map_username_to_symbol> \<sharing_link>* : server sends all the info about the requested (open/create) document.
- *insert \<document> \<symbol>* : insert symbol in an opened document.
- *erase \<document> \<symbol>* : erase symbol from an opened document.
- *cursor \<document> \<username> \<symbol>* : move cursor of a client in an opened document.
- *close \<document> [\<username>]* : client requests to close an opened document (without optional argument) or server notifies a close to the other editing users (with optional argument).
- *profile \<profile> [\<password>]* : client requests a profile update.
- *profile_ok* : server confirms success of profile update.

Further details:
- *\<document>* contains: *owner* (username) and *document_name*.
- *\<symbol>* contains: *site_id*, *site_counter*, *value*, and *position* (array of integers).
- *\<profile>* contains: *username*, *name*, *surname*, and *icon*.

Notes:
- *\<map_site_id_to_profile>* in *document* contains the *site_id* of each user with access to the document (even if currently offline), necessary for text coloring.
- *\<map_username_to_symbol>* in *document* contains the cursor (symbol) for each user currently editing the document.
- When a document is closed by a client, the server sends *close* to the other users editing that document, so that they can remove the cursor.
