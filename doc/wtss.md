# Web Time Series Service

**Web Time Series Service**, or **WTSS** for short, is a lightweight web service for handling time series data from remote sensing imagery.

In WTSS a coverage is a three dimensional array with spatial and temporal dimensions.

WTSS is based on three operations:
- **```list_coverages```:** returns the list of all available coverages in the service.
- **```describe_coverage```:** returns the metadata of a given coverage.
- **```time_series```:** query the database for the list of values for a given location and time interval.

The ```list_coverages``` operation can be used as follow:
```
http://myserver/wtss/list_coverages
```

It will return a JSON document such as:
```
{
  "coverages": [
    "chronos:modis:mod09q1",
    "chronos:modis:mod13q1"
  ]
}
```

If you need the metadata of a given coverage you can use the ```describe_coverage``` operation as follow:
```
http://myserver/wtss/describe_coverage?name=mod09q1
```

The result of ```describe_coverage``` operation is a JSON document such as:
```
{
    "name": "mod09q1",
    "cluster_id": "chronos:modis",
    "description": "Surface Reflectance 8-Day L3 Global 250m",
    "detail": "https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mod09q1",
    "dimensions": {
        "x": {
            "name": "col_id",
            "min_idx": 0,
            "max_idx": 172799
        },
        "y": {
            "name": "row_id",
            "min_idx": 0,
            "max_idx": 86399
        },
        "t": {
            "name": "time_id",
            "min_idx": 0,
            "max_idx": 3
        }
    },
    "attributes": [{
        "name": "red",
        "description": "250m Surface Reflectance Band 1 (620–670 nm)",
        "datatype": "int16",
        "valid_range": {
            "min": -100,
            "max": 16000
        },
        "scale_factor": 0.0001,
        "missing_value": -28672
    }, {
        "name": "nir",
        "description": "250m Surface Reflectance Band 2 (841–876 nm)",
        "datatype": "int16",
        "valid_range": {
            "min": -100,
            "max": 16000
        },
        "scale_factor": 0.0001,
        "missing_value": -28672
    }, {
        "name": "quality",
        "description": "250m Reflectance Band Quality",
        "datatype": "uint16",
        "valid_range": {
            "min": 0,
            "max": 32767
        },
        "scale_factor": 1,
        "missing_value": 65535
    }],
    "spatial_extent": {
        "xmin": -180.0,
        "ymin": -90.0,
        "xmax": 180.0,
        "ymax": 90.0
    },
    "spatial_resolution": {
        "x": 0.00208334,
        "y": 0.00208334
    },
    "srid": 4326,
    "time_line": ["2001-01-01", "2001-01-17", "2001-02-02", "2002-02-18"]
}
```

You can retrieve the time series for a given location through the  ```time_series```:
```
http://myserver/wtss/time_series?coverage=mod09q1&attributes=red,nir&longitude=-54.0&latitude=-5.0&start_date=2000-02-18&end_date=2000-03-21
```
The result of ```time_series``` is a JSON document such as:
```
{
  "result": {
    "attributes": [
      {
        "attribute": "red",
        "values": [ 3726, 2834, 4886, 231, 1264 ]
      },
      {
        "attribute": "nir",
        "values": [ 4646, 4580, 5443, 3955, 2987 ]
      }
    ],
    "timeline": [ "2000-02-18", "2000-02-26", "2000-03-05", "2000-03-13", "2000-03-21" ],
    "center_coordinates": {
      "latitude": -4.9989583328814176,
      "longitude": -54.000193143463676
    }
  },
  "query": {
    "coverage": "mod09q1",
    "attributes": [ "red", "nir" ],
    "latitude": ​-5,
    "longitude": ​-54
  }
}
```

