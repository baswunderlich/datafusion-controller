import requests
import sys

velocity_set_url = "http://192.168.1.1/velocity_set"

"""
-----------------------------39792976204277075229963084144
Content-Disposition: form-data; name="set_velocity_right"

0
-----------------------------39792976204277075229963084144
Content-Disposition: form-data; name="set_velocity_left"

0
-----------------------------39792976204277075229963084144--
"""

def send_command(left, right):
    data = {
        "set_velocity_left": left,
        "set_velocity_right": right
    }
    try:
        response = requests.post(
            velocity_set_url,
            data = data
        )
    except:
        print("end")

send_command(sys.argv[1],sys.argv[2])