asyncHTTPSserver on Boost
--------
commands to compile:

1.g++ *.cpp -o server -pthread -lboost_system -lcrypto -lssl -std=c++17


commands to generate PRIVATE KEY and CERTIFICATE:

1.openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem