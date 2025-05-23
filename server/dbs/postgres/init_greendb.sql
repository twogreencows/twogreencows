CREATE TABLE IF NOT EXISTS environments (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    name VARCHAR(256) NOT NULL,
    sem_version VARCHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS users (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    display_name VARCHAR(256) NOT NULL UNIQUE,
    salt VARCHAR(256) NOT NULL, 
    password VARCHAR(256) NOT NULL,
    country CHAR(3) NOT NULL,
    phone_number VARCHAR(32) UNIQUE,
    email VARCHAR(320) UNIQUE,
    user_level SMALLINT DEFAULT 0
);


CREATE INDEX IF NOT EXISTS idx_users_uuid ON users(uuid);
/*INSERT INTO USERS (uuid, object_version, data_version, created_at, updated_at, display_name, salt, password, country, phone_number, email, user_level) \
                values ('usr-' || replace(gen_random_uuid()::text, '-', ''), 1, 1, now(), now(), 'admin', , 128);*/


CREATE TABLE IF NOT EXISTS tokens (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    owner_uuid CHAR(36),
    expires_at TIMESTAMPTZ,
    is_valid BOOL,
    kind VARCHAR(4),
    value VARCHAR(256),
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid) ON DELETE CASCADE 
);

CREATE INDEX IF NOT EXISTS idx_tokens_uuid ON tokens(uuid);

CREATE TABLE IF NOT EXISTS devices (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    owner_uuid CHAR(36),
    
    kind VARCHAR(8),
    display_name VARCHAR(256) NOT NULL,
    vendor_uuid CHAR(36),
    platform VARCHAR(8),  
    os_version VARCHAR(16),
    last_connection_date TIMESTAMPTZ,
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid) ON DELETE CASCADE  
);

CREATE INDEX IF NOT EXISTS idx_devices_uuid ON devices(uuid);


CREATE TABLE IF NOT EXISTS devices_tokens (

    device_uuid CHAR(36),
    token_uuid CHAR(36),
    PRIMARY KEY(device_uuid, token_uuid),

    CONSTRAINT fk_device FOREIGN KEY(device_uuid) REFERENCES devices(uuid),   
    CONSTRAINT fk_token  FOREIGN KEY(token_uuid) REFERENCES tokens(uuid)   
);

CREATE TABLE IF NOT EXISTS sessions (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,
    
    terminated_at TIMESTAMPTZ,
    expires_at TIMESTAMPTZ,
    owner_uuid CHAR(36),
    user_uuid CHAR(36),
    device_uuid CHAR(36),
    token_uuid CHAR(36),
    is_new_user BOOLEAN NOT NULL,
    is_new_device BOOLEAN NOT NULL,
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid), 
    CONSTRAINT fk_user FOREIGN KEY(user_uuid) REFERENCES users(uuid), 
    CONSTRAINT fk_device FOREIGN KEY(device_uuid) REFERENCES devices(uuid), 
    CONSTRAINT fk_token FOREIGN KEY(token_uuid) REFERENCES tokens(uuid) 
);


CREATE INDEX IF NOT EXISTS idx_session_uuid ON sessions(uuid);


CREATE TABLE IF NOT EXISTS greenhouses (
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
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid) 
);

CREATE INDEX IF NOT EXISTS idx_greenhouses_uuid ON greenhouses(uuid);

CREATE TABLE IF NOT EXISTS growboxes (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,

    owner_uuid CHAR(36),
    greenhouse_uuid CHAR(36),
    
    display_name VARCHAR(256),
    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid), 
    CONSTRAINT fk_greenhouse FOREIGN KEY(greenhouse_uuid) REFERENCES greenhouses(uuid) 
);

CREATE INDEX  IF NOT EXISTS idx_growboxes_uuid ON growboxes(uuid);

CREATE TABLE IF NOT EXISTS plants (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT, 
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ,

    display_name VARCHAR(256),
    description TEXT,
    bio_reference VARCHAR(256)
);
 

CREATE INDEX IF NOT EXISTS idx_plants_uuid ON plants(uuid);

CREATE TABLE IF NOT EXISTS stems (
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


    CONSTRAINT fk_owner FOREIGN KEY(owner_uuid) REFERENCES users(uuid), 
    CONSTRAINT fk_growbox FOREIGN KEY(growbox_uuid) REFERENCES growboxes(uuid), 
    CONSTRAINT fk_plant FOREIGN KEY(plant_uuid) REFERENCES plants(uuid) 
); 

CREATE INDEX IF NOT EXISTS idx_stem_uuids ON stems(uuid);
