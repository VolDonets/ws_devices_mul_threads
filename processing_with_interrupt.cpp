//
// Created by vovan on 30.03.2020.
//

#include "processing_with_interrupt.h"

/**override function for working event handling
 * ~> public function override event_ws.h/EventWS::handleEventWS(EventWS&)*/
void ProcessingInter::handleEventWS(EventWS &event) {
    switch (event.getEventID()) {
        case EVENT_HAVE_CLIENT:
            connections++;
            myWebServerWorker->enableSending();
            break;
        case EVENT_HAVE_LEAVE_CLIENT:
            connections--;
            if (connections == 0)
                myWebServerWorker->disableSending();
            break;
        case EVENT_GIVE_MQTT_STATUS:
            if (isWorkingMQTT)
                myWebServerWorker->setUrgentMessage("MQTT_STATUS_ENABLE");
            else
                myWebServerWorker->setUrgentMessage("MQTT_STATUS_DISABLE");
            break;
        case EVENT_MQTT_CHANGE_STATUS:
            if (isWorkingMQTT) {
                myWebServerWorker->setUrgentMessage("MQTT_STATUS_DISABLE");
                disable_MQTT_server();
            } else {
                myWebServerWorker->setUrgentMessage("MQTT_STATUS_ENABLE");
                enable_MQTT_server();
            }
            break;
    }

    //std::cout << "ProcessingInter: happened the EVENT: " << event.getEventID() << std::endl;
}

/**Here constructor is a constructor of a class Processing
 * Here we create some objects for I2Driver and for MPU6050 Driver
 * for initialization and reading values from devices
 * ~>public constructor*/
ProcessingInter::ProcessingInter(MyI2CDriveMethods& driveMethods) {
    temperature = 0.0;
    connections = 0;
    isEnabledMQTT = false;
    isWorkingMQTT = false;
    mpu6050_drive = std::make_shared<MPU6050_Drive>(driveMethods.i2c_init, driveMethods.i2c_read_mpu, driveMethods.i2c_write_mpu);
    mpu6050_drive->init_mpu6050();

    bme280_drive = std::make_shared<BME280>("dev/i2c-1/", BME280_ADDRESS, driveMethods.i2c_init, driveMethods.i2c_read_bme, driveMethods.i2c_write_bme);
    bme280_drive->defaultInit();

    ssd1306_drive = std::make_shared<SSD1306_Drive>(driveMethods.i2c_init, driveMethods.i2c_read_ssd, driveMethods.i2c_write_ssd);
    ssd1306_drive->ssd1306_basic_init();

    myWebServerWorker = std::make_shared<MyWebServerWorker>();
    myMQTTWorker = std::make_shared<MyMQTTWorker>("62.113.118.12:1883", "", "drNamanaGraph", 0);
    sleep(1);
    DelegateWS* delegate = DelegateWS::getInstance();   //get instance of delegate
    delegate->addHandler((HandlerWS&) *this);       //add a new handler
}

/**This function activate server thread and threads for different devices
 * ~>public function*/
void ProcessingInter::start() {

    sleep(1);
    while (1) {
        bme280data = bme280_drive->getBMP280Data();
        mpu6050Data = mpu6050_drive->getActualData();
        temperature = bme280data->getTemperature();
        std::string values = to_json_process();
        if (connections > 0)
            this->myWebServerWorker->addJSONData(values);
        if (isEnabledMQTT)
            this->myMQTTWorker->addJSONData(values);
        show_on_display();
        sleep(1);
    }
}

/**this function get data from connected devices and convert it to JSON for sending this to server and clients*
 * ~>private*/
std::string ProcessingInter::to_json_process() {
    std::string result = "{\"accelX\": " + std::to_string(mpu6050Data->accelX)
                         + ", \"accelY\": " + std::to_string(mpu6050Data->accelY)
                         + ", \"accelZ\": " + std::to_string(mpu6050Data->accelZ)
                         + ", \"gyroX\": " + std::to_string(mpu6050Data->gyroX)
                         + ", \"gyroY\": " + std::to_string(mpu6050Data->gyroY)
                         + ", \"gyroZ\": " + std::to_string(mpu6050Data->gyroZ)
                         + ", \"temperature\": " + std::to_string(bme280data->getTemperature())
                         + ", \"pressure\": " + std::to_string(bme280data->getPressure())
                         + ", \"humidity\": " + std::to_string(bme280data->getHumidity())
                         + ", \"altitude\": " + std::to_string(bme280data->getAltitude())
                         + "}";

    return result;
}


/**this function print on display
 * current count of connections
 * current temperature
 * current MQTT_status
* ~>private function*/
void ProcessingInter::show_on_display() {
    char printed_str[80];
    sprintf(printed_str, "Current tmp: %0.2f*C\nConnections: %d\nMQTT_status: %s", temperature, connections,
            isWorkingMQTT ? "ENABLED" : "DISABLED");
    ssd1306_drive->ssd1306_clearDisplay();
    ssd1306_drive->ssd1306_drawString(printed_str);
    ssd1306_drive->ssd1306_display();
}

/**this function is used for enabling MQTT_server and starting MQTT_server thread*/
void ProcessingInter::enable_MQTT_server(){
    if (!isEnabledMQTT){
        isEnabledMQTT = true;
        myMQTTWorker->enableMQTTServer();
    }
    isWorkingMQTT = true;
    myMQTTWorker->enableSendingData();
}

/**this function is used for disabling MQTT_server and to stop MQTT_server thread*/
void ProcessingInter::disable_MQTT_server(){
    if (isEnabledMQTT){
        isWorkingMQTT = false;
        myMQTTWorker->disableSendingData();
    }
}
