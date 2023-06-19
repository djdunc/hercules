# COACH - Development notes

Notes on work in progress developing the COACH arduino scripts.

(note: listed in reverse chronology)


-----

19th June
created coach v3 based on feedback from DFRobot re I2C clash. Have used:

```
DFRobot_B_LUX_V30B    myLux(13,7,8);
```

and moved the lux sensor to pins 7 / 8 and moved the mmWave sensor to pin 5.

-----

19th June

TIG setup - updated telegraf with following script:

```
  # -----  Moorfields Environment -----------------------------------
[[inputs.mqtt_consumer]]
   servers = ["tcp://mqtt.cetools.org:1883"]

  ## Topics that will be subscribed to.
  topics = [
    "personal/ucjtdjw/moorfields/#"
  ]
  
  client_id = "ce-mqtt-telegraf-moorfields"
  topic_tag = "moorfields-environment"
  data_format = "value"
  data_type = "string"
  ```

Created a grafana dashboard to monitor readings during testing (behind CE login):

https://grafana.cetools.org/d/QvxxMcX4z/moorfields-bx-raw-environment-data?orgId=1&from=now-1h&to=now&refresh=1m

![Grafana screenshot](images/grafana-day1.png)

Influx query:
```
from(bucket: "ce_bucket")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer")
  |> filter(fn: (r) => r["moorfields-environment"] =~ /CDS/)
  |> toFloat()  
  |> aggregateWindow(every: v.windowPeriod, fn: last, createEmpty: false)
  ```

  repeated for LPS, PCS, SLS, TPS

To get formatted displayname (for when mulitple sensors will be included) used the following REGEX:

In Transform Tab:

Select "Rename by Regex"

Match: `.*moorfields\/(.*)\"}$`

Replace: `$1`

----

16th June

Setup coach v1 to test sensor with MKR1010

 * Light Sensor code based on sample at https://github.com/DFRobot/DFRobot_B_LUX_V30B 
 * DFRobot Gravity: Analog Sound Level Meter https://wiki.dfrobot.com/Gravity__Analog_Sound_Level_Meter_SKU_SEN0232
 * DFRobot Gravity: CO2 board - for 3.3v boards and v1.x sensors see: https://www.dfrobot.com/forum/topic/315493 
 * mmWave - using high low putput from GPIO2

Connected as follows:

 * GND and 5V to all sensors plus:
 * light - SCL and SDA (0x4A)
 * sound - A1
 * CO2 - A2
 * mmWave - GPIO2 on sensor to D6 on MKR1010
 * TMP36 on A0 

![Proto 1 connections](images/proto1.jpeg)

When connected the HDC1080 as a second I2C device was getting errors. Tried multiple HDC devices and multiple HDC libraries. All worked individually but not with the DFRobot light sensor. After a couple of hours of testing posted query to their wiki to see if others have suggestions. 

https://www.dfrobot.com/forum/topic/321033

Setup coach v2 to test wifi connectivity with all sensors.