# GE Washer Encoder Repair

This project repairs the selector knob on a GE Washing Machine.  The machine has a control board set (part number WH22X32060) with a 10-position rotary encoder that tends to wear out.  When you rotate it, instead of advancing to the next position, the election (as indicated by LEDs) jumps around semi randomly.  Even if you can manage to land it on the desired selection, it is likely to jump around during the run, which sometimes aborts the cycle.

You can buy a new control board set from GE for $400, or you can get a used one off eBay for about $60 - but the used ones don't last because their encoders are probably already worn.  I took apart an encoder, cleaned the contacts and retensioned them.  That made the washer work briefly, but it failed again after a few weeks.

The rotary encoder has one common contact and 4 other contacts that form switch closures with the common one depending on the rotation.  With 4 contacts there could be 16 possible combinations but only 10 are used on this washer.

This repo contains an ESP8266 (most any MCU would work) program that cycles through a list of 10 4-bit numbers when you press a pushbutton switch.  The bits correspond to the rotary encoder switch combinations.  The selected number is sent over I2C to an MCP23008 expander chip whose outputs drive reed relays via a ULN2003 driver chip.  The reed relay contacts are connected in place of the rotary encoder contacts.

It would be possible to use GPIOs to directly drive the ULN2003 inputs instead of going through the MCP23008 I2C chip, but I already had a board from an old project with that combo, so it was easier to use it as-is instead of rewiring.

I tried using 2N7000 MOSFETs instead of relays, but that didn't work.  The circuit on the washer control board does not ground the common pole of the switch, but rather modulates it in some funny way, so the MOSFETs did not do the job.  If I have to do it again (our dryer has the same kind of encoder), I might try to use CMOS transmission gates.

I 3D printed a simple cap to replace the existing knob, with a hole in it for the pushbutton.

The MCU and relay board are powered from the washer control board's 5V supply (conveniently labeled).  The extra current draw does not appear to cause problems.

I used a D1 Mini ESP8266, but I think that an ESP-01 would have been enough (alveit cumbersome to program and debug), since it only needs 2 pins for I2C (repurpose Rx and Tx) and one for the pushbutton.

So far it is working.  The wife is happy because she successfully did a load of laundry.
