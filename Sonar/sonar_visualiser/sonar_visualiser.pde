import processing.serial.*;
import controlP5.*;

Serial port;
float angle = 0;
float distance = 0;
String portName = "COM5";
int baud = 9600;
int lf = 10;
ControlP5 cp5;
PFont font;
PFont orcFont;

void setup() {
  size(1900, 1080);
  smooth();
  port = new Serial(this, portName, baud);
  port.bufferUntil(lf);
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 20);    // custom fonts for buttons and title

  cp5.addButton("Fast")     
    .setPosition(20, 20)  
    .setSize(90, 40)      
    .setFont(font)
    ;

  cp5.addButton("Quick")     
    .setPosition(20, 65)  
    .setSize(90, 40)     
    .setFont(font)
    ;

  cp5.addButton("Medium")    
    .setPosition(20, 110)  
    .setSize(90, 40)     
    .setFont(font)
    ;

  cp5.addButton("Slow")    
    .setPosition(20, 155)  
    .setSize(90, 40)      
    .setFont(font)
    ;
  cp5.addButton("Full")     
    .setPosition(1750, 20)  
    .setSize(90, 40)     
    .setFont(font)
    ;
  cp5.addButton("Wide")     
    .setPosition(1750, 65)  
    .setSize(90, 40)      
    .setFont(font)
    ;
  cp5.addButton("Narrow")     
    .setPosition(1750, 110)  
    .setSize(90, 40)      
    .setFont(font)
    ;
  cp5.addButton("Steer")     
    .setPosition(1750, 155)  
    .setSize(90, 40)     
    .setFont(font)
    ;
  cp5.addButton("Beam")     
    .setPosition(1750, 200) 
    .setSize(90, 40)      
    .setFont(font)
    ;

}

void draw() {
  fill(98, 245, 31);
  noStroke();
  fill(0, 4);
  rect(0, 0, width, height * 0.935);
  fill(98, 245, 31);
  drawRadar();
  drawLine();
  drawObject();
  drawText();
}

void serialEvent(Serial p) {
  try {
    String[] values = p.readStringUntil('\n').trim().split(",");
    if (values.length == 2) {
      angle = float(values[0]);
      distance = float(values[1]);
    }
  }
  catch (RuntimeException e) {
  }
}

void Fast() {
  port.write('r');
}

void Quick() {
  port.write('t');
}

void Medium() {
  port.write('b');
}

void Slow() {
  port.write('e');
}
void Full() {
  port.write('y');
}

void Wide() {
  port.write('u');
}

void Narrow() {
  port.write('i');
}

void Steer() {
  port.write('o');
}
void Beam() {
  port.write('p');
}


void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30, 250, 60);
  translate(width/2, height-height*0.074); // moves the starting coordinats to new location
  line(0, 0, (height-height*0.12)*cos(radians(angle)), -(height-height*0.12)*sin(radians(angle))); // draws the line according to the angle
  popMatrix();
}

// This function draws a radar screen on the display
void drawRadar() {
  // Save the current transformation matrix
  pushMatrix();
  // Move the origin to the center of the bottom edge of the display
  translate(width/2, 0.926 * height);
  // Do not fill the shapes
  noFill();
  // Set the stroke weight to 2 pixels
  strokeWeight(2);
  // Set the stroke color to green
  stroke(98, 245, 31);

  // Draw four concentric circles with different coefficients
  drawCircle(0.9375);
  drawCircle(0.7300);
  drawCircle(0.5210);
  drawCircle(0.3130);

  // Draw a horizontal line across the display
  line(-width/2, 0, width/2, 0);
  // Draw five lines at different angles from the origin
  for (int angle = 30; angle <= 150; angle+=30) {
    drawAngledLine(angle);
  }
  // Draw a line from the origin to the right edge of the display at a 30 degree angle
  line(-width/2 * cos(radians(30)), 0, width/2, 0);
  // Restore the previous transformation matrix
  popMatrix();
}

// This function draws a circle with a given coefficient
void drawCircle(final float coefficient) {
  // Draw an arc with the given coefficient, width, and height, from PI to TWO_PI radians
  arc(0, 0, coefficient * width, coefficient * width, PI, TWO_PI);
}

// This function draws a line with a given angle
void drawAngledLine(final int angle) {
  // Draw a line from the origin to the left edge of the display at the given angle
  line(0, 0, (-width/2) * cos(radians(angle)), (-width/2) * sin(radians(angle)));
}

