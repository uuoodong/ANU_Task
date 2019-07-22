import java.awt.Rectangle;
import processing.serial.*;
ArrayList particles;
Serial port;
Rectangle blackhole;


PImage overlay;

Maxim maxim;

ArrayList drops;

AudioPlayer dropsound;
AudioPlayer rain;
AudioPlayer thunder; 

void setup() {
  port = new Serial(this, "COM5", 9600);
  port.bufferUntil('\n');
  size(1024, 768);
  frameRate(120/2);

  blackhole = new Rectangle(0, 720, 1024, 68);
  particles = new ArrayList();

  drops = new ArrayList();


  maxim = new Maxim(this);

  smooth();
}


int pointX=1, pointY=1;
int x=0;
int y=0;
String hum = null;
String tem = null;
String cbs = null;
void draw() {
  if (x> 1024) {
    x = 1024;
  }
  if (x< 0) {
    x = 0;
  }
  if (y> 768) {
    y = 768;
  }
  if (y<0) {
    y=0;
  }
  background(13, 20, 29);
  noStroke();
  fill(10, 15, 17);
  rect(blackhole.x, blackhole.y, blackhole.width, blackhole.height);

  for (int i = 3; i >= 0; i-- ) {
    particles.add(new Particle(random(0, 1024), random(-40, -50)));
  }

  for (int i = particles.size() - 1; i >= 0; i-- ) {
    Particle p = (Particle) particles.get(i);

    p.run();
    p.gravity();
    p.display();

    if ((p.x > x - 150 ) && (p.x < x + 150 )  && (p.y > y - 150 ) && (p.y < y + 150 ) && !p.ismulticolor) {

      p.stop();
    }

    if (blackhole.contains(p.x, p.y)) { 
      p.stop();
    }

    if ((p.x > 750 - 10 ) && (p.x < 750 + 60 )  && (p.y > 640- 10 ) && (p.y < 640  ) ) {
      p.stop();
    }
    if (p.finished()) {
      particles.remove(i);
    }
  }
}
void serialEvent(Serial p) { 
  String inString = port.readStringUntil('\n');
  char ch=inString.charAt(0);
  println(ch); 

  if (ch == 'w') {
    y+=5;
  } else if (ch == 's') {
    y-=5;
  } else if (ch == 'a') {
    x+=5;
  } else if (ch == 'd') {
    x-=5;
  }
}
