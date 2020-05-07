# openBP
Open Brilliant Pebbles firmware

We are releasing an open version of the Brilliant Pebbles firmware and some documentation on the hardware to help you turn it into your very own device! With a barometer, piezo speaker, a battery and charging circuit you can turn it into a paragliding vario, a weather sensor, a generic timer, or whatever you want!

Please note – uploading another firmware to your Brilliant Pebble will stop it working with the Android and computer apps. We have a section at the bottom of this page that shows how to restore original functionality to your device, but do this at your own risk! We cannot accept any responsibility for any consequences of tinkering with your audible. We recommend doing this for advanced users only.



To make the code as easy to understand as possible, we have removed a lot of features from the example firmware. However, you can set yourself a challenge and add them back in with the information we’ve provided! The example firmware:

* alarms are in metres
* alarms are hard-coded in the firmware
* this does not communicate with the official Brilliant Pebbles Android app or computer app
* this does not have the “auto-off” function after 16 hours (see “turning off the device” section above for how to do it)
* no hard deck alarms
* no altitude offset
* battery life is shorter than the official firmware (this is because the low power library is quite risky and users might brick their device if not used correctly)
* climb detection algorithm is also much more simplified (compared to official firmware)



More information on: https://www.aon2.co.uk/openbp/


