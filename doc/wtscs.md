# Web Time Series Classification Service

Web Time Series Classification Service, or WTSCS for short, is a web service for classification of time series data from remote sensing imagery.

In WTSCS a coverage is a three dimensional array with spatial and temporal dimensions.

WTSCS is based on the following operations:
* ```list_algorithms```: ???.
* ```classify```: ???.
* ```status```: ???.


## ```list_algorithms```

Via HTTP Get:
```
http://myserver/wtscs/list_algorithms
```

Response:
```json
{
  "algorithms": [ "TWDTW_R", "TWDTW_SCIDB" ]
}
```


## ```classify```

Via HTTP Post:
```json
{
    "algorithm": "TWDTW_R",
    "input_parameters": {
        "coverage": "mod13q1",
        "attributes": ["ndvi"],
        "roi": {
            "geometry": {
                "type": "Polygon",
                "coordinates": [
                    [ [-10.0, -10.0], [10.0, -10.0], [10.0, 10.0], [-10.0, -10.0] ]
                ]
            }
        },
        "patterns": ...,
    }
}
```

Resposta:
```json
{ "status_code": "0AFB88990AFB88990AFB88990AFB8899" }
```


## ```status```
