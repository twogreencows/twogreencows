CREATE KEYSPACE tgc_data WITH replication = {'class': 'SimpleStrategy', 'replication_factor': '1'} AND durable_writes = true;


CREATE TABLE tgc_data.datapoints (
    uuid ascii, 
    data_version int,
    object_version int,
    sensor_uuid ascii,
    media_uuid ascii,
    type ascii,
    record_date timestamp,
    value double,
    calibration double,
    description text,
    PRIMARY KEY ( (uuid, sensor_uuid), timestamp)
) WITH CLUSTERING ORDER BY (timestamp ASC);


CREATE TABLE tgc_data.sensors( 
    uuid ascii,
    data_version int,
    object_version int,
    kind ascii,
    brand text,
    unit ascii,
    model text,
    precision double,
    PRIMARY KEY ((kind, brand), uuid)
) WITH CLUSTERING ORDER BY (uuid ASC);

CREATE TABLE tgc_data.media{
    uuid ascii,
    data_version int,
    object_version int,
    path ascii,
    hash_name asciim
    PRIMARY KEY (uuid)
} WITH CLUSTERING ORDER BY (uuid ASC);

