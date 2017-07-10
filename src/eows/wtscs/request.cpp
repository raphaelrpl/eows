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

string eows::wtscs::request::write_afl(eows::wtscs::twdtw_input_parameters* data)
{
  //TODO: write afl
  string afl = "time iquery -aq \"set no fetch; store(redimension(apply(stream(cast(apply(project(between(";

  afl.append(data->coverage);
  for(vector<int>::iterator it = data->roi.begin() ; it != data->roi.end(); ++it)
  {
    afl.append(", ");
    afl.append(to_string(*it));
  }

  afl.append(")");

  for(vector<string>::iterator it = data->bands.begin() ; it != data->bands.end(); ++it)
  {
    afl.append(", ");
    afl.append(*it);
  }

  afl.append("), colid, double(col_id), rowid, double(row_id), timeid, double(time_id)),<");

  for(vector<string>::iterator it = data->bands.begin() ; it != data->bands.end(); ++it)
  {
    afl.append(*it);
    afl.append(":int32, ");
  }


  afl.append("), i32col_id, int32(col_id), i32row_id, int32(row_id), i32time_id, int32(time_id)), i32col_id, i32row_id, i32time_id");

  afl.append("), \'Rscript /net/esensing-001/disks/d9/scidb15_12/scripts/test_twdtw/");
  afl.append(UUID);
  afl.append("_patterns.json");

  afl.append(" scale_factor bands alfa beta theta overlap\', \'format=df\',");
  cout << afl;
  cout << endl;

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
          pParameters->roi.push_back(255);
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
        pParameters->beta = itr->value.GetDouble();
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
        pParameters->span = itr->value.GetDouble();
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
