from simple_comm import pyJaco2
import numpy as np
import time

# Home = 0.21189536154270172, 'Y': -0.2750491499900818, 'Z': 0.4960561990737915, 'ThetaX': 1.5029977560043335, 'ThetaY': 1.0442252159118652, 'ThetaZ': 0.06850967556238174

j = pyJaco2(0)
j.Connect()
# j.Move()
j.Move(0.5)
j.Disconnect()
#%%
print(j.GetPosition())
# for i in range(10):
#     position = j.GetPosition()
#     print(position)
#     j.MoveToPos([
#         position["X"],
#         position["Y"] - 0.1 + np.random.rand() / 10,
#         position["Z"] - np.random.randn() / 20,
#         position["ThetaX"],
#         position["ThetaY"],
#         position["ThetaZ"],
#     ])
#     time.sleep(1)
# j.MoveToPos([0, 0, 0,0 , 0, 0])

print(j.GetPosition())
j.Home()
print(j.GetPosition())
j.Disconnect()