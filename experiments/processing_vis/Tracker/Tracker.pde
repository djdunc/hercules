
ArrayList<ParticleSystem> systems;

void setup() {
  size(640, 360);
  systems = new ArrayList<ParticleSystem>();
}

void draw() {
  background(0);
  for (ParticleSystem ps : systems) {
    ps.run();
    ps.addParticle();
  }
  if (systems.isEmpty()) {
    fill(255);
    textAlign(CENTER);
    text("click mouse to add particle systems", width/2, height/2);
  }
}

void mousePressed() {
  systems.add(new ParticleSystem(1, new PVector(mouseX, mouseY)));
}
