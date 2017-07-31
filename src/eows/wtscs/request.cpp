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

#include "request.hpp"

#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"

#include <fstream>
#include <sstream>

// Boost
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;

string eows::wtscs::request::get_UUID()
{
  return UUID;
}

string eows::wtscs::request::write_afl(eows::wtscs::twdtw_input_parameters* data)
{
  //TODO: write afl
  string afl = "time iquery -aq \"set no fetch; store(redimension(apply(stream(cast(apply(project(";

  ////// apply operator
  //////// project operator
  ////////// between operator
  afl.append("between(");
  afl.append(data->coverage);
  for(vector<int>::iterator it = data->roi.begin() ; it != data->roi.end(); ++it)
  {
    afl.append(", ");
    afl.append(to_string(*it));
  }
  afl.append(")");
  ////////// end between operator

  for(vector<string>::iterator it = data->bands.begin() ; it != data->bands.end(); ++it)
  {
    afl.append(", ");
    afl.append(*it);
  }
  afl.append(")");
  //////// end project operator

  afl.append(", colid, double(col_id), rowid, double(row_id), timeid, double(time_id)),<");

  for(vector<string>::iterator it = data->bands.begin() ; it != data->bands.end(); ++it)
  {
    afl.append(*it);
    afl.append(":int32, ");
  }

  afl.append("colid:double, rowid:double, timeid:double>");
  string attributes = get_scidb_schema(data->coverage);
  afl.append(attributes);
  afl.append("), \'Rscript /net/esensing-001/disks/d9/scidb15_12/scripts/test_twdtw/sits_scidb_streaming.R ");
  afl.append("patterns_json=/net/esensing-001/disks/d9/scidb15_12/scripts/test_twdtw/");
  afl.append(UUID);
  afl.append("_patterns.json ");
  afl.append("scale_factor=");
  afl.append(to_string(data->scale_factor));
  afl.append(" dist_method=");
  afl.append(data->dist_method);
  afl.append(" bands=");
  vector<string>::iterator it = data->bands.begin();
  afl.append(*it);
  ++it;
  for(; it != data->bands.end(); ++it)
  {
    afl.append(",");
    afl.append(*it);
  }
  afl.append(" alpha=");
  afl.append(to_string(data->alpha));
  afl.append(" beta=");
  afl.append(to_string(data->beta));
  afl.append(" theta=");
  afl.append(to_string(data->theta));
  afl.append(" interval=");
  // Find and replace content in string
  data->interval.replace(data->interval.find(" "), 1, ",");
  afl.append(data->interval);
  afl.append(" span=");
  afl.append(to_string(data->span));
  afl.append(" keep=");
  afl.append(data->keep);
  afl.append(" overlap=");
  afl.append(to_string(data->overlap));
  afl.append(" start_date=");
  afl.append(data->start_date);
  afl.append(" end_date=");
  afl.append(data->end_date);
  afl.append(" dates=");
  string dates = get_timeline(data->coverage);
  afl.append(dates);
  afl.append("', \'format=df\', \'types=double,double,double,int32,int32,int32,double\',");
  afl.append("\'names=colid,rowid,timeid,from,to,label,distance\'), ");
  afl.append("col_id, int64(colid), row_id, int64(rowid), time_id, int64(timeid)), <from:int32, to:int32, label:int32, distance:double>");
  afl.append("[col_id = 3500:3500, 40,  0, row_id = 3500:3500, 40, 0, time_id = 1:*, 16, 0]), ");
  afl.append(UUID);
  afl.append(")\"");

  return afl;
}

