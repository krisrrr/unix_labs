#!/bin/sh

INSTALL_PATH=/etc/hello
EXEC_PATH="$INSTALL_PATH/writer.sh"

echo "1. Creating install dir at $INSTALL_PATH"
mkdir -p "$INSTALL_PATH"
cp writer.sh "$EXEC_PATH"

chmod +x "$EXEC_PATH"

echo "2. Creating system service:"
sed 's,PATH,'"$EXEC_PATH," ./service.template > /etc/systemd/system/hello.service
cat /etc/systemd/system/hello.service

echo "3. Starting service with systemctl"
systemctl start hello.service

journalctl -u hello.service -f
