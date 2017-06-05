// EOWS
#include "utils.hpp"
#include "data_types.hpp"
#include "../exception.hpp"
#include "../../../core/utils.hpp"
// EOWS OWS module
#include "../../ows/manager.hpp"
// EOWS Geo Array
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"

void eows::ogc::wcs::core::read(const rapidjson::Value& doc, capabilities_t& capability)
{
  // Reading First Node "WCSCapabilities"
  if(!doc.IsObject())
    throw eows::parse_error("Key 'WCSCapabilities' must be a valid JSON object.");

  // Reading WCS Service Identification
  rapidjson::Value::ConstMemberIterator jit = doc.FindMember("ServiceIdentification");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'ServiceIdentification' was not found in JSON document.");

  read(jit->value, capability.service);

  // Setting meta ows provider
  capability.service_provider = ows::manager::instance().provider();

  // Reading WCS Service Metadata
  jit = doc.FindMember("ServiceMetadata");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'ServiceMetadata' was not found in JSON document.");

  read(jit->value, capability.service_metadata);

  // Reading WCS Operations Metadata
  jit = doc.FindMember("OperationsMetadata");
  if (jit == doc.MemberEnd())
    throw eows::parse_error("Key 'OperationsMetadata' were not found in JSON document");
  read(jit->value, capability.operation_metadata);
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_identification_t& service)
{
  // Validating ServiceIdentification structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceIdentification' must be a valid JSON object.");

  // Reading Elements
  service.title = eows::core::read_node_as_string(jservice, "Title");
  service.abstract = eows::core::read_node_as_string(jservice, "Abstract");
  service.service_type = eows::core::read_node_as_string(jservice, "ServiceType");
  service.service_type_version = eows::core::read_node_as_string(jservice, "ServiceTypeVersion");
  // Reading KeyWords list
  rapidjson::Value::ConstMemberIterator jit = jservice.FindMember("Profiles");
  // TODO: auto format function in common
  if((jit == jservice.MemberEnd()) || (!jit->value.IsArray()))
    throw eows::parse_error("Please, check the key 'Profiles' in JSON document.");

  for(auto& v: jit->value.GetArray())
    service.profiles.push_back(v.GetString());
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_metadata_t& metadata)
{
  // Validating ServiceProvider structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceMetadata' must be a valid JSON object.");

  rapidjson::Value::ConstMemberIterator jit = jservice.FindMember("formatSupported");
  if((jit == jservice.MemberEnd() || !jit->value.IsArray()))
    throw eows::parse_error("Key 'ServiceMetadata' must be a array.");

  for(auto& format: jit->value.GetArray())
    metadata.formats_supported.push_back(format.GetString());
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::operation_metadata_t& operation_meta)
{
  // Validating OperationsMetadata structure (must be an array)
  if(!jservice.IsArray())
    throw eows::parse_error("Key 'OperationsMetadata' must be a valid JSON Array.");

  for(const rapidjson::Value& json_operation: jservice.GetArray())
  {
    eows::ogc::wcs::core::operation_t op;
    op.name = eows::core::read_node_as_string(json_operation, "name");
    /*
      TODO: Is there other type like Distributed Computing Platforms(DCPs)?
            Support another Operations that just HTTP.. i.e SOAP?
    */
    rapidjson::Value::ConstMemberIterator jit = json_operation.FindMember("DCP");
    if (jit == json_operation.MemberEnd() || !jit->value.IsObject())
      throw eows::parse_error("Key 'DCP' must be a valid JSON object.");

    rapidjson::Value::ConstMemberIterator sub_it = jit->value.FindMember("HTTP");
    if (sub_it == jit->value.MemberEnd() || !sub_it->value.IsObject())
      throw eows::parse_error("Key 'HTTP' must be a valid JSON object.");

    // TODO: Add support for POST
    op.dcp.get = eows::core::read_node_as_string(sub_it->value, "GET");

    // Appending into loaded operations
    operation_meta.operations.push_back(op);
  }
}

void eows::ogc::wcs::core::make_coverage_bounded_by(rapidxml::xml_document<>* doc,
                                                    rapidxml::xml_node<>* node,
                                                    const eows::geoarray::geoarray_t& array,
                                                    const eows::geoarray::spatial_extent_t& extent,
                                                    const eows::geoarray::dimension_t& time_dimension)
{
  rapidxml::xml_node<>* bound = doc->allocate_node(rapidxml::node_element, "gml:boundedBy");
  node->append_node(bound);
  {
    rapidxml::xml_node<>* envelope = doc->allocate_node(rapidxml::node_element, "gml:Envelope");
    // Appending Envelope into bound
    bound->append_node(envelope);

    std::string axis_labels = array.dimensions.x.name + " " + array.dimensions.y.name + " " + time_dimension.name;
    envelope->append_attribute(doc->allocate_attribute("srsName", "http://www.opengis.net/def/crs/EPSG/0/4326"));
    envelope->append_attribute(doc->allocate_attribute("axisLabels", doc->allocate_string(axis_labels.c_str())));
    envelope->append_attribute(doc->allocate_attribute("srsDimension", "3"));

    const std::string lower = std::to_string(extent.xmin) + " " +
                              std::to_string(extent.ymin) + " " +
                              std::to_string(time_dimension.min_idx);
    const std::string upper = std::to_string(extent.xmax) + " " +
                              std::to_string(extent.ymax) + " " +
                              std::to_string(time_dimension.max_idx);

    envelope->append_node(doc->allocate_node(rapidxml::node_element, "gml:lowerCorner",
                                                doc->allocate_string(lower.c_str())));
    envelope->append_node(doc->allocate_node(rapidxml::node_element, "gml:upperCorner",
                                                doc->allocate_string(upper.c_str())));
  }
}

