# Web Time Series Processing Service

Web Time Series Processing Service, or WTSPS for short, is a web service for processing of time series data from remote sensing imagery.

In WTSPS a coverage is a three dimensional array with spatial and temporal dimensions.

WTSPS is based on the following operations:
* ```list_algorithms```: ???.
* ```run_process```: ???.
* ```cancel_process```: ???.
* ```status```: ???.
* ```describe_algorithm```: ???.


## ```list_algorithms```

Via HTTP Get:
```
http://myserver/wtscs/list_algorithms
```

Response:
```json
{
  "algorithms": [ "TWDTW", "TWDTW_SCIDB" ]
}
```


## ```run_process```

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

Response:
```json
{ "UUID": "WTSPSde9c687b4699465780a59a50e7e0457b" }
```

patterns:
```json
[{
	"longitude": 0,
	"latitude": 0,
	"start_date": "2000-09-13",
	"end_date": "2001-08-23",
	"label": "Cerrado",
	"coverage": "mod13q1_512",
	"time_series": [{
		"Index": "2000-09-13",
		"ndvi": 0.4635,
		"evi": 0.2374,
		"nir": 0.2011
	},...
  ]
}, {
	"longitude": 0,
	"latitude": 0,
	"start_date": "2003-09-14",
	"end_date": "2004-08-28",
	"label": "Clear_cut1",
	"coverage": "mod13q1_512",
	"time_series": [{
		"Index": "2003-09-14",
		"evi": 0.4648,
		"ndvi": 0.7647,
		"nir": 0.2868
	},...
  ],
	"original_label": "",
	"n_members": 1
},
 ... 
]
```

## ```cancel_process```

Via HTTP Get:
```
http://myserver/wtscs/cancel_process?UUID=WTSPSde9c687b4699465780a59a50e7e0457b
```

Response:
```json
{ "cancelled": "TRUE" or "FALSE" }
```


## ```status```

Via HTTP Get:
```
http://myserver/wtscs/status?UUID=0AFB88990AFB88990AFB88990AFB8899
```

Response:
```json
{ "status": "None" or "Scheduled" or "In progress" or "Completed" or "Cancelled" }
```


## ```describe_algorithm```

Via HTTP Get:
```
http://myserver/wtscs/describe_algorithm?algorithm=TWDTW
```

Response:
```json
{  }
```
