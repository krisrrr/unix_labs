#!/bin/sh

INSTALL_PATH=/etc/tmwriter
SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

EXEC_CMD="$(which python) $INSTALL_PATH/dbusmonitor.py"

echo "creating install dir: $INSTALL_PATH"
mkdir -p "$INSTALL_PATH"
cp "$SCRIPT_PATH/dbusmonitor.py" "$INSTALL_PATH/"


echo "creating system service with next config"
sed 's,PATH,'"$EXEC_CMD," ./service.template > /etc/systemd/system/tmwriter.service
cat /etc/systemd/system/tmwriter.service

echo "starting service"
systemctl start tmwriter.service

journalctl -u tmwriter.service -f
