# add main and others
CXXSRC += $(wildcard $(CURRENTPATH)/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/*.c)

CSRC += $(wildcard $(CURRENTPATH)/core/*.c)
CXXSRC += $(wildcard $(CURRENTPATH)/core/*.cpp)

CXXSRC += $(wildcard $(CURRENTPATH)/devices/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/devices/*.c)

CXXSRC += $(wildcard $(CURRENTPATH)/pus/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/pus/*.c)

CXXSRC += $(wildcard $(CURRENTPATH)/pus/ParameterManagement/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/pus/ParameterManagement/*.c)

CXXSRC += $(wildcard $(CURRENTPATH)/utility/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/utility/*.c)

CXXSRC += $(wildcard $(CURRENTPATH)/test/*.cpp)
CSRC += $(wildcard $(CURRENTPATH)/test/*.c)


