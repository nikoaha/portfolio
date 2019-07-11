# javascript-usb

**CONTENTS:**

1) Introduction
2) Documentation step-by-step
3) Known bugs

_________________________________________________________________

## 1) Introduction

In this project, the idea was to be able to access a physical USB customer
display device via Google Chrome browser. The WebUSB API was the solution for 
this kind of implementation.

The final version of this  project is meant to be run for a cloud service. When the seller adds 
items to the invoice during counter sale, each product's name and price are
printed to the customer display screen. Also when finalizing the invoice, 
the total price is shown to the screen to the customer.

The aim was to make the communication with the device as simple as possible 
without any special set ups. Plug-and-play is the desired action for this 
project.

The customer display product used for this project is *Partner CD7220-U*, which 
uses ESC-POS protocol for communicating. Check out the general ESC-POS
commands online.

The *Partner CD7220-U* device has 20x2 "resolution", so it can hold up to 40
characters at a time. The printing to the screen 
is set up in *overwrite mode* as default. The printing of the characters start 
from the upper-left side of the screen and then prints rightward. When the 
first row is full of characters, printing to the second row commences from left
to right. When the second row is full, the printing will return to the upper-left
part of the screen and printing characters from that on
overwrites the previously printed characters on the screen. The loop continues. 

Notice this action when printing to the screen. Other modes can be activated by 
using the corresponding ESC-POS commands and sending to the display device. Check the manual if needed.

The main functions that need to be used are:

*  `openDevice()`, in an `EventHandler`, for example. Check the doc how to use this.
*  `printToDisplayScreen(clrScrBeforePrint, stringToScreen)`, print a string to the customer display screen.

No other functions specifically needed for the user to use and call for this program.

**Other mentions and notices:**

Windows OS limits the closing and opening of the communication session to roughly 30 times.
After this it won't allow to claim the interface anymore. Only way to fix this problem is
then to restart the computer. On the other hand, it was tested that there are no limitations 
on UNIX based operating systems, like on Mac OS or on Linux Ubuntu systems.
It is good to keep this in mind.

It was also tested, that the connection session will stay connected for a long periods of time.
There was a connection even after one night of leaving the computer and Chrome Browser on. 
Using this program on a computer that is never turned off could result
that after 30 sessions the customer display cannot be turned off in Windows OS.
Though there is no solid evidence nor test results of this. Caution needed.

## 2) Documentation step-by-step

Here is the documentation from up to down just like seen in the JS file
source code.
This documentation is explained as detailed as possible.

## Constants and global variables

`MAX_DISP_COLUMN_CHARS = 20;`

A row in the Partner CD7220-U display can hold 20 characters. This constant is
used in `printToDisplayScreen()` function.

_________________________________________________________________

```
const USBfilters = [
    {vendorId: 0x076C, productId: 0x0204}
];
```
USBfilters constant holds the unique `vendorID` and `productID` variables issued by the
USB head organization itself. For our CD7220-U customer display, we have our 
Partner vendor ID `0x076C` and product ID `0x0204` for detecting this specific 
device. For any other vendor or product specific codes, googling helps a lot.

For our project, only *Partner CD7220-U* is allowed for a connection currently.

If we want allow multiple devices to connect with, we add them to 
the list. For ex.:
```
const USBfilters = [
    {vendorId: 0x076C, productId: 0x0204},
    {vendorId: 0xAAAA, productId: 0x1111},
    {vendorId: 0xBBBB, productId: 0x2222}
];

```
_________________________________________________________________

`let globalDevice = null;`

`globalDevice` is a global variable, which holds the successfully connected device
as a promise object returned from `openDevice()` or from `getDevices()`. Returns `null`, if no device 
is connected.

## navigator.usb.getDevices()

```
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
        console.log("Error @ getDevices(): " + error);
    }
});
```
`navigator.usb.getDevices()` automatically opens the session again with a previously
permitted USB device that was granted access and opened via `requestDevices()`. If the user refreshes
the page or browses to another page, the connection will close and then open up 
again automatically on-load and is then able to receive data.

