-- :name create-user! :! :n
-- :doc creates a new user record
INSERT INTO users
(uuid, object_version, data_version, created_date, updated_at, display_name, password, country,phone_number)
VALUES (:uuid, :object_version, :data_version, :created_at, :updated_at, :display_name, :password, :country, :phone_number) RETURNING *

-- :name update-user! :! :n
-- :doc updates an existing user record
UPDATE users
SET object_version = :object_version, data_version = :data_version, display_name = :display_name, password = :password, country = :country, phone_number = :phone_number
WHERE uuid = :uuid RETURNING *

-- :name get-user-by-uuid :? :1
-- :doc retrieves a user record given the uuid
SELECT * FROM users
WHERE uuid LIKE :uuid

-- :name get-user-by-display_name :? :1
-- :doc retrieves a user record given the display_name 
SELECT * FROM users
WHERE display_name LIKE :display_name

-- :name delete-user-with-uuid! :? :1
-- :doc deletes a user record given the uuid, if it exists.
DELETE FROM users
WHERE uuid = :uuid RETURNIN *

-- :name get-users :? :*
-- :doc retrieves all user records
SELECT uuid, object_version, data_version, creation_date, display_name, password, country, phone_number FROM users



-- :name create-token! :! :n
-- :doc creates a new token record
INSERT INTO tokenss
(uuid, object_version, data_version, creation_date, expiration_date, is_valid, value ,owner_uuid)
VALUES (:uuid, :object_version, :data_version, :creation_date, :expiration_date, :is_valid, :value, :owner_uuid) RETURNING *

-- :name update-token! :! :n
-- :doc updates an existing token record
UPDATE tokens
SET object_version = :object_version, data_version = :data_version, expiration_date= :expiration_date, is_valid= :is_valid, = :is_valid, owner_uuid= :owner_uuid
WHERE uuid = :uuid RETURNING *

-- :name get-token-by-uuid :? :1
-- :doc retrieves a token record given the uuid
SELECT * FROM tokens
WHERE uuid LIKE :uuid

-- :name get-tokens-for-owner :? :1
-- :doc retrieves a user record given the owner_uuid 
SELECT * FROM tokens 
WHERE owner_uuid LIKE :owner_uuid




-- :name create-device! :! :n
-- :doc creates a new device record
INSERT INTO devices
(uuid, object_version, data_version, creation_date, last_connection_date, kind, vendor_uuid, platform, os_version)
VALUES (:uuid, :object_version, :data_version, :creation_date, :last_connection_date, :kind, :vendor_uuid, :platform, :os_version) RETURNING *

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
SELECT uuid, object_version, data_version, creation_date, display_name, password, country, phone_number FROM users


-- :name get-environments :? :*
-- :doc retrieves all environment records
SELECT uuid, object_version, data_version, created_at, updated_at, name, sem_version FROM environments


-- :name create-environment! :! :n
-- :doc creates a new environment record
INSERT INTO environments
(uuid, object_version, data_version, created_at, updated_at, name , sem_version)
VALUES (:uuid, :object_version, :data_version, :created_at, :updated_at,:name, :sem_version)
RETURNING *;

-- :name get-environment-by-uuid :? :1
-- :doc retrieves a environment record given the uuid
SELECT * FROM environments
WHERE uuid LIKE :uuid
