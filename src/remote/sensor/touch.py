import sys
import time

import Adafruit_MPR121 as MPR121

touch = MPR121.MPR121()
touch.begin(address=0x5a, busnum=2)
