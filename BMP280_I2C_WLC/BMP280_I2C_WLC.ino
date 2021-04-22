
#include <Arduino.h>
#include <Wire.h>
#include <BMx280I2C.h>

#define I2C_ADDRESS 0x76
BMx280I2C bmx280(I2C_ADDRESS);
const int GND = 17;

void setup() 
{
  // put your setup code here, to run once:
	Serial.begin(9600);

  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  delay(100);

	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);

	Wire.begin();

	//begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
	//and reads compensation parameters.
	if (!bmx280.begin())
	{
		Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
		while (1);
	}

	if (bmx280.isBME280())
		Serial.println("sensor is a BME280");
	else
		Serial.println("sensor is a BMP280");

	//reset sensor to default parameters.
	bmx280.resetToDefaults();

	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
		bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
}

void loop() 
{
  // put your main code here, to run repeatedly:

	delay(1000);

	//start a measurement
	if (!bmx280.measure())
	{
		Serial.println("could not start measurement, is a measurement already running?");
		return;
	}

	//wait for the measurement to finish
	do
	{
		delay(100);
	} while (!bmx280.hasValue());

	Serial.print("P: "); 
	Serial.print(bmx280.getPressure()/10,0);
	//Serial.print("Pressure (64 bit): "); Serial.println(bmx280.getPressure64());
	Serial.print(", T: "); Serial.println(bmx280.getTemperature());

}
