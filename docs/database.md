# Database

## Conceptual schema
![](images/er_diagram.jpg)

## Logical schema

### Tables
User(**username**, password, name, surname, icon)  
Document(**owner**, **name**, sharing_link)  
Character(**document_name**, **document_owner**, **index**, author, value)   
Sharing(**sharing_user**, **document_owner**, **document_name**)  

### Referential integrity constraints
Document(owner) REFERENCES User(username)  
Character(document_owner, document_name) REFERENCES Document(owner, name)  
Sharing(sharing_user) REFERENCES User(username)  
Sharing(document_owner, document_name) REFERENCES Document(owner, name)