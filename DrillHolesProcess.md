# [NEW]

File -> Plot
Choose "Plot Format" SVG
Choose "Drill Marks" -> Actual Size
Tick "Sketch pads on Fabrication Layers"
In "Include Layers" section tick only "F Silkscreen"
In "Plot on All Layers" tick "Edge Cuts"
Set correct output directory at top (relative path)
Click "Plot" button, one file should be generated called "F Silkscreen"
In "Include Layers" section tick only "B Silkscreen"
In "Plot on All Layers" tick nothing
Load both files into Inkscape, tidy, make holes RGB 255 255 255 0 and 0.01mm thick with 75% power and 1 speed. Make silkcreen text RGB 0 0 0 0 and 0.01mm thick with 40% power and 1 speed. The former cuts through, latter cuts surface only
Only the former (front) file has/cuts holes border, the back file should have only silkscreening
Laser cut front file, flip the cut over in place (in the rectangular hole that was created), then laser cut back file

# [OLD] Process for exporting drill holes as SVG format for test laser cutting

In pcb editor choose File -> Plot -> Generate Drill Files: name-NPTH.drl and name-PTH.drl files will be made

double click project_name-NPTH.drl in project file window to open in gerber viewer
File -> Export to PCB Editor
save as NPTH.kicad_pcb
when `Layer Selection` box pops up, click three dots ... and choose Edge.Cuts layer, then click OK
open NPTH.kicad_pcb in project file window, choose File -> Plot, choose Plot Format `SVG`, Include Layers `Edge.Cuts` only and click Plot

repeat above steps for PTH.drl

open each file in SVG

Don't forget to plot also the actual Edge Cuts layer from the original project in the same way as above i.e.
- File -> Plot, choose Plot Format `SVG`, Include Layers `Edge.Cuts` only and click Plot

Now you have three .svg files
project_name-Edge_Cuts.svg
npth-Edge_Cuts.svg
pth-Edge_Cut.svg

Combine in Illustrator and cut all as red thin lines