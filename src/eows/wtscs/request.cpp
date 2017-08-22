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
#include "defines.hpp"

#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

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

string eows::wtscs::request::get_status(string UUID)
{
  //TODO: get_status
  return "Scheduled";
}

void eows::wtscs::request::set_status(string UUID, string status)
{
  FILE* pFile;
  string log_file;
  log_file.append(EOWS_WTSCS_DIR);
  log_file.append(UUID);
  log_file.append("_log.json");

  pFile = fopen(log_file.c_str(), "w+");
  if(pFile != NULL)
  {
    fputs("{ \"status\": \"", pFile);
    fputs(status.c_str(), pFile);
    fputs("\" }", pFile);
    fclose(pFile);
  }

  return;
}

string eows::wtscs::request::get_UUID()
{
  return UUID;
}

string eows::wtscs::request::write_afl(eows::wtscs::twdtw_input_parameters* data)
{
  //TODO: write afl
  string afl = "/usr/bin/time -p /opt/scidb/15.12/bin/iquery -aq \"set no fetch; store(redimension(apply(stream(cast(apply(project(";

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
  string const_dir = EOWS_WTSCS_DIR;
  string const_script =  EOWS_WTSCS_RSCRIPT;
  afl.append("), \'Rscript ");
  afl.append(const_dir);
  afl.append(const_script);
  afl.append(" patterns_json=");
  afl.append(const_dir);
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
  string dates = get_timeline(data->coverage, data->start_date, data->end_date);
  afl.append(dates);
  afl.append("', \'format=df\', \'types=double,double,double,int32,int32,int32,double\',");
  afl.append("\'names=colid,rowid,timeid,from,to,label,distance\'), ");
  afl.append("col_id, int64(colid), row_id, int64(rowid), time_id, int64(timeid)), <from:int32, to:int32, label:int32, distance:double>");
  afl.append(attributes);
  afl.append("), ");
  afl.append(UUID);
  afl.append(")\"");

  return afl;
}

string eows::wtscs::request::get_timeline(string coverage, string start_date, string end_date)
{
  pair<std::size_t, std::size_t> time_interval;

  const eows::geoarray::geoarray_t& geo_array = eows::geoarray::geoarray_manager::instance().get(coverage);
  time_interval = geo_array.timeline.find_interval(start_date, end_date);

  string timeline;
  timeline.append(geo_array.timeline.get(time_interval.first));

  for(size_t i = time_interval.first + 1; i <= time_interval.second; i++)
  {
    timeline.append(",");
    timeline.append(geo_array.timeline.get(i));
  }

  return timeline;
}

string eows::wtscs::request::get_scidb_schema(string coverage)
{
  FILE* fp;
  char file_type[500];
  string scidb_scheme, attributes;
  string afl_query;

  afl_query.append("/opt/scidb/15.12/bin/iquery -aq \"show(");
  afl_query.append(coverage);
  afl_query.append(")\"");

  fp = popen(afl_query.c_str(), "r");

//  if(fp == NULL)
//  {
//    printf("Failed to run command \n");
//  }

  while(fgets(file_type, sizeof(file_type), fp) != NULL)
  {
    scidb_scheme.append(file_type);
  }

  pclose(fp);

  size_t pos  = scidb_scheme.find("[");
  size_t len = scidb_scheme.find("]") - pos + 1;
  attributes.append(scidb_scheme.substr(pos, len));

  return attributes;
}

eows::wtscs::request::request()
{
  eows::wtscs::request::set_status(UUID, "Rejected");
}

void eows::wtscs::request::write_setting()
{
  //TODO: write files
}

void eows::wtscs::request::check_parameters()
{
  // TODO: Check parameters and change status to Scheduled
  eows::wtscs::request::set_status(UUID, "Scheduled");
}

void eows::wtscs::request::set_UUID(string nService)
{
  // Generate the UUID identifier to create the AFL syntax.

  boost::uuids::uuid u = boost::uuids::random_generator()();
  stringstream s;
  s << u;
  string mys = s.str();
  mys.erase(8,1); mys.erase(12,1);
  mys.erase(16,1); mys.erase(20,1);
  mys.insert(0, nService);
  UUID = mys;
}

void eows::wtscs::request::set_parameters(const char *request)
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
          assert((array[0]).IsInt());
          pParameters->roi.push_back((array[0]).GetInt());

          assert((array[1]).IsInt());
          pParameters->roi.push_back((array[1]).GetInt());

          pParameters->roi.push_back(0);

          assert((array[2]).IsInt());
          pParameters->roi.push_back((array[2]).GetInt());

          assert((array[3]).IsInt());
          pParameters->roi.push_back((array[3]).GetInt());
          pParameters->roi.push_back(391);
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
        string dir = EOWS_WTSCS_DIR;
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
