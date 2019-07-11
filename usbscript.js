MAX_DISP_COLUMN_CHARS = 20;

const USBfilters = [
    {vendorId: 0x076C, productId: 0x0204},
    {vendorId: 0x1504, productId: 0x008C}
];

/**
 * Holds the device promise object.
*/
let globalDevice = null;

/** 
 * Automatically open again the connection session with an already accepted device.
*/
navigator.usb.getDevices().then(async devices => {
    try {
        if (devices.length > 0) {
            console.log("Current number of devices available: " + devices.length);
            devices.forEach(device => {
                console.log("Opened device ID info: vendorId: " + device.vendorId + ", productId: " + device.productId);
            });

            globalDevice = devices[0];
            await globalDevice.open();
            await globalDevice.selectConfiguration(1);

            if (globalDevice.configuration.interfaces.length > 1) {
                await globalDevice.claimInterface(globalDevice.configuration.interfaces[1].interfaceNumber);
                console.log("AUTO-OPEN NOTICE: Claimed interface 1 in getDevices().");
            } else {
                await globalDevice.claimInterface(globalDevice.configuration.interfaces[0].interfaceNumber);
                console.log("AUTO-OPEN NOTICE: Claimed interface 0 in getDevices().");
            }

            console.log(globalDevice);
        }
    }
    catch (error) {
        console.log(globalDevice);
        console.log("Error @ getDevices(): " + error);
    }
});

/**
 * If the user closes the tab or the browser, this closes the open USB connection.
*/
window.onbeforeunload = closingCode;
function closingCode() {
    if (globalDevice) {
        globalDevice.close();
        return null;
    }
}

/** 
 *  Check if WebUSB is available in current browser.
 *  If available, attempt to connect the desired USB device.
*/
document.addEventListener('DOMContentLoaded', event => {
    let connectButton = document.getElementById('connect');

    connectButton.addEventListener('click', async () => {
        try {
            if (navigator.usb) {
                if (!globalDevice) {
                    globalDevice = await openDevice();
                    printToDisplayScreen(true, "  - Provet Cloud -  ");
                } else {
                    console.log("Connection OK: The device is already connected.");
                }
            } else {
                alert('WebUSB not supported. Please make sure you are using Google Chrome browser.');
            }
        } catch (error) {
            console.log("Error @ func onStartup(): " + error);
        }
    });
})

/**    
 *  Get clear screen buffer command (0x0C) for clearing the screen.
*/
function getClrScreenBuffer() {
	let buffer = new ArrayBuffer(1);
    var view = new DataView(buffer);
    view.setUint8(0, 0x0C);
    return buffer;
}

/**
 *  Get endpoint from the current device config for outward data. 
 *  The endpoint acquiring here is made dynamic.
 */
function getEndPointOut() {
    if (globalDevice.configuration.interfaces.length > 1) {
        if (globalDevice.configuration.interfaces[1].alternate.endpoints[0].direction == "out") {
            return globalDevice.configuration.interfaces[1].alternate.endpoints[0].endpointNumber;
        } else {
            return globalDevice.configuration.interfaces[1].alternate.endpoints[1].endpointNumber;
        }
    } else {
        if (globalDevice.configuration.interfaces[0].alternate.endpoints[0].direction == "out") {
            return globalDevice.configuration.interfaces[0].alternate.endpoints[0].endpointNumber;
        } else {
            return globalDevice.configuration.interfaces[0].alternate.endpoints[1].endpointNumber;
        }
    }
}

/** 
 *  Clear customer display screen.
*/
async function clearScreen() {
    await globalDevice.transferOut(getEndPointOut(), getClrScreenBuffer());
}


/**
 *  Turn the string that we want to print into ASCII char array.
 *  Always returns the char array length value as "20".
 */
function stringToCharArray(string) {
    var array = [];

    for (var i = 0; i < string.length; i++) { 
        array.push(string[i].charCodeAt(0));

        if (array.length == MAX_DISP_COLUMN_CHARS) {break;}
    }

    let len = array.length;

    if (array.length < MAX_DISP_COLUMN_CHARS) {
        for (len; len < MAX_DISP_COLUMN_CHARS; len++) {
            array.push(0x20);
        }
    }

    return array;
}

/*
 *  Print text to screen with this function. 
*/
async function printToDisplayScreen(clrScrBeforePrint = true, stringToScreen) {
    try {
        if (clrScrBeforePrint) {
            clearScreen();
        }

        var charArray = stringToCharArray(stringToScreen);

        var buffer = new Uint8Array(charArray).buffer;
        await globalDevice.transferOut(getEndPointOut(), buffer);
        console.log("Data transfer to USBDevice OK!");
    } catch (error) {
        console.log("Error @ func printToDisplayScreen(): " + error);
    }
}

/** 
 *  Establish a connection to the USB device and access the display.
*/
async function openDevice() {
    try {
        let usbDevice = await navigator.usb.requestDevice({ filters: USBfilters });

        await usbDevice.open();

        if (usbDevice) {
            console.log('opened:', usbDevice);
            await usbDevice.selectConfiguration(1);

            if (usbDevice.configuration.interfaces.length > 1) {
                await usbDevice.claimInterface(usbDevice.configuration.interfaces[1].interfaceNumber);
                console.log("NOTICE: Claimed interface 1.");
            } else {
                await usbDevice.claimInterface(usbDevice.configuration.interfaces[0].interfaceNumber);
                console.log("NOTICE: Claimed interface 0.");
            }

            return usbDevice; //Returns the USB device as an promise object to global variable globalDevice.
        }
    } catch (error) {
        console.log("Error @ func openDevice: " + error);
    }
}

/**
 *  Example: "Canine A-vitamin (newline) 2.51$" prints to the display screen when clicking a 'buyVitamin' button.
*/
document.addEventListener('DOMContentLoaded', event => {
    let buyVitBtn = document.getElementById('buyVitamin');

    buyVitBtn.addEventListener('click', async () => {
        try {
            printToDisplayScreen(true, "Canine A-vitamin");
            printToDisplayScreen(false, "2.51$");
        } catch (error) {
            console.log("Error @ btn buyVitamin " + error);
        }
    });
})