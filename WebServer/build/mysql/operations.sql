CREATE TABLE `sensors`.`dht` (
   `id` int NOT NULL AUTO_INCREMENT,
   `temperature` int DEFAULT NULL,
   `humidity` int DEFAULT NULL,
   `date` datetime DEFAULT NULL,
   PRIMARY KEY (`id`),
   UNIQUE KEY `id_UNIQUE` (`id` ASC) VISIBLE);