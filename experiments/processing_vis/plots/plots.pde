Table table;
PFont f;

String id;
int x,y,step,epoch, records, lastx, lasty, totalsecs, timesofar;
float scalefactor;
boolean redraw = true;

PImage layout;


// myRGB = new intr [9][3];
// myRGB = {{255,245,235},{254,230,206},{253,208,162},{253,174,107},{253,141,60},{241,105,19},{217,72,1},{166,54,3},{127,39,4}};

void setup() {

  size(1200, 637);
  colorMode(HSB);

  // Create the font
  //printArray(PFont.list());
  f = createFont("Georgia", 24);
  textFont(f);

  layout = loadImage("img/phase2.png");
  background(0);
  
//  table = loadTable("data/test1.csv", "header");
  table = loadTable("data/G1846_journey.csv", "header");
//  table = loadTable("data/P2_input_with_sec.csv", "header");
  records = table.getRowCount();
  println(records + " total rows in table");
  totalsecs = totaltime();
  println(totalsecs + " total time in seconds, or " + totalsecs/60 + " mins");
  epoch = 0;
  timesofar = 0;
  //printTableData();
  
  scalefactor = width/34.441; // wdith of image divided by greatest width
  lastx = int(table.getFloat(0, "xlocation")*scalefactor);
  lasty = int(table.getFloat(0, "ylocation")*scalefactor);
  lasty = height - lasty; // drop y value from top left to bottom left coord system
  
  id = table.getString(epoch, "Patient");

}

void draw() {
  
  if(redraw){
    tint(255, 80);
    image(layout, 0, 0);
    
    textAlign(LEFT);
    fill(151);
    text(id, 20, 30);
    
    redraw = false;
  }
  

  if(epoch < records){  
    // simplifying the x,y floats to ints and get the number of seconds at this location
    x = int(table.getFloat(epoch, "xlocation")*scalefactor); 
    y = int(table.getFloat(epoch, "ylocation")*scalefactor);
    y = height - y;
    step = table.getInt(epoch, "step_length_sec");
  }

  // define colours based on step time
  fill(map(timesofar,0,totalsecs,250,20), map(timesofar,0,totalsecs,20,250), 250, 70);
  //fill(50,200,200,50);
  stroke(map(timesofar,0,totalsecs,250,20), map(timesofar,0,totalsecs,20,250), 250, 50);
  //stroke(50,200,200,50);
  strokeWeight(3);
  
  // draw line adjusting for processing drawing from top left 
  // need to flip Y values down to bottom of sketch
  line(lastx, lasty, x, y);
  println(epoch + " (from " + lastx + ", " + lasty + ") " + " (to " + x + ", " + y + ") " + step);
  
  noStroke();
  circle(x,y,8);
  delay(step); // steps are in secs delay in millis 1000, so *10 is 100 times speed up 
  
  //println("timesofar " + timesofar + " totalsecs " + totalsecs + " ---- " + map(timesofar,0,totalsecs,20,250));
  
  // increment everything
  lastx = x;
  lasty = y;
  timesofar = timesofar + step;
  epoch++;
  
  // reset and start again

  //if(id.equals(table.getString(epoch, "Patient")) != true){
  if(epoch > records){
    epoch = 0;
    timesofar = 0;
    delay(10000);
    background(0, 50);
    println("---");
    redraw = true;
  }


  
}

void printTableData(){
  for (TableRow row : table.rows()) {

    id = row.getString("Patient");
    x = int(row.getFloat("xlocation")); // simplifying the input floats to ints
    y = int(row.getFloat("ylocation"));
    step = row.getInt("step_length_sec");
    
    println(id + " (" + x + ", " + y + ") " + step);
  }  
}

int totaltime(){
  int mytime = 0;
  for (TableRow row : table.rows()) {
    step = row.getInt("step_length_sec");
    mytime+=step;
  }
  return mytime;
}
