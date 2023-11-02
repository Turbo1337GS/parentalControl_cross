	echo "[Unit]
Description=https://main.gigasoft.com.pl 

[Service]
ExecStart=$(shell pwd)/GigaSoft
User=root
Group=root

[Install]
WantedBy=multi-user.target" | sudo tee /etc/systemd/system/GigaSoft.service