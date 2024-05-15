import controlP5.*;
ControlP5 cp5;

Table table;
PFont f;
PImage[] layout;

ArrayList<Patient> patients;
float scalefactor = 1;
int timestep = 0;
int timetostop = 60*60*6;
boolean play;
String filter = "A";
int phase = 1;

String GRcode = "all";


void setup() {
  size(1200, 637);
  colorMode(RGB);
  background(0);

  layout = new PImage[4]; // creates array that can hold 3 PImage objects
  layout[0] = loadImage("img/phase1.png");
  layout[1] = loadImage("img/phase2.png");
  layout[2] = loadImage("img/phase3.png");
  layout[3] = loadImage("img/phase4.png");

  frameRate(60);
  play = false;

  f = createFont("Georgia", 24);
  textFont(f);

  cp5 = new ControlP5(this);
  cp5.addTextfield("input")
    .setPosition(20, height-30)
    .setSize(50, 20)
    .setFont(f)
    .setFocus(true)
    .setColor(color(200, 220, 0))
    ;

  patients = new ArrayList<Patient>();
  loadData();

  scalefactor = width/34.441; // wdith of image divided by greatest width
}

void draw() {
  background(0);
  tint(255, 80);
  image(layout[phase-1], 0, 0);

  drawMenu();
  textSize(12);
  text(cp5.get(Textfield.class, "input").getText(), 360, 130);

  for (Patient journey : patients) {
    //journey.printID();
    //journey.printloc();

    String n = journey.getPatientID();
    // if a GRcode has been set then it means we want to view 1 record only
    if (n.equals(GRcode)) {
      println(n);
      journey.drawJourney(timestep);
    }
    // if GRcode is "all" then show all records
    if (GRcode.equals("all")) {
      journey.drawJourney(timestep);
    }
  }

  if (play) {

    timestep=timestep+20;
    //println(timestep);
    //println(scalefactor);
    println(filter);

    if (timestep >= timetostop) {
      noLoop();
    }
  }
}


void loadData() {
  // Load CSV file into a Table object - "header" option indicates the file has a header row
  switch(phase) {
  case 1:
    table = loadTable("data/P1_input_with_sec.csv", "header");
    break;
  case 2:
    table = loadTable("data/P2_input_with_sec.csv", "header");
    break;
  case 3:
    table = loadTable("data/P3_input_with_sec.csv", "header");
    break;
  case 4:
    table = loadTable("data/P4_input_with_sec.csv", "header");
    break;
  }



  int patientLocationCount = 1;  // for each patient we keep track of how many locations we have
  String n = "";                 // Temp string used to see if we are still reading the same patient
  String patientID = "xx";       // Used to save a string id for each of the patients - xx at start so first is different
  float x, y, step = 0;            // floats that will be used in the x,y,z PVector values
  float steptot = 0;             // a running total through the journey of the time spent
  int patientcount = -1;         // set at -1 since we increment to 0 first go around loop


  // Main loop of parsing the data - for each row we gather data,
  // if the patient id string is different then we create a new patient object
  // and add in location data for this cycle.
  // if the names dont match then we just add the location data to the existing id
  for (TableRow row : table.rows()) {

    n = row.getString("Patient");
    x = row.getFloat("xlocation");
    y = row.getFloat("ylocation");
    step = row.getInt("step_length_sec");



    if (n.equals(patientID)) {
      patientLocationCount++;
      //print(patientLocationCount + ": (" + x + ", " + y + ") " + step + "  ");
      steptot = steptot + step; // on each iteration we are adding up the time spent
      Patient record = patients.get(patientcount);
      record.addPatientLoc(new PVector(x, y, steptot));
    } else {
      //      println();
      //      println();
      //      println(patientID + " last steptot: " + steptot);
      patientID = row.getString("Patient");
      patientLocationCount = 1;
      //      println();
      //      println();
      //      println(patientID + " data below ");
      //      print(patientLocationCount + ": (" + x + ", " + y + ") " + step + "  ");
      patients.add(new Patient(patientID));
      patientcount++;
      Patient record = patients.get(patientcount);
      record.addPatientLoc(new PVector(x, y, step)); // first time around use the first step value
      steptot = 0;
    }
  }
}

void drawMenu() {
  // time
  textAlign(LEFT);
  fill(151);
  textSize(24);
  text(timestep / 60 + " mins", 20, 30);
  text("(" + timestep / 60 / 60 + " hours)", 140, 30);

  // menu
  textSize(12);
  text("spacebar to play / pause", 20, 60);
  text("g: Glaucoma, r: Retina, c: Cataract, a: All", 20, 80);
  text(GRcode, 20, height-40);
}

void printTableData() {
  for (TableRow row : table.rows()) {

    String id = row.getString("Patient");
    int x = int(row.getFloat("xlocation")); // simplifying the input floats to ints
    int y = int(row.getFloat("ylocation"));
    int step = row.getInt("step_length_sec");

    println(id + " (" + x + ", " + y + ") " + step);
  }
}

void mousePressed() {
  timestep=timestep+1;
  println(timestep);
}

void keyPressed() {
  if (key == ' ') {  // spacebar to togggle between play / no play
    play = !play;
  }
  if (key == 'g') {  // little g for glaucoma only view
    filter = "G";
  }
  if (key == 'r') {  // little r for retina only view
    filter = "R";
  }
  if (key == 'c') {  // little r for retina only view
    filter = "C";
  }
  if (key == 'a') {  // little a to jump back to all
    filter = "A";
  }
  if (key == 'G') {  // big G for jumping to end of Glaucoma only view
    filter = "G";
    timestep = timetostop;
  }
  if (key == 'R') {  // big R for jumping to end of retina only view
    filter = "R";
    timestep = timetostop;
  }
  if (key == '0') {  // big R for jumping to end of retina only view
    timestep = 0;
  }
  if (key == '.') {  // right arrow for adding 60 seconds
    timestep = timestep + 60;
  }
  if (key == ',') {  // right arrow for adding 60 seconds
    timestep = timestep - 60;
  }
  if (key == '1') {  
    phase = 1;
    for (int i = patients.size()-1; i >= 0; i--) {
      patients.remove(i);
    }
    loadData();
  }
  if (key == '2') {  
    phase = 2;
    for (int i = patients.size()-1; i >= 0; i--) {
      patients.remove(i);
    }
    loadData();
  }
  if (key == '3') {  
    phase = 3;
    for (int i = patients.size()-1; i >= 0; i--) {
      patients.remove(i);
    }
    loadData();
  }
  if (key == '4') {  
    phase = 4;
    for (int i = patients.size()-1; i >= 0; i--) {
      patients.remove(i);
    }
    loadData();
  }

  if (key == 'p') {  // p - show the location data for each patient record
    int i=0;
    for (Patient journey : patients) {
      journey.printID();
      //journey.printloc();
      i++;
    }
    print("Number of Patients = " + i);
  }
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isAssignableFrom(Textfield.class)) {
    println("controlEvent: accessing a string from controller '"
      +theEvent.getName()+"': "
      +theEvent.getStringValue()
      );
    GRcode = theEvent.getStringValue();
  }
}
