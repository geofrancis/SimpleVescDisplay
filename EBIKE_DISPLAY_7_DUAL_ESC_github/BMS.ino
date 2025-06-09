

void BMS() {

  bms.main_task(true);

  Serial.println("***********************************************");
  Serial.print("State of charge:\t");
  Serial.print(bms.get_state_of_charge());
  Serial.println("\t% ");
  Serial.print("Current:\t\t");
  Serial.print(bms.get_current());
  Serial.println("\tA  ");
  Serial.print("Voltage:\t\t");
  Serial.print(bms.get_voltage());
  Serial.println("\tV  ");

BMSvoltage = bms.get_voltage();

  for (uint8_t i = 0; i < bms.get_num_cells(); i++) {
    Serial.print((String) "Cell " + (i + 1) + " -\t\t");
    Serial.print(bms.get_cell_voltage(i), 3);
    Serial.print("\tV\t");
    Serial.println(bms.get_balance_status(i) ? "(balancing)" : "(not balancing)");
    CELLS[i] = bms.get_cell_voltage(i);
  }
  //      bms.query_0x03_basic_info();

  //
  Serial.print("Balance capacity:\t");
  Serial.print(bms.get_balance_capacity());
  Serial.println("\tAh  ");
  Serial.print("Rate capacity:\t\t");
  Serial.print(bms.get_rate_capacity());
  Serial.println("\tAh  ");


  for (uint8_t i = 0; i < bms.get_num_ntcs(); i++) {
    Serial.print((String) "Termometer " + (i + 1) + " -\t\t");
    Serial.print(bms.get_ntc_temperature(i));
    Serial.println("\tdeg.\t");
  }
  Serial.print((String) "Charge mosfet" + " -\t\t");
  bms.get_charge_mosfet_status() ? Serial.print("Enabled") : Serial.print("Disabled");
  Serial.println("");
  Serial.print((String) "Discharge mosfet" + " -\t");
  bms.get_discharge_mosfet_status() ? Serial.print("Enabled") : Serial.print("Disabled");
  Serial.println("");

  Serial.print((String) "Cycle count" + " -\t\t");
  Serial.print(bms.get_cycle_count());
  Serial.println("");
  Serial.print((String) "protection_status" + " -\t");
  Serial.print(bms.get_protection_status_summary());
  Serial.println("");
  Serial.print((String) "fault_count" + " -\t\t");
  Serial.print(bms.get_fault_count());
  Serial.println("");
  Serial.print((String) "get_bms_name" + " -\t\t");
  Serial.print(bms.get_bms_name());
  Serial.println("");

  // Serial.println(ESP.getFreeHeap());
  //
  //    bms.set_0xE1_mosfet_control(1,1);
  //    bms.set_0xE1_mosfet_control_charge(1);
  //    bms.set_0xE1_mosfet_control_discharge(1);
}
