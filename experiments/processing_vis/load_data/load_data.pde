Table table;
PFont f;
PImage layout;

ArrayList<Patient> patients;
float scalefactor = 1;
int timestep = 0;
int timetostop = 60*60*6;
boolean play;
String filter = "A";

void setup() {
  size(1200, 637);
  colorMode(RGB);
  background(0);
  layout = loadImage("img/phase2.png");
  frameRate(60);
  play = false;

  f = createFont("Georgia", 24);
  textFont(f);

  patients = new ArrayList<Patient>();
  loadData();

  scalefactor = width/34.441; // wdith of image divided by greatest width
}

void draw() {
  background(0);
  tint(255, 80);
  image(layout, 0, 0);

  drawMenu();
    

  for (Patient journey : patients) {
    //journey.printID();
    //journey.printloc();
    journey.drawJourney(timestep);
  }
  
  if(play){

    timestep=timestep+120;
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
  table = loadTable("data/P2_input_with_sec.csv", "header");

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

void drawMenu(){
  // time
  textAlign(LEFT);
  fill(151);
  textSize(24);
  text(timestep / 60 + " mins", 20, 30);
  text("(" + timestep / 60 / 60 + " hours)", 140, 30);
  
  // menu
  textSize(12);
  text("spacebar to play / pause", 20, 60);
  text("g: Glaucoma, r: Retina, a: All", 20,80);
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
  if (key == ' '){  // spacebar to togggle between play / no play
    play = !play;
  }
  if (key == 'g'){  // little g for glaucoma only view
    filter = "G";
  }
  if (key == 'r'){  // little r for retina only view
    filter = "R";
  }
  if (key == 'a'){  // little a to jump back to all
    filter = "A";
  }
  if (key == 'G'){  // big G for jumping to end of Glaucoma only view
    filter = "G";
    timestep = timetostop;
  }
  if (key == 'R'){  // big R for jumping to end of retina only view
    filter = "R";
    timestep = timetostop;
  }
  if (key == '0'){  // big R for jumping to end of retina only view
    timestep = 0;
  }
  
}
