import processing.serial.*;

Serial myPort; // Create object from Serial class
String incomingData; // Variable to hold incoming data
float accX, accY, accZ, gyroX, gyroY, gyroZ; // Sensor values

void setup() {
    size(800, 600, P3D); // Set the size of the window and enable 3D rendering
    println(Serial.list()); // Print available ports for debugging
    myPort = new Serial(this, Serial.list()[0], 9600); // Change the index if necessary
}

void draw() {
    background(255); // Clear the window with a white background
    lights(); // Enable lighting

    // Draw the MPU 3D block in the center of the screen
    drawMPUBlock();

    // Removed the display of sensor values
}

void drawMPUBlock() {
    // Set the position and size of the cube
    float cubeSize = 100;
    translate(width / 2, height / 2, 0); // Move the origin to the center of the window

    // Rotate the cube based on gyroscope values
    rotateY(radians(gyroY)); // Rotate around Y-axis
    rotateX(radians(gyroX)); // Rotate around X-axis
    rotateZ(radians(gyroZ)); // Rotate around Z-axis

    // Draw the 3D cube
    fill(100, 150, 200);
    box(cubeSize); // Draw the cube
}

// The displaySensorValues() function is removed to avoid showing values

void serialEvent(Serial myPort) {
    incomingData = myPort.readStringUntil('\n'); // Read data until a newline character
    if (incomingData != null) {
        incomingData = trim(incomingData); // Remove any extra whitespace
        println("Received data: " + incomingData); // Print the raw data for debugging

        // Split the data based on commas and extract values
        String[] values = split(incomingData, ", ");
        if (values.length >= 6) { // Check if we have at least 6 values
            try {
                // Extract the numeric value and parse it as a float
                accX = Float.parseFloat(values[0].split(": ")[1].split(" ")[0]); // Parse AccX
                accY = Float.parseFloat(values[1].split(": ")[1].split(" ")[0]); // Parse AccY
                accZ = Float.parseFloat(values[2].split(": ")[1].split(" ")[0]); // Parse AccZ
                gyroX = Float.parseFloat(values[3].split(": ")[1].split(" ")[0]); // Parse GyroX
                gyroY = Float.parseFloat(values[4].split(": ")[1].split(" ")[0]); // Parse GyroY
                gyroZ = Float.parseFloat(values[5].split(": ")[1].split(" ")[0]); // Parse GyroZ
            } catch (NumberFormatException e) {
                println("Error parsing numbers: " + e.getMessage());
            }
        } else {
            println("Received invalid data format: " + incomingData);
        }
    }
}
