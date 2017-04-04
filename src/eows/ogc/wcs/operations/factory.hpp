#ifndef __EOWS_OGC_WCS_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_HPP__

// EOWS
#include "../../../core/data_types.hpp"
// STL
#include <memory>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        //! Forward declarations
        class operation;
      }

      namespace operations
      {
        /**
         * @brief It builds a OGC WCS operation based on query string
         * @throws missing_parameter_error When request does not match required arguments
         * @return Unique Ptr to the built operation
         */
        std::unique_ptr<eows::ogc::wcs::core::operation> build_operation(const eows::core::query_string_t&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_HPP__
