#!/bin/bash
# c++ server on boost (ssl)
# chmod +x ./send_pack.bash
# password - uQqPa6vHJtEN
echo "sending..."
echo "password: uQqPa6vHJtEN"
scp -r site root@45.143.137.49:/root/serverhttps/
scp -r server root@45.143.137.49:/root/serverhttps/
scp -r start_server.bash root@45.143.137.49:/root/serverhttps/
scp -r redirect/redirect root@45.143.137.49:/root/serverhttps/
scp -r redirect/start_redirect.bash root@45.143.137.49:/root/serverhttps/
echo "done"