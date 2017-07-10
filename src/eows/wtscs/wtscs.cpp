/*
  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.

  This file is part of Earth Observation Web Services (EOWS).

  EOWS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  EOWS is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with EOWS. See LICENSE. If not, write to
  e-sensing team at <esensing-team@dpi.inpe.br>.
 */

// EOWS
#include "wtscs.hpp"
#include "request.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"
//#include "../exception.hpp"

//// C++ Standard Library
//#include <memory>


// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cstdlib>

using namespace std;

string scidb_shared_directory;
string config_directory;
string name_service;

eows::wtscs::request* pRequestList;
static void return_exception(const char* msg, eows::core::http_response& res);

void eows::wtscs::status_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: See how to implement the status_handler methods!.

  string return_msg("\"status_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::list_algorithms_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  vector<string> algorithms{ "twdtw", "bfast", "bfast-monitor" };

  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

  writer.StartObject();

  writer.Key("algorithms", static_cast<rapidjson::SizeType>(sizeof("algorithms") -1));
  eows::core::write_string_array(algorithms.begin(), algorithms.end(), writer);

  writer.EndObject();

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.GetString(), buff.GetSize());
}

void eows::wtscs::classify_handler::do_post(const eows::core::http_request& req,
                                            eows::core::http_response& res)
{

  // Parsing the request into a Document.
  request* oRequest = new request;

  try
  {
    oRequest->set_UUID(name_service);
    oRequest->set_parameters(req.content(), scidb_shared_directory);

    oRequest->check_parameters();
    if((oRequest->get_status()).compare("Scheduled") == 0)
    {
      eows::wtscs::request* pTempPosition = pRequestList;
      eows::wtscs::request* pPrevPosition = pTempPosition;
      pTempPosition = pTempPosition->pNext;
      while(pTempPosition)
      {
        pPrevPosition = pTempPosition;
        pTempPosition = pTempPosition->pNext;
      }
      pPrevPosition->pNext = oRequest;

      // TODO: Save the request list on disk (wtscs_request_list.json)
      // You will use the config_directory string

      // Write afl
      string afl = oRequest->write_afl(dynamic_cast<eows::wtscs::twdtw_input_parameters*>(oRequest->input_parameters.get()));
   }

/*
iquery -aq "set no fetch; store(redimension(apply(stream(cast(apply(project(between(npr, 3500, 3500, 0, 3500, 3500, 92),
ndvi, evi, nir), colid, double(col_id), rowid, double(row_id), timeid, double(time_id)),<ndvi:int32,
evi:int32, nir:int32, colid:double, rowid:double, timeid:double>[col_id = 0:7299, 100, 0, row_id = 0:7299,
100, 0,time_id = 0:92, 93, 0]), 'Rscript /net/esensing-001/disks/d9/scidb15_12/scripts/test_twdtw/sits_scidb_streaming.R
patterns_json=/net/esensing-001/disks/d9/scidb15_12/scripts/test_twdtw/patterns_Damien_Ieda_Rodrigo_17classes_3bands.json
scale_factor=0.0001 dist_method=euclidean bands=ndvi,evi,nir alpha=-0.1 beta=100 theta=0.5 interval=12,month
span=250 keep=FALSE overlap=0.5 start_date=2000-09-01 end_date=2017-08-31 dates=2013-04-27,2013-05-13,2013-05-29,2013-06-14,2013-06-30,
2013-07-16,2013-08-01,2013-08-17,2013-09-02,2013-09-18,2013-10-04,2013-10-20,2013-11-05,2013-11-21,
2013-12-07,2013-12-23,2014-01-08,2014-01-24,2014-02-09,2014-02-25,2014-03-13,2014-03-29,2014-04-14,
2014-04-30,2014-05-16,2014-06-01,2014-06-17,2014-07-03,2014-07-19,2014-08-04,2014-08-20,2014-09-05,
2014-09-21,2014-10-07,2014-10-23,2014-11-08,2014-11-24,2014-12-10,2014-12-26,2015-01-11,2015-01-27,
2015-02-12,2015-02-28,2015-03-16,2015-04-01,2015-04-17,2015-05-03,2015-05-19,2015-06-04,2015-06-20,
2015-07-06,2015-07-22,2015-08-07,2015-08-23,2015-09-08,2015-09-24,2015-10-10,2015-10-26,2015-11-11,
2015-11-27,2015-12-13,2015-12-29,2016-01-14,2016-01-30,2016-02-15,2016-03-02,2016-03-18,2016-04-03,
2016-04-19,2016-05-05,2016-05-21,2016-06-06,2016-06-22,2016-07-08,2016-07-24,2016-08-09,2016-08-25,
2016-09-10,2016-09-26,2016-10-12,2016-10-28,2016-11-13,2016-11-29,2016-12-15,2016-12-31,2017-01-16,
2017-02-01,2017-02-17,2017-03-05,2017-03-21,2017-04-06,2017-04-22,2017-05-08',
'format=df', 'types=double,double,double,int32,int32,int32,double',
'names=colid,rowid,timeid,from,to,label,distance'), col_id, int64(colid), row_id, int64(rowid),
time_id, int64(timeid)), <from:int32, to:int32, label:int32, distance:double>
[col_id = 3500:3500, 40,  0, row_id = 3500:3500, 40, 0, time_id = 1:*, 16, 0]), algumacoisaArray)"
*/
    system("/opt/scidb/15.12/bin/iquery -aq \"remove(Array1)\"");
    int a = 2;
    // It Sends the AFL request.
    // The answer is a URL wrapping the request UUID identifier.
    // Ex. http://localhost:7654/wtscs/status?UUID=123456687


//    // assembly the response
//    rapidjson::StringBuffer buff;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

//    writer.StartObject();

//    writer.Key("status", static_cast<rapidjson::SizeType>(sizeof("status") -1));
//    writer.String(parRequest.algorithm.c_str(), static_cast<rapidjson::SizeType>(parRequest.algorithm.length()));
//    writer.EndObject();

//    res.set_status(eows::core::http_response::OK);

//    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
//    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

//    res.write(buff.GetString(), buff.GetSize());
  }
  catch(const exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSCS classify operation.", res);
  }
}

void eows::wtscs::open_request_list()
{
  scidb_shared_directory = "/home/edullapa/mydevel/eows/scidb/";
  config_directory = "/home/edullapa/mydevel/eows/codebase/share/eows/config/";
  name_service = "WTSCS";

  pRequestList = new request;


  EOWS_LOG_INFO("request file was analized!");
}

void eows::wtscs::initialize()
{
  EOWS_LOG_INFO("Initializing WTSCS...");

  //Read UUID using WTSCS
  eows::wtscs::open_request_list();

  unique_ptr<eows::wtscs::status_handler> s_h(new eows::wtscs::status_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/status", move(s_h));

  unique_ptr<list_algorithms_handler> la_h(new list_algorithms_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/list_algorithms", move(la_h));

  unique_ptr<classify_handler> c_h(new classify_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/classify", move(c_h));

  EOWS_LOG_INFO("WTSCS service initialized!");
}

void return_exception(const char* msg, eows::core::http_response& res)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

  writer.StartObject();

  writer.Key("exception", static_cast<rapidjson::SizeType>(sizeof("exception") -1));
  writer.String(msg);

  writer.EndObject();

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.GetString(), buff.GetSize());
}
