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
            break;
        case EVENT_HAVE_LEAVE_CLIENT:
            connections--;
            break;
    }

    std::cout << "ProcessingInter: happened the EVENT: " << event.getEventID() << std::endl;
}

/**Here constructor is a constructor of a class Processing
 * Here we create some objects for I2Driver and for MPU6050 Driver
 * for initialization and reading values from devices
 * ~>public constructor*/
ProcessingInter::ProcessingInter(MyI2CDriveMethods& driveMethods) {
    temperature = 0.0;
    connections = 0;
    mpu6050_drive = new MPU6050_Drive(driveMethods.i2c_init, driveMethods.i2c_read_mpu, driveMethods.i2c_write_mpu);
    mpu6050_drive->init_mpu6050();

    bme280_drive = new BME280("dev/i2c-1/", BME280_ADDRESS, driveMethods.i2c_init, driveMethods.i2c_read_bme, driveMethods.i2c_write_bme);
    bme280_drive->defaultInit();

    ssd1306_drive = new SSD1306_Drive(driveMethods.i2c_init, driveMethods.i2c_read_ssd, driveMethods.i2c_write_ssd);
    ssd1306_drive->ssd1306_basic_init();
}

/**This function activate server thread and threads for different devices
 * ~>public function*/
void ProcessingInter::start() {
    DelegateWS* delegate = DelegateWS::getInstance();   //get instance of delegate
    delegate->addHandler((HandlerWS&) *this);       //add a new handler

    init_server();      //it starts web_socket server
    sleep(1);
    bme280data = bme280_drive->getBMP280Data();
    mpu6050Data = mpu6050_drive->getActualData();
    temperature = bme280data->getTemperature();
    while (1) {
        if (connections > 0) {
            std::string values = to_json_process();
            this->processing_data_to_websocket(values);
        }
        show_on_display();
        sleep(1);

        bme280data = bme280_drive->getBMP280Data();
        mpu6050Data = mpu6050_drive->getActualData();
        temperature = bme280data->getTemperature();
    }
}

/**This function configures a web-server and starts a server-thread
 * ~>private function*/
void ProcessingInter::init_server(){
    this->server_thread = thread([this]() {
        logger = std::make_shared<PrintfLogger>();
        ws_server = make_shared<MyServer>(logger);
        handler = std::make_shared<MyHandler>(ws_server.get());
        ws_server->addPageHandler(std::make_shared<MyAuthHandler>());
        ws_server->addWebSocketHandler("/chart", handler);
        ws_server->serve("src/server_files", 9090);
    });
    //this->server_thread.join();
}

/**This function processes data from different devices and sends it to server
 * ~>private function*/
void ProcessingInter::processing_data_to_websocket(std::string values) {
    handler->sendValuesJSON(values);
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


/**this function print on display current count of connections and current count of
* ~>private function*/
void ProcessingInter::show_on_display() {
    char printed_str[80];
    sprintf(printed_str, "Current tmp: %0.2f*C\nConnections: %d", temperature, connections);
    ssd1306_drive->ssd1306_clearDisplay();
    ssd1306_drive->ssd1306_drawString(printed_str);
    ssd1306_drive->ssd1306_display();
}

/**this function is used for updating "temperature" value of class ProcessingInter,
 * which used for showing on connected display
 * ~>private function*/
void ProcessingInter::read_temperature() {
    temperature = bme280data->getTemperature();
}
