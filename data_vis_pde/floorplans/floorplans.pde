import java.awt.datatransfer.StringSelection;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;


PFont f;
float scalefactor;
PImage layout;

void setup() {
  size(1200, 637);
  colorMode(HSB);

  // Create the font
  f = createFont("Georgia", 24);
  textFont(f);

  layout = loadImage("img/phase1x.png");
  background(0);

  scalefactor = width / 34.441; // width of image divided by greatest width

}

void draw() {
  image(layout, 0, 0);

  // Display scaled mouse coordinates
  displayScaledMouseCoords();


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
    //println("Coords: (" + nf(scaledX, 0, 2) + ", " + nf(scaledY, 0, 2) + ")");
}

void copyToClipboard(String text) {
  StringSelection stringSelection = new StringSelection(text);
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  clipboard.setContents(stringSelection, null);
  println("Coordinates copied to clipboard: " + text); // Optional feedback
}
