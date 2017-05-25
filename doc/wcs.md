# Web Coverage Service 2.0

Web Coverage Service, or WCS for short, is a web service for retrieving multidimensional array database as coverage.

As WCS 2.0 standard has been develop by OGC, it is based on the following operations:
* ```GetCapabilities```: For retrieving service metadata and available coverages;
* ```DescribeCoverage```: For retrieving coverage metadata like spatio temporal limits, formats supported and attributes;
* ```GetCoverage```: For retrieving coverage.


## ```GetCapabilities```

Via HTTP Get:
```
http://myserver/wcs?service=WCS&version=2.0.1&request=GetCapabilities
```

Response:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<wcs:Capabilities version="2.0.1" xmlns="http://www.opengis.net/ows/2.0" xmlns:ows="http://www.opengis.net/ows/2.0" xmlns:wcs="http://www.opengis.net/wcs/2.0" xmlns:gml="http://www.opengis.net/gml/3.2" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsAll.xsd">
	<ows:ServiceIdentification>
		<ows:Title>e-Sensing Web Coverage Service</ows:Title>
		<ows:Abstract>Web Coverage Service maintained by the e-Sensing team</ows:Abstract>
		<ows:ServiceType>OGC WCS</ows:ServiceType>
		<ows:ServiceTypeVersion>2.0.1</ows:ServiceTypeVersion>
	</ows:ServiceIdentification>
	<ows:ServiceProvider>
		<ows:ProviderName>National Institute for Spatial Research</ows:ProviderName>
		<ows:ProviderSite>http://www.dpi.inpe.br</ows:ProviderSite>
		<ows:ServiceContact>
			<ows:IndividualName>Gilberto Queiroz</ows:IndividualName>
			<ows:PositionName>Computer Scientist</ows:PositionName>
		</ows:ServiceContact>
	</ows:ServiceProvider>
	<ows:OperationsMetadata>
		<ows:Operation name="GetCapabilities">
			<ows:DCP>
				<ows:HTTP>
					<ows:Get xlink:href="http://localhost:7654/wcs"/>
				</ows:HTTP>
			</ows:DCP>
		</ows:Operation>
		<ows:Operation name="DescribeCoverage">
			<ows:DCP>
				<ows:HTTP>
					<ows:Get xlink:href="http://localhost:7654/wcs"/>
				</ows:HTTP>
			</ows:DCP>
		</ows:Operation>
		<ows:Operation name="GetCoverage">
			<ows:DCP>
				<ows:HTTP>
					<ows:Get xlink:href="http://localhost:7654/wcs"/>
				</ows:HTTP>
			</ows:DCP>
		</ows:Operation>
	</ows:OperationsMetadata>
	<wcs:ServiceMetadata>
		<wcs:formatSupported>application/gml+xml</wcs:formatSupported>
	</wcs:ServiceMetadata>
	<wcs:Contents>
		<wcs:CoverageSummary>
			<wcs:CoverageId>landsat</wcs:CoverageId>
			<wcs:CoverageSubtype>GridCoverage</wcs:CoverageSubtype>
		</wcs:CoverageSummary>
		<wcs:CoverageSummary>
			<wcs:CoverageId>mod09q1</wcs:CoverageId>
			<wcs:CoverageSubtype>GridCoverage</wcs:CoverageSubtype>
		</wcs:CoverageSummary>
		<wcs:CoverageSummary>
			<wcs:CoverageId>mod13q1</wcs:CoverageId>
			<wcs:CoverageSubtype>GridCoverage</wcs:CoverageSubtype>
		</wcs:CoverageSummary>
	</wcs:Contents>
