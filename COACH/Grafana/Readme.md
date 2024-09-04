## Top used Influx Queries

### Offline times over selected time period:
``` import "strings"
from(bucket: "ce_bucket")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer")
  |> filter(fn: (r) => r["moorfields-environment"] =~ /PCS/)
  |> keep(columns: ["_time", "moorfields-environment"])
  |> elapsed(unit: 1m)
  |> group(columns: ["moorfields-environment"])
  |> sum(column: "elapsed") 
  ```

### Occupancy during work hours (includes rounding for one digit after the floating point):
``` import "math"
import "date"
import "regexp"

from(bucket: "ce_bucket")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer")
  |> filter(fn: (r) => r["moorfields-environment"] =~ /PCS/)
  |> filter(fn: (r) => {
      t = uint(v: date.hour(t: r._time))
      return t >= 8 and t < 18
  })
  |> toFloat()
  |> filter(fn: (r) => r["_value"] == 1.0)  
  |> aggregateWindow(every: 1m, fn: last, createEmpty: false)
  |> sum()
  |> map(fn: (r) => {
      // Calculate integer part & convert to hours
      integer_part = math.trunc(x: r._value / 60.0)

      // Calculate decimal part
      decimal_part = r._value / 60.0 - integer_part

      // Round decimal part to one decimal place
      rounded_decimal = math.round(x: decimal_part * 10.0) / 10.0

      // Combine integer and rounded decimal parts
      combined_value = float(v: integer_part) + rounded_decimal
      
      //Converted to string for Grafana canvas:
      combined_value_str = string(v: combined_value)
      
      // Return the result with the combined value //old: r.value
      return { r with _value: combined_value_str }
  })
```