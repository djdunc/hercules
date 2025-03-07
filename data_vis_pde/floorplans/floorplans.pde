import megamu.mesh.*;

import java.awt.datatransfer.StringSelection;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;


PFont f;
float scalefactor;
PImage layout;
HashMap<String, PVector> locations;

void setup() {
  size(1200, 637);
  colorMode(HSB);

  // Create the font
  f = createFont("Georgia", 24);
  textFont(f);

  layout = loadImage("img/phase1x.png");
  background(0);

  scalefactor = width / 34.441; // width of image divided by greatest width

  locations = new HashMap<String, PVector>();
  locations.put("Reception", new PVector(4.59, 10.10));
  locations.put("WC", new PVector(26.12, 2.55));
  locations.put("MR 1", new PVector(27.06, 11.05));
  locations.put("MR 2", new PVector(24.65, 10.91));
  locations.put("MR 3", new PVector(18.94, 14.78));
  locations.put("MR 4", new PVector(15.24, 14.72));
  locations.put("AF LHS", new PVector(3.73, 5.02));
  locations.put("AF RHS", new PVector(5.85, 5.02));
  locations.put("HFA 1", new PVector(0.98, 3.27));
  locations.put("HFA 2", new PVector(0.95, 1.21));
  locations.put("HFA 3", new PVector(3.19, 1.18));
  locations.put("HFA 4", new PVector(4.88, 1.26));
  locations.put("HFA 5", new PVector(6.54, 1.15));
  locations.put("HFA 6", new PVector(8.27, 1.15));
  locations.put("Vision 1", new PVector(24.83, 5.60));
  locations.put("Vision 2", new PVector(26.46, 5.68));
  locations.put("Vision 3", new PVector(32.26, 6.52));
  locations.put("Vision 4", new PVector(32.09, 8.50));
  locations.put("Vision 5", new PVector(32.12, 10.27));
  locations.put("Vision 6", new PVector(32.17, 12.20));
  locations.put("Vision 7", new PVector(32.12, 14.01));
  locations.put("MR Dilation 1", new PVector(30.42, 15.44));
  locations.put("MR Dilation 2", new PVector(32.06, 15.47));
  locations.put("MR Dilation 3", new PVector(33.67, 15.41));
  locations.put("MR Dilation 4", new PVector(33.49, 17.22));
  locations.put("MR Dilation 5", new PVector(31.57, 17.13));
  locations.put("MR Dilation 6", new PVector(29.71, 17.16));
  locations.put("MR Dilation 7", new PVector(28.10, 17.16));
  locations.put("Research 1", new PVector(10.50, 4.85));
  locations.put("Research 2", new PVector(10.36, 7.69));
  locations.put("Research 3", new PVector(10.30, 9.59));
  locations.put("Research 4", new PVector(10.25, 11.60));
  locations.put("Research 5", new PVector(10.33, 13.58));
  locations.put("Research 6", new PVector(14.44, 17.42));
  locations.put("Glaucoma 1 ORA", new PVector(19.69, 5.08));
  locations.put("Glaucoma 1 OCT", new PVector(16.65, 5.25));
  locations.put("Glaucoma 1 WF", new PVector(14.61, 5.17));
  locations.put("Glaucoma 2 ORA", new PVector(19.69, 7.81));
  locations.put("Glaucoma 2 OCT", new PVector(16.70, 7.92));
  locations.put("Glaucoma 2 WF", new PVector(14.64, 7.81));
  locations.put("Glaucoma 3 ORA", new PVector(19.60, 10.30));
  locations.put("Glaucoma 3 OCT", new PVector(16.73, 10.30));
  locations.put("Glaucoma 3 WF", new PVector(14.58, 10.30));

}

void draw() {
  image(layout, 0, 0);

  // Display scaled mouse coordinates
  displayScaledMouseCoords();

  drawVoronoi();
  drawPoints();

}

void drawVoronoi() {
  float[] points = new float[locations.size() * 2]; // Allocate space for x and y
  int i = 0;
  for (PVector loc : locations.values()) {
    points[i++] = loc.x * scalefactor; // x coordinate
    points[i++] = loc.y * scalefactor; // y coordinate
  }

  Voronoi voronoi = new Voronoi(points);
  MPolygon[] regions = voronoi.getRegions();

  noStroke();
  for (MPolygon region : regions) {
    fill(random(255), 255, 255);
    beginShape();
    for (int j = 0; j < region.getVertexCount(); j++) {
      PVector vertex = region.getVertex(j);
      vertex(vertex.x, vertex.y);
    }
    endShape(CLOSE);
  }
}

void drawPoints() {
  fill(0);
  for (PVector loc : locations.values()) {
    ellipse(loc.x * scalefactor, loc.y * scalefactor, 8, 8);
  }
  for (String name : locations.keySet()) {
    PVector loc = locations.get(name);
    text(name, loc.x * scalefactor + 10, loc.y * scalefactor + 5);
  }
}

void displayScaledMouseCoords() {
  float scaledX = mouseX / scalefactor;
  float scaledY = (height - mouseY) / scalefactor;

  String coordsText = "Coords: (" + nf(scaledX, 0, 2) + ", " + nf(scaledY, 0, 2) + ")";
  String coordsTextPasteable = nf(scaledX, 0, 2) + ", " + nf(scaledY, 0, 2) + ",";

  // Set text color and position
  fill(244,0,0); 
  textAlign(LEFT, TOP); // Align text to the top-left
  text(coordsText, 10, 10); // Display at the top-left corner (adjust as needed)

  // Check for mouse click to copy coordinates
  if (mousePressed) {
    copyToClipboard(coordsTextPasteable);
    mousePressed = false; // Reset mousePressed so it only copies once per click
  }
    // Optional: Display coordinates in the console as well
    println("Coords: (" + nf(scaledX, 0, 2) + ", " + nf(scaledY, 0, 2) + ")");
}

void copyToClipboard(String text) {
  StringSelection stringSelection = new StringSelection(text);
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  clipboard.setContents(stringSelection, null);
  println("Coordinates copied to clipboard: " + text); // Optional feedback
}
