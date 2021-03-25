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

-- :name delete-user! :! :n
-- :doc deletes a user record given the uuid
DELETE FROM users
WHERE uuid = :uuid

-- :name get-users :? :*
-- :doc retrieves all user records
SELECT * FROM users

