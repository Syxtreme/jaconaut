# import numpy as np
# cimport numpy as np
# from libcpp cimport bool

cdef extern from "kinova_api.h":
    cdef cppclass Jaco2:
        Jaco2(int display_error_level)
        # main functions
        void Connect()
        void Move()
        void MoveToPos(CartesianInfo position)
        CartesianInfo GetPosition()
        void Disconnect()
        void Home()
        void InitPositionMode()

cdef extern from "KinovaTypes.h":
    struct CartesianInfo:
        float X
        float Y
        float Z
        float ThetaX
        float ThetaY
        float ThetaZ

    struct FingersPosition:
        float Finger1
        float Finger2
        float Finger3

    struct CartesianPosition:
        CartesianInfo Coordinates
        FingersPosition Fingers


cdef class pyJaco2:
    cdef Jaco2* thisptr # hold a C++ instance

    def __cinit__(self, display_error_level):
        self.thisptr = new Jaco2(display_error_level)

    def __dealloc__(self):
        del self.thisptr

    def Connect(self):
        self.thisptr.Connect()

    def GetPosition(self):
        return self.thisptr.GetPosition()

    def Move(self):
        self.thisptr.Move()

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

    def InitPositionMode(self):
        self.thisptr.InitPositionMode()