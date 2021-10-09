CREATE TABLE IF NOT EXISTS environments (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    name VARCHAR(256) NOT NULL,
    sem_version VARCHAR(256) NOT NULL,
);

CREATE TABLE IF NOT EXISTS users (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMPTZ,
    display_name VARCHAR(256) NOT NULL,
    password VARCHAR(256) NOT NULL,
    country CHAR(3) NOT NULL,
    phone VARCHAR(32)
);


CREATE INDEX IF NOT EXISTS idx_users_uuid ON users(uuid);

CREATE TABLE IF NOT EXISTS tokens (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMPTZ,
    termination_date TIMESTAMPTZ,
    is_valid BOOL,
    user_uuid CHAR(36)
);

CREATE INDEX IF NOT EXISTS idx_tokens_uuid ON tokens(uuid);

CREATE TABLE IF NOT EXISTS sessions (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMPTZ,
    termination_date TIMESTAMPTZ,
    user_uuid CHAR(36),
    device_uuid CHAR(36),
    token_uuid CHAR(36),
    is_new_user BOOLEAN NOT NULL,
    is_new_device BOOLEAN NOT NULL
);


CREATE INDEX IF NOT EXISTS idx_sessions_uuid ON sessions(uuid);

CREATE TABLE IF NOT EXISTS devices (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMPTZ,
    kind VARCHAR(8),
    vendor_uuid CHAR(36),
    platform VARCHAR(8),  
    os_version VARCHAR(16),
    last_connection_date TIMESTAMPTZ
);

CREATE INDEX IF NOT EXISTS idx_devices_uuid ON devices(uuid);

CREATE TABLE IF NOT EXISTS greenhouses (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    user_uuid CHAR(36),
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMPTZ,
    latitude DOUBLE PRECISION,
    longitude DOUBLE PRECISION,
    display_name VARCHAR(256),
    description TEXT
);

CREATE INDEX IF NOT EXISTS idx_greenhouses_uuid ON greenhouses(uuid);

CREATE TABLE IF NOT EXISTS growboxes (
    uuid CHAR(36) UNIQUE NOT NULL PRIMARY KEY,
    user_uuid CHAR(36),
    object_version SMALLINT,
    data_version SMALLINT,
    display_name VARCHAR(256)
);

CREATE INDEX  IF NOT EXISTS idx_growboxes_uuid ON growboxes(uuid);

CREATE TABLE IF NOT EXISTS plants (
    uuid CHAR(36)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT, 
    creation_date TIMESTAMPTZ,
    modification_date TIMESTAMPTZ,
    display_name VARCHAR(256),
    description TEXT,
    bio_reference VARCHAR(256),
    growbox_position_idx SMALLINT,

    one_cycle BOOL,
    sowing_date  TIMESTAMPTZ,
    growing_start_date TIMESTAMPTZ,
    flowering_date TIMESTAMPTZ,
    harvest_date TIMESTAMPTZ

); 

CREATE INDEX IF NOT EXISTS idx_plants_uuid ON plants(uuid);

 

