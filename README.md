# CNC Plotter and SerialSender

## thanks to [Teunis van Beelen](https://www.teuniz.net/RS-232/)

### Inkscape: extensions: gcodetools

0. File : Document Properties -> width and heigth to 4000
1. draw cleen with draw lines tool
2. select drawings 
3. gcodetools : tools library -> use default tool and search for green thing set diameter to 1.0
4. select drawings
5. gcodetools : Orientation Points -> to points mode
6. find orientation thingy and drag it way out of the right upper corner of drawing
7. set right middle value to 500 (x and y are a bit messed up this way the image is drawn in Landscape)
8. gcodetools : path to gcode
9. tab preferences set file name and location (for output) press apply 
10. tab preferences set Post-processor to Round all values to 4 digits
11. tab options minimum arc radius to 50 press apply
12. tab path to gcode select cutting order subpath by subpath and Sort paths to reduse rapid distance
13. press apply and gcode should be generated
14. working example: 
  * Test3_ink.scg	
  * Test4_gcode_0005.ngc