void eows::ogc::wcs::core::make_coverage_range_type(rapidxml::xml_document<>* doc,
                                                    rapidxml::xml_node<>* node,
                                                    const std::vector<eows::geoarray::attribute_t>& attributes)
{
  rapidxml::xml_node<>* range_type = doc->allocate_node(rapidxml::node_element, "gmlcov:rangeType");
  node->append_node(range_type);
  rapidxml::xml_node<>* data_record = doc->allocate_node(rapidxml::node_element, "swe:DataRecord");
  range_type->append_node(data_record);
  for(const geoarray::attribute_t& attribute: attributes)
  {
    rapidxml::xml_node<>* field = doc->allocate_node(rapidxml::node_element, "swe:field");
    data_record->append_node(field);
    field->append_attribute(doc->allocate_attribute("name", attribute.name.c_str()));
    rapidxml::xml_node<>* quantity = doc->allocate_node(rapidxml::node_element, "swe:Quantity");
    field->append_node(quantity);
    // Alloc swe description
    quantity->append_node(doc->allocate_node(rapidxml::node_element,
                                             "swe:Description",
                                             attribute.description.c_str()));
    // Alloc swe uom
    rapidxml::xml_node<>* uom = doc->allocate_node(rapidxml::node_element, "swe:uom");
    quantity->append_node(uom);
    uom->append_attribute(doc->allocate_attribute("code", attribute.name.c_str()));

    // Formatting attributes limits and setting to string allocator to remove after generation
    std::string interval = std::to_string(attribute.valid_range.min_val) + " " +
                           std::to_string(attribute.valid_range.max_val);

    rapidxml::xml_node<>* constraint = doc->allocate_node(rapidxml::node_element, "swe:constraint");
    quantity->append_node(constraint);
    rapidxml::xml_node<>* allowed_value = doc->allocate_node(rapidxml::node_element, "swe:AllowedValues");
    constraint->append_node(allowed_value);

    allowed_value->append_node(doc->allocate_node(rapidxml::node_element,
                                                  "swe:interval",
                                                  doc->allocate_string(interval.c_str())));
  }
}

void eows::ogc::wcs::core::make_coverage_domain_set(rapidxml::xml_document<>* doc,
                                                    rapidxml::xml_node<>* node,
                                                    const eows::geoarray::geoarray_t& array,
                                                    const std::vector<eows::geoarray::dimension_t>& dimensions)
{
  rapidxml::xml_node<>* domain_set = doc->allocate_node(rapidxml::node_element, "gml:domainSet");
  node->append_node(domain_set);
  {
    rapidxml::xml_node<>* grid = doc->allocate_node(rapidxml::node_element, "gml:Grid");
    grid->append_attribute(doc->allocate_attribute("gml:id", array.name.c_str()));
    grid->append_attribute(doc->allocate_attribute("dimension", doc->allocate_string(std::to_string(dimensions.size()).c_str())));
    domain_set->append_node(grid);
    {
      rapidxml::xml_node<>* limits = doc->allocate_node(rapidxml::node_element, "gml:limits");
      grid->append_node(limits);

      {
        rapidxml::xml_node<>* grid_envelope = doc->allocate_node(rapidxml::node_element, "gml:GridEnvelope");
        limits->append_node(grid_envelope);

        std::string low;/* = (std::to_string(array.dimensions.x.min_idx) + " ") +
                          (std::to_string(array.dimensions.y.min_idx) + " ") +
                           std::to_string(array.dimensions.t.min_idx);*/

        std::string high; /*(std::to_string(array.dimensions.x.max_idx) + " " +
                            std::to_string(array.dimensions.y.max_idx) + " " +
                            std::to_string(array.dimensions.t.max_idx));*/

        for(const eows::geoarray::dimension_t& dimension: dimensions)
        {
          low += std::to_string(dimension.min_idx) + " ";
          high += std::to_string(dimension.max_idx) + " ";
        }
        low.pop_back();
        high.pop_back();

        rapidxml::xml_node<>* elm = doc->allocate_node(rapidxml::node_element,
                                                       "gml:low",
                                                       doc->allocate_string(low.c_str()));

        grid_envelope->append_node(elm);

        elm = doc->allocate_node(rapidxml::node_element,
                                 "gml:high",
                                 doc->allocate_string(high.c_str()));
        grid_envelope->append_node(elm);
      }

      std::string array_limits_str = array.dimensions.x.name + " ";
      array_limits_str += array.dimensions.y.name + " ";
      array_limits_str += array.dimensions.t.name;
      limits->append_node(doc->allocate_node(rapidxml::node_element, "gml:axisLabels", doc->allocate_string(array_limits_str.c_str())));
    }
  }
}
