// +--------------+
// | DHT11 sensor |
// +--------------+
//DHT dht( PIN_DHT11, DHT11 );
//float dht_temp, dht_humid, dht_temp_old, dht_humid_old, DHT_TEMP_ADD = -2.5f;
//float sens_light;

// +----------------------+
// | DST - HC-SR04 sensor |
// +----------------------+
//#define DST_MEM_CNT 4
//void loop_dst_meter(); // linear distance meter
//float dst, dst_mem[ DST_MEM_CNT ];
//const char dst_Unit[] PROGMEM = { "[cm]" };
//float dst_in_us = 0.03438f; // v_wave/100000 [cm], 20C
//
//void int_echo();
//unsigned long ul_dst_time_us = 0, us_max_time = 58823;
//bool b_dst_measuring = false;






// +------------------------+
// | Spirit level - MPU6050 |
// +------------------------+
// #define ADR_MPU6050 0x68
// int16_t gyro_x, gyro_y, gyro_z;
// int16_t acc_x, acc_y, acc_z;
// int16_t i2_MPU_rot_x, i2_MPU_rot_y, i2_MPU_rot_z;
// int16_t i2_circ_x = 97, i2_circ_y = 33, i2_dot_x, i2_dot_y, i2_max_dot_r = 21;
// void MPU_getData();
// void loop_spirit_level();
// int8_t i1_rot_x, i1_rot_y;






// +---------------------+
// | IR Receiver VS1838B |
// +---------------------+
//IRrecv irrecv(PIN_IR_RECV);
//decode_results IRresults;
