#include <Control_Surface.h> // Include the Control Surface library
#include <Adafruit_TinyUSB.h> //USB library for ESP32-S2


//BACKEND
// This class defines how to send and receive MIDI data using your custom
// backend.
struct MyUSBMIDIBackend {
  // USB MIDI packages are 4 bytes.
  using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
  // This method is optional. It is called once upon initialization.
  void begin() { backend.begin(); }
  // Read a single packet. Return a packet of all zeros if there is no packet.
  MIDIUSBPacket_t read() { MIDIUSBPacket_t packet {};
        if (TinyUSBDevice.mounted())
            backend.readPacket(packet.data);
        return packet;}
  // Write a single packet to the output buffer.
  void write(MIDIUSBPacket_t p) { if (TinyUSBDevice.mounted())
            backend.writePacket(p.data);}
  // Transmit the output buffer immediately.
  void sendNow() { if (TinyUSBDevice.mounted())
            backend.flush(); }
  // Should the sendNow method be called after each message? Return false if
  // your backend can send the data in the output buffer asynchronously and
  // automatically. Return true if that's not the case, and an explicit call to
  // sendNow is required.
  bool preferImmediateSend() { return false; }

  Adafruit_USBD_MIDI backend;
};
 
// This is the actual MIDI interface that makes use of the backend defined above.
struct MyUSBMIDI_Interface : GenericUSBMIDI_Interface<MyUSBMIDIBackend> {
  MyUSBMIDI_Interface() = default;
  using MIDIUSBPacket_t = MyUSBMIDIBackend::MIDIUSBPacket_t;
};
 
//BACKEND


// Instantiate the MIDI interface to use.
MyUSBMIDI_Interface midi;


void setup() {
  midi.begin(); // Initialize the MIDI interface

}

// MIDI note number, channel, and velocity to use
const MIDIAddress address {MIDI_Notes::C(4), Channel_1};
const uint8_t velocity = 0x7F;

void loop() {
  midi.sendNoteOn(address, velocity);
  delay(500);
  midi.sendNoteOff(address, velocity);
  delay(500);

  midi.update(); // Handle or discard MIDI input
}
