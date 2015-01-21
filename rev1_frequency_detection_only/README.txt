Arduino MRI Triggering Device Revision 1

Overview

This sketch is an attempt to use the autocorrelation frequency detection algorithm to bring the triggering signal into synch with ideal input waveforms from the frequency generator. The frequency detection works very well, but this does not yet fulfill the requirement for the triggering signals to occur at the same point in the pressure waveform every time (for example we might wish to always trigger immediately after the peak of the waveform). This solution drifts accross the waveform as error accumulates.

Technical Notes

Working in this revision:
	-Detection of ideal signal's (from frequency generator) period to an accuracy of +/- 2ms
	-Circular Datastructure for an arbitrary number of samples
	-Hardware Timer based sample timing (timing the sampling directly from the hardware clock is much better than Sarah Joy's previous polling based timing)
	-Trigger signal output timed with precision +/- 1ms for the rising edge

Not Working in this revision
	-Serial communication with the Samba Board
	-Long term drift stability (the triggers slowly drift away from the correct portion of the waveform as error accumulates)

How to run:
	Open freq_detectin.ino in the Arduino IDE (version >=1.5.8) and press upload.-Set the signal generator to produce a wave between 0-3.3V (VOLTAGE >3.3V WILL DESTROY THE ARDUINO) by setting the amplitude and offset knob (you have to pull on the offset knob for it to work).
	-Set the frequency to a reasonable fetal heartrate value. 2 hz is a good starting poing (corresponds to 120bpm)
	-connect the signal generator lower output to analog input 1 on the Arduino using a BNC to pincer connector
	-connect powerlab's digital ouput pin #23 to input 3 on the powerlab using a BNC to pincer connector
	-connect the signal generator's upper output (TTL) to powerlab's input 2 using the short double ended BNC cable
	
	
The value of what I've written so far is that it allows us to predict with reasonable accuracy, when the next heartbeat will take place. This is a capability that the Siemens scanner's triggering is missing. The disadvantage of this approach is that it doesn't have sufficient long term stability for anything but the shortest of scans. I think that the value of this method will be in combining it with a peak detection or zero crossing algorithm so that peaks are timed based on predictive modelling, but those predictions are tweaked based on when they are actually observed.

Pictures of my setup in OneNote.