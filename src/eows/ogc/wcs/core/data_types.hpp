#ifndef __EOF_OGC_WCS_CORE_DATA_TYPES__
#define __EOF_OGC_WCS_CORE_DATA_TYPES__

// EOWS ogc ows
#include "../../ows/data_types.hpp"

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
          ows::provider_t service_provider;
          operation_metadata_t operation_metadata;
          service_metadata_t service_metadata;
          content_t content;
        };

        /*!
         * \brief Represents a WCS subset given by client
         */
        struct subset_t
        {
          std::string name; //!< Axis name
          std::size_t srid; //!< Projection SRID (TODO: Implement it)
          double min; //!< Axis minimum value
          double max; //!< Axis maximum value
        };

        struct range_subset_t
        {
          std::string raw;
          std::vector<std::string> attributes;
        };

        struct axis_t
        {
          std::string name;
          double x;
          double y;
        };

        struct envelope_with_time_period_t
        {
          std::string srs_name;
          std::string dimension;

          std::vector<axis_t> axis;
        };

        struct coverage_description_t
        {
          std::string name;
          envelope_with_time_period_t envelope;
        };
      }
    }
  }
}

#endif
