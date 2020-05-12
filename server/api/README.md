# TWOGREENCOWS SERVER SPECIFICATION



## Introduction
This document describes the elements composing the twogreencows platform. Its main goal is to provide an
information system that will help the grow of many plants. Growing cnan be difficult and a learning curve 
is necessary. The first question is what are the expectation and how to act. The pltform wwants to answer the 
following questions:
- Can I monitor the growth my plants with safety and trust
- Can I get tangible information and write notes to help me get better
- Can I get a catalog of plants
- Can I use collective intelligence to classify and understand problems.

From a high level point of view the following entities contributes to the general system. A user is owning greenhouses. 
In Each greenhouse, there are growboxes. Each growbox represent a physical device recording information about plants.
A growbox may monitor multiple plant, but it is not part of their role to know this. A growboz is basically a recording information.
To each growbox are attached one or multiple plants. If desired, the user may decide to assign plants growboxes. Each plant may be part of a pool common to all users in order to provide collective knowledge.

The system is comprised of multiple elemeents
- A **server** is acting as a central point. The platform can be made available  in multiple ways. On the main twogreencows 
server which is available on the Internet OR ONon a local server which can bbe installed in a home. In both cases protection 
will be enable to guarantee a protected access to the user database.
- AN **agent** is a small entity running on a device. It is basically ZZa recorder


However a local server may provide autoconfigurable elements via Bonjour protocol.
From a fundamental point of 


## Entities

The entry point  entitiy is the **user**. It basically corresponds to someone who has an account on a twogreencow platform

CREATE TABLE IF NOT EXISTS users (
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    display_name VARCHAR(256) NOT NULL,
    password VARCHAR(256) NOT NULL,
    country CHAR(3) NOT NULL,
    phone VARCHAR(32)
);
CREATE INDEX IF NOT EXISTS idx_users_uuid ON users(uuid);

CREATE TABLE IF NOT EXISTS sessions (
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    termination_date TIMESTAMP,
    user_uuid CHAR(34),
    device_uuid CHAR(34),
    is_new_user BOOLEAN NOT NULL,
    is_new_device BOOLEAN NOT NULL
);
CREATE INDEX IF NOT EXISTS idx_sessions_uuid ON sessions(uuid);

CREATE TABLE IF NOT EXISTS devices (
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    kind VARCHAR(8),
    vendor_uuid CHAR(34),
    platform VARCHAR(8),
    os_version VARCHAR(12),
    last_connection_date TIMESTAMP
);
CREATE INDEX IF NOT EXISTS idx_devices_uuid ON devices(uuid);

CREATE TABLE IF NOT EXISTS greenhouses (
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    latitude DOUBLE PRECISION,
    longitude DOUBLE PRECISION,
    display_name VARCHAR(256),
    description TEXT
);
CREATE INDEX IF NOT EXISTS idx_greenhouses_uuid ON greenhouses(uuid);

CREATE TABLE IF NOT EXISTS plants (
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    modificaiton_date TIMESTAMP,
    scientific_name VARCHAR(256),
    common_name VARCHAR(256),
    flavor_name VARCHAR(256),
    family VARCHAR(256)
);
CREATE INDEX IF NOT EXISTS idx_plants_uuid ON plants(uuid);

CREATE TABLE IF NOT EXISTS recordpoints(
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    creation_date TIMESTAMP,
    unit VARCHAR(24),
    value VARCHAR(36),
    description TEXT,
    sensor_uuid CHAR(34),
    media_uuid CHAR(34)
);
CREATE INDEX IF NOT EXISTS idx_recordpoints_uuid ON recordpoints(uuid);


CREATE TABLE IF NOT EXISTS media(
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    path  VARCHAR(512),
    hash_name VARCHAR(256)
);
CREATE INDEX IF NOT EXISTS idx_media_uuid ON media(uuid);

CREATE TABLE IF NOT EXISTS sensors(
    uuid CHAR(34)  UNIQUE NOT NULL PRIMARY KEY,
    object_version SMALLINT,
    data_version SMALLINT,
    precision DOUBLE PRECISION,
    sensor_type VARCHAR(24),
    unit VARCHAR(24),
    model VARCHAR(256)
);
