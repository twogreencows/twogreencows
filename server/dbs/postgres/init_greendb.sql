CREATE TABLE IF NOT EXISTS environment (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    name VARCHAR(256) NOT NULL,
    sem_version VARCHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS user (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    display_name VARCHAR(256) NOT NULL,
    password VARCHAR(256) NOT NULL,
    country CHAR(3) NOT NULL,
    phone_number VARCHAR(32)
);


CREATE INDEX IF NOT EXISTS idx_user_uuid ON user(uuid);

CREATE TABLE IF NOT EXISTS token (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    owner_uuid CHAR(36),
    expiration_date TIMESTAMPTZ,
    is_valid BOOL,
    value CHAR(256),
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid) 
);

CREATE INDEX IF NOT EXISTS idx_token_uuid ON token(uuid);

CREATE TABLE IF NOT EXISTS device (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    owner_uuid CHAR(36),
    
    kind VARCHAR(8),
    vendor_uuid CHAR(36),
    platform VARCHAR(8),  
    os_version VARCHAR(16),
    last_connection_date TIMESTAMPTZ,
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid) 
);

CREATE INDEX IF NOT EXISTS idx_device_uuid ON device(uuid);


CREATE TABLE IF NOT EXISTS session (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    termination_date TIMESTAMPTZ,
    owner_uuid CHAR(36),
    user_uuid CHAR(36),
    device_uuid CHAR(36),
    token_uuid CHAR(36),
    is_new_user BOOLEAN NOT NULL,
    is_new_device BOOLEAN NOT NULL,
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid), 
    CONSTRAINT fk_user FOREIGN KEY(user_uuid) REFERENCES user(uuid), 
    CONSTRAINT fk_device FOREIGN KEY(device_uuid) REFERENCES device(uuid), 
    CONSTRAINT fk_token FOREIGN KEY(token_uuid) REFERENCES token(uuid) 
);


CREATE INDEX IF NOT EXISTS idx_session_uuid ON session(uuid);


CREATE TABLE IF NOT EXISTS greenhouse (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    owner_uuid CHAR(36),
    
    latitude DOUBLE PRECISION,
    longitude DOUBLE PRECISION,
    display_name VARCHAR(256),
    description TEXT,
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid) 
);

CREATE INDEX IF NOT EXISTS idx_greenhouse_uuid ON greenhouse(uuid);

CREATE TABLE IF NOT EXISTS growbox (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,

    owner_uuid CHAR(36),
    greenhouse_uuid CHAR(36),
    
    display_name VARCHAR(256),
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid), 
    CONSTRAINT fk_greenhouse FOREIGN KEY(greenhouse_uuid) REFERENCES greenhouse(uuid) 
);

CREATE INDEX  IF NOT EXISTS idx_growbox_uuid ON growbox(uuid);

CREATE TABLE IF NOT EXISTS plant (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT, 
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,

    display_name VARCHAR(256),
    description TEXT,
    bio_reference VARCHAR(256)
};
 

CREATE INDEX  IF NOT EXISTS idx_plant_uuid ON plant(uuid);

CREATE TABLE IF NOT EXISTS stem (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT, 
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,

    owner_uuid CHAR(36),
    plant_uuid CHAR(36),
    growbox_uuid CHAR(36),
    
    display_name VARCHAR(256),
    description TEXT,
    growbox_position_idx SMALLINT,

    one_cycle BOOL,
    sowing_date  TIMESTAMPTZ,
    growing_start_date TIMESTAMPTZ,
    flowering_date TIMESTAMPTZ,
    harvest_date TIMESTAMPTZ,


    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES user(uuid), 
    CONSTRAINT fk_growbox FOREIGN KEY(growbox_uuid) REFERENCES growbox(uuid), 
    CONSTRAINT fk_plant FOREIGN KEY(plant_uuid) REFERENCES plant(uuid) 
); 

CREATE INDEX IF NOT EXISTS idx_stem_uuid ON stem(uuid);

 

