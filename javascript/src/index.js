import { print } from "util";
import keypress from "keypress";
import _ from "lodash";

import BleUart from "./lib/BleUart";
import DEVICES from "./data/devices";

// make `process.stdin` begin emitting "keypress" events
keypress(process.stdin);

// use a predefined UART service (nordic, redbear, laird, bluegiga)
// const bleSerial = new BleUart('redbear');

// optionally define a custom service
const uart = {
    serviceUUID: "ffe0",
    txUUID: "ffe1",
    rxUUID: "ffe1",
};

const bleSerial = new BleUart(DEVICES.TESTVEC2, uart);

// this function gets called when new data is received from
// the Bluetooth LE serial service:
bleSerial.on("data", function(data) {
    //received data here, need to split
    // console.log(String(data));
    const printData = String(data);
    console.log(printData);
});

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
                    bleSerial.write("e");
                    break;
            }
        }
    });

    process.stdin.resume();
});
