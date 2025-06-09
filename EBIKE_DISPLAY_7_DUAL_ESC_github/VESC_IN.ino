

void VESCR() {

  UART.getVescValues();
  tacho = (UART.data.tachometerAbs / (MOTOR_POLES * 3));
  rpm = (UART.data.rpm / (MOTOR_POLES / 2));
  trip = tacho / 1000;
  wheel_diameter = (PI * WHEEL_DIAMETER_MM / 1000);
  speed = ((rpm * wheel_diameter * GEAR_RAITO) / 1000) * 60;

  calvolts = 0.681 * UART.data.inpVoltage;
  watts = calvolts * UART.data.avgInputCurrent;
  CELLc = calvolts / 13;

  current = (UART.data.avgMotorCurrent);
  amps = (UART.data.avgInputCurrent);

  mottemp = (UART.data.tempMosfet);
  esctemp = (UART.data.tempMotor);


  Serial.print("rear esc cal");
  Serial.println(calvolts);

  Serial.print("rear esc rpm");
  Serial.println(rpm);
}
// 89 front  62 rear



void VESCF() {

  UART.getVescValues(89);
  tacho1 = (UART.data.tachometerAbs / (MOTOR_POLES * 3));
  rpm1 = (UART.data.rpm / (MOTOR_POLES / 2));
  trip1 = tacho / 1000;
  wheel_diameter1 = (PI * WHEEL_DIAMETER_MM / 1000);
  speed1 = ((rpm * wheel_diameter * GEAR_RAITO) / 1000) * 60;

  calvolts1 = 0.681 * UART.data.inpVoltage;
  watts1 = calvolts * UART.data.avgInputCurrent;
  Serial.println(CELLc);

  current1 = (UART.data.avgMotorCurrent);
  amps1 = (UART.data.avgInputCurrent);

  mottemp1 = (UART.data.tempMosfet);
  esctemp1 = (UART.data.tempMotor);

  Serial.print("calvolts front");
  Serial.println(calvolts1);

  Serial.print("==============------------rpm1");
  Serial.println(rpm1);
}
// 89 front  62 rear