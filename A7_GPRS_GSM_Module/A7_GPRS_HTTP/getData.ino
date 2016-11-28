void getData(Data* data){
	Serial.println(F("Hello I am getting your DATA!!! :)"));
	uint8_t temperature = analogRead(pinTemperature);
	uint8_t ph = analogRead(pinPH);
	uint8_t DO = analogRead(pinDO);
	uint8_t turbidity = analogRead(pinTurbidity);

	randomSeed(analogRead(0));
	// data->temperature = 30.34;
	// data->pH = 7.0;
	// data->DO = 5.01;
	// data->turbidity = 50;	
	data->temperature = random(0, 30);
	data->pH = random(0, 14);	
	data->DO = random(0, 100);
	data->turbidity = random(0, 1000);
}

void printData(Data* data){
	Serial.print(F("Temperature is: "));
	Serial.println(data->temperature);
	Serial.print(F("pH is: "));
	Serial.println(data->pH);
	Serial.print(F("DO is: "));
	Serial.println(data->DO);
	Serial.print(F("Turbidity is: "));
	Serial.println(data->turbidity);
}