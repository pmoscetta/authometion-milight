# authometion-milight
Arduino library for MiLight and Limitless led bulbs control using a LYT/WiFi shield or a PL1167 pcb module.

Please remember that MiLight/Limitlessled radio bulbs can only receive commands without giving a reply (half duplex transmission).
For this reason all the commands related to set bulb address (SEA) and clear bulb address (CLA) need to be sent within the first 3 seconds from bulb power on.
We are still writing a manual for the library but you can find commands explanation inside the MiLight.cpp file.
This library can manage both RGBW and WW radio bulbs.
For any requests please use our on line forum @ authometion.com/forum.


Pietro Moscetta
