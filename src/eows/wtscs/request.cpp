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
#include "../core/defines.hpp"
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
  string dates = get_timeline(data->coverage);
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

string eows::wtscs::request::get_timeline(string coverage)
{
  //TODO: Get timeline from geo_arrays.json file

  string timeline;
  timeline.append("2000-02-18,2000-03-05,2000-03-21,2000-04-06,2000-04-22,2000-05-08,2000-05-24,2000-06-09,2000-06-25,2000-07-11,2000-07-27,");
  timeline.append("2000-08-12,2000-08-28,2000-09-13,2000-09-29,2000-10-15,2000-10-31,2000-11-16,2000-12-02,2000-12-18,");
  timeline.append("2001-01-01,2001-01-17,2001-02-02,2001-02-18,2001-03-06,2001-03-22,2001-04-07,2001-04-23,2001-05-09,2001-05-25,2001-06-10,2001-06-26,2001-07-12,2001-07-28,");
  timeline.append("2001-08-13,2001-08-29,2001-09-14,2001-09-30,2001-10-16,2001-11-01,2001-11-17,2001-12-03,2001-12-19,");
  timeline.append("2002-01-01,2002-01-17,2002-02-02,2002-02-18,2002-03-06,2002-03-22,2002-04-07,2002-04-23,2002-05-09,2002-05-25,2002-06-10,2002-06-26,2002-07-12,2002-07-28,");
  timeline.append("2002-08-13,2002-08-29,2002-09-14,2002-09-30,2002-10-16,2002-11-01,2002-11-17,2002-12-03,2002-12-19,");
  timeline.append("2003-01-01,2003-01-17,2003-02-02,2003-02-18,2003-03-06,2003-03-22,2003-04-07,2003-04-23,2003-05-09,2003-05-25,2003-06-10,2003-06-26,2003-07-12,2003-07-28,");
  timeline.append("2003-08-13,2003-08-29,2003-09-14,2003-09-30,2003-10-16,2003-11-01,2003-11-17,2003-12-03,2003-12-19,");
  timeline.append("2004-01-01,2004-01-17,2004-02-02,2004-02-18,2004-03-05,2004-03-21,2004-04-06,2004-04-22,2004-05-08,2004-05-24,2004-06-09,2004-06-25,2004-07-11,2004-07-27,");
  timeline.append("2004-08-12,2004-08-28,2004-09-13,2004-09-29,2004-10-15,2004-10-31,2004-11-16,2004-12-02,2004-12-18,");
  timeline.append("2005-01-01,2005-01-17,2005-02-02,2005-02-18,2005-03-06,2005-03-22,2005-04-07,2005-04-23,2005-05-09,2005-05-25,2005-06-10,2005-06-26,2005-07-12,2005-07-28,");
  timeline.append("2005-08-13,2005-08-29,2005-09-14,2005-09-30,2005-10-16,2005-11-01,2005-11-17,2005-12-03,2005-12-19,");
  timeline.append("2006-01-01,2006-01-17,2006-02-02,2006-02-18,2006-03-06,2006-03-22,2006-04-07,2006-04-23,2006-05-09,2006-05-25,2006-06-10,2006-06-26,2006-07-12,2006-07-28,");
  timeline.append("2006-08-13,2006-08-29,2006-09-14,2006-09-30,2006-10-16,2006-11-01,2006-11-17,2006-12-03,2006-12-19,");
  timeline.append("2007-01-01,2007-01-17,2007-02-02,2007-02-18,2007-03-06,2007-03-22,2007-04-07,2007-04-23,2007-05-09,2007-05-25,2007-06-10,2007-06-26,2007-07-12,2007-07-28,");
  timeline.append("2007-08-13,2007-08-29,2007-09-14,2007-09-30,2007-10-16,2007-11-01,2007-11-17,2007-12-03,2007-12-19,");
  timeline.append("2008-01-01,2008-01-17,2008-02-02,2008-02-18,2008-03-05,2008-03-21,2008-04-06,2008-04-22,2008-05-08,2008-05-24,2008-06-09,2008-06-25,2008-07-11,2008-07-27,");
  timeline.append("2008-08-12,2008-08-28,2008-09-13,2008-09-29,2008-10-15,2008-10-31,2008-11-16,2008-12-02,2008-12-18,");
  timeline.append("2009-01-01,2009-01-17,2009-02-02,2009-02-18,2009-03-06,2009-03-22,2009-04-07,2009-04-23,2009-05-09,2009-05-25,2009-06-10,2009-06-26,2009-07-12,2009-07-28,");
  timeline.append("2009-08-13,2009-08-29,2009-09-14,2009-09-30,2009-10-16,2009-11-01,2009-11-17,2009-12-03,2009-12-19,");
  timeline.append("2010-01-01,2010-01-17,2010-02-02,2010-02-18,2010-03-06,2010-03-22,2010-04-07,2010-04-23,2010-05-09,2010-05-25,2010-06-10,2010-06-26,2010-07-12,2010-07-28,");
  timeline.append("2010-08-13,2010-08-29,2010-09-14,2010-09-30,2010-10-16,2010-11-01,2010-11-17,2010-12-03,2010-12-19,");
  timeline.append("2011-01-01,2011-01-17,2011-02-02,2011-02-18,2011-03-06,2011-03-22,2011-04-07,2011-04-23,2011-05-09,2011-05-25,2011-06-10,2011-06-26,2011-07-12,2011-07-28,");
  timeline.append("2011-08-13,2011-08-29,2011-09-14,2011-09-30,2011-10-16,2011-11-01,2011-11-17,2011-12-03,2011-12-19,");
  timeline.append("2012-01-01,2012-01-17,2012-02-02,2012-02-18,2012-03-05,2012-03-21,2012-04-06,2012-04-22,2012-05-08,2012-05-24,2012-06-09,2012-06-25,2012-07-11,2012-07-27,");
  timeline.append("2012-08-12,2012-08-28,2012-09-13,2012-09-29,2012-10-15,2012-10-31,2012-11-16,2012-12-02,2012-12-18,");
  timeline.append("2013-01-01,2013-01-17,2013-02-02,2013-02-18,2013-03-06,2013-03-22,2013-04-07,2013-04-23,2013-05-09,2013-05-25,2013-06-10,2013-06-26,2013-07-12,2013-07-28,");
  timeline.append("2013-08-13,2013-08-29,2013-09-14,2013-09-30,2013-10-16,2013-11-01,2013-11-17,2013-12-03,2013-12-19,");
  timeline.append("2014-01-01,2014-01-17,2014-02-02,2014-02-18,2014-03-06,2014-03-22,2014-04-07,2014-04-23,2014-05-09,2014-05-25,2014-06-10,2014-06-26,2014-07-12,2014-07-28,");
  timeline.append("2014-08-13,2014-08-29,2014-09-14,2014-09-30,2014-10-16,2014-11-01,2014-11-17,2014-12-03,2014-12-19,");
  timeline.append("2015-01-01,2015-01-17,2015-02-02,2015-02-18,2015-03-06,2015-03-22,2015-04-07,2015-04-23,2015-05-09,2015-05-25,2015-06-10,2015-06-26,2015-07-12,2015-07-28,");
  timeline.append("2015-08-13,2015-08-29,2015-09-14,2015-09-30,2015-10-16,2015-11-01,2015-11-17,2015-12-03,2015-12-19,");
  timeline.append("2016-01-01,2016-01-17,2016-02-02,2016-02-18,2016-03-05,2016-03-21,2016-04-06,2016-04-22,2016-05-08,2016-05-24,2016-06-09,2016-06-25,2016-07-11,2016-07-27,");
  timeline.append("2016-08-12,2016-08-28,2016-09-13,2016-09-29,2016-10-15,2016-10-31,2016-11-16,2016-12-02,2016-12-18,");
  timeline.append("2017-01-01,2017-01-17,2017-02-02,2017-02-18");
return timeline;
}

string eows::wtscs::request::get_scidb_schema(string coverage)
{
  //TODO: Get scidb schema. Ex. iquery -aq "show(mod13q1_512)"
  string attributes = "[col_id=0:172799,40,0,row_id=0:86399,40,0,time_id=0:511,512,0]";

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
