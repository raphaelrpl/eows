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
	"algorithm": "TWDTW",
	"input_parameters": {
		"coverage": "mod13q1_512",
		"scale_factor": 0.0001,
		"bands": ["ndvi", "evi", "nir"],
		"roi": {
			"bbox": [60120, 48622, 60564, 49049]
		},
		"patterns": [{
			"patterns": "patterns"
		}],
		"dist.method": "euclidean",
		"alpha": -0.1,
		"beta": 100,
		"theta": 0.5,
		"interval": "12 month",
		"span": 250,
		"keep": "FALSE",
		"overlap": 0.5,
		"start_date": "2000-09-01",
		"end_date": "2017-08-31"
	}
}
```

Resposta:
```json
{ "status_code": "0AFB88990AFB88990AFB88990AFB8899" }
```


## ```status```
