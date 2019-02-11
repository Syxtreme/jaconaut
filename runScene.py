from simple_comm import pyJaco2
import numpy as np
import time
from pypylon import pylon
import os
import pylab

# True == camera will grab images
ENABLE_GRABBING = False

if ENABLE_GRABBING:
    camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
    camera.Open()
    pylon.FeaturePersistence.Load("/home/jaco/daA2500-14uc_22304238.pfs", camera.GetNodeMap(), True)
imagePath = "/home/Documents/data"

j = pyJaco2(0)
j.Connect()
#%%

j.Home()
j.Move(X=0.5, Y=-0.3, Z=0.3, ThetaX=1.7, ThetaY=0.4, ThetaZ=0)

poses = [{'X': 0.3, 'Y': -0.1, 'Z': 0.2, 'ThetaX': 1.773277759552002, 'ThetaY': 0.38, 'ThetaZ': -0.011},
         {'X': 0.45, 'Y': -0.1, 'Z': 0.2, 'ThetaX': 1.7717132568359375, 'ThetaY': 0.2387140691280365, 'ThetaZ': -0.011},
         {'X': 0.6, 'Y': -0.1, 'Z': 0.2, 'ThetaX': 1.7717132568359375, 'ThetaY': 0.09342087805271149, 'ThetaZ': -0.008283807896077633},
         {'X': 0.3, 'Y': -0.2, 'Z': 0.3, 'ThetaX': 1.9229133129119873, 'ThetaY': 0.38089561462402344, 'ThetaZ': -0.006011247169226408},
         {'X': 0.3, 'Y': -0.2, 'Z': 0.3, 'ThetaX': 1.9229133129119873, 'ThetaY': 0.38089561462402344, 'ThetaZ': -0.006011247169226408},
         {'X': 0.45, 'Y': -0.2, 'Z': 0.3, 'ThetaX': 1.860852599143982, 'ThetaY': 0.207, 'ThetaZ': -0.00506486976519227},
         {'X': 0.6, 'Y': -0.2, 'Z': 0.3, 'ThetaX': 1.860852599143982, 'ThetaY': 0.05912857875227928, 'ThetaZ': 0.006313962396234274},
         {'X': 0.3, 'Y': -0.3, 'Z': 0.4, 'ThetaX': 1.9809057712554932, 'ThetaY': 0.36974531412124634, 'ThetaZ': -0.007577492855489254},
         {'X': 0.45, 'Y': -0.3, 'Z': 0.4, 'ThetaX': 1.9802522659301758, 'ThetaY': 0.21307916939258575, 'ThetaZ': -0.007170067634433508},
         {'X': 0.6, 'Y': -0.3, 'Z': 0.4, 'ThetaX': 1.9794842004776, 'ThetaY': 0.044914864003658295, 'ThetaZ': -0.00653738621622324},
         {'X': 0.3, 'Y': -0.35, 'Z': 0.5, 'ThetaX': 2.12, 'ThetaY': 0.3535624146461487, 'ThetaZ': 0.00841387640684843},
         {'X': 0.45, 'Y': -0.35, 'Z': 0.5, 'ThetaX': 2.12, 'ThetaY': 0.19786226749420166, 'ThetaZ': 0.00841387640684843},
         {'X': 0.6, 'Y': -0.35, 'Z': 0.5, 'ThetaX': 2.10302734375, 'ThetaY': 0.03235049545764923, 'ThetaZ': 0.015736011788249016}
        ]


def composePath(idx: int):
    return os.path.join(imagePath, ''.join(["scene_", str(idx)]))

sidx = 0
scenePath = composePath(sidx)
while os.path.isdir(scenePath):
    sidx += 1
    scenePath = composePath(sidx)

fidx = 0
#%%
for pose in poses:
    j.ResetInstr()

    # run to pose
    j.Move(**pose, maxWaitTime=15)

    time.sleep(1)

    if ENABLE_GRABBING:
        # save image
        fileName = os.path.join(scenePath, ''.join(["scene_", sidx, "_view_", fidx, ".png"]))
        result = camera.GrabOne(300)

        

if ENABLE_GRABBING:
    camera.Close()
#%%
j.Disconnect()
