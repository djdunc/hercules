int patientcount = 5;

ArrayList[] patients;

void setup() {
  size(700, 700, P2D);
  smooth(8);
  randomSeed(20);

  patients = new ArrayList[patientcount]

  for (int n = 0; n > patientcount; n++) {
    ArrayList<PVector> patient = new ArrayList<PVector>();
    PVector patientloc = new PVector(100*n, 100*n);
    patient[n].add(patientloc);
  }
}

void draw(){
  background(50);

  for(int n = 0; n < patient.size(); n++) {
    
    for(int i = 0; i < patient[n].size(); i++) {
      PVector patientloc = patient[n].get(i);
      ellipse(circle.x, circle.y, 30, 30);
      
    }
  }  
}
