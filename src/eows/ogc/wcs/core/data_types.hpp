#ifndef __EOF_OGC_WCS_CORE_DATA_TYPES__
#define __EOF_OGC_WCS_CORE_DATA_TYPES__

// STL
#include <string>
#include <vector>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        /**
         * @brief It defines a WCS online resource (Host link)
         */
        struct online_resource_t
        {
          std::string xlink;
          std::string href;
        };

        /**
         * @brief It represents a contact information based on WCS 2.0 spec
         */
        struct contact_person_primary_t
        {
          std::string contact_person;
          std::string contact_organization;
        };

        /**
         * @brief The represents contact information, defining who maintain the project, etc.
         */
        struct contact_address_t
        {
          std::string address_type;
          std::string address;
          std::string city;
          std::string state_or_province;
          std::string post_code;
          std::string country;
        };

        /**
         * @brief It represents a contact phone
         * @todo Support every contact information
         */
        struct phone_t
        {
          std::string voice;
        };

        /**
         * @brief It represents a contact ways (phone, email etc)
         * @todo describe others types
         */
        struct contact_info_t
        {
          phone_t phone;
        };

        struct service_contact_t
        {
          std::string individual_name;
          std::string position_name;
          contact_info_t contact_info;
        };

        struct service_provider_t
        {
          std::string provider_name;
          std::string provider_site;
          service_contact_t service_contact;
        };

        struct service_identification_t
        {
          std::string title;
          std::string abstract;
          std::string service_type;
          std::string service_type_version;
          std::vector<std::string> profiles;
        };

        struct dcp_t
        {
          std::string get;
        };

        struct operation_t
        {
          std::string name;
          dcp_t dcp;
        };

        struct operation_metadata_t
        {
          std::vector<operation_t> operations;
        };

        struct service_metadata_t
        {
          std::vector<std::string> formats_supported;
        };

        struct coverage_summary_t
        {
          std::string coverage_id;
          std::string coverage_subtype;
        };

        struct content_t
        {
          std::vector<coverage_summary_t> summaries;
        };

        struct capabilities_t
        {
          service_identification_t service;
          service_provider_t service_provider;
          operation_metadata_t operation_metadata;
          service_metadata_t service_metadata;
          content_t content;
        };
      }
    }
  }
}

#endif
