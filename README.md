# OSC-light
Light, fast but limited OSC library

## Basic implementation
To get this library working, start with the following implementation.

### Specify message outline
First, start with a struct to indicate what the message looks like:

```
struct Data {
	uint32_t int1;
	uint32_t int2;
	uint32_t int3;
	uint32_t int4;
	uint32_t int5;
};
```

### Create a consumer
To receive message, create a new class which consumes messages with a specified OSC address:

```
class OSCStructDataConsumer : public OSC::MessageConsumer<OSC::StructMessage<Data, uint32_t>> {
public:
  OSC::StructMessage<Data, uint32_t> * structMessage;

  const char * pattern()
  {
    return "/Some/Address";
  }
  void callbackMessage(OSC::StructMessage<Data, uint32_t> * message)
  {
    structMessage = message;
  }
};
```

For this example, this class returns a fixed address as `pattern`, and only stores the recieved message in a member.  

### Create the receiving hub
One piece of plumbing is required to get this library working, which is an instance of `OSC::Arduino`:

```
auto OSC = OSC::Arduino<OSC::StructMessage<Data, uint32_t>>(1, 0);
auto consumer = OSCStructDataConsumer();

OSC.bindStream(&Serial);
OSC.addConsumer(&consumer);

```

This binds the `Serial` interface to the `OSC::Arduino` instance.

### Sending messages
Putting all this code in a simple sketch, and get it running on an Arduino (preferably Arduinos with intergrated USB support 
on the Atmel chip). 

If you send the following OSC message to the Arduino, it should receive it, and the message consumer should consume it
(replace _ with \x0):

```
/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5
```

## More info
