#ifndef RCINPUTDEVICE_H
#define RCINPUTDEVICE_H

class RCInputDevice {
public:
  virtual void attach() = 0;
  virtual void updateFromRC(uint16_t pulse) = 0;
  virtual uint8_t getRCChannel() const = 0;
  virtual uint8_t getInputPin() const = 0;
};

#endif // RCINPUTDEVICE_H
