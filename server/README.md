Catfish server
==============

Build API
docker build -t greenhouse .
docker run --publish 6060:8080 -name test rm outyet

build with docker stack
A user has a session. He also has  greenhouses


device
uuid - deviceUUID - locale - type (mobile/web/desktop) - OSVersion 

session
Time started - Device - 

Launch DB Locally
- docker-compose up 
- psql -h localhost -p 5432 -d twogreencows -U onebigcow -W


Launch and run test DB
- docker image build -t server_db_test - < DockerfileDB-Test
- docker run -p 5433:5432 server_db_test
- psql -h localhost -p 5433 -d twogreencows-test -U onebigcow -W
