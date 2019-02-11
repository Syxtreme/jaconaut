from simple_comm import pyJaco2
import numpy as np
import time

# Home = 0.21189536154270172, 'Y': -0.2750491499900818, 'Z': 0.4960561990737915, 'ThetaX': 1.5029977560043335, 'ThetaY': 1.0442252159118652, 'ThetaZ': 0.06850967556238174

j = pyJaco2(0)
j.Connect()
#%%

j.Home()
j.Move(X=0.3, Y=-0.3, Z=0.3, ThetaX=1.7, ThetaY=0.4, ThetaZ=0)


#%%
j.ResetInstr()
j.Move(**{'X': 0.3,
 'Y': -0.1,
 'Z': 0.2,
 'ThetaX': 1.773277759552002,
 'ThetaY': 0.38,
 'ThetaZ': -0.011},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.45,
 'Y': -0.1,
 'Z': 0.2,
 'ThetaX': 1.7717132568359375,
 'ThetaY': 0.2387140691280365,
 'ThetaZ': -0.011},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.6,
 'Y': -0.1,
 'Z': 0.2,
 'ThetaX': 1.7717132568359375,
 'ThetaY': 0.09342087805271149,
 'ThetaZ': -0.008283807896077633},
    maxWaitTime=5)
#%%<<<<<<<<<<<<<<<<<<<<<<<<<
#%%
j.ResetInstr()
j.Move(**{'X': 0.3,
 'Y': -0.2,
 'Z': 0.3,
 'ThetaX': 1.9229133129119873,
 'ThetaY': 0.38089561462402344,
 'ThetaZ': -0.006011247169226408},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.45,
 'Y': -0.2,
 'Z': 0.3,
 'ThetaX': 1.860852599143982,
 'ThetaY': 0.207,
 'ThetaZ': -0.00506486976519227},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.6,
 'Y': -0.2,
 'Z': 0.3,
 'ThetaX': 1.860852599143982,
 'ThetaY': 0.05912857875227928,
 'ThetaZ': 0.006313962396234274},
    maxWaitTime=5)

#%%<<<<<<<<<<<<<<<<<<<<<<<<<
#%%
j.ResetInstr()
j.Move(**{'X': 0.3,
 'Y': -0.3,
 'Z': 0.4,
 'ThetaX': 1.9809057712554932,
 'ThetaY': 0.36974531412124634,
 'ThetaZ': -0.007577492855489254},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.45,
 'Y': -0.3,
 'Z': 0.4,
 'ThetaX': 1.9802522659301758,
 'ThetaY': 0.21307916939258575,
 'ThetaZ': -0.007170067634433508},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.6,
 'Y': -0.3,
 'Z': 0.4,
 'ThetaX': 1.9794842004776,
 'ThetaY': 0.044914864003658295,
 'ThetaZ': -0.00653738621622324},
    maxWaitTime=5)

#%%<<<<<<<<<<<<<<<<<<<<<<<<<
#%%
j.ResetInstr()
j.Move(**{'X': 0.3,
 'Y': -0.35,
 'Z': 0.5,
 'ThetaX': 2.12,
 'ThetaY': 0.3535624146461487,
 'ThetaZ': 0.00841387640684843},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.45,
 'Y': -0.35,
 'Z': 0.5,
 'ThetaX': 2.12,
 'ThetaY': 0.19786226749420166,
 'ThetaZ': 0.00841387640684843},
    maxWaitTime=5)

#%%
j.ResetInstr()
j.Move(**{'X': 0.6,
 'Y': -0.35,
 'Z': 0.5,
 'ThetaX': 2.10302734375,
 'ThetaY': 0.03235049545764923,
 'ThetaZ': 0.015736011788249016},
    maxWaitTime=5)



#%%
j.Disconnect()
#xLocs = [0.2, 0.4, 0.6]
#yLocs = [-0.1, -0.2, -0.3, -0.325, -0.35]
#zLocs = [0.3, 0.4, 0.45, 0.5, 0.55]
#
#for y, z in zip(yLocs, zLocs):
#    j.ResetInstr()
#    j.Move(Y=y, Z=z)
#    print("***>")
#    print(f"Moved to Y = {y} and Z={z}.")
#    time.sleep(0.5)
#    print("Actual coordinates:")
#    print(j.GetPosition())
#    for x in xLocs:
#        j.ResetInstr()
#        j.Move(X=x, maxWaitTime=15)
#        print(f"Moved to X = {x}.")
#        time.sleep(0.5)
#        print("Actual coordinates:")
#        print(j.GetPosition())
#    print("<***")