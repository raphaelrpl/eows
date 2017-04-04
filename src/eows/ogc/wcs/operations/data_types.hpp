#ifndef __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
#define __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__

// EOWS
#include "../../../core/data_types.hpp"
// STL
#include <string>
#include <vector>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        /**
         * @brief It represents a base request structure for WCS access
         */
        struct BaseRequest
        {
          BaseRequest();
          std::string request;
          std::string version;
          std::string service;
        };
        /**
         * @brief Represents a GetCapabilities request structure
         */
        struct GetCapabilitiesRequest : public BaseRequest { };
        /**
         * @brief Represents DescriveCoverage Request structure
         */
        struct DescribeCoverageRequest : public BaseRequest
        {
          std::string coverageId;
        };
        /**
         * @brief Represents GetCoverage request structure
         */
        struct GetCoverageRequest : public BaseRequest
        {
          std::string coverageId;
          // TODO: implement
        };
      }
    }
  }
}

#endif // __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
