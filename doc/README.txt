
Enterprise Architect
====================

Generate Images
---------------

To generate useable vector images from the EA diagrams the following steps are necessary:

Change global Settings::

    Tools > Options > Standard Colors
      Select White (right lower corner) for "Paper" and "Element Fill".

    Tools > Options > Diagram > Appearance
      Gardient Fill Direction for an Element: <none>

    Tools > Options > Diagram
      Deselect "Print with Border".

The last options is needed to allow the automatic cropping of the generated PDFs files via Inkscape later.

Open the diagram and select ``File > Print``. Select ``Adobe PDF`` as Printer, hit Ok and save the file in the desired location.

Crop the files with Inkscape. This can be done on the command line::

    inkscape --verb=FitCanvasToDrawing -D --export-pdf=out.pdf in.pdf

Or you can use ``File > Document Properties > Resize page to content > Resize page to drawing or selection``.

