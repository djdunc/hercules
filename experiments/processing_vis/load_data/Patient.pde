class Patient {

  ArrayList<PVector> patientjourney;
  String patientID;

  Patient(String ID) {
    patientID = ID;
    patientjourney = new ArrayList<PVector>();
  }

  void printID() {
    println(patientID);
  }

  void printloc() {
    int total = patientjourney.size();
    println("The total number of journey locations is: " + total);


    for (int i = 0; i < patientjourney.size(); i++) {
      PVector pj = patientjourney.get(i);
      //      println("i:" + i + "x:" + pj.x + "y:" + pj.y + "z:" + pj.z);
      print(" z:" + pj.z);
    }
  }

  void addPatientLoc(PVector position) {
    patientjourney.add(position);
  }

  void drawJourney(int timer) {
    boolean init = true;
    float x, y;
    float lastx = 0;
    float lasty = 0;

    for (int i = 0; i < patientjourney.size(); i++) {
      PVector pj = patientjourney.get(i);

      if (init) {
        lastx = pj.x * scalefactor;
        lasty = pj.y * scalefactor;
        lasty = height - lasty; // drop y value from top left to bottom left coord system
        init = false;
      }

      String code = "G";
      int r = 50;
      int g = 250;
      int b = 75;
      if (patientID.indexOf(code) >=0) {
        g = 250;
        b = 75;
      } else {
        g = 150;
        b = 250;
      }


      if (pj.z <= timer) {

        if (patientID.indexOf(filter) >=0 || filter.equals("A")) {

          // draw dots
          noStroke();
          fill(r, g, b, 5);
          x = pj.x * scalefactor;
          y = pj.y * scalefactor;
          y = height - y;
          circle(x, y, 8);

          // darw lines
          stroke(r, g, b, 5);
          strokeWeight(1);
          line(lastx, lasty, x, y);

          // increment everything
          lastx = x;
          lasty = y;
        }
      }
    }
  }

  void drawtoscreen() {
  }

  // A method to test if the particle system still has particles
  boolean dead() {
    return patientjourney.isEmpty();
  }
}
