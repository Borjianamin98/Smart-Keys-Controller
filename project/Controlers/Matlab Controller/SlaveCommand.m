classdef SlaveCommand < Simulink.IntEnumType
  enumeration
    SLAVE_IDLE(0)
    SLAVE_PUT(1)
    SLAVE_READ(2)
    SLAVE_IGNORE(3)
  end
end