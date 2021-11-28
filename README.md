**************** ESP-32 SD-Card & Sensors Interface ****************


*********************************************************************
* Dependancy Libraries
*********************************************************************
WiFi<br />
SD<br />
FS<br />
BH1750<br />
ESP32Time<br />
EspMQTTClient<br />
i2cdetect<br />

*********************************************************************
* Pin Connection
*********************************************************************
* ESP-32 -> SD-Card (SPI)<br />
GPIO5 -- CS <br />
GPIO18 -- SCK<br />
GPIO23 -- MOSI<br />
GPIO19 -- MISO<br />
3V3 -- VCC  (3V3)<br />
GND -- GND<br />
* ESP-32 -> PIR<br />
GPIO34 -- PIR1<br />
GPIO35 -- PIR2<br />
GPIO32 -- PIR3<br />
* ESP-32 -> BH1750 (I2C)<br />
GPIO16 (U2_TXD) -- SDA<br />
GPIO17 (U2_RXD) -- SCL<br />


*********************************************************************
* Test Function
*********************************************************************
To remove the test mode do the following changes<br />
1. To update the file creation time on daily basis.<br />
	* uncomment following line in date_time.cpp <br />
		if((dateTime_now.tm_hour == 0) && (dateTime_now.tm_min == 0) && (dateTime_now.tm_sec == 0))<br />
	* remove or comment next line <br />
		// if(dateTime_now.tm_min == minut)	<br />
2. To get the file with its name as per the file creation date.<br />
	* uncomment following line in sd_card.cpp <br />
		snprintf (file, FILE_PATH_LEN, "%s%c%s_%04d_%02d_%02d.csv", directory, '/', device_id, dateTime_sample.tm_year, dateTime_sample.tm_mon, dateTime_sample.tm_mday);<br />
	* remove or comment next line <br />
		// snprintf (file, FILE_PATH_LEN, "%s%c%s_%04d_%02d_%02d.csv", directory, '/', device_id, dateTime_sample.tm_year, dateTime_sample.tm_mon, dateTime_sample.tm_min);<br />
3. sd card configurations are set in to the sd_config file.
	* following apis/ functions will used to get the sd card configurations.
		uint16_t sd_config_get_no_of_entries(void);
		uint16_t sd_config_get_no_of_files(void);
		uint32_t sd_config_get_duration(void);
	