CREATE TABLE IF NOT EXISTS WebIMGppUsers (
   id INT AUTO_INCREMENT PRIMARY KEY,
   username VARCHAR(50) NOT NULL UNIQUE,
   password VARCHAR(255) NOT NULL
);