</wcs:Capabilities>
```


## ```DescribeCoverage```

Via HTTP GET:
```
http://myserver/wcs?service=WCS&version=2.0.1&request=DescribeCoverage&coverageID=mod13q1
```

Response:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<wcs:CoverageDescriptions xmlns:wcs="http://www.opengis.net/wcs/2.0" xmlns:ows="http://www.opengis.net/ows/2.0" xmlns:gml="http://www.opengis.net/gml/3.2" xmlns:gmlcov="http://www.opengis.net/gmlcov/1.0" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:swe="http://www.opengis.net/swe/2.0" xsi:schemaLocation=" http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsDescribeCoverage.xsd">
	<wcs:CoverageDescription gml:id="mod13q1">
		<gml:boundedBy>
			<gml:Envelope srsName="http://www.opengis.net/def/crs/EPSG/0/4326" axisLabels="col_id row_id" srsDimension="3">
				<gml:lowerCorner>-180.000000 -90.000000</gml:lowerCorner>
				<gml:upperCorner>180.000000 90.000000</gml:upperCorner>
			</gml:Envelope>
		</gml:boundedBy>
		<wcs:CoverageId>mod13q1</wcs:CoverageId>
		<gml:domainSet>
			<gml:Grid gml:id="mod13q1" dimension="3">
				<gml:limits>
					<gml:GridEnvelope>
						<gml:low>0 0 0</gml:low>
						<gml:high>35 17 3</gml:high>
					</gml:GridEnvelope>
					<gml:axisLabels>x y t</gml:axisLabels>
				</gml:limits>
			</gml:Grid>
		</gml:domainSet>
		<gmlcov:rangeType>
			<swe:DataRecord>
				<swe:field name="evi">
					<swe:Quantity>
						<swe:Description>250m 16 days EVI</swe:Description>
						<swe:uom code="evi"/>
						<swe:constraint>
							<swe:AllowedValues>
								<swe:interval>-2000.000000 10000.000000</swe:interval>
							</swe:AllowedValues>
						</swe:constraint>
					</swe:Quantity>
				</swe:field>
				<swe:field name="red">
					<swe:Quantity>
						<swe:Description>250m 16 days red reflectance (Band 1)</swe:Description>
						<swe:uom code="red"/>
						<swe:constraint>
							<swe:AllowedValues>
								<swe:interval>0.000000 10000.000000</swe:interval>
							</swe:AllowedValues>
						</swe:constraint>
					</swe:Quantity>
				</swe:field>
				<swe:field name="nir">
					<swe:Quantity>
						<swe:Description>250m 16 days NIR reflectance (Band 2)</swe:Description>
						<swe:uom code="nir"/>
						<swe:constraint>
							<swe:AllowedValues>
								<swe:interval>0.000000 10000.000000</swe:interval>
							</swe:AllowedValues>
						</swe:constraint>
					</swe:Quantity>
				</swe:field>
				<swe:field name="blue">
					<swe:Quantity>
						<swe:Description>250m 16 days blue reflectance (Band 3)</swe:Description>
						<swe:uom code="blue"/>
						<swe:constraint>
							<swe:AllowedValues>
								<swe:interval>0.000000 10000.000000</swe:interval>
							</swe:AllowedValues>
						</swe:constraint>
					</swe:Quantity>
				</swe:field>
				<swe:field name="reliability">
					<swe:Quantity>
						<swe:Description>250m 16 days pixel reliability summary QA</swe:Description>
						<swe:uom code="reliability"/>
						<swe:constraint>
							<swe:AllowedValues>
								<swe:interval>0.000000 3.000000</swe:interval>
							</swe:AllowedValues>
						</swe:constraint>
					</swe:Quantity>
				</swe:field>
			</swe:DataRecord>
		</gmlcov:rangeType>
		<wcs:ServiceParameters>
			<wcs:CoverageSubtype>GridCoverage</wcs:CoverageSubtype>
			<wcs:nativeFormat>image/tiff</wcs:nativeFormat>
		</wcs:ServiceParameters>
	</wcs:CoverageDescription>
</wcs:CoverageDescriptions>
```

## ```GetCoverage```

Via HTTP GET:
```
http://myserver/wcs?service=WCS&version=2.0.1&request=GetCoverage&coverageID=mod13q1
```

