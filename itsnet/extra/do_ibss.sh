#!/bin/bash

PATH_ITSNET_CONF="/home/pfe-aes-18/covCrav/itsnet/extra"
sed -i.bak 's/OCB = true/OCB = false/' $PATH_ITSNET_CONF/itsnet.conf
#sed -i.bak 's/Device = wlp1s0u1u4/Device = wlan0/' $PATH_ITSNET_CONF/itsnet.conf


