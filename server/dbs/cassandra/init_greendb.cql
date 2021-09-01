CREATE KEYSPACE tgc_data WITH replication = {'class': 'SimpleStrategy', 'replication_factor': '1'} AND durable_writes = true;


CREATE TABLE tgc_data.datapoints (
    uuid ascii, 
    data_version int,
    sensor_uuid ascii
    type ascii,
    object_identifier ascii,
    timestamp timestamp,
    value double,
    calibration double,
    attachments text
    PRIMARY KEY ((uuid,sensor_uuid), timestamp)
) WITH CLUSTERING ORDER BY (timestamp ASC);



CREATE TABLE tgc_data.sensors ( text, PRIMARY KEY (search_hash))
CREATE TABLE tgc_data.( 
    uuid ascii
    data_version int
    kind ascii,
    brand text,
    unit ascii
    model text,
    
    PRIMARY KEY ((id, month), timestamp)
) WITH CLUSTERING ORDER BY (timestamp ASC);
