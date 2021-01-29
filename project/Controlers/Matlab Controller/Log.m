classdef Log < Simulink.IntEnumType
  enumeration
    LOG_NOTHING(0)
    LOG_SEND(1)
    LOG_RECEIVE(2)
    LOG_REPLY(3) % Only Slave
    LOG_BUTTON_PRESSED(4)
    LOG_RETRIEVE_STATE(5) % Only Master
  end
end