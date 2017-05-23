#include "parserequest.hpp"

#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"

#include <fstream>

// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


void eows::wtscs::parseRequest::setParameters(const char* request)
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


  eows::wtscs::parseRequest::algorithm = jalgorithm->value.GetString();
  if(eows::wtscs::parseRequest::algorithm == "TWDTW")
  {

    eows::wtscs::parseRequest::input_parameters = std::unique_ptr<eows::wtscs::base_input_parameters>(new eows::wtscs::twdtw_input_parameters);
    eows::wtscs::twdtw_input_parameters* pParameters = dynamic_cast<eows::wtscs::twdtw_input_parameters*>(eows::wtscs::parseRequest::input_parameters.get());

    for (rapidjson::Value::ConstMemberIterator itr = jinput.MemberBegin(); itr != jinput.MemberEnd(); ++itr)
    {
      if(std::string(itr->name.GetString()) == "coverage")
      {
        pParameters->cv_name = itr->value.GetString();
      }
      if(std::string(itr->name.GetString()) == "attributes")
      {
        assert(itr->value.IsArray());
        for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++)
          pParameters->attributes.push_back(std::string(itr->value[i].GetString()));
      }
      if(std::string(itr->name.GetString()) == "roi")
      {
        assert(itr->value.IsObject());

        if(std::string((itr->value.MemberBegin())->name.GetString()) == "geometry")
        {
          rapidjson::Value::ConstMemberIterator  i = (itr->value.MemberBegin())->value.MemberBegin();
          // TODO: Remember that a geometry is a GeoJSON object where the type member's value is one of the following strings:
          // "Point", "MultiPoint", "LineString", "MultiLineString", "Polygon", "MultiPolygon", or "GeometryCollection".
          if(std::string(i->value.GetString()) == "Polygon")
          {
            i++; // coodinates
            assert(i->value.IsArray());
            for (rapidjson::SizeType j = 0; j < i->value.Size(); j++)
            {
              const rapidjson::Value &array = i->value[j];
              for (rapidjson::SizeType k = 0; k < array.Size(); k++)
              {
                assert(array[k].IsArray());
                // int x = array[k].Size();
                double data1 = array[k][0].GetDouble();
                double data2 = array[k][1].GetDouble();
                pParameters->roi.push_back(eows::wtscs::coordinate(data1, data2));
              }
            }
          }
        }
      }
      if(std::string(itr->name.GetString()) == "patterns")
      {
        // Storing the patterns file in a directory visible by SciDB.
        std::ofstream stream;
        assert(itr->value.IsArray());
        rapidjson::Value &jpatterns = jinput["patterns"];
        rapidjson::Document d2;
        rapidjson::Document::AllocatorType &allocator = d2.GetAllocator();
        d2.CopyFrom(jpatterns, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer>writer(buffer);
        d2.Accept(writer);
        const char *json = buffer.GetString();
        // TODO: Read the correct directory to save file.
        stream.open("/home/edullapa/texto.txt");
        if(!stream)
          throw eows::parse_error("Opening file failed.");
        stream << json << std::endl;
        if(!stream)
          throw eows::parse_error("Write patterns.json failed.");
      }
      if(std::string(itr->name.GetString()) == "start_date")
      {
        pParameters->start_date = itr->value.GetString();
      }
      if(std::string(itr->name.GetString()) == "end_date")
      {
        pParameters->end_date = itr->value.GetString();
      }
      if(std::string(itr->name.GetString()) == "by")
      {
        pParameters->by = itr->value.GetString();
      }
      if(std::string(itr->name.GetString()) == "overlap")
      {
        pParameters->overlap = itr->value.GetDouble();
      }
      if(std::string(itr->name.GetString()) == "alpha")
      {
        pParameters->alpha = itr->value.GetDouble();
      }
      if(std::string(itr->name.GetString()) == "beta")
      {
        pParameters->beta = itr->value.GetDouble();
      }
      if(std::string(itr->name.GetString()) == "output_array_name")
      {
        pParameters->output_array_name = itr->value.GetString();
      }
    }
  }
}
