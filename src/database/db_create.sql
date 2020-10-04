-- configurations
SET default_storage_engine = InnoDB;

-- create DB
CREATE DATABASE IF NOT EXISTS collaborative_text_editor CHARACTER SET utf16;
use collaborative_text_editor;

SET AUTOCOMMIT = 0;
START TRANSACTION;

-- create tables
CREATE TABLE IF NOT EXISTS `user`
(
	`username` VARCHAR(100) NOT NULL,
	`password` VARCHAR(200) NOT NULL,
	`name` VARCHAR(100),
	`surname` VARCHAR(100),
	`icon` BLOB,
	PRIMARY KEY (username)
);

CREATE TABLE IF NOT EXISTS `document`
(
	`owner` VARCHAR(100) NOT NULL,
	`name` VARCHAR(100) NOT NULL,
	`sharing_link` VARCHAR(200) UNIQUE NOT NULL,
	PRIMARY KEY (`owner`, `name`),
	FOREIGN KEY (`owner`) REFERENCES user(`username`) ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS `character`
(
	`document_owner` VARCHAR(100) NOT NULL,
	`document_name` VARCHAR(100) NOT NULL,
	`index` INT NOT NULL,
	`value` VARCHAR(1) NOT NULL,    -- not CHAR because CHAR removes trailing spaces
    `author` VARCHAR(100) NOT NULL,
	PRIMARY KEY (`document_owner`, `document_name`, `index`),
	FOREIGN KEY (`document_owner`, `document_name`) REFERENCES `document`(`owner`, `name`) ON UPDATE CASCADE,
	FOREIGN KEY (`author`) REFERENCES user(`username`) ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS sharing
(
	`sharing_user` VARCHAR(100) NOT NULL,
	`document_owner` VARCHAR(100) NOT NULL,
	`document_name` VARCHAR(100) NOT NULL,
	FOREIGN KEY (`sharing_user`) REFERENCES user(`username`) ON UPDATE CASCADE,
	FOREIGN KEY (`document_owner`, `document_name`) REFERENCES `document`(`owner`, `name`) ON UPDATE CASCADE
);


-- create application user
CREATE USER IF NOT EXISTS 'collaborative_text_editor'@'%' IDENTIFIED BY '?PdSPr0j3ct!';
CREATE USER IF NOT EXISTS 'collaborative_text_editor'@'localhost' IDENTIFIED BY '?PdSPr0j3ct!';
GRANT SELECT, INSERT, UPDATE, DELETE ON collaborative_text_editor.* TO 'collaborative_text_editor'@'%';
GRANT SELECT, INSERT, UPDATE, DELETE ON collaborative_text_editor.* TO 'collaborative_text_editor'@'localhost';

COMMIT;