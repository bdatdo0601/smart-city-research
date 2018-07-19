import { print } from "util";

const BleUart = require("./bleUtil");
const _ = require("lodash");
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

const bleSerial = new BleUart("TESTVEC", "foo", uart);
const bleSerial2 = new BleUart("TESTVEC2", "foo", uart)
const stringToHex = (char) => {
  if (char.length > 1) throw new Error("invalid char");
  return char.charCodeAt(0);
}

// this function gets called when new data is received from
// the Bluetooth LE serial service:
bleSerial.on("data", function(data) {
  //received data here, need to split
  // console.log(String(data));
  const printData = String(data);
  console.log(printData);
});

bleSerial2.on("data", function(data) {
  //received data here, need to split
  // console.log(String(data));
  const printData = String(data);
  console.log(printData);
})

bleSerial2.on("connected", function(data) {
  console.log("test");
  // listen for the "keypress" event
  process.stdin.on("keypress", function(ch, key) {
    if (key && key.ctrl && key.name == "c") {
      bleSerial.write("x");
      process.stdin.pause();
    }
    if (key) {
      switch (key.name) {
        case "w":
          bleSerial2.write("w");
          break;
        case "a":
          bleSerial2.write("a");
          break;
        case "s":
          bleSerial2.write("s");
          break;
        case "d":
          bleSerial2.write("d");
          break;
        case "x":
          bleSerial2.write("x");
          break;
        case "f":
          bleSerial2.write("f");
          break;
        case "g":
          bleSerial2.write("g");
          break;
        case "h":
          bleSerial2.write("h");
          break;
        case "z":
          bleSerial2.write("z");
          break;
        case "c":
          bleSerial2.write("c");
          break;
        case "e":
          bleSerial2.write("e");
          break;
      }
    }
  });


  process.stdin.resume();
})

// this function gets called when the program
// establishes a connection with the remote BLE radio:
bleSerial.on("connected", function(data) {
  console.log("Connected to BLE. Sending a hello message");
  // listen for the "keypress" event
  process.stdin.on("keypress", function(ch, key) {
    if (key && key.ctrl && key.name == "c") {
      bleSerial.write("x");
      process.stdin.pause();
    }
    if (key) {
      switch (key.name) {
        case "w":
          bleSerial.write("w");
          break;
        case "a":
          bleSerial.write("a");
          break;
        case "s":
          bleSerial.write("s");
          break;
        case "d":
          bleSerial.write("d");
          break;
        case "x":
          bleSerial.write("x");
          break;
        case "f":
          bleSerial.write("f");
          break;
        case "g":
          bleSerial.write("g");
          break;
        case "h":
          bleSerial.write("h");
          break;
        case "z":
          bleSerial.write("z");
          break;
        case "c":
          bleSerial.write("c");
          break;
        case "e":
          bleSerial2.write("e");
          break;
      }
    }
  });


  process.stdin.resume();
});