Response:
```xml
<gmlcov:GridCoverage version="2.0.1" xsi:schemaLocation="http://www.opengis.net/swe/2.0 http://schemas.opengis.net/sweCommon/2.0/swe.xsd http://www.opengis.net/gmlcov/1.0 http://schemas.opengis.net/gmlcov/1.0/gmlcovAll.xsd" gml:id="mod13q1"><gml:boundedBy><gml:Envelope srsName="http://www.opengis.net/def/crs/EPSG/0/4326" axisLabels="col_id row_id" srsDimension="3"><gml:lowerCorner>-180.000000 -90.000000</gml:lowerCorner><gml:upperCorner>180.000000 90.000000</gml:upperCorner></gml:Envelope></gml:boundedBy><gml:domainSet><gml:Grid gml:id="mod13q1" dimension="3"><gml:limits><gml:GridEnvelope><gml:low>0 0 0</gml:low><gml:high>35 17 3</gml:high></gml:GridEnvelope><gml:axisLabels>col_id row_id time_id</gml:axisLabels></gml:limits></gml:Grid></gml:domainSet><gml:rangeSet><gml:DataBlock><gml:tupleList ts="," cs=" ">3182 3000 91 1964 ,6008 4515 4919 441 ,721 3421 7273 3859 ,9117 5291 2751 3281 ,5879 3248 3066 8498 ,5290 5196 35 8607 ,1753 2113 3888 835 ,5023 7711 3270 4898 ,561 6297 9785 4546 ,2515 5583 5019 643 ,9286 1298 7638 9549 ,7086 6722 736 259 ,8595 2565 9762 3847 ,5359 5378 429 9486 ,4495 4355 1965 3218 ,9709 3469 7226 5063 ,39 9744 7190 5856 ,737 579 1971 7530 ,7691 863 7932 2986 ,4170 7099 9169 6091 ,6870 8266 2004 9608 ,1490 9069 1394 1105 ,6252 8564 696 386 ,4237 8525 2827 3771 ,7238 4492 1482 4511 ,6931 5855 6452 6034 ,5534 4287 3082 5625 ,6233 1969 2791 1152 ,2896 7787 1249 3137 ,7795 9534 2415 1065 ,2519 7031 2010 1937 ,5918 7267 4152 5954 ,1610 1592 7173 5600 ,1151 9023 4956 1887 ,6512 7978 7081 6297 ,8587 2004 557 9958 ,2431 6382 9990 748 ,7062 5310 4610 640 ,8094 8553 5213 4208 ,3879 8008 7903 4308 ,5035 8351 4710 9319 ,6087 8378 2204 5257 ,4636 679 6469 8179 ,8645 1486 6532 616 ,9557 7861 9386 6605 ,8929 9908 6215 8108 ,8364 3838 7283 368 ,5466 9669 1956 3212 ,2037 8746 1414 1471 ,5559 6601 1768 4702 ,4276 5346 4524 2110 ,547 9888 454 7000 ,4178 3574 7488 6188 ,369 7390 7990 4521 ,9590 9896 61 7191 ,9228 9905 2096 7995 ,2881 4507 5301 6314 ,5036 2130 1390 5593 ,217 2484 1288 3742 ,9370 7340 4535 4326 ,4038 40 6994 5603 ,4178 6545 5165 2081 ,6521 3109 1962 6511 ,8782 804 9746 9744 ,6471 733 4827 2941 ,7915 1508 5343 4447 ,5363 3468 5557 73 ,2099 1690 8979 4303 ,4360 9163 8667 7995 ,7714 8407 8581 9670 ,9801 212 1160 9187 ,3247 5354 2696 5815 ,8466 9906 2485 234 ,5708 8059 6717 3474 ,5759 6086 3058 6213 ,2728 6443 8809 3604 ,7440 3843 3273 1782 ,2942 764 8182 431 ,1371 2833 4811 3468 ,8981 2155 9335 9042 ,8720 430 7829 6462 ,2134 6349 1688 3788 ,3694 6153 6996 4581 ,2969 1999 7201 609 ,8844 7966 5561 361 ,6270 500 6064 9616 ,5285 9391 2897 1384 ,2576 7454 9940 890 ,3797 8805 1826 9141 ,7045 7855 6221 1872 ,6664 1715 4146 1995 ,2767 5725 9868 8324 ,5133 5938 5555 3714 ,6655 9853 8767 5322 ,9008 7924 3857 8056 ,7445 1796 2582 8606 ,5752 4365 3422 8811 ,7320 784 6534 2481 ,4650 140 4106 3487 ,7174 5774 20 3891

...
```