string eows::wtscs::request::get_timeline(string coverage)
{
  //TODO: Get timeline from geo_arrays.json file

  string timeline;
  timeline.append("2013-04-27,2013-05-13,2013-05-29,2013-06-14,2013-06-30,2013-07-16,2013-08-01,2013-08-17,2013-09-02,2013-09-18,2013-10-04,");
  timeline.append("2013-10-20,2013-11-05,2013-11-21,2013-12-07,2013-12-23,2014-01-08,2014-01-24,2014-02-09,2014-02-25,2014-03-13,2014-03-29,");
  timeline.append("2014-04-14,2014-04-30,2014-05-16,2014-06-01,2014-06-17,2014-07-03,2014-07-19,2014-08-04,2014-08-20,2014-09-05,2014-09-21,");
  timeline.append("2014-10-07,2014-10-23,2014-11-08,2014-11-24,2014-12-10,2014-12-26,2015-01-11,2015-01-27,2015-02-12,2015-02-28,2015-03-16,");
  timeline.append("2015-04-01,2015-04-17,2015-05-03,2015-05-19,2015-06-04,2015-06-20,2015-07-06,2015-07-22,2015-08-07,2015-08-23,2015-09-08,");
  timeline.append("2015-09-24,2015-10-10,2015-10-26,2015-11-11,2015-11-27,2015-12-13,2015-12-29,2016-01-14,2016-01-30,2016-02-15,2016-03-02,");
  timeline.append("2016-03-18,2016-04-03,2016-04-19,2016-05-05,2016-05-21,2016-06-06,2016-06-22,2016-07-08,2016-07-24,2016-08-09,2016-08-25,");
  timeline.append("2016-09-10,2016-09-26,2016-10-12,2016-10-28,2016-11-13,2016-11-29,2016-12-15,2016-12-31,2017-01-16,2017-02-01,2017-02-17,");
  timeline.append("2017-03-05,2017-03-21,2017-04-06,2017-04-22,2017-05-08");
  return timeline;
}

string eows::wtscs::request::get_scidb_schema(string coverage)
{
  //TODO: Get scidb schema. Ex. iquery -aq "show(mod13q1)"
  string attributes = "[col_id = 0:7299, 100, 0, row_id = 0:7299, 100, 0, time_id = 0:92, 93, 0]";

  return attributes;
}

eows::wtscs::request::request()
{
  pNext = NULL;
  status = "Rejected";
}

void eows::wtscs::request::write_setting()
{
  //TODO: write files
}


string eows::wtscs::request::get_status()
{
  return status;
}

void eows::wtscs::request::check_parameters()
{
  // TODO: Check parameters and change status to Scheduled
  status = "Scheduled";
}

void eows::wtscs::request::set_UUID(string nService)
{
  // Generate the UUID identifier to create the AFL syntax.

  boost::uuids::uuid u = boost::uuids::random_generator()();
  int d = u.size();
  stringstream s;
  s << u;
  string mys = s.str();
  mys.erase(8,1); mys.erase(12,1);
  mys.erase(16,1); mys.erase(20,1);
  mys.insert(0, nService);
  UUID = mys;
}

