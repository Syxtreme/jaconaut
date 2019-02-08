# import numpy as np
# cimport numpy as np
from libcpp cimport bool

cdef extern from "kinova_api.h":
    struct CartesianDictionary:
        float X
        bool xset
        float Y
        bool yset
        float Z
        bool zset
        float ThetaX
        bool thetaxset
        float ThetaY
        bool thetayset
        float ThetaZ
        bool thetazset

    cdef cppclass Jaco2:
        Jaco2(int display_error_level)
        # main functions
        void Connect()
        void Disconnect()
        void ResetInstr()
        void SetFrameType(int type)

        void Home()
        CartesianInfo GetPosition()
        AngularInfo GetAngularPosition()

        void Move(CartesianDictionary position)
        void MoveToPos(CartesianInfo position)

        void PrintQuickStatus()
        void PrintGeneralInfo()

cdef extern from "KinovaTypes.h":
    struct CartesianInfo:
        float X
        float Y
        float Z
        float ThetaX
        float ThetaY
        float ThetaZ

    struct AngularInfo:
        float Actuator1
        float Actuator2
        float Actuator3
        float Actuator4
        float Actuator5
        float Actuator6
        float Actuator7


cdef class pyJaco2:
    cdef Jaco2* thisptr # hold a C++ instance

    def __cinit__(self, display_error_level = 0):
        self.thisptr = new Jaco2(display_error_level)

    def __dealloc__(self):
        del self.thisptr

    def Connect(self):
        self.thisptr.Connect()

    def ResetInstr(self):
        self.thisptr.ResetInstr()
    
    def SetFrameType(self, ftype):
        self.thisptr.SetFrameType(ftype)

    def GetAngularPosition(self):
        return self.thisptr.GetAngularPosition()

    def GetPosition(self):
        return self.thisptr.GetPosition()

    def Move(self, *args, **kwargs):
        cpdef CartesianDictionary pos
        tempdict = {}
        for i, coord in enumerate(args):
            if i == 0:
                tempdict["X"] = coord
            elif i == 1:
                tempdict["Y"] = coord
            elif i == 2:
                tempdict["Z"] = coord
            elif i == 3:
                tempdict["ThetaX"] = coord
            elif i == 4:
                tempdict["ThetaY"] = coord
            elif i == 5:
                tempdict["ThetaZ"] = coord

        kwargs.update(tempdict)

        for k, v in kwargs.items():
            print(type(pos))
#            setattr(pos, "X", 1)
#            setattr(pos, k, v)
#            setattr(pos, ''.join([k.lower(), "set"]), True)
            pos[k] = v
            pos[''.join([k.lower(), "set"])] = True
            
        self.thisptr.Move(pos)

    def Home(self):
        self.thisptr.Home()

    def MoveToPos(self, position):
        cdef CartesianInfo pos
        pos.X = position[0]
        pos.Y = position[1]
        pos.Z = position[2]
        pos.ThetaX = position[3]
        pos.ThetaY = position[4]
        pos.ThetaZ = position[5]
        self.thisptr.MoveToPos(pos)

    def Disconnect(self):
        self.thisptr.Disconnect()

    def PrintQuickStatus(self):
        self.thisptr.PrintQuickStatus()

    def PrintGeneralInfo(self):
        self.thisptr.PrintGeneralInfo()