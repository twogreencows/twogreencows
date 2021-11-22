Twogreencoes  server
==============

Build API


Run Server
docker-compose up 

build with docker stack
A user has a session. He also has  greenhouses

POST /user username-password-confirmpass
GET  /user/me 
PUT  /user/me username. / password+confirm
POST /user/me/token
GET  /user/me/tokens
DELETE /user/me
DELETE /user/username Super admin only

GET /user/me/greenhouses
POST /user/me/greenhouse
POST /user/me/greenhouses/UUID/growbox
GET /user/me/greehouses/UUID/growboxes
POST /user/me/greenhouses/UUID/ growboxes/UUID/plant
GET /user/me/greenhouse/UUID
GET /useer/me/greenhouse


POST /session token or password+username - should come with a device (a device will log in with a token). A device will be part of a SESSION
(to do client :  pass a token to a device esp RPI)


device
uuid - deviceUUID - locale - type (mobile/web/desktop) - OSVersion 

session
Time started - Device - 

Launch DB Locally
- docker-compose up 
- psql -h localhost -p 5432 -d twogreencows -U onebigcow -W


Launch and run test DB
- docker image build -t tgc_server_db_test - < DockerfileDB-Test
- docker run -p 5433:5432 server_db_test
- psql -h localhost -p 5433 -d twogreencows-test -U onebigcow -W


User and session management
- We'll start with a classical user/password schema. The user is not mandatory an email. We'd like by not storing user data like email.
- a user can also generates tokens for login from a device
- a registration should be a POST /user ?
- there is a need of session concept that is linking the user and the device
- should there be a login
- POST session is with user and password and returns a session
- DELETE session close a session
- User contains all tokens
- if there is a token this one can be send in a header X-Session

-user  if a user is not existing there is a 401 error 



All answer from server have a "data" component and a "server" component (that may be empty i prod but returns serveur information.Like processing time)


For cassandra

docker build -t  my-cassandra-image --progress=plain --file DockerfileDB_CAS .
docker run --rm -p 9042:9042 --name cassandra-container -d my-cassandra-image
docker exec -it 851 /bin/bash

