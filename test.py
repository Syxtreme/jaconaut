from simple_comm import pyJaco2
import numpy as np
import time

j = pyJaco2(0)
j.Connect()
# j.Move()
# print(j.GetPosition())
for i in range(10):
    position = j.GetPosition()
    print(position)
    j.MoveToPos([
        position["X"],
        position["Y"] - 0.1 + np.random.rand() / 10,
        position["Z"] - np.random.randn() / 20,
        position["ThetaX"],
        position["ThetaY"],
        position["ThetaZ"],
    ])
    time.sleep(1)

j.Home()
j.Disconnect()