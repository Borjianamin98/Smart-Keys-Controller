classdef MasterCommand < Simulink.IntEnumType
  enumeration
    MASTER_IDLE(0)
    MASTER_PUT(1)
    MASTER_READ(2)
  end
end