```
if (devices.length > 0) {
    [...]
}
```
This automatical connecting to the device will only happen when the `getDevices()`
can detect previously opened devices. If devices are found at `if (devices.length > 0) { [...] }`,
then we can establish a connection. 

```
console.log("Current number of devices available: " + devices.length);
devices.forEach(device => {
    console.log("Opened device ID info: vendorId: " + device.vendorId + ", productId: " + device.productId);
});
```
The quantity of connected devices are indicated
in `devices.length` and
each device available are printed with `devices.forEach()` to the console with corresponding `vendorId` and `productId` data.
For our project, if there are no other USB device data added to `const UBSfilters`, 
only one device should be available, which is the *Partner CD7220-U*
customer display screen. 

If errors occur during `getDevices()` function, it is always good to check these 
two console comments if any devices are connected or not.

```
let device = devices[0];
globalDevice = device;
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
```
`getDevices()` returns an array of available, previously opened devices. Since there are no other 
devices than one available, we choose the first element in the array index, which is the
*Partner* customer display, and assing it to the device and from there to the 
`globalVariable`. We open the session and configure and then the device is ready
for use again. If the functions of `open()`, `selectConfiguration()` and `claimInterface()` are 
not familiar to you yet, check out documentation for `openDevice()` below.

Here the we need to access the `interfaceNumber` variable inside the `globalDevice` 
global variable. Accessing this inteface number is made *dynamic*, as each USB 
device's `interfaceNumber` location path could be case-sensitive.


## closingCode()

```
window.onbeforeunload = closingCode;
function closingCode(){
    if (globalDevice) {
        globalDevice.close();
        return null;
    }
}
```

If the user closes the tab or the browser, the opened session with device will
be closed. This ensures that no session will be left opened, as leaving
each session open could lead to communication problems later on.

## openDevice(): An example of how to use openDevice()

```
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
```
When desiring to connect with a device for the first time by using the WebUSB API,
the command must be user-initiated, such as clicking or mouseovering a button.
For security reasons, making a connection automatically on-load is not 
possible. Thus, we use an `EventListener` here to connect.

The WebUSB API (`navigator.usb`) is only available for Google Chrome browser 
currently. If the user attempts to connect with any other browser, an alert 
error message is shown.

If there is no connection yet opened with the device, meaning that 
`globalDevice` is `null`, `openDevice()` function is called to establish a 
connection.

When the connection is established successfully, "  - Provet Cloud -  " is 
printed to the display screen to indicate and the display is ready for use.

Once this function has been called and a session has been successfully opened with a USB device,
the `getDevices()` will automatically connect to the selected device if the user
refreshes or browses out of the current web page.

## getClrScreenBuffer()

```
function getClrScreenBuffer() {
    let buffer = new ArrayBuffer(1);
    var view = new DataView(buffer);
    view.setUint8(0, 0x0C);
    return buffer;
}
```
Calling `getClrScreenBuffer()` quickly sets up the needed data for 
clearing the screen. The ESC-POS command `0x0C` is a command to literally clear
the screen out of any characters. This function is used for `clearScreen()` function.

For more technical explanations, check out function documentation
for `printToDisplayScreen()`.

## getEndPointOut()

```
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
```
Every USB device contain *endpoints*, which act like data transfer ports. 
We need to find the endpoint where we are required send our **outward** (as in the code `"out"`) data commands. Unfortunately the USB
devices have tendencies to be case-sensitive devices. Luckily, the endpoint
allocated for our communication can be found in the `globalVariable`
promise object. `getEndPointOut()` easily returns this path dynamically for you from the `globalVariable`.

