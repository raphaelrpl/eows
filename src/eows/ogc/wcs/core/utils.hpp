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

        /**
         * \brief It reads WCS Capabilities from JSON document and fill values into capabilities object
         */
        void read(const rapidjson::Value&, capabilities_t&);

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

        /*!
         * \brief It generates WCS Coverage bounded by element.
         * \param node - XML element to append
         * \param array - Geo array metadata to generate gml::boundedBy element
         */
        void make_coverage_bounded_by(rapidxml::xml_document<>*, rapidxml::xml_node<>*, const geoarray::geoarray_t&);

        /*!
         * \brief It generates WCS Coverage range type with SWE elements.
         * \param doc - XML root document to generate children nodes/attributes
         * \param node - XML element to append
         * \param array - Geo array metadata to generate gml::boundedBy element
         */
        void make_coverage_range_type(rapidxml::xml_document<>*,
                                      rapidxml::xml_node<>*,
                                      const geoarray::geoarray_t&);

        /*!
         * \brief It generates WCS Coverage domain set
         * \param doc - XML root document to generate children nodes/attributes
         * \param node - XML parent element
         * \param array - Geo array with metadata information to generate domain element
         */
        void make_coverage_domain_set(rapidxml::xml_document<>*,
                                      rapidxml::xml_node<>*,
                                      const geoarray::geoarray_t&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_UTILS_HPP__
