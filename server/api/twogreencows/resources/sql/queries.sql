-- :name create-user! :! :n
-- :doc creates a new user record
INSERT INTO users
(uuid, object_version, data_version, creation_date, display_name, password, country,phone)
VALUES (:uuid, :object_version, :data_version, :creation_date, :display_name, :password, :country, :phone)

-- :name update-user! :! :n
-- :doc updates an existing user record
UPDATE users
SET object_version = :object_version, data_version = :data_version, display_name = :display_name, password = :password, country = :country, phone = :phone 
WHERE uuid = :uuid

-- :name get-user-by-uuid :? :1
-- :doc retrieves a user record given the uuid
SELECT * FROM users
WHERE uuid LIKE :uuid

-- :name delete-user-with-uuid! :? :1
-- :doc deletes a user record given the uuid, if it exists.
DELETE FROM users
WHERE uuid = :uuid

-- :name get-users :? :*
-- :doc retrieves all user records
SELECT uuid, object_version, data_version, creation_date, display_name, password, country, phone FROM users



-- :name create-device! :! :n
-- :doc creates a new device record
INSERT INTO devices
(uuid, object_version, data_version, creation_date, last_connection_date, kind, vendor_uuid, platform, os_version)
VALUES (:uuid, :object_version, :data_version, :creation_date, :last_connection_date, :kind, :vendor_uuid, :platform, :os_version)

-- :name update-device! :! :n
-- :doc updates an existing device record
UPDATE devices
SET object_version = :object_version, data_version = :data_version, last_connection_date= :last_connection_date, kind = :kind, vendor_uuid= :vendor_uuid, platform = :platform, os_version= :os_version 
WHERE uuid = :uuid

-- :name get-device-by-uuid :? :1
-- :doc retrieves a device record given the uuid
SELECT * FROM devices
WHERE uuid LIKE :uuid

-- :name delete-device-with-uuid! :? :1
-- :doc deletes a user record given the uuid, if it exists.
DELETE FROM devices
WHERE uuid = :uuid

-- :name get-devices-with-owner-uuid :? :*
-- :doc retrieves all device records for a user
SELECT uuid, object_version, data_version, creation_date, display_name, password, country, phone FROM users

