from controller.controller import send_command

import time
import sys

print("start")

if len(sys.argv) > 1:
    send_command(sys.argv[1],sys.argv[2])
else:
    send_command(0,0)

        
print("end.")