If errors still occur even with this dynamical action,
[recheck the opened connection endpoint path in the Chrome console section.](https://ibb.co/BBj7wdG)

## clearScreen()

```
async function clearScreen() {
    await globalDevice.transferOut(getEndPointOut(), getClrScreenBuffer());
}
```
`clearScreen()` function uses the `getEndPointOut()` and `getClrScreenBuffer()` 
functions as parameters and executes the clear screen command in the device. 
The screen should be cleared after this command.

## stringToCharArray(string)

```
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
```
`printToDisplayScreen()` function's `string` parameter takes the string that 
is desired to be printed to the display screen.

```
for (var i = 0; i < string.length; i++) { 
    array.push(string[i].charCodeAt(0));

    if (array.length == MAX_DISP_COLUMN_CHARS) {break;}
}
```
This for-loop checks each character of the parameter `string`, turns them into 
ASCII codes and pushes them into the array one at a time. 

We want to return this `array` character array *always as 20 characters long*.
This way we fill the first row with the product name and the second row
with the price neatly in the 20x2 resolution screen. Thus, it is checked upon every push
that if the character array length is exceeding the length of 20. If so, the loop is broken and no more 
characters are pushed into the array.

```
let len = array.length;

if (array.length < MAX_DISP_COLUMN_CHARS) {
    for (len; len < MAX_DISP_COLUMN_CHARS; len++) {
        array.push(0x20);
    }
}

return array;
```

If the char array turns out to be less than 20 characters, then we fill the remaining
empty char slots with *0x20* space characters. The `array` is then returned to 
the `printToDisplayScreen()` to variable `charArray`.

## printToDisplayScreen(clrScrBeforePrint, stringToScreen)

```
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
```
This `printToDisplayScreen()` is the core function of this application as it sends
the desired string to the screen. Here's explained what it does step-by-step.

```
if (clrScrBeforePrint) {
    clearScreen();
}
```
If desiring to clear the screen before printing new stuff to the screen, 
`clrScrBeforePrint` parameter is to be set as `true`. Notice, that 
`clrScrBeforePrint` is `true` by default, if no boolean parameter is set to the
function call.

```
var charArray = stringToCharArray(stringToScreen);

var buffer = new Uint8Array(charArray).buffer;
await globalDevice.transferOut(getEndPointOut(), buffer);
console.log("Data transfer to USBDevice OK!");
```

`stringToCharArray(stringToScreen)` takes the string that we want print to the 
display screen as parameter and returns it as a ASCII character array to `charArray`.
This character array now must be transformed into an unsigned 8-bit array `Uint8Array`
and then buffered as an `ArrayBuffer`. An `ArrayBuffer` is an array consisting of raw 
bytes, without any solid data unless value is given to them. Otherwise just raw 
bytes. The commands and data will be sent to the USB customer display device in 
this format.

The `Uint8Array` is processed into an `ArrayBuffer` by adding the `.buffer`
after it and the final result of this package is returned to the variable `buffer` 
and then it is sent to the
device by using the WebUSB API function `transferOut()`. The first parameter of `transferOut()` is the 
endpoint, which is the data receiver in the USB device, and the second parameter
is the data array package variable `buffer`, which holds the string we want to
send and print to the customer display screen.

All in all: the string is chopped up into a ASCII character array, transformed 
into an `ArrayBuffer` form and then sent to the device. The string should be displayed
on the customer display screen.

In addition, if we want to send ESC-POS commands to the customer display, we can use the 
following piece of code.

```
var data = array.concat([0x01B, 0x64, 2])
var buffer = new Uint8Array(data).buffer;
await globalDevice.transferOut(getEndPointOut(), buffer);
console.log("Data transfer to USBDevice OK!");
```

The ESC-POS commands are also provided in hexadecimal form in any manual.
For example, here we have the command [0x01B, 0x64, 2] or "print the preceeding line of text for 2 times".
We append this command in the character array that was
constructed from the string we want to print to the display screen.
Ultimately two lines of the string you want to print should appear to the
customer display screen.

## openDevice()

```
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

            return usbDevice; //Returns the USB device as an promise object.
        }
    } catch (error) {
        console.log("Error @ func openDevice: " + error);
    }
}
```
`openDevice()` function initiates the communication with the device for usage.
The process is done in phases.

### Opening the device

`let usbDevice = await navigator.usb.requestDevice({ filters: USBfilters });`

Everything starts with the WebUSB API's function `requestDevice()` function. It 
requests plugged-in USB devices for a permission to access them and start a 
communication session. *It requests only for the devices you have set up in the
`const USBfilters` variable.* Remember to set them up if changing USB devices. A
box in the upper-left side of chrome should pop up and ask you to grant
permission for the device. 

If nothing pops up, then check the USB filters constant variables again and 
make sure you are using the correct ID's. Sometimes it might take time for 
Chrome to detect new devices. Eventually Chrome should show you the desired 
USB device you want to connect with in the request pop up list.

`await usbDevice.open();`

Next we open the session with the desired device with `open()` function. If 
no errors occurred, it means that you have now a connection established with the device.

**Take notice: If the connection attempt fails when trying to open the session
with the device, the main reason could be that there is a session opened with the 
device already. Make sure no other sessions are open with the device. In many 
cases, Windows OS could also "own" the whole session with the device. To get rid
of this problem, change the device's driver to `libusb` instead of the default
driver.**

```
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

    return usbDevice; //Returns the USB device as an promise object.
}
```

### Configurating the opened device

`console.log('opened:', usbDevice);`

It is possible to view the contents of the device when printing out the opened session
variable. Note that this `usbDevice` is just a temporary device promise object and eventually will be
returned to the `globalVariable` in `openDevice()` function. It is useful for debugging.

`await usbDevice.selectConfiguration(1);`

We select the configuration `1` for our session. Not absolutely sure of what 
this is meant to be for, but selecting configuration `1` works for this for now.

```
if (usbDevice.configuration.interfaces.length > 1) {
    await usbDevice.claimInterface(usbDevice.configuration.interfaces[1].interfaceNumber);
    console.log("NOTICE: Claimed interface 1.");
} else {
    await usbDevice.claimInterface(usbDevice.configuration.interfaces[0].interfaceNumber);
    console.log("NOTICE: Claimed interface 0.");
}
```

We select the `interfaceNumber` we want to claim for data transfer operations. The path to 
the `interfaceNumber` could be case-sensitive in each USB device, so this if-condition dynamically
locates the path without any extra actions.

Many problems occur also right here when trying to claim the interface.
If problems occur, make sure that no other sessions are running at the same 
time. If nothing helps, then in many cases the problem is solved by restarting
the computer.

Finally, the opened `usbDevice` is returned to the `globalDevice` in the 'connect'
button's EventListener and is now fully ready for use.

## 3) Known bugs

At the moment, the program is working, but there are a few known bugs. 

1) The program is not fully complete. We need to add WebUSB's `getDevices()` 
   function to the program, to prevent constant opening and closing of the 
   device and giving permission to the device every time the page is refreshed 
   or browsing to another page. Should be implemented later on to the program.

**  EDIT 26.6.2019: getDevices() is now working great. Notice, that Windows OS
    might limit the opening and closing of the device session to roughly 30 times.
    Linux and Apple do not have any limitations.**


2) If we print the product's name to the screen that is under 20 characters, 
   then the product's price following the name will be sliced up, since the
   printing of the price's characters will continue to the second row. We
   need a solution to prevent this.
   
**  EDIT 26.6.2019: This problems is also fixed. The printing works great whatever
    the length the string has.**

  
3) Occasionally and randomly, when closing the tab or the browser where the opened session
   is running, for some reason the printing of "  - Provet Cloud -  " to the
   screen in the `closingCode();` will mess up. Though no other major problems 
   occur because of this bug.
  
   
EDIT:
4) This program is always case-sensitive when trying it with other USB customer 
   display devices. Extra programming might be needed for correct functioning.
   

_________________________________________________________________


   **Happy hunting.**
   -nikoaha








