const BleUart = require("./bleUtil");

const keypress = require("keypress");

// make `process.stdin` begin emitting "keypress" events
keypress(process.stdin);

// use a predefined UART service (nordic, redbear, laird, bluegiga)
// const bleSerial = new BleUart('redbear');

// optionally define a custom service
const uart = {
  serviceUUID: "ffe0",
  txUUID: "ffe1",
  rxUUID: "ffe1"
};
const bleSerial = new BleUart("foo", uart);

const stringToHex = (char) => {
  if (char.length > 1) throw new Error("invalid char");
  return char.charCodeAt(0);
}

// this function gets called when new data is received from
// the Bluetooth LE serial service:
bleSerial.on("data", function(data) {
  console.log("Console log: " + String(data));
});

// thus function gets called if the radio successfully starts scanning:
bleSerial.on("scanning", function(status) {
  console.log("radio status: " + status);
});

// this function gets called when the program
// establishes a connection with the remote BLE radio:
bleSerial.on("connected", function(data) {
  console.log("Connected to BLE. Sending a hello message");
  bleSerial.write("s");
  // listen for the "keypress" event
  process.stdin.on("keypress", function(ch, key) {
    console.log('got "keypress"', key);
    if (key && key.ctrl && key.name == "c") {
      bleSerial.write("s");
      process.stdin.pause();
    }
    if (key && key.name == "s") {
      bleSerial.write("b");
    }
    if (key && key.name == "w") {
      bleSerial.write("f");
    }
    if (key && key.name == "a") {
      bleSerial.write("l");
    }
    if (key && key.name == "d") {
      bleSerial.write("r");
    }
    if (key && key.name == "x") {
      bleSerial.write("s");
    }
  });


  process.stdin.resume();
});
