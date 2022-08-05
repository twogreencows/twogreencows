#!/bin/sh

echo "Test GET environment"
curl -s -N -X GET "http://localhost:3000/api/V1/environment" | python3 -m json.tool
echo ""
sleep 2

#echo "Test POST user"
#curl -X POST "http://localhost:3000/api/V1/users"

echo "Test GET all users"
#curl -X GET "http://localhost:3000/api/V1/users"
curl -s -N -X GET "http://localhost:3000/api/V1/users"| python3 -m json.tool
echo ""
sleep 2

echo "Test POST one user - missing parameters"
curl -s -N -X POST -H "Content-Type: application/json" -d '{"display_name":"paul", "password":"yesterday","phone_number":"+33687853132"}' "http://localhost:3000/api/V1/users" | python3 -m json.tool
# www-forme-url-encode
#
echo "Test POST one user - unmatched parameters"
curl -s -N -X POST -H "Content-Type: application/json" -d '{"display_name":"paul", "password":"yesterday","confirm_password":"yerblues","phone_number":"+33687853132"}' "http://localhost:3000/api/V1/users" | python3 -m json.tool


echo "Test POST one user - good parameters"
curl -s -N -X POST -H "Content-Type: application/json" -d '{"display_name":"paul", "password":"yesterday","confirm_password":"yesterday","phone_number":"+33687853132"}' "http://localhost:3000/api/V1/users" | python3 -m json.tool
echo ""
sleep 2

echo "Test POST one user - mismatch password on existing user"
curl -s -N -X POST -H "Content-Type: application/json" -d '{"display_name":"paul", "password":"yerblues","confirm_password":"yerblues","phone_number":"+33687853132"}' "http://localhost:3000/api/V1/users" | python3 -m json.tool
echo ""
sleep 2



#echo "Test GET one user"
#curl -X GET "http://localhost:3000/api/V1/user/usr-ewklewlkewkwekelwk222kl"

echo "Test POST session"

