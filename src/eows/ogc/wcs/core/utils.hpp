#ifndef __EOWS_OGC_WCS_CORE_UTILS_HPP__
#define __EOWS_OGC_WCS_CORE_UTILS_HPP__

// STL
#include <vector>
#include <string>

// RapidJSON
#include <rapidjson/document.h>

// RapidXML
#include <rapidxml/rapidxml.hpp>

namespace eows
{
  namespace geoarray
  {
    struct geoarray_t;
    struct dimension_t;
    struct spatial_extent_t;
    struct attribute_t;
  }

  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        // Forward declarations
        struct capabilities_t;
        struct service_identification_t;
        struct service_provider_t;
        struct service_metadata_t;
        struct content_t;
        struct operation_metadata_t;
        struct coverage_description_t;

        /**
         * \brief It reads WCS Capabilities from JSON document and fill values into capabilities object
         * \param doc - JSON document
         * \param capability - WCS Capabilities object to fill
         */
        void read(const rapidjson::Value& doc, capabilities_t& capability);

        /**
         * \brief It reads WCS Service Identification from JSON document and fill values into object
         */
        void read(const rapidjson::Value&, service_identification_t&);

        /**
         * \brief It reads WCS Service Metadata from JSON document and fill values into object
         */
        void read(const rapidjson::Value&, service_metadata_t&);

        /**
         * \brief It reads WCS Operations metadata from JSON document and fill values into object
         */
        void read(const rapidjson::Value &, operation_metadata_t&);

        void read_coverage_descriptions(const capabilities_t& capability, std::vector<coverage_description_t>& coverages);

        /*!
         * \brief It generates WCS Coverage bounded by element.
         * \param doc - XML root document
         * \param node - XML element to append
         * \param array - Geo array metadata to generate gml::boundedBy element
         * \param extent - Extent values used in request to expand in GML
         */
        void make_coverage_bounded_by(rapidxml::xml_document<>*,
                                      rapidxml::xml_node<>*,
                                      const geoarray::geoarray_t& array,
                                      const geoarray::spatial_extent_t&,
                                      const geoarray::dimension_t& time_dimension);

        /*!
         * \brief It generates WCS Coverage range type with SWE elements.
         * \todo Populate XML elements from attributes directly instead array, due in GetCoverage operation must retrive only affected
         * fields.
         * \param doc - XML root document to generate children nodes/attributes
         * \param node - XML element to append
         * \param array - Geo array metadata to generate gml::boundedBy element
         */
        void make_coverage_range_type(rapidxml::xml_document<>* doc,
                                      rapidxml::xml_node<>* node,
                                      const std::vector<eows::geoarray::attribute_t>& attributes);

        /*!
         * \brief It generates WCS Coverage domain set
         * \param doc - XML root document to generate children nodes/attributes
         * \param node - XML parent element
         * \param array - Geo array with metadata information to generate domain element
         */
        void make_coverage_domain_set(rapidxml::xml_document<>* doc,
                                      rapidxml::xml_node<>* node,
                                      const geoarray::geoarray_t& array,
                                      const std::vector<geoarray::dimension_t>& dimensions);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_UTILS_HPP__
