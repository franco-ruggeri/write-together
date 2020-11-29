-- configurations
SET default_storage_engine = InnoDB;

-- create DB
CREATE DATABASE IF NOT EXISTS write_together CHARACTER SET utf16;
use write_together;

SET AUTOCOMMIT = 0;
START TRANSACTION;

-- create tables
CREATE TABLE IF NOT EXISTS `user`
(
	`username` VARCHAR(100) NOT NULL,
	`password` VARCHAR(200) NOT NULL,
	`name` VARCHAR(100),
	`surname` VARCHAR(100),
	`email` VARCHAR(320),   -- standard says max 64 for username + 1 for '@' + max 255 for domain name
	`icon` LONGBLOB,
	PRIMARY KEY (username)
);

CREATE TABLE IF NOT EXISTS `document`
(
	`owner` VARCHAR(50) NOT NULL,
	`name` VARCHAR(50) NOT NULL,
	`sharing_link` VARCHAR(150) UNIQUE NOT NULL,
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
    `bold` BOOLEAN NOT NULL,
    `italic` BOOLEAN NOT NULL,
    `underlined` BOOLEAN NOT NULL,
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
CREATE USER IF NOT EXISTS 'write_together'@'%' IDENTIFIED BY '?PdSPr0j3ct!';
CREATE USER IF NOT EXISTS 'write_together'@'localhost' IDENTIFIED BY '?PdSPr0j3ct!';
GRANT SELECT, INSERT, UPDATE, DELETE ON write_together.* TO 'write_together'@'%';
GRANT SELECT, INSERT, UPDATE, DELETE ON write_together.* TO 'write_together'@'localhost';

COMMIT;