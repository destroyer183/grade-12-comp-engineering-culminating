/* Put Header Here



WHAT IF WE USE A JOYSTICK TO DETECT WRIST MOVEMENT SO WE CAN GET SIDE TO SIDE WRIST MOTION IN ADDITION TO UPAND DOWN WRIST MOTION?



if the current smoothing code doesn't work, here are a few other options:
round every angle value down to the nearest multiple of 10, and don't send an angle value to the servo if it is the same as the last angle value sent



   
 */
#include <Servo.h>

#define getSize(inputArray) ({\
    int containerSize = 0;\
    for (int element: inputArray) {containerSize++;}\
    containerSize;\
})



double arraySum;
int i;



double getAverage(int inputArray[], int arraySize) {

    arraySum = 0;

    for (i = 0; i < arraySize; i++) {
        arraySum += inputArray[i];
    }

    return (double(arraySum) / arraySize);
}



class HingeJoint {

    public:

        HingeJoint(int servoPinInput, int flexPinInput, int[] mapValuesInput) {

            servoPin = servoPinInput;
            flexPin = flexPinInput;
            mapValues[0] = mapValuesInput[0];
            mapValues[1] = mapValuesInput[1];
            mapValues[2] = mapValuesInput[2];
            mapValues[3] = mapValuesInput[3];

            servoVar.attach(servoPin);
            pinMode(flexPin, INPUT);
            updateServo(100);
        }

        // function to take in each new flexInput value, and handle it accordingly
        void takeFlexInput() {

            flexValue = analogRead(flexPin);

            if (flexValue > 100) {
                flexValue = 100;
            } else if (flexValue < 20) {
                flexValue = 20;
            }

            if (loopCounter >= 9) {

                flexArray[loopCounter] = flexValue;

                arraySize = getSize(flexArray);

                flexAverage = getAverage(flexArray, arraySize);
            }

            if (servoLagCounter >= servoLagCounterMax - 1) {
                updateServo(flexAverage);

                if (servoLagCounter > ServoLagCounterMax - 1) {
                    servoLagCounter = ServoLagCounterMax - 1;
                }
            }

            loopCounter = (loopCounter + 1) % 10;
            servoLagCounter = (servoLagCounter + 1) % servoLagCounterMax;
        }



    private:

        Servo servoVar;
        int servoPin;
        int servoLagCounter = 0;
        int servoLagCounterMax;

        int flexPin;
        int flexValue;
        int flexCounter = 0;
        double flexAverage;
        int flexArray[10];

        int[4] mapValues;
        int angleValue;
        int loopCounter = 0;
        int angleDifference;
        int lastAngleValue = 0;

        void updateServo() {
            
            angleValue = map(flexValue, mapValues[0], mapValues[1], mapValues[2], mapValues[3]);

            Serial.println(flexValue);
            Serial.println(angleValue);
            Serial.println();

            servoVar.write(angleValue);

            calculateServoLag(angleValue);
        }



        // function to determine how long the class should wait before sending a new value to the servo
        void calculateServoLag(int newAngleValue) {

            angleDifference = abs(lastAngleValue - newAngleValue);

            ServoLagCounterMax = ceil(angleDifference);

            // servo takes 1800ms to turn 180 degrees
            // servo takes 10ms to turn 1 degree
            // servo turns 1 degree per void loop
        }

        
  
}



void setup() {

    Serial.begin(9600);

    const HingeJoint fingerJoint0 = Joint(A0, A8, [20, 100, 0, 175]);
    const HingeJoint fingerJoint1 = Joint(A1, A9, [20, 100, 0, 175]);
    const HingeJoint fingerJoint2 = Joint(A2, A10, [20, 100, 0, 175]);
    const HingeJoint fingerJoint3 = Joint(A3, A11, [20, 100, 0, 175]);
    const HingeJoint wristJoint = Joint(A4, A12, []);
    const HingeJoint elbowJoint = Joint(A5, A13, []);
    const Joint shoulderJoint = Joint(A6, A14, []);
}



void loop() {

    fingerJoint0.takeFlexInput();
    fingerJoint1.takeFlexInput();
    fingerJoint2.takeFlexInput();
    fingerJoint3.takeFlexInput();
    wristJoint.takeFlexInput();
    elbowJoint.takeFlexInput();

    delay(10);
}