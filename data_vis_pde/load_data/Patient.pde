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

  String getPatientID() {
    return patientID;
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
        r = 10;
        g = 250;
        b = 105;
      } else {
        r = 200;
        g = 250;
        b = 50;
      }

      // for few locations show them with a higher intensity
      // - this is the alpha level applied to each journey
      int a=100;
      if (GRcode.equals("all")) {
        a = 8;
      } else {
        a = 80;
      }

      int timeWindow = 300;
      if ((pj.z <= timer)) {  // && (pj.z > timer-timeWindow)

        //float aa = map(timer-pj.z, 0, timeWindow, 80, 5);
        //a =int(aa);

        if (patientID.indexOf(filter) >=0 || filter.equals("A")) {

          // draw dots
          noStroke();
          fill(r, g, b, a);
          x = pj.x * scalefactor;
          y = pj.y * scalefactor;
          y = height - y;
          circle(x, y, 8);

          // darw lines
          stroke(r, g, b, a);
          strokeWeight(1);
          line(lastx, lasty, x, y);

          // increment everything
          lastx = x;
          lasty = y;
        }
      }

      if (timestep == timetostop) {

        if (patientID.indexOf(filter) >=0 || filter.equals("A")) {
          
          a = 5;

          // draw dots
          noStroke();
          fill(r, g, b, a);
          x = pj.x * scalefactor;
          y = pj.y * scalefactor;
          y = height - y;
          circle(x, y, 8);

          // darw lines
          stroke(r, g, b, a);
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

  void deletePhase() {
    for (Patient journey : patients) {
      patients.remove(journey);
    }
  }


  // A method to test if the particle system still has particles
  boolean dead() {
    return patientjourney.isEmpty();
  }
}
