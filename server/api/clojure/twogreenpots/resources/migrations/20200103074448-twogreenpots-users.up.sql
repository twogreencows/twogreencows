CREATE TABLE IF NOT EXISTS users (uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    display_name VARCHAR(256) NOT NULL,
    password VARCHAR(256) NOT NULL,
    country CHAR(3) NOT NULL,
    phone VARCHAR(32)
);
