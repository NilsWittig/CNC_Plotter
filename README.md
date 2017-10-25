
CNC Plotter and SerialSender


Inkscape:
	extensions: gcodetools
	0. File : Document
	1. draw cleen with draw lines tool
	2. select drawings 
	3. gcodetools : tools library -> use default tool and search for green thing set diameter to 1.0
	4. select drawings
	4.1 gcodetools : Orientation Points -> to points mode
	4.2 find orientation thingy and drag it way out of the right upper corner of drawing
	4.3 set right middle value to 500 (x and y are a bit messed up this way the image is drawn in Landscape)
	5. gcodetools : path to gcode
	6. tab preferences set file name and location (for output) press apply 
	6.1 tab preferences set Post-processor to Round all values to 4 digits
	7. tab options minimum arc radius to 50 press apply
	8. tab path to gcode select cutting order subpath by subpath and Sort paths to reduse rapid distance
	9. press apply and gcode should be generated

working example: Test3_ink.scg
		 Test4_gcode_0005.ngc