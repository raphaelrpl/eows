#ifndef __EOWS_OGC_WCS_CORE_UTILS_HPP__
#define __EOWS_OGC_WCS_CORE_UTILS_HPP__

// STL
#include <string>
#include <map>
#include <vector>

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
         * @brief It tries to read rapidjson node value as string.
         * @throws eows::parse_error When could not read or process as string like
         * @return String value of node
         */
        const std::string read_node_as_string(const rapidjson::Value&, const std::string&);

        /**
         * @brief It reads WCS Capabilities from JSON document and fill values into capabilities object
         */
        void read(const rapidjson::Value&, capabilities_t&);

        /**
         * @brief It reads WCS Service Provider from JSON document and fill values into provider object
         */
        void read(const rapidjson::Value&, service_provider_t &);

        /**
         * @brief It reads WCS Service Identification from JSON document and fill values into object
         */
        void read(const rapidjson::Value&, service_identification_t&);

        /**
         * @brief It reads WCS Service Metadata from JSON document and fill values into object
         */
        void read(const rapidjson::Value&, service_metadata_t&);

        /**
         * @brief It reads WCS Contents from JSON document and fill values into object
         */
        void read(const rapidjson::Value&, content_t&);

        /**
         * @brief It reads WCS Operations metadata from JSON document and fill values into object
         */
        void read(const rapidjson::Value &, operation_metadata_t&);

        /**
         * @brief It transforms a string into lowercase
         * @return String lowercase
         */
        std::string to_lower(const std::string&);

        /**
         * @brief It applies lower case on given map keys and return a new map with these values
         * @return Copy map with keys in lowercase
         */
        std::map<std::string, std::string> lowerify(const std::map<std::string, std::string>&);

        /**
         * @brief It splits string using delimiter
         * @param string - String to split
         * @param delimiter - Delimiter to use during string parse
         * @return Splited strings
         */
        std::vector<std::string> split(const std::string&, const char&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_UTILS_HPP__
