#!/bin/bash

PATH_ITSNET_CONF="/home/pfe-aes-18/covCrav/itsnet/extra"
sed -i.bak 's/OCB = false/OCB = true/' $PATH_ITSNET_CONF/itsnet.conf
#sed -i.bak 's/Device = wlan0/Device = wlp1s0u1u4/' $PATH_ITSNET_CONF/itsnet.conf