void eows::wtscs::request::set_parameters(const char *request, string dir)
{
  rapidjson::Document doc;

  // Parsing the request into a Document.
  doc.Parse(request);

  if(doc.HasParseError())
  {
    boost::format err_msg("Error parsing wtscs classify request: %1%.");
    throw eows::parse_error((err_msg % doc.GetParseError()).str());
  }

  // Checking parameters parsed into a Document.
  if(!doc.IsObject() || doc.IsNull())
    throw eows::parse_error("Error parsing wtscs classify request: unexpected request format.");

  // Read the "algoritm" and "input_parameters" members.
  rapidjson::Value::ConstMemberIterator jalgorithm = doc.FindMember("algorithm");
  rapidjson::Value& jinput = doc["input_parameters"];
  assert(jinput.IsObject());

  if((jalgorithm == doc.MemberEnd()) || (!jalgorithm->value.IsString()))
    throw eows::parse_error("Please check key 'algorithm' in WTSCS classify request.");


  eows::wtscs::request::algorithm = jalgorithm->value.GetString();
  if(eows::wtscs::request::algorithm == "TWDTW")
  {

    eows::wtscs::request::input_parameters = unique_ptr<eows::wtscs::base_input_parameters>(new eows::wtscs::twdtw_input_parameters);
    eows::wtscs::twdtw_input_parameters* pParameters = dynamic_cast<eows::wtscs::twdtw_input_parameters*>(eows::wtscs::request::input_parameters.get());

    for (rapidjson::Value::ConstMemberIterator itr = jinput.MemberBegin(); itr != jinput.MemberEnd(); ++itr)
    {
      if(string(itr->name.GetString()) == "coverage")
      {
        pParameters->coverage = itr->value.GetString();
      }
      if(string(itr->name.GetString()) == "scale_factor")
      {
        pParameters->scale_factor = itr->value.GetDouble();
      }
      if(string(itr->name.GetString()) == "bands")
      {
        assert(itr->value.IsArray());
        for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++)
          pParameters->bands.push_back(string(itr->value[i].GetString()));
      }
      if(string(itr->name.GetString()) == "roi")
      {
        assert(itr->value.IsObject());
        rapidjson::Value::ConstMemberIterator i = itr->value.MemberBegin();
        if(string(i->name.GetString()) == "bbox")
        {
          assert(i->value.IsArray());

          const rapidjson::Value &array = i->value;

          // TODO: You must treat the dates to extract the indices
          int data = array.Size();
          assert((array[0]).IsInt());
          pParameters->roi.push_back((array[0]).GetInt());

          assert((array[1]).IsInt());
          pParameters->roi.push_back((array[1]).GetInt());

          pParameters->roi.push_back(0);

          assert((array[2]).IsInt());
          pParameters->roi.push_back((array[2]).GetInt());

          assert((array[3]).IsInt());
          pParameters->roi.push_back((array[3]).GetInt());
          pParameters->roi.push_back(92);
        }
      }
      if(string(itr->name.GetString()) == "patterns")
      {
        // Storing the patterns file in a directory visible by SciDB.
        ofstream stream;
        assert(itr->value.IsArray());
        rapidjson::Value &jpatterns = jinput["patterns"];
        rapidjson::Document d2;
        rapidjson::Document::AllocatorType &allocator = d2.GetAllocator();
        d2.CopyFrom(jpatterns, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer>writer(buffer);
        d2.Accept(writer);
        const char *json = buffer.GetString();
        dir.append(UUID);
        dir.append("_patterns.json");
        stream.open(dir);
        if(!stream)
          throw eows::parse_error("Opening file failed.");
        stream << json << endl;
        if(!stream)
          throw eows::parse_error("Write patterns.json failed.");
      }
      if(string(itr->name.GetString()) == "dist.method")
      {
        pParameters->dist_method = itr->value.GetString();
      }
      if(string(itr->name.GetString()) == "alpha")
      {
        pParameters->alpha = itr->value.GetDouble();
      }
      if(string(itr->name.GetString()) == "beta")
      {
        pParameters->beta = itr->value.GetInt();
      }
      if(string(itr->name.GetString()) == "theta")
      {
        pParameters->theta = itr->value.GetDouble();
      }
      if(string(itr->name.GetString()) == "interval")
      {
        pParameters->interval = itr->value.GetString();
      }
      if(string(itr->name.GetString()) == "span")
      {
        pParameters->span = itr->value.GetInt();
      }
      if(string(itr->name.GetString()) == "keep")
      {
        pParameters->keep = itr->value.GetString();
      }
      if(string(itr->name.GetString()) == "overlap")
      {
        pParameters->overlap = itr->value.GetDouble();
      }
      if(string(itr->name.GetString()) == "start_date")
      {
        pParameters->start_date = itr->value.GetString();
      }
      if(string(itr->name.GetString()) == "end_date")
      {
        pParameters->end_date = itr->value.GetString();
      }
    }
  }
}
