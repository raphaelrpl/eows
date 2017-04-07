#ifndef __EOWS_OGC_WCS_CORE_UTILS_HPP__
#define __EOWS_OGC_WCS_CORE_UTILS_HPP__

// STL
#include <string>

// RapidJSON
#include <rapidjson/document.h>

namespace eows
{
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
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_UTILS_HPP__