// This function draws an object on the screen based on the distance and angle values from the ultrasonic sensor and the servo motor
void drawObject()
{
  // Save the current transformation matrix
  pushMatrix();
  // Move the origin to the center of the bottom edge of the screen
  translate(width/2, 0.926 * height);
  // Set the stroke weight to 9 pixels
  strokeWeight(9);
  // Set the stroke color to red
  stroke(255, 10, 10);
  // Calculate the distance in pixels from the origin to the object
  int pixsDistance = int(distance * 0.020835 * height);
  // If the distance is less than 40 units and not zero
  if (distance < 40 && distance != 0) {
    // Calculate the cosine and sine of the angle in radians
    float cos = cos(radians(angle));
    float sin = sin(radians(angle));
    // Calculate the x and y coordinates of the object relative to the origin
    int x1 = +int(pixsDistance * cos);
    int y1 = -int(pixsDistance * sin);
    // Calculate the x and y coordinates of the end point of the line relative to the origin
    int x2 = +int(0.495 * width * cos);
    int y2 = -int(0.495 * width * sin);

    // Draw a line from the object to the end point
    line(x1, y1, x2, y2);
  }
  // Restore the previous transformation matrix
  popMatrix();
}

// This function draws some text on the screen to display the information about the object
void drawText()
{
  // Save the current transformation matrix
  pushMatrix();
  // Set the fill color to black
  fill(0, 0, 0);
  // Do not draw the outline of the shapes
  noStroke();
  // Draw a rectangle at the bottom of the screen
  rect(0, 0.9352 * height, width, height);
  // Set the fill color to green
  fill(98, 245, 31);
  // Set the text size to 25 pixels
  textSize(25);
  // Draw the text for the distance markers
  text("10cm", 0.6146 * width, 0.9167 * height);
  text("20cm", 0.7190 * width, 0.9167 * height);
  text("30cm", 0.8230 * width, 0.9167 * height);
  text("40cm", 0.9271 * width, 0.9167 * height);

  // Set the text size to 40 pixels
  textSize(40);
  // Draw the text for the object status
  text("Object: " + (distance > 40 ? "Out of Range" : "In Range"), 0.125 * width, 0.9723 * height);
  // Draw the text for the angle value
  text("Angle: " + angle + " °", 0.52 * width, 0.9723 * height);
  // Draw the text for the distance value
  text("Distance: ", 0.74 * width, 0.9723 * height);
  // If the distance is less than 40 units
  if (distance < 40) {
    // Draw the text for the distance value in centimeters
    text("        " + distance +" cm", 0.775 * width, 0.9723 * height);
  }
  // Set the text size to 25 pixels
  textSize(25);
  // Set the fill color to light green
  fill(98, 245, 60);
  // Move the origin to the position of the 30 degree marker
  translate(0.5006 * width + width/2 * cos(radians(30)), 0.9093 * height - width/2 * sin(radians(30)));
  // Rotate the coordinate system by -60 degrees
  rotate(-radians(-60));
  // Draw the text for the 30 degree marker
  text("30°", 0, 0);

  // Reset the transformation matrix
  resetMatrix();

  // Move the origin to the position of the 60 degree marker
  translate(0.497 * width + width/2 * cos(radians(60)), 0.9112 * height - width/2 * sin(radians(60)));
  // Rotate the coordinate system by -30 degrees
  rotate(-radians(-30));
  // Draw the text for the 60 degree marker
  text("60°", 0, 0);
  // Reset the transformation matrix
  resetMatrix();
  // Move the origin to the position of the 90 degree marker
  translate(0.493 * width + width/2 * cos(radians(90)), 0.9167 * height - width/2 * sin(radians(90)));
  // Rotate the coordinate system by 0 degrees
  rotate(radians(0));
  // Draw the text for the 90 degree marker
  text("90°", 0, 0);
  // Reset the transformation matrix
  resetMatrix();

  // Move the origin to the position of the 120 degree marker
  translate(0.487 * width + width/2 * cos(radians(120)), 0.92871 * height - width/2 * sin(radians(120)));
  // Rotate the coordinate system by 30 degrees
  rotate(radians(-30));
  // Draw the text for the 120 degree marker
  text("120°", 0, 0);
  // Reset the transformation matrix
  resetMatrix();

  // Move the origin to the position of the 150 degree marker
  translate(0.4896 * width + width/2 * cos(radians(150)), 0.9426 * height - width/2 * sin(radians(150)));
  // Rotate the coordinate system by 60 degrees
  rotate(radians(-60));
  // Draw the text for the 150 degree marker
  text("150°", 0, 0);
  // Restore the previous transformation matrix
  popMatrix();
}
