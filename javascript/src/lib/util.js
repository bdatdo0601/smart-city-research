export const stringToHex = char => {
    if (char.length > 1) throw new Error("invalid char");
    return char.charCodeAt(0);
};

export const uuid = dashedUUID => dashedUUID.replace(/-/g, "");

export const checkUartServiceFormat = (name, uart) => {
    let err = false;
    name = name ? name : "UART Service";
    if (Object.keys(uart).length === 0) {
        console.log("Or pass serviceUUID, txUUID, & rxUUID in object literal as second argument.");
        err = true;
    } else {
        if (!uart.serviceUUID) {
            console.log("ERROR: Expecting serviceUUID for " + name);
            err = true;
        }
        if (!uart.txUUID) {
            console.log("ERROR: Expecting txUUID for " + name);
            err = true;
        }
        if (!uart.rxUUID) {
            console.log("ERROR: Expecting rxUUID for " + name);
            err = true;
        }
    }
    if (err) {
        console.log(name + " " + JSON.stringify(uart));
        process.exit(-1);
    }
};
