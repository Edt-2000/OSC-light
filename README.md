# OSC-light
Light, fast but limited OSC library

## Sample implementations
This library was designed to work with the Edt-2000 OSC ecosystem. The main focus was to be extermely low latency and fast, giving up a lot of features. It only supports a very small part of the OSC spec, and only supports int's and float's in OSC messages.

Some sample implementations and usage:

- `OSC::Arduino`: https://github.com/Edt-2000/Edt-2000-microcontrollers/blob/master/src/Edt-LedHub/lib/EdtLed/ledApp.h
- `OSC::MessageConsumer`: https://github.com/Edt-2000/Edt-2000-microcontrollers/blob/master/src/Edt-Core/lib/Messages/CommandMessageConsumer.h

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
Putting all this code in a simple sketch, and get it running on an Arduino (preferably Arduinos with intergrated USB support on the Atmel chip). 

If you send the following OSC message to the Arduino, it should receive it, and the message consumer should consume it (replace _ with \x0):

```
/Some/Address___,iiiii_____\x1___\x2___\x3___\x4___\x5
```

## More info

### Messages
There are two types of messages in this library, `OSC::StructMessage` and `OSC::Message`, which have the following characteristics:

#### OSC::Message
- Easiest to work with
- Receives arbitrary amount of ints and floats
- Puts all data in array which can be get and set using `getInt(nr)` and `getFloat(nr)`.
- All data is 32 bit wide.
- 'Slow'.

#### OSC::StructMessage
- Harder to work with
- Tries to map every message recieved to the predefined struct.
- Struct may only contain 32 bit or 8 bit integers (which can be configured using second template parameter).
- Data can be get and set using the struct, available using `messageStruct`.
- Much faster than `OSC::Message`.

### Consumers and producers
The `OSC::MessageConsumer` and `OSC::MessageProducer` abstract classes must be used to consume and produce messages. They must use either `OSC::Message` or `OSC::StructMessage` as message type.

#### OSC::MessageConsumer
The consumer has two abstract methods which must be implented:

- `pattern()`: must return the pattern of the messages it can read. Pattens and rules can be found in the comments here: https://github.com/Edt-2000/OSC-light/blob/master/OSC-light/OSCMatch.h.
- `callbackMessage(MessageType *)`: is called when the pattern matches the incoming message. Must return `void`. 

#### OSC::MessageProducer
The producer has two abstract methods which must be implemented:

- `loop()`: this method is called everytime `OSC::Arduino` `loop()`s. This method can be used to communicate with IO or do some calculations.
- `generateMessage()`: this message is called everytime `OSC::Arduino` `loop(true)`s. Must return a message. Do not forget to call `setValidData(true)` on the message it returns: this indicates the message contains valid data. If the IO failed or the producer is not ready to generate a message, call `setValidData(false)` on the message.

### Plumbing
In order to bind Arduio's `Print` and `Stream` classes to the consumers and producers, a bit of plumbing is required. This is done by the `OSC::Arduino` class. It must know what type of message are being consumed and produces by its consumers and producers, so for example:

```
auto osc = OSC::Arduino<OSC::StructMessage<Data, uint8_t>>(5, 1);
```

This creates an instance which handles 5 consumers which consume `OSC::StructMessage<Data, uint8_t>` messages, and one producer which produces `OSC::StructMessage<Data, uint8_t>`. If various types of message are required, consider using multiple instances or use an instance that handles `OSC::Message`. The number of consumers and producers it handles is fixed, and must be defined at creation. 

The class exposes the following methods:

- `addConsumer(MessageConsumer<MessageType> * consumer)`: Adds a consumer to its internal list.
- `addProducer(MessageProducer<MessageType> * producer)`: Adds a producer to its internal list.
- `bindUDP(UDP * udp, IPAddress remoteIP, int remotePort)`: Binds the UDP class provided by Arduino, and starts communicating using UDP, use this for when communicating over Ethernet.
- `bindStream(Stream * serial)`: Binds the Serial class provided by Arduino, and starts communicating using Serial, use this for when communicating over USB.
- `bindBoth(UDP * udp, IPAddress remoteIP, int remotePort, Stream * serial)`: Binds both the UDP and Serial classes. 
- `loop(bool send)`: Call this message every loop of the Arduino, this method will read the incoming buffers of its bounded communication classes. When the `send` argument is `true`, it will call `generateMessage` on every producer. Do not send every loop as it will absolutely flood every communication buffer. About 40 times per second is more than enough. 



