
import rclpy
from rclpy.executors import ExternalShutdownException
from std_msgs.msg import Float32
from fastapi import FastAPI, Request

app = FastAPI()


rclpy.init(args=args)

node = rclpy.create_node("my_publisher")
publisher_L = node.create_publisher(Float32, "VelocitySetL", 10)
publisher_R = node.create_publisher(Float32, "VelocitySetR", 10)

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.post("/instruction")
async def read_item(request: Request):
    content = await request.json()
    left_v = float(content["left"])
    right_v = float(content["right"])
    send_instruction(left_v=left_v, right_v=right_v)
    return

def send_instruction(left_v: float, right_v: float):
    msg_left = Float32()
    msg_left.data = left_v

    msg_right = Float32()
    msg_right.data = right_v

    publisher_L.publish(msg_left)
    publisher_R.publish(msg_right)

