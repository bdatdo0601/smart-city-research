import os from "os";
import noble from "noble";
import nobleUWP from "noble-uwp";
import util from "util";
import EventEmitter from "events";

import { uuid, checkUartServiceFormat } from "./util";

const bleModule = os.platform() === "win32" ? nobleUWP : noble;

const BT_STATE = Object.freeze({
    POWERED_ON: "poweredOn",
    STATE_CHANGE: "stateChange",
    SCANNING: "scanning",
    DISCOVER: "discover",
    CONNECTED: "connected",
});

const PERIPHERAL_STATE = Object.freeze({
    CONNECT: "connect",
    DISCONNECT: "disconnect",
});

const CHARACTERISTIC_STATE = Object.freeze({
    DATA_RECEIVED: "data",
    READ: "read",
});

class BleUart extends EventEmitter {
    constructor({ deviceName, macAddr }, options) {
        super();
        this.uart = { ...options };
        checkUartServiceFormat(null, this.uart);
        const { serviceUUID, txUUID, rxUUID } = this.uart;
        this.serviceUUID = uuid(serviceUUID);
        this.transmitUUID = uuid(txUUID);
        this.receiveUUID = uuid(rxUUID);
        this.receive = null;
        this.transmit = null;
        this.writeWithoutResponse = null;
        this.connected = false;
        this.peripheral = null;
        this.deviceName = deviceName;
        this.macAddr = macAddr;
        bleModule.on(BT_STATE.STATE_CHANGE, this.scan);
        bleModule.on(BT_STATE.DISCOVER, this.connect);
    }

    scan = state => {
        if (state === BT_STATE.POWERED_ON) {
            bleModule.startScanning([], false);
        }
        this.emit(BT_STATE.SCANNING, state);
    };

    explore = (error, services) => {
        const getCharacteristics = (error, characteristics) => {
            characteristics.forEach(characteristic => {
                if (characteristic.uuid === this.receiveUUID) {
                    this.receive = characteristic;
                }
                if (characteristic.properties.indexOf("notify") < 0) {
                    console.log("ERROR: expecting " + characteristic.uuid + " to have 'notify' property.");
                }
                this.receive.notify(true); // turn on notifications

                this.receive.on(CHARACTERISTIC_STATE.READ, (data, notification) => {
                    this.emit(CHARACTERISTIC_STATE.DATA_RECEIVED, data); // emit a data event
                });
                if (characteristic.uuid === this.transmitUUID) {
                    this.transmit = characteristic;
                    // Older Adafruit hardware is writeWithoutResponse
                    if (characteristic.properties.indexOf("writeWithoutResponse") > -1) {
                        this.writeWithoutResponse = true;
                    } else {
                        this.writeWithoutResponse = false;
                    }
                }
            });
            // if you've got a valid transmit and receive characteristic,
            // then you're truly connected. Emit a connected event:
            if (this.transmit && this.receive) {
                this.connected = true;
                this.emit(BT_STATE.CONNECTED, this.connected);
            }
        };

        // iterate over the services discovered. If one matches
        // the UART service, look for its characteristics:
        for (let s in services) {
            if (services[s].uuid === this.serviceUUID) {
                services[s].discoverCharacteristics([], getCharacteristics);
                return;
            }
        }
    };

    write = data => {
        if (this.transmit) {
            while (data.length > 20) {
                const output = data.slice(0, 19);
                transmit.write(new Buffer(output), this.writeWithoutResponse);
                data = data.slice(20);
            }
            this.transmit.write(new Buffer(data), this.writeWithoutResponse);
        }
    };

    disconnect = () => {
        this.connected = false;
    };

    connect = peripheral => {
        const peripheralName = peripheral.advertisement.localName;
        if (!peripheralName || (peripheralName !== this.deviceName && peripheral.address !== this.macAddr)) return;
        console.log(peripheral);
        this.peripheral = peripheral;
        console.log(`Connecting to ${peripheralName}`);
        peripheral.connect();
        peripheral.on(PERIPHERAL_STATE.CONNECT, () => {
            bleModule.stopScanning();
            peripheral.discoverServices([this.serviceUUID], this.explore);
        });
        peripheral.on(PERIPHERAL_STATE.DISCONNECT, this.disconnect);
    };
}

// util.inherits(BleUart, EventEmitter);
export default BleUart;
